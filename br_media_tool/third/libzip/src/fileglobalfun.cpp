#include "fileglobalfun.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "globaldefine.h"
#include "errorcode.h"
#include "codeconvert.h"
#include "guid.h"
#include "ac_ioutils.h"


#if defined(WIN32) && !defined(WINCE)
#include  <io.h>
#include <direct.h>
#pragma comment(lib,"version.lib")
#elif defined(LINUXBASE)
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#endif

#if defined(APPLEOS)
#   import <Foundation/Foundation.h>
#endif

#ifdef ANDROID
extern void ConvertMbcs2UTF8(const char* inStr, char* outStr, int outsize);
#endif

#if !defined(WIN32) || ( _MSC_VER < 1500 )
#define _fseeki64(_stm,_offset,_origin)		fseek(_stm,_offset,_origin)
#define _stat64		_stat
#define _tstat64	_tstat
#endif


// 转换文件名格式为操作系统支持的格式
CHAR* CFileGlobalFunc::ConvertFileNameOSSupport(CHAR* szSrcFileName, BOOL bSrcUTF8, CHAR* szTarFileName, DWORD dwSize)
{
	// Win32支持GBK，其它操作系统都是UTF8编码
#if defined(WIN32)
	if(bSrcUTF8)
		AC_CodeConvert::UTF82GBK(szSrcFileName, szTarFileName, dwSize);
	else
		_snprintf(szTarFileName, dwSize, "%s", szSrcFileName);
#elif defined(ANDROID)
	if(bSrcUTF8)
		_snprintf(szTarFileName, dwSize, "%s", szSrcFileName);
	else
		ConvertMbcs2UTF8((const char*)szSrcFileName, szTarFileName, dwSize);
#elif defined(APPLEOS)
	if(bSrcUTF8)
		_snprintf(szTarFileName, dwSize, "%s", szSrcFileName);
	else
	{
		NSString *nsFileNameStr = C2NS(szSrcFileName);
		_snprintf(szTarFileName, dwSize, "%s", [nsFileNameStr UTF8String]);
	}
#else
	if(bSrcUTF8)
		_snprintf(szTarFileName, dwSize, "%s", szSrcFileName);
	else
		AC_CodeConvert::GBK2UTF8((char*)szSrcFileName, szTarFileName, dwSize);
#endif
	return szTarFileName;
}

/**
 *	判断文件是否存在
 *	@param strFilePath 需要判断的文件路径
 *	@return 文件存在返回TRUE，否则返回FALSE
 */
BOOL CFileGlobalFunc::IsFileExist(CHAR* lpFilePath, BOOL bUTF8Encode)
{
	CHAR szOSSupportPathName[MAX_PATH] = {0};
	ConvertFileNameOSSupport((CHAR*)lpFilePath, bUTF8Encode, szOSSupportPathName, sizeof(szOSSupportPathName));
#if defined(WIN32)
	return (GetFileAttributesA(szOSSupportPathName) != -1);
#else
	return access(szOSSupportPathName, R_OK) == 0;
#endif
}

/**
 *	获取文件长度
 *	@param lpFilePath 需要获取长度的文件名
 *	@return 读取文件长度成功返回其长度，否则返回0
 */
DWORD CFileGlobalFunc::GetFileLength(CHAR* lpFilePath, BOOL bUTF8Encode)
{
    int len = 0;
	CHAR szOSSupportPathName[MAX_PATH] = {0};
	ConvertFileNameOSSupport((CHAR*)lpFilePath, bUTF8Encode, szOSSupportPathName, sizeof(szOSSupportPathName));
    FILE *fp = fopen(szOSSupportPathName, "r");
    if(fp != NULL)//fp==NULL时返回该文件不能打开
    {
        fseek(fp, 0, SEEK_END);
        len = (int)ftell(fp);
        fclose(fp);
    }
	return len;
}

