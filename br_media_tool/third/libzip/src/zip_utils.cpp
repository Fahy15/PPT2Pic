#include "stdafx.h"
#include "zip_utils.h"
#include "fileassist.h"
#include "fileglobalfun.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "osdef.h"

#   ifndef WIN32
#include<sys/types.h>
#include<dirent.h>
#include <sys/stat.h>
#include <sys/io.h>
#   endif // WIN32

using std::vector;
using std::string;
using std::pair;

CZipUtils::CZipUtils()
{
}

CZipUtils::~CZipUtils()
{

}

// �ļ���ȡ
//bool CZipUtils::extractPacket(const char *lpszSrc, const char *lpszDes)
//{
//	bool ret = false;
//
//	do
//	{
//		if (NULL == lpszSrc || strlen(lpszSrc) == 0)
//			break;
//
//		char szZipFile[MAX_PATH] = {0};
//		strcpy(szZipFile, lpszSrc);
//
//		char szDesPath[MAX_PATH] = {0};
//		if (NULL != lpszDes){
//			strcpy(szDesPath, lpszDes);
//		}
//		else{
//			// ���Ŀ���ļ��в�ָ����ʹ���������ļ�ͬ��Ŀ¼
//			const char *p = strrchr(lpszSrc, DIRECTORY_SEPARATED_CHAR);
//			if (NULL == p)
//				break;
//			strcpy(szDesPath, lpszSrc, p - lpszSrc);
//		}
//
//		// �ж�Ŀ���ǲ���һ���ļ���(·��)
//		if(!CFileAssist::IsDirectoryExist(szDesPath))
//			if (!CFileAssist::MakeIntegrityPath(szDesPath))
//				break;
//
//		char szUTF8[MAX_PATH] = {0};
//		ZipArchive hz(_GBK2UTF8(szZipFile, szUTF8, sizeof(szUTF8)));
//		hz.open(ZipArchive::READ_ONLY);
//		if (hz.isOpen())
//		{
//			bool bEndIsSlash = false;	// ������Ŀ¼�ǲ�����б�ܽ�β
//			if (lpszSrc[strlen(szDesPath) - 1] == DIRECTORY_SEPARATED_CHAR)
//				bEndIsSlash = true;
//
//			// ����ѹ�����ڵ�����
//			std::vector<ZipEntry> verEntries = hz.getEntries();
//			for (std::vector<ZipEntry>::iterator iter = verEntries.begin();
//				iter != verEntries.end(); iter++)
//			{
//				std::string strAbsolutePath(szDesPath);
//				if (!bEndIsSlash)
//					strAbsolutePath += DIRECTORY_SEPARATED_CHAR;
//#ifdef __TEMP____//_WINDOWS_	// [AC_CodeConvert::UTF82GBK]ò�������⣬������ʱ���ε�,����֤ 2016-01-21 00:09:22
//
//				char strGBK[MAX_PATH] = {0};
//				char strUtf8Name[MAX_PATH] = {0};
//				strcpy_s(strUtf8Name, sizeof(strUtf8Name), (*iter).getName().c_str());
//				strAbsolutePath += _UTF82GBK(strUtf8Name, strGBK, sizeof(strGBK));
//#else
//				strAbsolutePath += (*iter).getName();
//#endif
//
//
//				// ��Ϊ�ײ�ⷵ�ص��ļ����������ǡ�/����β�������ǲ���windowsƽ̨������ȥ�ĵײ���룬����������򵥴���һ��
//#ifdef WIN32
//				while (string::npos != strAbsolutePath.find("/")){
//					strAbsolutePath = strAbsolutePath.replace(strAbsolutePath.find("/"), 1, "\\");
//				}
//#endif
//				if ( !(*iter).isFile() )
//				{
//					// ��������·��
//					ret = CFileAssist::MakeIntegrityPath(strAbsolutePath.c_str());
//				}
//				else
//				{
//					// ����ͬ�����ļ�����ȡ�ļ������ݣ�д���ļ���
//					FILE *file = fopen(strAbsolutePath.c_str(), "wb+");
//					if (file != NULL)
//					{
//						int len = fwrite((*iter).readAsText().c_str(), 1, (size_t)(*iter).getSize(), file);
//						fclose(file);
//						file = NULL;
//
//						if (len == (*iter).getSize())
//							ret = true;
//						else
//							ret = false;
//					}
//					else
//						ret = false;
//				}
//
//				// �ж��Ƿ�ɹ�
//				if (!ret)
//				 break;
//			}
//
//			hz.close();
//		}
//
//	} while (false);
//
//	return ret;
//}

