#include "fileassist.h"
#include <stdio.h>
#include <stdlib.h>

#ifdef WIN32
#	include <Windows.h>
#	include <direct.h>
#	include <io.h>
#else
#   include "osdef.h"
#	include <unistd.h>
#	include <sys/stat.h>
#	include <sys/types.h>
#   include <errno.h>
#	include <dirent.h>
#   include <fcntl.h>
#	include <sys/dir.h>
#	include <sys/io.h>
#	include <signal.h>
#endif

CFileAssist::CFileAssist()
{

}

CFileAssist::~CFileAssist()
{

}

// �ж��ļ��Ƿ����
BOOL CFileAssist::IsFileExist(const char * lpFilePath)
{
#if defined(WIN32)
	return CFileGlobalFunc::IsFileExist((CHAR*)lpFilePath, FALSE);
#else
	return CFileGlobalFunc::IsFileExist(lpFilePath, TRUE);
#endif
}

// �ж�Ŀ¼�Ƿ����
BOOL CFileAssist::IsDirectoryExist(const char * lpPathName)
{
#if defined(WIN32)
	return CFileGlobalFunc::IsDirectoryExist((CHAR*)lpPathName, FALSE);
#else
	return CFileGlobalFunc::IsDirectoryExist(lpPathName, TRUE);
#endif
}

// ɾ���ļ�
void CFileAssist::RemoveFile(const char * lpFilePath)
{
	CFileGlobalFunc::RemoveFile((CHAR*)lpFilePath);
}

// ɾ���ļ���(ɾ���ǿ��ļ���), bOnlyInternal == true ��ֻ����ڲ���������ͬ�ļ���һ��ɾ��
BOOL CFileAssist::RemoveDir(const char * lpPathName, BOOL bOnlyInternal)
{
    BOOL bRet = TRUE;
#ifdef	WIN32
	{
		if (lpPathName == NULL)
			return FALSE;

		CHAR szBuffer[MAX_PATH] = { 0 };
		if (STR_END_IS_SEPARATED(lpPathName))
			sprintf_s(szBuffer, sizeof(szBuffer), "%s%s", lpPathName, "*.*");
		else
			sprintf_s(szBuffer, sizeof(szBuffer), "%s%c%s", lpPathName, DIRECTORY_SEPARATED_CHAR, "*.*");

		WIN32_FIND_DATAA FindFileData;
		HANDLE hFind = FindFirstFileA(szBuffer, &FindFileData);

		if (hFind != INVALID_HANDLE_VALUE)
		{
			do
			{
				if (STR_END_IS_SEPARATED(lpPathName))
					sprintf_s(szBuffer, sizeof(szBuffer), "%s%s", lpPathName, FindFileData.cFileName);
				else
					sprintf_s(szBuffer, sizeof(szBuffer), "%s%c%s", lpPathName, DIRECTORY_SEPARATED_CHAR, FindFileData.cFileName);

				if (strcmp(FindFileData.cFileName, ".") != 0 && strcmp(FindFileData.cFileName, "..") != 0){

					if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY){
						CHAR szDir[MAX_PATH] = { 0 };
						sprintf_s(szDir, sizeof(szDir), "%s\\", szBuffer);
						RemoveDir(szDir, FALSE);	// �ݹ�
					}
					else
						RemoveFile(szBuffer);
				}
			} while (FindNextFileA(hFind, &FindFileData));
		}

		FindClose(hFind);

		// �ж��Ƿ��ǽ��������ļ����ڲ�
		if (!bOnlyInternal)
			bRet = RemoveDirectoryA(lpPathName);
	}

#else
	{
		struct dirent	*lpDirent = NULL;
		DIR				*pDir = NULL;
		pDir=opendir(lpPathName);		// ���ļ���
		if (pDir == NULL)
			return bRet;

		bRet = true;
		while (NULL != (lpDirent=readdir(pDir)))
		{
			if (!strcmp(lpDirent->d_name, "..") || !strcmp(lpDirent->d_name, "."))
				continue;

			// ��������·��
			char szIntegrityPath[PATH_MAX] = {0};
			if (STR_END_IS_SEPARATED(lpPathName))
				sprintf(szIntegrityPath, "%s%s", lpPathName, lpDirent->d_name);
			else
				sprintf(szIntegrityPath, "%s%c%s", lpPathName, DIRECTORY_SEPARATED_CHAR, lpDirent->d_name);

			struct stat fileInfo = {0};
			if(0 != stat(szIntegrityPath, &fileInfo))
			{
				printf("stat error:%d, path:%s \n", errno, szIntegrityPath);	
				bRet = FALSE;
				break;
			}

			// 4 is dir, 8 is file
			if (fileInfo.st_mode & S_IFDIR)
			{
				// �ݹ�ɾ���ļ���
				bRet = RemoveDir(szIntegrityPath, FALSE);	
				if (!bRet)
					break;
			}
			else if (fileInfo.st_mode & S_IFREG)
			{
				// ɾ���ļ�
				bRet  = unlink(szIntegrityPath) == 0 ? TRUE : FALSE;
				if (!bRet)
					break;
			}
		}
		closedir(pDir);		// �ر��ļ���

		// �ж��Ƿ��ǽ��������ļ����ڲ�
		if (!bOnlyInternal)
			bRet = rmdir(lpPathName) == 0 ? TRUE : FALSE;
	}

#endif
	return bRet;
}

