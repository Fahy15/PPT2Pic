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

// 判断文件是否存在
BOOL CFileAssist::IsFileExist(const char * lpFilePath)
{
#if defined(WIN32)
	return CFileGlobalFunc::IsFileExist((CHAR*)lpFilePath, FALSE);
#else
	return CFileGlobalFunc::IsFileExist(lpFilePath, TRUE);
#endif
}

// 判断目录是否存在
BOOL CFileAssist::IsDirectoryExist(const char * lpPathName)
{
#if defined(WIN32)
	return CFileGlobalFunc::IsDirectoryExist((CHAR*)lpPathName, FALSE);
#else
	return CFileGlobalFunc::IsDirectoryExist(lpPathName, TRUE);
#endif
}

// 删除文件
void CFileAssist::RemoveFile(const char * lpFilePath)
{
	CFileGlobalFunc::RemoveFile((CHAR*)lpFilePath);
}

// 删除文件夹(删除非空文件夹), bOnlyInternal == true 则只清空内部，否则连同文件夹一并删除
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
						RemoveDir(szDir, FALSE);	// 递归
					}
					else
						RemoveFile(szBuffer);
				}
			} while (FindNextFileA(hFind, &FindFileData));
		}

		FindClose(hFind);

		// 判断是否是仅仅清理文件夹内部
		if (!bOnlyInternal)
			bRet = RemoveDirectoryA(lpPathName);
	}

#else
	{
		struct dirent	*lpDirent = NULL;
		DIR				*pDir = NULL;
		pDir=opendir(lpPathName);		// 打开文件夹
		if (pDir == NULL)
			return bRet;

		bRet = true;
		while (NULL != (lpDirent=readdir(pDir)))
		{
			if (!strcmp(lpDirent->d_name, "..") || !strcmp(lpDirent->d_name, "."))
				continue;

			// 保存完整路径
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
				// 递归删除文件夹
				bRet = RemoveDir(szIntegrityPath, FALSE);	
				if (!bRet)
					break;
			}
			else if (fileInfo.st_mode & S_IFREG)
			{
				// 删除文件
				bRet  = unlink(szIntegrityPath) == 0 ? TRUE : FALSE;
				if (!bRet)
					break;
			}
		}
		closedir(pDir);		// 关闭文件夹

		// 判断是否是仅仅清理文件夹内部
		if (!bOnlyInternal)
			bRet = rmdir(lpPathName) == 0 ? TRUE : FALSE;
	}

#endif
	return bRet;
}

// 删除文件夹下面的所有文件，受保护的文件夹除外, vtrProtected:受保护文件夹名单，不对名单中的文件夹操作
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
						
						// 判断是不是受保护文件夹，如果是那就什么都不用做
						std::vector<std::string>::const_iterator iter = find(vtrProtected.begin(), vtrProtected.end(), FindFileData.cFileName);
						if (iter != vtrProtected.end())
							continue;
						
						bRet = RemoveDir(szBuffer);
						if (!bRet)
							break;	// 删除失败,跳出循环
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
		pDir=opendir(lpPathName);		// 打开文件夹
		if (pDir == NULL)
			return bRet;

		bRet = true;
		while (NULL != (lpDirent=readdir(pDir)))
		{
			if (!strcmp(lpDirent->d_name, "..") || !strcmp(lpDirent->d_name, "."))
				continue;

			// 保存完整路径
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
				// 判断是不是受保护文件夹，如果是那就什么都不用做
				std::vector<std::string>::const_iterator iter = find(vtrProtected.begin(), vtrProtected.end(), lpDirent->d_name);
				if (iter != vtrProtected.end())
					continue;

				// 删除文件夹
				bRet = RemoveDir(szIntegrityPath, FALSE);
				if (!bRet)
					break;
			}
			else if (fileInfo.st_mode & S_IFREG)
			{
				// 删除文件
				bRet  = unlink(szIntegrityPath) == 0 ? TRUE : FALSE;
				if (!bRet)
					break;
			}
		}
		closedir(pDir);		// 关闭文件夹
	}

#endif
	return bRet;
}