// ѹ���ļ���
bool CZipUtils::compressFolder(const char *lpszSrc, const char *lpszDes)
{
	bool ret = false;

	if (lpszSrc == NULL || !CFileAssist::IsDirectoryExist(lpszSrc))
		return false;

	// ���Ŀ���û�����ָ�����Ǿ�ʹ�������ļ��е����ڵ�ǰĿ¼�´����ļ�
	char szPath[MAX_PATH] = {0};
	if (lpszDes == NULL){
		strcpy(szPath,  lpszSrc);

		if (STR_END_IS_SEPARATED(lpszSrc))
			szPath[strlen(lpszSrc) - 1] = 0;		// ȥ�������DIRECTORY_SEPARATED_CHAR
		strcat(szPath, ".zip");
	}
	else{
		strcpy(szPath, lpszDes);
	}

	ZipArchive hz(szPath);
	hz.open(ZipArchive::WRITE);
	if (hz.isOpen())
	{
		// ���õݹ麯�������Ŀ¼�����ļ��к��ļ�
		char szLastDirName[MAX_PATH] = {0};
		ret = compressSubFolder(hz, lpszSrc, CFileAssist::getLastDirName(lpszSrc, szLastDirName, sizeof(szLastDirName)));
		hz.close();
		if (!ret)
			hz.unlink();

	}
	else
		ret = false;

	return ret;
}

bool CZipUtils::compressSubFolder(ZipArchive &hz, const char *lpszSrc, const char *lpszDes)
{
	if (lpszSrc == NULL)
		return false;

	// ����ļ��е�ѹ����
	std::string strDes(lpszDes);
	if (!IS_DIRECTORY(strDes))
		strDes += DIRECTORY_SEPARATOR;		// zip�ײ���ļ��зָ��ַ�������ƽ̨��ȫ���á�/��

	if (!hz.addEntry(strDes))
		return false;

	CHAR szBuffer[MAX_PATH] = { 0 };
    if (STR_END_IS_SEPARATED(lpszSrc))
		_snprintf(szBuffer, sizeof(szBuffer), "%s%s", lpszSrc, "*.*");
	else
		_snprintf(szBuffer, sizeof(szBuffer), "%s%c%s", lpszSrc, DIRECTORY_SEPARATED_CHAR, "*.*");
	

#ifdef WIN32

	WIN32_FIND_DATAA FindFileData;
	HANDLE hFind = FindFirstFileA(szBuffer, &FindFileData);

	if (hFind != INVALID_HANDLE_VALUE)
	{
		do
		{
			if (strcmp(FindFileData.cFileName, ".") != 0 && strcmp(FindFileData.cFileName, "..") != 0){

				// ��ӵ�zip�ļ��е�·��(ͳһ�� '/' ���ļ��зָ��)
				char szDestToZIP[MAX_PATH] = {0};
				if(lpszDes[strlen(lpszDes) - 1] == '/')
					sprintf_s(szDestToZIP, sizeof(szDestToZIP),"%s%s", lpszDes, FindFileData.cFileName);
				else
					sprintf_s(szDestToZIP, sizeof(szDestToZIP), "%s%c%s", lpszDes, '/', FindFileData.cFileName);

				// Դ�ļ�·��
				if (lpszSrc[strlen(lpszSrc) - 1] == DIRECTORY_SEPARATED_CHAR)
					sprintf_s(szBuffer, sizeof(szBuffer), "%s%s", lpszSrc, FindFileData.cFileName);
				else
					sprintf_s(szBuffer, sizeof(szBuffer), "%s%c%s", lpszSrc, DIRECTORY_SEPARATED_CHAR, FindFileData.cFileName);

				if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY){

					compressSubFolder(hz, szBuffer, szDestToZIP);	// �ݹ�
				}
				else{
					// ����ļ�
					hz.addFile(szDestToZIP, szBuffer);
				}

			}
		} while (FindNextFileA(hFind, &FindFileData));
	}

	FindClose(hFind);