// ɾ���ļ�������������ļ����ܱ������ļ��г���, vtrProtected:�ܱ����ļ������������������е��ļ��в���
BOOL CFileAssist::RemoveFileOfDir(const char * lpPathName, const std::vector<std::string> &vtrProtected)
{
	BOOL bRet = TRUE;
#ifdef	WIN32
	{
		if (lpPathName == NULL)
			return FALSE;

		CHAR szBuffer[MAX_PATH] = { 0 };
		if (STR_END_IS_SEPARATED(lpPathName))
			sprintf_s(szBuffer, sizeof(szBuffer), "%s%s", lpPathName, "*.*");
		else
			sprintf_s(szBuffer, sizeof(szBuffer), "%s%c%s", lpPathName, DIRECTORY_SEPARATED_CHAR, "*.*");

		WIN32_FIND_DATAA FindFileData;
		HANDLE hFind = FindFirstFileA(szBuffer, &FindFileData);

		if (hFind != INVALID_HANDLE_VALUE)
		{
			do
			{
				if (STR_END_IS_SEPARATED(lpPathName))
					sprintf_s(szBuffer, sizeof(szBuffer), "%s%s", lpPathName, FindFileData.cFileName);
				else
					sprintf_s(szBuffer, sizeof(szBuffer), "%s%c%s", lpPathName, DIRECTORY_SEPARATED_CHAR, FindFileData.cFileName);

				if (strcmp(FindFileData.cFileName, ".") != 0 && strcmp(FindFileData.cFileName, "..") != 0){

					if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY){
						
						// �ж��ǲ����ܱ����ļ��У�������Ǿ�ʲô��������
						std::vector<std::string>::const_iterator iter = find(vtrProtected.begin(), vtrProtected.end(), FindFileData.cFileName);
						if (iter != vtrProtected.end())
							continue;
						
						bRet = RemoveDir(szBuffer);
						if (!bRet)
							break;	// ɾ��ʧ��,����ѭ��
					}
					else
						RemoveFile(szBuffer);
				}
			} while (FindNextFileA(hFind, &FindFileData));
		}

		FindClose(hFind);
	}

#else
	{
		struct dirent	*lpDirent = NULL;
		DIR				*pDir = NULL;
		pDir=opendir(lpPathName);		// ���ļ���
		if (pDir == NULL)
			return bRet;

		bRet = true;
		while (NULL != (lpDirent=readdir(pDir)))
		{
			if (!strcmp(lpDirent->d_name, "..") || !strcmp(lpDirent->d_name, "."))
				continue;

			// ��������·��
			char szIntegrityPath[PATH_MAX] = {0};
			if (STR_END_IS_SEPARATED(lpPathName))
				sprintf(szIntegrityPath, "%s%s", lpPathName, lpDirent->d_name);
			else
				sprintf(szIntegrityPath, "%s%c%s", lpPathName, DIRECTORY_SEPARATED_CHAR, lpDirent->d_name);

			struct stat fileInfo = {0};
			if(0 != stat(szIntegrityPath, &fileInfo))
			{
				printf("stat error:%d, path:%s \n", errno, szIntegrityPath);	
				bRet = FALSE;
				break;
			}
		
			// 4 is dir, 8 is file
			if (fileInfo.st_mode & S_IFDIR)
			{
				// �ж��ǲ����ܱ����ļ��У�������Ǿ�ʲô��������
				std::vector<std::string>::const_iterator iter = find(vtrProtected.begin(), vtrProtected.end(), lpDirent->d_name);
				if (iter != vtrProtected.end())
					continue;

				// ɾ���ļ���
				bRet = RemoveDir(szIntegrityPath, FALSE);
				if (!bRet)
					break;
			}
			else if (fileInfo.st_mode & S_IFREG)
			{
				// ɾ���ļ�
				bRet  = unlink(szIntegrityPath) == 0 ? TRUE : FALSE;
				if (!bRet)
					break;
			}
		}
		closedir(pDir);		// �ر��ļ���
	}

#endif
	return bRet;
}

// �����ļ���
BOOL CFileAssist::MakeDirectory(const char * lpPathName)
{
	//���������Ե�·��������Ҫת���ɾ���·��
	char szPath[MAX_PATH] = {0};
	if (relativePath2AbsolutePath(lpPathName, szPath, sizeof(szPath)) == NULL)
		return FALSE;
	
#ifdef WIN32
	BOOL bRet = _mkdir(szPath) == 0 ? TRUE : FALSE;
#else
	BOOL bRet = mkdir(szPath, S_IRWXU) == 0 ? TRUE : FALSE;
#endif

	return bRet;
}

// ����һ��·������·���ĵ�һ�����ж��Ƿ���ڣ������ھʹ�����
bool CFileAssist::MakeIntegrityPath(const char* lpIntegrityPath)
{
	if (lpIntegrityPath == NULL)
		return false;

	// ��������� �ļ��зָ���('\' �� '/')��β�ľ�����Ϸָ���
	char szBuffer[MAX_PATH] = {0};
	strcpy(szBuffer, lpIntegrityPath);
	STR_END_ADD_SEPARATED(szBuffer);
	
	CFileGlobalFunc::MakeSureDirectory(szBuffer);
	return true;
}