// 创建文件夹
BOOL CFileAssist::MakeDirectory(const char * lpPathName)
{
	//　如果是相对的路径，还需要转换成绝对路径
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

// 创建一条路径（从路径的第一级起判断是否存在，不存在就创建）
bool CFileAssist::MakeIntegrityPath(const char* lpIntegrityPath)
{
	if (lpIntegrityPath == NULL)
		return false;

	// 如果不是以 文件夹分隔符('\' 或 '/')结尾的就添加上分隔符
	char szBuffer[MAX_PATH] = {0};
	strcpy(szBuffer, lpIntegrityPath);
	STR_END_ADD_SEPARATED(szBuffer);
	
	CFileGlobalFunc::MakeSureDirectory(szBuffer);
	return true;
}

// 拷贝文件
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

		// 开始拷贝
		bool bError = false;
		char buf[1024] = {0};
		int len = fread(buf, 1, 1024, fileSrc);
		while(len != 0)
		{
			if (len != fwrite(buf, 1, len, fileDes))
			{
				bError = true;
				break;	// 出错
			}

			len = fread(buf,1, 1024, fileSrc);
		}

		fclose(fileSrc);
		fclose(fileDes);

		if (bError)
			RemoveFile(lpFileDest);	// 拷贝失败，将残缺文件删除
		else
			bRet = true;
	} while (false);

	return bRet;
}

// 拷贝文件夹下面所有的文件到指定路径(目标路径必需存在)
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

					// 源完整路径
					char szFileSrc[MAX_PATH] = {0};
					if (STR_END_IS_SEPARATED(lpDirSrc))
						sprintf_s(szFileSrc, sizeof(szFileSrc), "%s%s", lpDirSrc, FindFileData.cFileName);
					else
						sprintf_s(szFileSrc, sizeof(szFileSrc), "%s%c%s", lpDirSrc, DIRECTORY_SEPARATED_CHAR, FindFileData.cFileName);

					// 目标完整路径
					char szFileDes[MAX_PATH] ={0};
					if (STR_END_IS_SEPARATED(lpDirDest))
						sprintf_s(szFileDes, sizeof(szFileDes), "%s%s", lpDirDest, FindFileData.cFileName);
					else
						sprintf_s(szFileDes, sizeof(szFileDes), "%s%c%s", lpDirDest, DIRECTORY_SEPARATED_CHAR, FindFileData.cFileName);

					if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY){

						//  添加文件夹后面的"//"
						STR_END_ADD_SEPARATED(szFileSrc);
						STR_END_ADD_SEPARATED(szFileDes);
						
						// 复制文件夹,创建目标文件夹，然后递归拷贝文件
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
						// 复制文件
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
		pDir=opendir(lpDirSrc);		// 打开文件夹
		if (pDir == NULL)
			return bRet;

		bRet = true;
		while (NULL != (lpDirent=readdir(pDir)))
		{
			if (!strcmp(lpDirent->d_name, "..") || !strcmp(lpDirent->d_name, "."))
				continue;

			// 保存完整路径
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

			// 完整的目标路径
			char szIntegrityDesPath[MAX_PATH] = {0};
			if (STR_END_IS_SEPARATED(lpDirDest))
				sprintf(szIntegrityDesPath, "%s%s", lpDirDest, lpDirent->d_name);
			else
				sprintf(szIntegrityDesPath, "%s%c%s", lpDirDest, DIRECTORY_SEPARATED_CHAR, lpDirent->d_name);

			// 判断目录还是普通文件
			if (fileInfo.st_mode & S_IFDIR)
			{
				if (!STR_END_IS_SEPARATED(szIntegrityDesPath))
					STR_END_ADD_SEPARATED(szIntegrityDesPath);

				if (!STR_END_IS_SEPARATED(szIntegritySrcPath))
					STR_END_ADD_SEPARATED(szIntegritySrcPath);

				// 创建这个文件夹
				bRet = MakeIntegrityPath(szIntegrityDesPath);
				if (!bRet)
					break;

				// 递归
				bRet = CopyFileOfDir(szIntegritySrcPath, szIntegrityDesPath);
				if (!bRet)
					break;
			}
			else if (fileInfo.st_mode & S_IFREG)
			{
				// 拷贝文件
				bRet = CopySingleFile(szIntegritySrcPath, szIntegrityDesPath);
				if (!bRet)
					break;
			}
		}
		closedir(pDir);		// 关闭文件夹
	}
#endif

	return bRet;
}