// 获取文件长度(64bit)
INT64 CFileGlobalFunc::GetFileLength64(CHAR* lpFilePath, BOOL bUTF8Encode)
{
	CHAR szOSSupportPathName[MAX_PATH] = {0};
	ConvertFileNameOSSupport((CHAR*)lpFilePath, bUTF8Encode, szOSSupportPathName, sizeof(szOSSupportPathName));
	struct _stat64 info;
	memset(&info, 0, sizeof(struct stat));
	int ret = _stat64(szOSSupportPathName, &info);
	if(ret != 0)
		return 0;
	else
		return info.st_size;
}

/**
 *	创建一个空文件，即长度为零字节的文件
 *	@param lpFilePath 需要创建的文件名
 *	@return 文件创建成功返回TRUE，否则返回FALSE
 */
BOOL CFileGlobalFunc::CreateEmptyFile(CHAR* lpFilePath, BOOL bUTF8Encode)
{
	CHAR szOSSupportPathName[MAX_PATH] = {0};
	ConvertFileNameOSSupport((CHAR*)lpFilePath, bUTF8Encode, szOSSupportPathName, sizeof(szOSSupportPathName));
	FILE* f = fopen(szOSSupportPathName, "w ");
	BOOL bRet = FALSE;
	if(f) {
		bRet = TRUE;
		fclose(f);
	}
	return bRet;
}

// 删除文件
void CFileGlobalFunc::RemoveFile(CHAR* lpFilePath, BOOL bUTF8Encode)
{
	CHAR szOSSupportPathName[MAX_PATH] = {0};
	ConvertFileNameOSSupport((CHAR*)lpFilePath, bUTF8Encode, szOSSupportPathName, sizeof(szOSSupportPathName));
#if defined(WIN32)
	DeleteFileA(szOSSupportPathName);
#else
	remove(szOSSupportPathName);
#endif
}

// 拷贝文件
BOOL CFileGlobalFunc::CopyFile(CHAR* lpSrcFile, CHAR* lpDesFile, BOOL bUTF8Encode)
{
	CHAR szOSSupportSrcFile[MAX_PATH] = {0};
	ConvertFileNameOSSupport((CHAR*)lpSrcFile, bUTF8Encode, szOSSupportSrcFile, sizeof(szOSSupportSrcFile));
	CHAR szOSSupportDesFile[MAX_PATH] = {0};
	ConvertFileNameOSSupport((CHAR*)lpDesFile, bUTF8Encode, szOSSupportDesFile, sizeof(szOSSupportDesFile));

#if defined(WIN32)
	return ::CopyFileA(szOSSupportSrcFile, szOSSupportDesFile, TRUE);
#endif

	BOOL bSuccess = FALSE;
	do 
	{
		FILE * fIn = fopen(szOSSupportSrcFile, "rb");
		if(!fIn)
			break;
		FILE* fOut = fopen(szOSSupportDesFile, "wb");
		if(!fOut)
			break;
		fseek(fIn, 0, SEEK_SET);
		CHAR szTempBuf[8192] = {0};
		while(TRUE)
		{
			int dwSize = (int)fread(szTempBuf, 1, sizeof(szTempBuf), fIn);
			if(dwSize <= 0)
				break;
			fwrite(szTempBuf, 1, dwSize, fOut);
		}
		fclose(fIn);
		fclose(fOut);
		bSuccess = TRUE;
	} while (FALSE);
	return bSuccess;
}

// 文件改名
int CFileGlobalFunc::RenameFile(CHAR* lpSrcFile, CHAR* lpDesFile, BOOL bUTF8Encode)
{
	CHAR szOSSupportSrcFile[MAX_PATH] = {0};
	ConvertFileNameOSSupport((CHAR*)lpSrcFile, bUTF8Encode, szOSSupportSrcFile, sizeof(szOSSupportSrcFile));
	CHAR szOSSupportDesFile[MAX_PATH] = {0};
	ConvertFileNameOSSupport((CHAR*)lpDesFile, bUTF8Encode, szOSSupportDesFile, sizeof(szOSSupportDesFile));
	return rename(szOSSupportSrcFile, szOSSupportDesFile);
}