// �����ļ�
bool CFileAssist::CopySingleFile(const char * lpFileSrc, const char * lpFileDest)
{
	bool bRet = false;
	if (lpFileSrc == NULL || lpFileDest == NULL|| !IsFileExist(lpFileSrc))
		return bRet;

	do
	{
		FILE *fileDes = fopen(lpFileDest, "wb+");
		if (fileDes == NULL)
			break;

		FILE *fileSrc = fopen(lpFileSrc, "rb");
		if (fileSrc == NULL)
			break;

		// ��ʼ����
		bool bError = false;
		char buf[1024] = {0};
		int len = fread(buf, 1, 1024, fileSrc);
		while(len != 0)
		{
			if (len != fwrite(buf, 1, len, fileDes))
			{
				bError = true;
				break;	// ����
			}

			len = fread(buf,1, 1024, fileSrc);
		}

		fclose(fileSrc);
		fclose(fileDes);

		if (bError)
			RemoveFile(lpFileDest);	// ����ʧ�ܣ�����ȱ�ļ�ɾ��
		else
			bRet = true;
	} while (false);

	return bRet;
}

// �����ļ����������е��ļ���ָ��·��(Ŀ��·���������)
bool CFileAssist::CopyFileOfDir(const char * lpDirSrc, const char * lpDirDest)
{
	bool bRet = false;
	if(lpDirSrc == NULL || lpDirDest == NULL || !CFileAssist::IsDirectoryExist(lpDirSrc) || !CFileAssist::IsDirectoryExist(lpDirDest))
		return bRet;

#ifdef WIN32
	{
		CHAR szBuffer[MAX_PATH] = { 0 };
		if (STR_END_IS_SEPARATED(lpDirSrc))
			sprintf_s(szBuffer, sizeof(szBuffer), "%s%s", lpDirSrc, "*.*");
		else
			sprintf_s(szBuffer, sizeof(szBuffer), "%s%c%s", lpDirSrc, DIRECTORY_SEPARATED_CHAR, "*.*");

		bRet = true;
		WIN32_FIND_DATAA FindFileData;
		HANDLE hFind = FindFirstFileA(szBuffer, &FindFileData);
		if (hFind != INVALID_HANDLE_VALUE)
		{
			do
			{
				if (strcmp(FindFileData.cFileName, ".") != 0 && strcmp(FindFileData.cFileName, "..") != 0){

					// Դ����·��
					char szFileSrc[MAX_PATH] = {0};
					if (STR_END_IS_SEPARATED(lpDirSrc))
						sprintf_s(szFileSrc, sizeof(szFileSrc), "%s%s", lpDirSrc, FindFileData.cFileName);
					else
						sprintf_s(szFileSrc, sizeof(szFileSrc), "%s%c%s", lpDirSrc, DIRECTORY_SEPARATED_CHAR, FindFileData.cFileName);

					// Ŀ������·��
					char szFileDes[MAX_PATH] ={0};
					if (STR_END_IS_SEPARATED(lpDirDest))
						sprintf_s(szFileDes, sizeof(szFileDes), "%s%s", lpDirDest, FindFileData.cFileName);
					else
						sprintf_s(szFileDes, sizeof(szFileDes), "%s%c%s", lpDirDest, DIRECTORY_SEPARATED_CHAR, FindFileData.cFileName);

					if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY){

						//  ����ļ��к����"//"
						STR_END_ADD_SEPARATED(szFileSrc);
						STR_END_ADD_SEPARATED(szFileDes);
						
						// �����ļ���,����Ŀ���ļ��У�Ȼ��ݹ鿽���ļ�
						if (!CFileAssist::IsDirectoryExist(szFileDes)){
							if (!CFileAssist::MakeDirectory(szFileDes))
							{
								bRet = false;
								break;
							}
						}
						bRet = CopyFileOfDir(szFileSrc, szFileDes);
						if (!bRet)
							break;
					}
					else{
						// �����ļ�
						bRet = CopySingleFile(szFileSrc, szFileDes);
						if (!bRet)
							break;
					}
				}
			} while (FindNextFileA(hFind, &FindFileData));
		}

		FindClose(hFind);
	}
#else
	{
		struct dirent	*lpDirent = NULL;
		DIR				*pDir = NULL;
		pDir=opendir(lpDirSrc);		// ���ļ���
		if (pDir == NULL)
			return bRet;

		bRet = true;
		while (NULL != (lpDirent=readdir(pDir)))
		{
			if (!strcmp(lpDirent->d_name, "..") || !strcmp(lpDirent->d_name, "."))
				continue;

			// ��������·��
			char szIntegritySrcPath[PATH_MAX] = {0};
			if (STR_END_IS_SEPARATED(lpDirSrc))
				sprintf(szIntegritySrcPath, "%s%s", lpDirSrc, lpDirent->d_name);
			else
				sprintf(szIntegritySrcPath, "%s%c%s", lpDirSrc, DIRECTORY_SEPARATED_CHAR, lpDirent->d_name);

			struct stat fileInfo = {0};
			if(0 != stat(szIntegritySrcPath, &fileInfo))
			{
				printf("stat error:%d, path:%s \n", errno, szIntegritySrcPath);	
				bRet = FALSE;
				break;
			}

			// ������Ŀ��·��
			char szIntegrityDesPath[MAX_PATH] = {0};
			if (STR_END_IS_SEPARATED(lpDirDest))
				sprintf(szIntegrityDesPath, "%s%s", lpDirDest, lpDirent->d_name);
			else
				sprintf(szIntegrityDesPath, "%s%c%s", lpDirDest, DIRECTORY_SEPARATED_CHAR, lpDirent->d_name);

			// �ж�Ŀ¼������ͨ�ļ�
			if (fileInfo.st_mode & S_IFDIR)
			{
				if (!STR_END_IS_SEPARATED(szIntegrityDesPath))
					STR_END_ADD_SEPARATED(szIntegrityDesPath);

				if (!STR_END_IS_SEPARATED(szIntegritySrcPath))
					STR_END_ADD_SEPARATED(szIntegritySrcPath);

				// ��������ļ���
				bRet = MakeIntegrityPath(szIntegrityDesPath);
				if (!bRet)
					break;

				// �ݹ�
				bRet = CopyFileOfDir(szIntegritySrcPath, szIntegrityDesPath);
				if (!bRet)
					break;
			}
			else if (fileInfo.st_mode & S_IFREG)
			{
				// �����ļ�
				bRet = CopySingleFile(szIntegritySrcPath, szIntegrityDesPath);
				if (!bRet)
					break;
			}
		}
		closedir(pDir);		// �ر��ļ���
	}