// 拷贝文件夹下面所有的文件到指定路径(目标路径必需存在，否则返回false),vtrProtected:受保护文件夹名单，不对名单中的文件夹操作
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

					// 源完整路径
					char szFileSrc[MAX_PATH] = {0};
					if (STR_END_IS_SEPARATED(lpDirSrc))
						sprintf_s(szFileSrc, sizeof(szFileSrc), "%s%s", lpDirSrc, FindFileData.cFileName);
					else
						sprintf_s(szFileSrc, sizeof(szFileSrc), "%s%c%s", lpDirSrc, DIRECTORY_SEPARATED_CHAR, FindFileData.cFileName);

					// 目标完整路径
					char szFileDes[MAX_PATH] ={0};
					if (STR_END_IS_SEPARATED(lpDirDest))
						sprintf_s(szFileDes, sizeof(szFileDes), "%s%s", lpDirDest, FindFileData.cFileName);
					else
						sprintf_s(szFileDes, sizeof(szFileDes), "%s%c%s", lpDirDest, DIRECTORY_SEPARATED_CHAR, FindFileData.cFileName);

					if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY){

						// 判断是不是受保护文件夹，如果是那就什么都不用做
						std::vector<std::string>::const_iterator iter = find(vtrProtected.begin(), vtrProtected.end(), FindFileData.cFileName);
						if (iter != vtrProtected.end())
							continue;

						//  添加文件夹后面的"//"
						STR_END_ADD_SEPARATED(szFileSrc);
						STR_END_ADD_SEPARATED(szFileDes);

						// 复制文件夹,创建目标文件夹，然后递归拷贝文件
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
						// 复制文件
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
		pDir=opendir(lpDirSrc);		// 打开文件夹
		if (pDir == NULL)
			return bRet;

		bRet = true;
		while (NULL != (lpDirent=readdir(pDir)))
		{
			if (!strcmp(lpDirent->d_name, "..") || !strcmp(lpDirent->d_name, "."))
				continue;

			// 保存完整路径
			char szIntegritySrcPath[PATH_MAX] = {0};
			if (STR_END_IS_SEPARATED(lpDirSrc))
				sprintf(szIntegritySrcPath, "%s%s", lpDirSrc, lpDirent->d_name);
			else
				sprintf(szIntegritySrcPath, "%s%c%s", lpDirSrc, DIRECTORY_SEPARATED_CHAR, lpDirent->d_name);

			// 完整的目标路径
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
				// 判断是不是受保护文件夹，如果是那就什么都不用做
				std::vector<std::string>::const_iterator iter = find(vtrProtected.begin(), vtrProtected.end(), lpDirent->d_name);
				if (iter != vtrProtected.end())
					continue;

				if (!STR_END_IS_SEPARATED(szIntegrityDesPath))
					STR_END_ADD_SEPARATED(szIntegrityDesPath);

				if (!STR_END_IS_SEPARATED(szIntegritySrcPath))
					STR_END_ADD_SEPARATED(szIntegritySrcPath);

				// 创建这个文件夹
				bRet = MakeIntegrityPath(szIntegrityDesPath);
				if (!bRet)
					break;

				// 递归
				bRet = CopyFileOfDir(szIntegritySrcPath, szIntegrityDesPath);
				if (!bRet)
					break;
			}
			else if (fileInfo.st_mode & S_IFREG)
			{
				// 拷贝文件
				bRet = CopySingleFile(szIntegritySrcPath, szIntegrityDesPath);
				if (!bRet)
					break;
			}
		}
		closedir(pDir);		// 关闭文件夹
	}
#endif

	return bRet;
}

// 文件重命名(对空的文件夹也有效)
bool CFileAssist::RenameFile(const char * lpSrcFile, const char * lpDesFile)
{
	return CFileGlobalFunc::RenameFile((CHAR*)lpSrcFile, (CHAR*)lpDesFile) == 0 ? true : false;
}