/**
 *	判断指定的目录是否存在
 *	@param lpPathName 需要判断的目录名
 *	@return 目录存在返回TRUE，否则返回FALSE
 */
BOOL CFileGlobalFunc::IsDirectoryExist(CHAR* lpPathName, BOOL bUTF8Encode)
{
	CHAR szOSSupportPathName[MAX_PATH] = {0};
	ConvertFileNameOSSupport((CHAR*)lpPathName, bUTF8Encode, szOSSupportPathName, sizeof(szOSSupportPathName));
   	BOOL rc = FALSE;
#ifdef WIN32
	// 检查目录的合法性
	DWORD  dwFlag = GetFileAttributesA(szOSSupportPathName);
	if ( (0xFFFFFFFF == dwFlag) || !(FILE_ATTRIBUTE_DIRECTORY & dwFlag) ) //目录不存在或者不是文件夹
		rc = FALSE;
	else
		rc = TRUE;
#else
    DIR *dir = opendir(szOSSupportPathName);
	if (dir) {
		closedir(dir);
		rc =TRUE;
	}
#endif
	return rc;
}

#ifndef HINSTANCE_ERROR
#define HINSTANCE_ERROR		32
#endif

// 组件注册
void CFileGlobalFunc::RegisterComponent(CHAR* lpFileName)
{
#if defined(WIN32) || defined(WINCE)
	HINSTANCE  hLib=::LoadLibraryExA(lpFileName,NULL,AC_LOADLIBRARYEX_FLAGS);
	if (hLib  >= (HINSTANCE)HINSTANCE_ERROR)
	{
		FARPROC lpDllEntryPoint;
#ifdef WINCE
		lpDllEntryPoint = GetProcAddress(hLib,_T("DllRegisterServer"));
#else
		lpDllEntryPoint = GetProcAddress(hLib,"DllRegisterServer");
#endif

		if(lpDllEntryPoint!=NULL)
			(*lpDllEntryPoint)();
		FreeLibrary(hLib);
	}
#endif
}

/**
 *	判断一个组件是否需要注册
 *	@param strFilePath 组件文件名，包含路径
 *	@return 如果需要注册，则返回TRUE，否则返回FALSE
 */
BOOL CFileGlobalFunc::IsFileNeedRegister(CHAR* strFilePath)
{
	BOOL rc = FALSE;
#ifdef WIN32
	HINSTANCE  hLib=::LoadLibraryExA(strFilePath,NULL,AC_LOADLIBRARYEX_FLAGS);

	if (hLib  >= (HINSTANCE)HINSTANCE_ERROR)
	{
#ifdef WINCE
		if(GetProcAddress(hLib,_T("DllRegisterServer")))
			rc = TRUE;
#else
		if(GetProcAddress(hLib,"DllRegisterServer"))
			rc = TRUE;
#endif

		FreeLibrary(hLib);
	}
#endif
	return rc;
}

/**
 *	获取文件版本信息
 *	@param szPathName 文件件，含路径
 *	@param strVersion 版本信息，获取成功后，通过该变量返回版本字符串
 *	@return 获取成功返回TRUE，否则返回FALSE
 */