#else
	{
		struct dirent	*lpDirent = NULL;
		DIR				*pDir = NULL;
		pDir=opendir(szBuffer);		// ���ļ���
		if (pDir == NULL)
			return false;

		while (NULL != (lpDirent=readdir(pDir)))
		{
			if (!strcmp(lpDirent->d_name, "..") || !strcmp(lpDirent->d_name, "."))
				continue;

			// ��ӵ�zip�ļ��е�·��
			char szDestToZIP[MAX_PATH] = {0};
			if (STR_END_IS_SEPARATED(lpszDes))
				_sntprintf(szDestToZIP, sizeof(szDestToZIP),"%s%s", lpszDes, lpDirent->d_name);
			else
				_sntprintf(szDestToZIP, sizeof(szDestToZIP), "%s%c%s", lpszDes, DIRECTORY_SEPARATED_CHAR, lpDirent->d_name);

			// Դ�ļ�·��
            if (STR_END_IS_SEPARATED(lpszSrc))
				_sntprintf(szBuffer, sizeof(szBuffer), "%s%s", lpszSrc, lpDirent->d_name);
			else
				_sntprintf(szBuffer, sizeof(szBuffer), "%s%c%s", lpszSrc, DIRECTORY_SEPARATED_CHAR, lpDirent->d_name);

			if (lpDirent->d_type==8)
			{
				// �ļ���
				compressSubFolder(hz, szBuffer, szDestToZIP);	// �ݹ�

			}
			else if (lpDirent->d_type==4)
			{
				// �ļ�
				hz.addFile(szDestToZIP, szBuffer);
			}
		}
		closedir(pDir);		// �ر��ļ���
	}

#endif  // WIN32

	return true;
}

// ѹ���ļ����ļ���
bool CZipUtils::compressFile(const std::vector<std::string> &vtrFile, const char *lpszDes)
{
	if (lpszDes == NULL)
		return false;

	bool ret = true;

	ZipArchive hz(lpszDes);
	hz.open(ZipArchive::WRITE);
	if (hz.isOpen())
	{
		// ֻҪ��һ�����ɹ��Ǿͷ���ʧ��
		char szFile[MAX_PATH] = {0};
		for (std::vector<std::string>::const_iterator iter = vtrFile.begin();
			iter != vtrFile.end(); iter++)
		{
			if (hz.addFile(CFileAssist::getLastDirName((*iter).c_str(), szFile, sizeof(szFile)), (*iter).c_str()) == false){
				fprintf(stderr, "call [addFile] fail!  filename = %s", (*iter).c_str());
				ret = false;
				// ��������һ����ļ�
				hz.close();
				hz.unlink();
				break;
			}
		}

		hz.close();
	}
	else
		ret = false;

	return ret;
}

//CHAR * CZipUtils::_GBK2UTF8(CHAR* lpGbkStr, CHAR* lpUtf8Str, DWORD dwBufSize)
//{
//	if (AC_CodeConvert::GBK2UTF8(lpGbkStr, lpUtf8Str, dwBufSize))
//		return lpUtf8Str;
//	else
//		return lpGbkStr;
//}
//
//// UTF8תGBK
//CHAR * CZipUtils::_UTF82GBK(CHAR* lpUtf8Str, CHAR* lpGbkStr, DWORD dwBufSize)
//{
//	if ((AC_CodeConvert::UTF82GBK(lpUtf8Str, lpGbkStr, dwBufSize)))
//		return lpGbkStr;
//	else
//		return lpUtf8Str;
//}