// 文件夹重命名
bool CFileAssist::RenameDir(const char * lpSrcDir, const char * lpDestDir)
{
	bool bRet = false;

	// 源目录不存在或目标目录已存在，都返回失败
	if (!IsDirectoryExist(lpSrcDir) || IsDirectoryExist(lpDestDir))
		return false;

	// 创建新路径
	if (!MakeIntegrityPath(lpDestDir))
		return false;

	//　遍历文件夹，如果是子文件夹就在新的路径中创建，如果是文件就调用文件改名的接口进行路径修改，操作操作完后删除老文件夹
#ifdef WIN32
	{
		CHAR szBuffer[MAX_PATH] = { 0 };
		strcpy(szBuffer, lpSrcDir);
		PATHSTR_END_ADDSTR(szBuffer, "*.*");	// 路径末尾添加字符串（自动补全文件夹分割符）

		bRet = true;
		WIN32_FIND_DATAA FindFileData;
		HANDLE hFind = FindFirstFileA(szBuffer, &FindFileData);
		if (hFind != INVALID_HANDLE_VALUE)
		{
			do
			{
				if (strcmp(FindFileData.cFileName, ".") != 0 && strcmp(FindFileData.cFileName, "..") != 0){

					// 源完整路径
					char szFileSrc[MAX_PATH] = {0};
					strcpy(szFileSrc, lpSrcDir);
					PATHSTR_END_ADDSTR(szFileSrc, FindFileData.cFileName);

					// 目标完整路径
					char szFileDes[MAX_PATH] ={0};
					strcpy(szFileDes, lpDestDir);
					PATHSTR_END_ADDSTR(szFileDes, FindFileData.cFileName);

					if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
					{
						// 递归创建文件夹
						bRet = RenameDir(szFileSrc, szFileDes);
						if (!bRet)
							break;
					}
					else
					{
						// 迁移文件
						bRet = RenameFile(szFileSrc, szFileDes);
						if (!bRet)
							break;
					}
				}
			} while (FindNextFileA(hFind, &FindFileData));
		}

		FindClose(hFind);

		// 删除源目录
		if (bRet)
			bRet = RemoveDirectoryA(lpSrcDir) ? true : false;
	}
#else
	{
		struct dirent	*lpDirent = NULL;
		DIR				*pDir = NULL;
		pDir=opendir(lpSrcDir);		// 打开文件夹
		if (pDir == NULL)
			return bRet;

		bRet = true;
		while (NULL != (lpDirent=readdir(pDir)))
		{
			if (!strcmp(lpDirent->d_name, "..") || !strcmp(lpDirent->d_name, "."))
				continue;

			// 保存完整路径
			char szIntegritySrcPath[PATH_MAX] = {0};
			if (STR_END_IS_SEPARATED(lpSrcDir))
				sprintf(szIntegritySrcPath, "%s%s", lpSrcDir, lpDirent->d_name);
			else
				sprintf(szIntegritySrcPath, "%s%c%s", lpSrcDir, DIRECTORY_SEPARATED_CHAR, lpDirent->d_name);

			// 完整的目标路径
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
				// 递归创建文件夹
				bRet = RenameDir(szIntegritySrcPath, szIntegrityDesPath);
				if (!bRet)
					break;
			}
			else if (fileInfo.st_mode & S_IFREG)
			{
				// 迁移文件
				bRet = RenameFile(szIntegritySrcPath, szIntegrityDesPath);
				if (!bRet)
					break;
			}
		}
		closedir(pDir);		// 关闭文件夹

		// 删除源目录
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

// 文件夹重命名(调用命令行方式)
bool CFileAssist::RenameDirEx(const char * lpSrcDir, const char * lpDestDir)
{
	bool bRet = true;

	// 源目录不存在或目标目录已存在，都返回失败
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

// 遍历目标文件夹，查找出所有过滤范围内文件(只查找一层目录)
std::vector<std::string> CFileAssist::FindFileForDir(const char * lpDestDir, const char *lpszFilespec)
{
	std::vector<std::string> verFile;

	// 检查文件夹是否存在
	if (NULL == lpDestDir || !CFileAssist::IsDirectoryExist(lpDestDir)){
		return verFile;
	}

	// 过滤条件为空
	if(lpszFilespec == NULL || strlen(lpszFilespec) == 0)
		return verFile;

#ifdef WIN32
	{
		//首先查找dir中符合要求的文件
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
				//检查是不是目录
				//如果不是,则进行处理
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
		// 判断是否带*号，如果带*号就是模糊查找，不带*号就是特指查询
		bool bDim = false;
		std::vector<std::string> vtrMatching;	// 保存下以"*"为标记截取的匹配字符串
		const char *pos = strchr(lpszFilespec, '*');
		if (pos != NULL)
		{
			vtrMatching = catString(lpszFilespec, '*');
			bDim = true;
		}

		struct dirent	*lpDirent = NULL;
		DIR				*pDir = NULL;
		pDir=opendir(lpDestDir);		// 打开文件夹
		if (pDir == NULL)
			return verFile;

		while (NULL != (lpDirent=readdir(pDir)))
		{
			if (!strcmp(lpDirent->d_name, "..") || !strcmp(lpDirent->d_name, "."))
				continue;

			// 保存完整路径
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

			// 判断是不是文件，文件才处理
			if (fileInfo.st_mode & S_IFREG)
			{
				// 判断是否需要模糊查找
				if (!bDim)
				{
					if (strcmp(lpDirent->d_name, lpszFilespec) == 0)	// 直接匹配文件
					{
						verFile.push_back(lpDirent->d_name);
						break;	// 只找到一个同名的目标文件就完成任务，退出
					}
				}
				else
				{
					// 按照过滤规则判断是否符合查找规则(目前时间关键只做比较简单过滤规则先满足需求，后面如果有时间再完善)
					bool bPass = true;
					for (std::vector<std::string>::iterator iter = vtrMatching.begin();
						iter != vtrMatching.end(); iter++)
					{
						//  检查搜出来的文件名是否包含过滤子串，只要有一个不存在，那就不符合过滤规则
						if (strstr(lpDirent->d_name, (*iter).c_str()) == NULL)
						{
							bPass = false;
							break;
						}
					}
					if (bPass)
						verFile.push_back(lpDirent->d_name);	// 通过校验，符合条件
				}
			}
		}
		closedir(pDir);		// 关闭文件夹
	}
#endif

	return verFile;

}

// 遍历目标文件夹，查找出所有过滤范围内文件夹(只查找一层目录)
std::vector<std::string> CFileAssist::FindDirForDir(const char * lpDestDir, const char *lpszFilespec)
{
	std::vector<std::string> verDir;

	// 检查文件夹是否存在
	if (NULL == lpDestDir || !CFileAssist::IsDirectoryExist(lpDestDir)){
		return verDir;
	}

	// 过滤条件为空
	if(lpszFilespec == NULL || strlen(lpszFilespec) == 0)
		return verDir;

#ifdef WIN32
	{
		//首先查找dir中符合要求的文件
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
				

				//检查是不是目录,如果是,则进行处理
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
		// 判断是否带*号，如果带*号就是模糊查找，不带*号就是特指查询
		bool bDim = false;
		std::vector<std::string> vtrMatching;	// 保存下以"*"为标记截取的匹配字符串
		const char *pos = strchr(lpszFilespec, '*');
		if (pos != NULL)
		{
			vtrMatching = catString(lpszFilespec, '*');
			bDim = true;
		}

		struct dirent	*lpDirent = NULL;
		DIR				*pDir = NULL;
		pDir=opendir(lpDestDir);		// 打开文件夹
		if (pDir == NULL)
			return verDir;

		while (NULL != (lpDirent=readdir(pDir)))
		{
			if (!_stricmp(lpDirent->d_name, "..") || !_stricmp(lpDirent->d_name, "."))
				continue;

			// 保存完整路径
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

			// 判断是不是目录，目录才处理
			if (fileInfo.st_mode & S_IFDIR)
			{
				// 判断是否需要模糊查找
				if (!bDim)
				{
					if (_stricmp(lpDirent->d_name, lpszFilespec) == 0)	// 直接匹配文件
					{
						verDir.push_back(lpDirent->d_name);
						break;	// 只找到一个同名的目标文件就完成任务，退出
					}
				}
				else
				{
					// 按照过滤规则判断是否符合查找规则(目前时间关键只做比较简单过滤规则先满足需求，后面如果有时间再完善)
					bool bPass = true;
					for (std::vector<std::string>::iterator iter = vtrMatching.begin();
						iter != vtrMatching.end(); iter++)
					{
						//  检查搜出来的文件名是否包含过滤子串，只要有一个不存在，那就不符合过滤规则
						if (strstr(lpDirent->d_name, (*iter).c_str()) == NULL)
						{
							bPass = false;
							break;
						}
					}
					if (bPass)
						verDir.push_back(lpDirent->d_name);	// 通过校验，符合条件
				}
			}
		}
		closedir(pDir);		// 关闭文件夹
	}
#endif

	return verDir;
}

// 剪切字符串(从字符串中以“mark”为标记剪切)
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

// 获取文件的当前路径（or 获取文件夹的上级目录）,注意：直接字符串作处理，不做有效性判断,且返回的字符串不包括最末尾的路径分割符
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

// 获取最后一级目录(如果是文件，就获取到文件名称)
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

// 相对路径转绝对路径(对带..的字符串进行重新拼接)
char * CFileAssist::relativePath2AbsolutePath(const char * lpSrc, char * lpBuffer, DWORD dwSizeBytes)
{
	// 查找字符串中的 ".." , 如果找到就截取 '..' 本身和前面一层目录
	std::vector<std::string> verTemp = catString(lpSrc, DIRECTORY_SEPARATED_CHAR);
	std::vector<std::string>::iterator iter = find(verTemp.begin(), verTemp.end(), "..");
	while (iter != verTemp.end())
	{
		// 如果“..”已经是在字符串的最开始位置，那么就无法转换了
		if (iter == verTemp.begin())
			return NULL;

		verTemp.erase(iter--);		// 删除 '..' 本身
		verTemp.erase(iter);		// 删除 '..' 前面的一层路径

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

	// 为了保持与输入的一致性，也要根据输入时的字符串是否以文件夹分隔符结束来添加末尾的字符
	if (STR_END_IS_SEPARATED(lpSrc))
		STR_END_ADD_SEPARATED(lpBuffer);

	return lpBuffer;
}