BOOL CFileGlobalFunc::GetLocalFileVersion(CHAR* szPathName, CHAR* strVersion, DWORD dwSize)
{
#if defined(WIN32)
	BOOL rc = FALSE;
	INT nVersionLen = ::GetFileVersionInfoSizeA(szPathName,NULL);
	if(nVersionLen > 0)
	{
		CHAR* pBuffer = new CHAR[nVersionLen];
		if(pBuffer)
		{
			if(::GetFileVersionInfoA(szPathName,NULL,nVersionLen,pBuffer))
			{
				struct   LANGANDCODEPAGE   {
					WORD   wLanguage;
					WORD   wCodePage;
				}   *lpTranslate;

				//   Read   the   list   of   languages   and   code   pages.
				lpTranslate = NULL;
				UINT   cbTranslate;
				::VerQueryValue(pBuffer,_T("\\VarFileInfo\\Translation"),(LPVOID*)&lpTranslate,&cbTranslate);

				UINT   dwBytes;
				CHAR*  lpVersionBuffer;
				CHAR   szSubBlock[256] = {0};
				if(lpTranslate && cbTranslate)
					_snprintf(szSubBlock, 256, "\\StringFileInfo\\%04x%04x\\FileVersion", lpTranslate[0].wLanguage, lpTranslate[0].wCodePage);
				else
					_snprintf(szSubBlock, 256, "\\StringFileInfo\\%04x%04x\\FileVersion", 0, 0);
				if(::VerQueryValueA(pBuffer, szSubBlock,(LPVOID*)&lpVersionBuffer,&dwBytes))
				{
					_snprintf(strVersion, dwSize, "%s", lpVersionBuffer);
					rc = TRUE;
				}
			}
		}
		delete []pBuffer;
	}
	return rc;
#else
	return FALSE;
#endif
}
//
// 为了方便 strchr() 或 strrchr() 函数正确查找字符，将字符串中的中文字符串用指定的字符替换掉
//
void ReplaceChineseStrToEnglish ( CHAR *szBuf, CHAR cReplace )
{
	if ( !szBuf ) return;
	for ( int i=0; szBuf[i] != _T('\0'); i++ )
	{
		if ( szBuf[i] < 0 && szBuf[i+1] != _T('\0'))
		{
			szBuf[i] = cReplace;
			szBuf[i+1] = cReplace;
			i ++;
		}
	}
}

CHAR *hwStrChr(const CHAR *string, int c)
{
	if ( !string ) return NULL;
	CHAR* lpTempBuf = (CHAR*)malloc(strlen(string)*sizeof(CHAR)+2);
	if(!lpTempBuf)
        return NULL;
	strcpy(lpTempBuf,string);
	CHAR* ptr = strchr(lpTempBuf, c);
    if(!ptr)
    {
        free(lpTempBuf);
        return NULL;
    }
	int nPos = (int)(ptr - lpTempBuf);
    free(lpTempBuf);
	return ( (CHAR*)string + nPos );
}

#if !defined (INVALID_FILE_ATTRIBUTES)
#define INVALID_FILE_ATTRIBUTES ((DWORD)-1)
#endif

#if !defined (ALLPERMS) && defined(ANDROID)
#define ALLPERMS (S_ISUID|S_ISGID|S_ISVTX|S_IRWXU|S_IRWXG|S_IRWXO)
#endif

/********************************************************************************
* Function Type	:	Global
* Parameter		:	lpszDirName		-	[in] 目录名
* Return Value	:	没有路径的文件名
* Description	:	确保路径存在，如果目录不存在就创建目录,可以创建多层次的目录
*********************************************************************************/
void CFileGlobalFunc::MakeSureDirectory(CHAR* lpszDirName, BOOL bUTF8Encode)
{
	CHAR szOSSupportPathName[MAX_PATH] = {0};
	ConvertFileNameOSSupport((CHAR*)lpszDirName, bUTF8Encode, szOSSupportPathName, sizeof(szOSSupportPathName));

	CHAR tempbuf[255] = { 0 };
	CHAR *p1 = NULL, *p2 = (CHAR*)szOSSupportPathName;
	int len;
	int iParentRule = 0;
	while(1)
	{
		p1 = hwStrChr((const CHAR*)p2, DIRECTORY_SEPARATED_CHAR);
		if( p1 )
		{
			memset(tempbuf,0,sizeof(tempbuf));
			len = (int)((p1 - szOSSupportPathName > sizeof(tempbuf)) ? sizeof(tempbuf) : (p1 - szOSSupportPathName));
			if(len < 1)	//如：“\123\456\”目录，第一个就是“\”
			{
				p2 = p1 + 1;
				continue;
			}
			strncpy((CHAR*)tempbuf, (const CHAR*)szOSSupportPathName, len);

#if defined(WINCE) || defined(WIN32)
			DWORD dwFlags = GetFileAttributesA(tempbuf);
			if(!(dwFlags&FILE_ATTRIBUTE_DIRECTORY) || dwFlags==INVALID_FILE_ATTRIBUTES)	//Not exist
			{
				if(!CreateDirectoryA(tempbuf,NULL))
					return;
			}
#else
			if(access(tempbuf,0) == -1)	//Not exist
			{
                		if(mkdir((const char*)tempbuf, iParentRule==0 ? S_IRWXU : iParentRule) != 0)
                    			return;
			}
			else
			{
				struct stat st;
				if(stat(tempbuf,&st)==0)
					iParentRule = st.st_mode & ALLPERMS;
			}
#endif
			p2 = p1 + 1;
		}
		else break;
	}
}