#endif

	return bRet;
}

// �����ļ����������е��ļ���ָ��·��(Ŀ��·��������ڣ����򷵻�false),vtrProtected:�ܱ����ļ������������������е��ļ��в���
bool CFileAssist::CopyFileOfDir(const char * lpDirSrc, const char * lpDirDest, const std::vector<std::string> &vtrProtected)
{
	bool bRet = false;
	if(lpDirSrc == NULL || lpDirDest == NULL || !CFileAssist::IsDirectoryExist(lpDirSrc) || !CFileAssist::IsDirectoryExist(lpDirDest))
		return bRet;

#ifdef WIN32
	{
		CHAR szBuffer[MAX_PATH] = { 0 };
		if (STR_END_IS_SEPARATED(lpDirSrc))
			sprintf_s(szBuffer, sizeof(szBuffer), "%s%s", lpDirSrc, "*.*");
		else
			sprintf_s(szBuffer, sizeof(szBuffer), "%s%c%s", lpDirSrc, DIRECTORY_SEPARATED_CHAR, "*.*");

		bRet = true;
		WIN32_FIND_DATAA FindFileData;
		HANDLE hFind = FindFirstFileA(szBuffer, &FindFileData);
		if (hFind != INVALID_HANDLE_VALUE)
		{
			do
			{
				if (strcmp(FindFileData.cFileName, ".") != 0 && strcmp(FindFileData.cFileName, "..") != 0){

					// Դ����·��
					char szFileSrc[MAX_PATH] = {0};
					if (STR_END_IS_SEPARATED(lpDirSrc))
						sprintf_s(szFileSrc, sizeof(szFileSrc), "%s%s", lpDirSrc, FindFileData.cFileName);
					else
						sprintf_s(szFileSrc, sizeof(szFileSrc), "%s%c%s", lpDirSrc, DIRECTORY_SEPARATED_CHAR, FindFileData.cFileName);

					// Ŀ������·��
					char szFileDes[MAX_PATH] ={0};
					if (STR_END_IS_SEPARATED(lpDirDest))
						sprintf_s(szFileDes, sizeof(szFileDes), "%s%s", lpDirDest, FindFileData.cFileName);
					else
						sprintf_s(szFileDes, sizeof(szFileDes), "%s%c%s", lpDirDest, DIRECTORY_SEPARATED_CHAR, FindFileData.cFileName);

					if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY){

						// �ж��ǲ����ܱ����ļ��У�������Ǿ�ʲô��������
						std::vector<std::string>::const_iterator iter = find(vtrProtected.begin(), vtrProtected.end(), FindFileData.cFileName);
						if (iter != vtrProtected.end())
							continue;

						//  ����ļ��к����"//"
						STR_END_ADD_SEPARATED(szFileSrc);
						STR_END_ADD_SEPARATED(szFileDes);

						// �����ļ���,����Ŀ���ļ��У�Ȼ��ݹ鿽���ļ�
						if (!CFileAssist::IsDirectoryExist(szFileDes)){
							if (!CFileAssist::MakeDirectory(szFileDes))
							{
								bRet = false;
								break;
							}
						}
						
						bRet = CopyFileOfDir(szFileSrc, szFileDes);
						if (!bRet)
							break;
					}
					else{
						// �����ļ�
						bRet = CopySingleFile(szFileSrc, szFileDes);
						if (!bRet)
							break;
					}
				}
			} while (FindNextFileA(hFind, &FindFileData));
		}

		FindClose(hFind);
	}
#else
	{
		struct dirent	*lpDirent = NULL;
		DIR				*pDir = NULL;
		pDir=opendir(lpDirSrc);		// ���ļ���
		if (pDir == NULL)
			return bRet;

		bRet = true;
		while (NULL != (lpDirent=readdir(pDir)))
		{
			if (!strcmp(lpDirent->d_name, "..") || !strcmp(lpDirent->d_name, "."))
				continue;

			// ��������·��
			char szIntegritySrcPath[PATH_MAX] = {0};
			if (STR_END_IS_SEPARATED(lpDirSrc))
				sprintf(szIntegritySrcPath, "%s%s", lpDirSrc, lpDirent->d_name);
			else
				sprintf(szIntegritySrcPath, "%s%c%s", lpDirSrc, DIRECTORY_SEPARATED_CHAR, lpDirent->d_name);

			// ������Ŀ��·��
			char szIntegrityDesPath[MAX_PATH] = {0};
			if (STR_END_IS_SEPARATED(lpDirDest))
				sprintf(szIntegrityDesPath, "%s%s", lpDirDest, lpDirent->d_name);
			else
				sprintf(szIntegrityDesPath, "%s%c%s", lpDirDest, DIRECTORY_SEPARATED_CHAR, lpDirent->d_name);

			struct stat fileInfo = {0};
			if(0 != stat(szIntegritySrcPath, &fileInfo))
			{
				printf("stat error:%d, path:%s \n", errno, szIntegritySrcPath);	
				bRet = FALSE;
				break;
			}

			// 4 is dir, 8 is file
			if (fileInfo.st_mode & S_IFDIR)
			{
				// �ж��ǲ����ܱ����ļ��У�������Ǿ�ʲô��������
				std::vector<std::string>::const_iterator iter = find(vtrProtected.begin(), vtrProtected.end(), lpDirent->d_name);
				if (iter != vtrProtected.end())
					continue;

				if (!STR_END_IS_SEPARATED(szIntegrityDesPath))
					STR_END_ADD_SEPARATED(szIntegrityDesPath);

				if (!STR_END_IS_SEPARATED(szIntegritySrcPath))
					STR_END_ADD_SEPARATED(szIntegritySrcPath);

				// ��������ļ���
				bRet = MakeIntegrityPath(szIntegrityDesPath);
				if (!bRet)
					break;

				// �ݹ�
				bRet = CopyFileOfDir(szIntegritySrcPath, szIntegrityDesPath);
				if (!bRet)
					break;
			}
			else if (fileInfo.st_mode & S_IFREG)
			{
				// �����ļ�
				bRet = CopySingleFile(szIntegritySrcPath, szIntegrityDesPath);
				if (!bRet)
					break;
			}
		}
		closedir(pDir);		// �ر��ļ���
	}
#endif

	return bRet;
}

// �ļ�������(�Կյ��ļ���Ҳ��Ч)
bool CFileAssist::RenameFile(const char * lpSrcFile, const char * lpDesFile)
{
	return CFileGlobalFunc::RenameFile((CHAR*)lpSrcFile, (CHAR*)lpDesFile) == 0 ? true : false;
}

// �ļ���������
bool CFileAssist::RenameDir(const char * lpSrcDir, const char * lpDestDir)
{
	bool bRet = false;

	// ԴĿ¼�����ڻ�Ŀ��Ŀ¼�Ѵ��ڣ�������ʧ��
	if (!IsDirectoryExist(lpSrcDir) || IsDirectoryExist(lpDestDir))
		return false;

	// ������·��
	if (!MakeIntegrityPath(lpDestDir))
		return false;

	//�������ļ��У���������ļ��о����µ�·���д�����������ļ��͵����ļ������Ľӿڽ���·���޸ģ������������ɾ�����ļ���
#ifdef WIN32
	{
		CHAR szBuffer[MAX_PATH] = { 0 };
		strcpy(szBuffer, lpSrcDir);
		PATHSTR_END_ADDSTR(szBuffer, "*.*");	// ·��ĩβ����ַ������Զ���ȫ�ļ��зָ����

		bRet = true;
		WIN32_FIND_DATAA FindFileData;
		HANDLE hFind = FindFirstFileA(szBuffer, &FindFileData);
		if (hFind != INVALID_HANDLE_VALUE)
		{
			do
			{
				if (strcmp(FindFileData.cFileName, ".") != 0 && strcmp(FindFileData.cFileName, "..") != 0){

					// Դ����·��
					char szFileSrc[MAX_PATH] = {0};
					strcpy(szFileSrc, lpSrcDir);
					PATHSTR_END_ADDSTR(szFileSrc, FindFileData.cFileName);

					// Ŀ������·��
					char szFileDes[MAX_PATH] ={0};
					strcpy(szFileDes, lpDestDir);
					PATHSTR_END_ADDSTR(szFileDes, FindFileData.cFileName);

					if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
					{
						// �ݹ鴴���ļ���
						bRet = RenameDir(szFileSrc, szFileDes);
						if (!bRet)
							break;
					}
					else
					{
						// Ǩ���ļ�
						bRet = RenameFile(szFileSrc, szFileDes);
						if (!bRet)
							break;
					}
				}
			} while (FindNextFileA(hFind, &FindFileData));
		}

		FindClose(hFind);

		// ɾ��ԴĿ¼
		if (bRet)
			bRet = RemoveDirectoryA(lpSrcDir) ? true : false;
	}
#else
	{
		struct dirent	*lpDirent = NULL;
		DIR				*pDir = NULL;
		pDir=opendir(lpSrcDir);		// ���ļ���
		if (pDir == NULL)
			return bRet;

		bRet = true;
		while (NULL != (lpDirent=readdir(pDir)))
		{
			if (!strcmp(lpDirent->d_name, "..") || !strcmp(lpDirent->d_name, "."))
				continue;

			// ��������·��
			char szIntegritySrcPath[PATH_MAX] = {0};
			if (STR_END_IS_SEPARATED(lpSrcDir))
				sprintf(szIntegritySrcPath, "%s%s", lpSrcDir, lpDirent->d_name);
			else
				sprintf(szIntegritySrcPath, "%s%c%s", lpSrcDir, DIRECTORY_SEPARATED_CHAR, lpDirent->d_name);

			// ������Ŀ��·��
			char szIntegrityDesPath[MAX_PATH] = {0};
			if (STR_END_IS_SEPARATED(lpDestDir))
				sprintf(szIntegrityDesPath, "%s%s", lpDestDir, lpDirent->d_name);
			else
				sprintf(szIntegrityDesPath, "%s%c%s", lpDestDir, DIRECTORY_SEPARATED_CHAR, lpDirent->d_name);

			struct stat fileInfo = {0};
			if(0 != stat(szIntegritySrcPath, &fileInfo))
			{
				printf("stat error:%d, path:%s \n", errno, szIntegritySrcPath);	
				bRet = FALSE;
				break;
			}

			// 4 is dir, 8 is file
			if (fileInfo.st_mode & S_IFDIR)
			{
				// �ݹ鴴���ļ���
				bRet = RenameDir(szIntegritySrcPath, szIntegrityDesPath);
				if (!bRet)
					break;
			}
			else if (fileInfo.st_mode & S_IFREG)
			{
				// Ǩ���ļ�
				bRet = RenameFile(szIntegritySrcPath, szIntegrityDesPath);
				if (!bRet)
					break;
			}
		}
		closedir(pDir);		// �ر��ļ���

		// ɾ��ԴĿ¼
		if (bRet)
			bRet = RemoveDir(lpSrcDir) ?  true : false;
	}
#endif

	return bRet;
}