// 调整磁盘，使得路径可用
BOOL CFileGlobalFunc::AdjustDiskAndCreateDirectory(CHAR* lpPathName, DWORD dwSize)
{
	CHAR szTempFile[MAX_PATH] = {0};
	MakeSureDirectory(lpPathName);
	_snprintf(szTempFile, sizeof(szTempFile), "%sanychat%d.txt", lpPathName, GetTickCount());
	if(IsDirectoryExist(lpPathName) && CreateEmptyFile(szTempFile))
	{
		RemoveFile(szTempFile);
		return TRUE;
	}
#if defined(WIN32)
	CHAR c = tolower(lpPathName[0]);
	if(c < 'c' || c > 'z')
		return FALSE;

	for (int i='c'; i<'z'; i++)
	{
		if(c == i)
			continue;
		lpPathName[0] = i;
		MakeSureDirectory(lpPathName);
		// 创建一个临时文件验证目录是否可写
		_snprintf(szTempFile, sizeof(szTempFile), "%sanychat%d.txt", lpPathName, GetTickCount());
		if(IsDirectoryExist(lpPathName) && CreateEmptyFile(szTempFile))
		{
			RemoveFile(szTempFile);
			return TRUE;
		}
	}

	// 没有对磁盘的写权限，获取系统的临时目录
	GetTempPathA(dwSize, lpPathName);
	return TRUE;
#endif
	return FALSE;
}

// 文件AES加解密
DWORD CFileGlobalFunc::FileAesEncDec(const CHAR* lpPassword, BOOL bEncrypt, const CHAR* lpSrcFile, const CHAR* lpDesFile, BOOL bUTF8Encode)
{
	if (!IsFileExist((CHAR*)lpSrcFile, bUTF8Encode))
		return AC_ERROR_TRANSFILE_OPENFAIL;

	CHAR szOSSupportSrcFile[MAX_PATH] = {0};
	ConvertFileNameOSSupport((CHAR*)lpSrcFile, bUTF8Encode, szOSSupportSrcFile, sizeof(szOSSupportSrcFile));
	CHAR szOSSupportDesFile[MAX_PATH] = {0};
	ConvertFileNameOSSupport((CHAR*)lpDesFile, bUTF8Encode, szOSSupportDesFile, sizeof(szOSSupportDesFile));
	CHAR szTempFile[MAX_PATH] = {0};
	BOOL bSameFileName = FALSE;
	if(strcmp(lpSrcFile, lpDesFile) == 0 || !strlen(lpDesFile))
	{
		bSameFileName = TRUE;
		_snprintf(szTempFile, sizeof(szTempFile), "%s", lpSrcFile);
		// 产生一个临时文件名
		CHAR szGuid[100] = {0};
		CGuidUtils::GuidToString(CGuidUtils::CreateGuid(), szGuid, sizeof(szGuid));
		(strrchr(szTempFile, DIRECTORY_SEPARATED_CHAR))[1] = 0;
		strcat(szTempFile, szGuid);
		ConvertFileNameOSSupport((CHAR*)szTempFile, bUTF8Encode, szOSSupportDesFile, sizeof(szOSSupportDesFile));
	}
	else
	{
		RemoveFile((CHAR*)lpDesFile, bUTF8Encode);
		MakeSureDirectory((CHAR*)lpDesFile, bUTF8Encode);
	}
	FILE* fIn = NULL;
	FILE* fOut = NULL;
	DWORD dwRet = -1;
	do 
	{
		fIn = fopen(szOSSupportSrcFile, "rb");
		if(!fIn)
			break;
		fOut = fopen(szOSSupportDesFile, "wb");
		if(!fOut)
			break;
		fseek(fIn, 0, SEEK_SET);
		CHAR szTempBuf[8192] = {0};
		CHAR szOutBuf[10240] = {0};
		while(TRUE)
		{
			int dwSize = (int)fread(szTempBuf, 1, sizeof(szTempBuf), fIn);
			if(dwSize <= 0)
				break;
			DWORD dwOutSize = sizeof(szOutBuf);
			if(bEncrypt)
				AC_IOUtils::AESEncodeBuf(lpPassword, (const CHAR*)szTempBuf, dwSize, szOutBuf, dwOutSize);
			else
				AC_IOUtils::AESDecodeBuf(lpPassword, (const CHAR*)szTempBuf, dwSize, szOutBuf, dwOutSize);
			fwrite(szOutBuf, 1, dwOutSize, fOut);
		}
		dwRet = 0;
	} while (FALSE);
	if(fIn)
		fclose(fIn);
	if(fOut)
		fclose(fOut);
	if(dwRet == 0)
	{
		if(bSameFileName)
		{
			RemoveFile((CHAR*)lpSrcFile, bUTF8Encode);
			RenameFile(szTempFile, (CHAR*)lpSrcFile, bUTF8Encode);
		}
	}
	return dwRet;
}