#ifndef WIN32
typedef void (*sighandler_t)(int);
int pox_system(const char *cmd_line)
{
	int ret = 0;
	sighandler_t old_handler;
	old_handler = signal(SIGCHLD, SIG_DFL);
	ret = system(cmd_line);
	signal(SIGCHLD, old_handler);
	return ret;
}
#endif

// �ļ���������(���������з�ʽ)
bool CFileAssist::RenameDirEx(const char * lpSrcDir, const char * lpDestDir)
{
	bool bRet = true;

	// ԴĿ¼�����ڻ�Ŀ��Ŀ¼�Ѵ��ڣ�������ʧ��
	if (!IsDirectoryExist(lpSrcDir) || IsDirectoryExist(lpDestDir))
		return false;
	
#ifdef WIN32
	{
		CHAR szNewPathForm[1024] = {0};
		_snprintf(szNewPathForm, sizeof(szNewPathForm), "%s", lpSrcDir);

		SHFILEOPSTRUCTA FileOp;
		ZeroMemory((void*)&FileOp,sizeof(SHFILEOPSTRUCT));

		FileOp.fFlags = FOF_NOCONFIRMATION | FOF_NOERRORUI | FOF_NO_UI ;
		FileOp.hNameMappings = NULL;
		FileOp.hwnd = NULL;
		FileOp.lpszProgressTitle = NULL;
		FileOp.pFrom = szNewPathForm;
		FileOp.pTo = lpDestDir;
		FileOp.wFunc = FO_RENAME;

		bRet = (SHFileOperationA(&FileOp) == 0 ? true : false);
	}
#else
	{
		char szCML[1024] = {0};
		_snprintf(szCML, sizeof(szCML), "mv %s %s", lpSrcDir, lpDestDir);
		int ret = pox_system(szCML);
		if(ret != 0)
			bRet = false;
	}
#endif
	
	return bRet;
}

// ����Ŀ���ļ��У����ҳ����й��˷�Χ���ļ�(ֻ����һ��Ŀ¼)
std::vector<std::string> CFileAssist::FindFileForDir(const char * lpDestDir, const char *lpszFilespec)
{
	std::vector<std::string> verFile;

	// ����ļ����Ƿ����
	if (NULL == lpDestDir || !CFileAssist::IsDirectoryExist(lpDestDir)){
		return verFile;
	}

	// ��������Ϊ��
	if(lpszFilespec == NULL || strlen(lpszFilespec) == 0)
		return verFile;

#ifdef WIN32
	{
		//���Ȳ���dir�з���Ҫ����ļ�
		long hFile = 0;
		_finddata_t fileinfo = {0};
		char szFilespec[MAX_PATH] = {0};
		if (STR_END_IS_SEPARATED(lpDestDir))
			sprintf(szFilespec, "%s%s", lpDestDir, lpszFilespec);
		else
			sprintf(szFilespec, "%s%c%s", lpDestDir, DIRECTORY_SEPARATED_CHAR, lpszFilespec);
		
		if ((hFile = _findfirst(szFilespec, &fileinfo)) != -1)
		{
			do
			{
				//����ǲ���Ŀ¼
				//�������,����д���
				if (!(fileinfo.attrib & _A_SUBDIR)){
					verFile.push_back(std::string(fileinfo.name));
				}
			} while (_findnext(hFile, &fileinfo) == 0);
			_findclose(hFile);
		}

		return verFile;
	}
#else
	{
		// �ж��Ƿ��*�ţ������*�ž���ģ�����ң�����*�ž�����ָ��ѯ
		bool bDim = false;
		std::vector<std::string> vtrMatching;	// ��������"*"Ϊ��ǽ�ȡ��ƥ���ַ���
		const char *pos = strchr(lpszFilespec, '*');
		if (pos != NULL)
		{
			vtrMatching = catString(lpszFilespec, '*');
			bDim = true;
		}

		struct dirent	*lpDirent = NULL;
		DIR				*pDir = NULL;
		pDir=opendir(lpDestDir);		// ���ļ���
		if (pDir == NULL)
			return verFile;

		while (NULL != (lpDirent=readdir(pDir)))
		{
			if (!strcmp(lpDirent->d_name, "..") || !strcmp(lpDirent->d_name, "."))
				continue;

			// ��������·��
			char szIntegrityPath[PATH_MAX] = {0};
			if (STR_END_IS_SEPARATED(lpDestDir))
				sprintf(szIntegrityPath, "%s%s", lpDestDir, lpDirent->d_name);
			else
				sprintf(szIntegrityPath, "%s%c%s", lpDestDir, DIRECTORY_SEPARATED_CHAR, lpDirent->d_name);

			struct stat fileInfo = {0};
			if(0 != stat(szIntegrityPath, &fileInfo))
			{
				printf("stat error:%d, path:%s \n", errno, szIntegrityPath);	
				continue;
			}

			// �ж��ǲ����ļ����ļ��Ŵ���
			if (fileInfo.st_mode & S_IFREG)
			{
				// �ж��Ƿ���Ҫģ������
				if (!bDim)
				{
					if (strcmp(lpDirent->d_name, lpszFilespec) == 0)	// ֱ��ƥ���ļ�
					{
						verFile.push_back(lpDirent->d_name);
						break;	// ֻ�ҵ�һ��ͬ����Ŀ���ļ�����������˳�
					}
				}
				else
				{
					// ���չ��˹����ж��Ƿ���ϲ��ҹ���(Ŀǰʱ��ؼ�ֻ���Ƚϼ򵥹��˹������������󣬺��������ʱ��������)
					bool bPass = true;
					for (std::vector<std::string>::iterator iter = vtrMatching.begin();
						iter != vtrMatching.end(); iter++)
					{
						//  ����ѳ������ļ����Ƿ���������Ӵ���ֻҪ��һ�������ڣ��ǾͲ����Ϲ��˹���
						if (strstr(lpDirent->d_name, (*iter).c_str()) == NULL)
						{
							bPass = false;
							break;
						}
					}
					if (bPass)
						verFile.push_back(lpDirent->d_name);	// ͨ��У�飬��������
				}
			}
		}
		closedir(pDir);		// �ر��ļ���
	}
#endif

	return verFile;

}

// ����Ŀ���ļ��У����ҳ����й��˷�Χ���ļ���(ֻ����һ��Ŀ¼)
std::vector<std::string> CFileAssist::FindDirForDir(const char * lpDestDir, const char *lpszFilespec)
{
	std::vector<std::string> verDir;

	// ����ļ����Ƿ����
	if (NULL == lpDestDir || !CFileAssist::IsDirectoryExist(lpDestDir)){
		return verDir;
	}

	// ��������Ϊ��
	if(lpszFilespec == NULL || strlen(lpszFilespec) == 0)
		return verDir;

#ifdef WIN32
	{
		//���Ȳ���dir�з���Ҫ����ļ�
		long hFile = 0;
		_finddata_t fileinfo = {0};
		char szFilespec[MAX_PATH] = {0};
		strcpy(szFilespec, lpDestDir);
		PATHSTR_END_ADDSTR(szFilespec, lpszFilespec);

		if ((hFile = _findfirst(szFilespec, &fileinfo)) != -1)
		{
			do
			{
				if (0 == _stricmp(fileinfo.name, ".") || 0 == _stricmp(fileinfo.name, ".."))
					continue;
				

				//����ǲ���Ŀ¼,�����,����д���
				if (fileinfo.attrib & _A_SUBDIR){
					verDir.push_back(std::string(fileinfo.name));
				}
			} while (_findnext(hFile, &fileinfo) == 0);
			_findclose(hFile);
		}

		return verDir;
	}
#else
	{
		// �ж��Ƿ��*�ţ������*�ž���ģ�����ң�����*�ž�����ָ��ѯ
		bool bDim = false;
		std::vector<std::string> vtrMatching;	// ��������"*"Ϊ��ǽ�ȡ��ƥ���ַ���
		const char *pos = strchr(lpszFilespec, '*');
		if (pos != NULL)
		{
			vtrMatching = catString(lpszFilespec, '*');
			bDim = true;
		}

		struct dirent	*lpDirent = NULL;
		DIR				*pDir = NULL;
		pDir=opendir(lpDestDir);		// ���ļ���
		if (pDir == NULL)
			return verDir;

		while (NULL != (lpDirent=readdir(pDir)))
		{
			if (!_stricmp(lpDirent->d_name, "..") || !_stricmp(lpDirent->d_name, "."))
				continue;

			// ��������·��
			char szIntegrityPath[PATH_MAX] = {0};
			if (STR_END_IS_SEPARATED(lpDestDir))
				sprintf(szIntegrityPath, "%s%s", lpDestDir, lpDirent->d_name);
			else
				sprintf(szIntegrityPath, "%s%c%s", lpDestDir, DIRECTORY_SEPARATED_CHAR, lpDirent->d_name);

			struct stat fileInfo = {0};
			if(0 != stat(szIntegrityPath, &fileInfo))
			{
				printf("stat error:%d, path:%s \n", errno, szIntegrityPath);	
				continue;
			}

			// �ж��ǲ���Ŀ¼��Ŀ¼�Ŵ���
			if (fileInfo.st_mode & S_IFDIR)
			{
				// �ж��Ƿ���Ҫģ������
				if (!bDim)
				{
					if (_stricmp(lpDirent->d_name, lpszFilespec) == 0)	// ֱ��ƥ���ļ�
					{
						verDir.push_back(lpDirent->d_name);
						break;	// ֻ�ҵ�һ��ͬ����Ŀ���ļ�����������˳�
					}
				}
				else
				{
					// ���չ��˹����ж��Ƿ���ϲ��ҹ���(Ŀǰʱ��ؼ�ֻ���Ƚϼ򵥹��˹������������󣬺��������ʱ��������)
					bool bPass = true;
					for (std::vector<std::string>::iterator iter = vtrMatching.begin();
						iter != vtrMatching.end(); iter++)
					{
						//  ����ѳ������ļ����Ƿ���������Ӵ���ֻҪ��һ�������ڣ��ǾͲ����Ϲ��˹���
						if (strstr(lpDirent->d_name, (*iter).c_str()) == NULL)
						{
							bPass = false;
							break;
						}
					}
					if (bPass)
						verDir.push_back(lpDirent->d_name);	// ͨ��У�飬��������
				}
			}
		}
		closedir(pDir);		// �ر��ļ���
	}
#endif

	return verDir;
}