#if defined(WIN32)
// 从注册表中读取字符串类型数据
BOOL CFileGlobalFunc::RegReadStringValue(CHAR* lpKeyName, CHAR*  lpValueName, BYTE* lpOutBuf, DWORD& dwBufSize)
{
	BOOL rcValue = FALSE;
	HKEY hKey = NULL;
	do
	{
		DWORD ret = ::RegOpenKeyExA(HKEY_LOCAL_MACHINE,lpKeyName,0,KEY_READ,&hKey);
		if(ret != ERROR_SUCCESS)
			break;
		DWORD ValueType=REG_SZ;
		ret =::RegQueryValueExA(hKey,lpValueName,NULL,&ValueType,lpOutBuf,&dwBufSize);
		if(ret != ERROR_SUCCESS)
			break;
		rcValue = TRUE;
	} while (FALSE);
	if(hKey)
		RegCloseKey(hKey);
	return rcValue;
}
// 将字符串类型数据写入到注册表中
BOOL CFileGlobalFunc::RegWriteStringValue(CHAR* lpKeyName, CHAR*  lpValueName, BYTE* lpStrBuf, DWORD dwBufSize)
{
	BOOL rcValue = FALSE;
	HKEY hKey = NULL;
	do
	{
		DWORD Disposition;
		DWORD ret = ::RegCreateKeyExA(HKEY_LOCAL_MACHINE,lpKeyName,0,NULL,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,NULL,&hKey,&Disposition);
		if(ret != ERROR_SUCCESS)
			break;
		ret =::RegSetValueExA(hKey,lpValueName,NULL,REG_SZ,lpStrBuf,dwBufSize);
		if(ret != ERROR_SUCCESS)
			break;
		rcValue = TRUE;
	} while (FALSE);
	if(hKey)
		RegCloseKey(hKey);
	return rcValue;
}
// 从注册表中读取DWORD类型数据
BOOL CFileGlobalFunc::RegReadDWordValue(CHAR* lpKeyName, CHAR*  lpValueName, DWORD& dwValue)
{
	BOOL rcValue = FALSE;
	HKEY hKey = NULL;
	do
	{
		DWORD ret = ::RegOpenKeyExA(HKEY_LOCAL_MACHINE,lpKeyName,0,KEY_READ,&hKey);
		if(ret != ERROR_SUCCESS)
			break;
		DWORD ValueType=REG_DWORD;
		DWORD size= sizeof(DWORD);
		ret =::RegQueryValueExA(hKey,lpValueName,NULL,&ValueType,(LPBYTE)&dwValue,&size);
		if(ret != ERROR_SUCCESS)
			break;
		rcValue = TRUE;
	} while (FALSE);
	if(hKey)
		RegCloseKey(hKey);
	return rcValue;
}
// 将DWORD类型数据写入到注册表中
BOOL CFileGlobalFunc::RegWriteDWordValue(CHAR* lpKeyName, CHAR*  lpValueName, DWORD dwValue)
{
	BOOL rcValue = FALSE;
	HKEY hKey = NULL;
	do
	{
		DWORD Disposition;
		DWORD ret = ::RegCreateKeyExA(HKEY_LOCAL_MACHINE,lpKeyName,0,NULL,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,NULL,&hKey,&Disposition);
		if(ret != ERROR_SUCCESS)
			break;
		ret = ::RegSetValueExA(hKey,lpValueName,NULL,REG_DWORD,(LPBYTE)&dwValue,sizeof(DWORD));
		if(ret != ERROR_SUCCESS)
			break;
		rcValue = TRUE;
	} while (FALSE);
	if(hKey)
		RegCloseKey(hKey);
	return rcValue;
}
#endif