// �����ַ���(���ַ������ԡ�mark��Ϊ��Ǽ���)
std::vector<std::string> CFileAssist::catString(const char * lpszSrc, char mark)
{
	std::vector<std::string> vtrRet;
	std::string strSrc(lpszSrc);

	size_t posScissorBegin = 0;
	size_t pos = strSrc.find(mark, 0);

	while (pos != std::string::npos)
	{
		vtrRet.push_back(strSrc.substr(posScissorBegin, pos - posScissorBegin));
		posScissorBegin = ++pos;
		pos = strSrc.find(mark, pos);
	}

	if (posScissorBegin < strSrc.size())
		vtrRet.push_back(strSrc.substr(posScissorBegin, std::string::npos));
	return vtrRet;
}

// ��ȡ�ļ��ĵ�ǰ·����or ��ȡ�ļ��е��ϼ�Ŀ¼��,ע�⣺ֱ���ַ���������������Ч���ж�,�ҷ��ص��ַ�����������ĩβ��·���ָ��
char * CFileAssist::getNearbyPath(const char * lpSrc, char * lpBuffer, DWORD dwSizeInBytes)
{
	if (lpSrc == NULL)
		return NULL;

	if (STR_END_IS_SEPARATED(lpSrc)){
		strncpy(lpBuffer, lpSrc, strlen(lpSrc) - 1);
	}
	else{
		strcpy(lpBuffer, lpSrc);
	}

	const char *pos = strrchr(lpBuffer, DIRECTORY_SEPARATED_CHAR);
	if (NULL != pos){
		lpBuffer[pos - lpBuffer + 1] = 0;
	}
	else{
		return NULL;
	}

	return lpBuffer;
}

// ��ȡ���һ��Ŀ¼(������ļ����ͻ�ȡ���ļ�����)
char * CFileAssist::getLastDirName(const char * lpSrc, char * lpBuffer, DWORD dwSizeBytes)
{
	if (lpSrc == NULL)
		return NULL;

	memset(lpBuffer, 0, dwSizeBytes);

	if (STR_END_IS_SEPARATED(lpSrc)){
		strncpy(lpBuffer, lpSrc, strlen(lpSrc) - 1);
	}
	else{
		strcpy(lpBuffer, lpSrc);
	}

	const char *pos = strrchr(lpBuffer, DIRECTORY_SEPARATED_CHAR);
	if (NULL != pos){
		memmove(lpBuffer, pos + 1, strlen(pos) - 1);
		lpBuffer[strlen(pos) - 1] = 0;
	}

	return lpBuffer;

}

// ���·��ת����·��(�Դ�..���ַ�����������ƴ��)
char * CFileAssist::relativePath2AbsolutePath(const char * lpSrc, char * lpBuffer, DWORD dwSizeBytes)
{
	// �����ַ����е� ".." , ����ҵ��ͽ�ȡ '..' �����ǰ��һ��Ŀ¼
	std::vector<std::string> verTemp = catString(lpSrc, DIRECTORY_SEPARATED_CHAR);
	std::vector<std::string>::iterator iter = find(verTemp.begin(), verTemp.end(), "..");
	while (iter != verTemp.end())
	{
		// �����..���Ѿ������ַ������ʼλ�ã���ô���޷�ת����
		if (iter == verTemp.begin())
			return NULL;

		verTemp.erase(iter--);		// ɾ�� '..' ����
		verTemp.erase(iter);		// ɾ�� '..' ǰ���һ��·��

		iter = find(verTemp.begin(), verTemp.end(), "..");
	}

#ifndef WIN32
	STR_END_ADD_SEPARATED(lpBuffer);
#endif

	for (iter = verTemp.begin(); iter != verTemp.end(); iter++)
	{
		if (strlen(lpBuffer) > 1)
			STR_END_ADD_SEPARATED(lpBuffer);
		
		strcat(lpBuffer, iter->c_str());
	}

	// Ϊ�˱����������һ���ԣ�ҲҪ��������ʱ���ַ����Ƿ����ļ��зָ������������ĩβ���ַ�
	if (STR_END_IS_SEPARATED(lpSrc))
		STR_END_ADD_SEPARATED(lpBuffer);

	return lpBuffer;
}