// 枚举目录下单级所有文件, 失败返回非0值
DWORD CFileGlobalFunc::EnumFiles(CHAR* lpFolderPath, std::vector<std::string>& vtrFiles)
{
	// 检查文件夹是否存在
	if (NULL == lpFolderPath || !CFileGlobalFunc::IsDirectoryExist(lpFolderPath))
		return -1;

#ifdef WIN32

	//首先查找dir中符合要求的文件
	char szFilespec[MAX_PATH] = {0};
	if(strlen(lpFolderPath)>0 && lpFolderPath[strlen(lpFolderPath)-1]==DIRECTORY_SEPARATED_CHAR)
		_snprintf(szFilespec, sizeof(szFilespec)-1, "%s*.*", lpFolderPath);
	else
		_snprintf(szFilespec, sizeof(szFilespec)-1, "%s%c*.*", lpFolderPath, DIRECTORY_SEPARATED_CHAR);
	_finddata_t fileinfo = {0};
	long hFile = _findfirst(szFilespec, &fileinfo);
	if(hFile != -1)
	{
		do
		{
			//检查是不是目录
			if(!(fileinfo.attrib & _A_SUBDIR))
				vtrFiles.push_back(std::string(fileinfo.name));

		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
	return 0;

#else
	
	struct dirent	*lpDirent = NULL;
	DIR				*pDir = NULL;
	pDir = opendir(lpFolderPath);		// 打开文件夹
	if(pDir == NULL)
		return -1;
	while(NULL != (lpDirent=readdir(pDir)))
	{
		if(!strcmp(lpDirent->d_name, "..") || !strcmp(lpDirent->d_name, "."))
			continue;

		// 保存完整路径
		char szIntegrityPath[MAX_PATH] = {0};
		if(strlen(lpFolderPath)>0 && lpFolderPath[strlen(lpFolderPath)-1]==DIRECTORY_SEPARATED_CHAR)
			_snprintf(szIntegrityPath, sizeof(szIntegrityPath)-1, "%s%s", lpFolderPath, lpDirent->d_name);
		else
			_snprintf(szIntegrityPath, sizeof(szIntegrityPath)-1, "%s%c%s", lpFolderPath, DIRECTORY_SEPARATED_CHAR, lpDirent->d_name);

		struct stat fileInfo = {0};
		if(0 != stat(szIntegrityPath, &fileInfo))
		{
			printf("stat error:%d, path:%s \n", errno, szIntegrityPath);	
			continue;
		}

		// 判断是不是文件，文件才处理
		if(fileInfo.st_mode & S_IFREG)
			vtrFiles.push_back(lpDirent->d_name);
	}
	closedir(pDir);		// 关闭文件夹
	return 0;
	
#endif
	return -1;
}


