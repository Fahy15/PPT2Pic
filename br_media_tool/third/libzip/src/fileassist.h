#ifndef _FILE_ASSIST_H_
#define _FILE_ASSIST_H_

#include <fileglobalfun.h>
#include <vector>
#include <string>
#include <algorithm>	// STL算法头文件

// 判断字符串是否是“目录分割符结尾”
#define	STR_END_IS_SEPARATED(lpPath) (strlen(lpPath)>0 && lpPath[strlen(lpPath)-1]==DIRECTORY_SEPARATED_CHAR)

// 如果字符串是不以文件夹分割符结尾的就添加上
#define STR_END_ADD_SEPARATED(lpStr) {				\
			if (!STR_END_IS_SEPARATED(lpStr)){		\
				char szSeparated[10] = { 0 };		\
				_snprintf(szSeparated, sizeof(szSeparated), "%c", DIRECTORY_SEPARATED_CHAR);	\
				strcat(lpStr, szSeparated);		\
			}										\
		}

// 路径末尾添加字符串（自动补全文件夹分割符）
#define PATHSTR_END_ADDSTR(lpPath, lpNeedAddTheStr) {	\
			STR_END_ADD_SEPARATED(lpPath);				\
			strcat(lpPath, lpNeedAddTheStr);			\
		}					
		

class CFileAssist
{
public:
	CFileAssist();
	virtual~CFileAssist();

public:
	// 判断文件是否存在
	static BOOL IsFileExist(const char * lpFilePath);
	// 判断目录是否存在
	static BOOL IsDirectoryExist(const char * lpPathName);

	// 删除文件
	static void RemoveFile(const char * lpFilePath);
	// 删除文件夹(删除非空文件夹), bOnlyInternal == true 则只清空内部，否则连同文件夹一并删除
	static BOOL RemoveDir(const char * lpPathName, BOOL bOnlyInternal = FALSE);
	// 删除文件夹下面的所有文件，受保护的文件夹除外, vtrProtected:受保护文件夹名单，不对名单中的文件夹操作
	static BOOL RemoveFileOfDir(const char * lpPathName, const std::vector<std::string> &vtrProtected);

	// 创建文件夹
	static BOOL MakeDirectory(const char * lpPathName);
	// 创建一条路径（从路径的第一级起判断是否存在，不存在就创建）
	static bool MakeIntegrityPath(const char* lpIntegrityPath);

	// 拷贝文件
	static bool CopySingleFile(const char * lpFileSrc, const char * lpFileDest);
	// 拷贝文件夹下面所有的文件到指定路径(目标路径必需存在，否则返回false)
	static bool CopyFileOfDir(const char * lpDirSrc, const char * lpDirDest);
	// 拷贝文件夹下面所有的文件到指定路径(目标路径必需存在，否则返回false), vtrProtected:受保护文件夹名单，不对名单中的文件夹操作
	static bool CopyFileOfDir(const char * lpDirSrc, const char * lpDirDest, const std::vector<std::string> &vtrProtected);

	// 文件重命名(对空的文件夹也有效)
	static bool RenameFile(const char * lpSrcFile, const char * lpDesFile);
	// 文件夹重命名
	static bool RenameDir(const char * lpSrcDir, const char * lpDestDir);
	static bool RenameDirEx(const char * lpSrcDir, const char * lpDestDir);	// 调用命令行方式

	// 遍历目标文件夹，查找出所有过滤范围内文件(只查找一层目录)
	static std::vector<std::string> FindFileForDir(const char * lpDestDir, const char *lpszFilespec);
	// 遍历目标文件夹，查找出所有过滤范围内文件夹(只查找一层目录)
	static std::vector<std::string> FindDirForDir(const char * lpDestDir, const char *lpszFilespec);

public:	// 辅助函数
	// 剪切字符串(从字符串中以“mark”为标记剪切)
	static std::vector<std::string> catString(const char * lpszSrc, char mark);
	// 获取文件的当前路径（or 获取文件夹的上级目录）,注意：直接字符串作处理，不做有效性判断
	static char * getNearbyPath(const char * lpSrc, char * lpBuffer, DWORD dwSizeInBytes);
	// 获取最后一级目录(如果是文件，就获取到文件名称)
	static char * getLastDirName(const char * lpSrc, char * lpBuffer, DWORD dwSizeBytes);
	// 相对路径转绝对路径(对带..的字符串进行重新拼接)
	static char * relativePath2AbsolutePath(const char * lpSrc, char * lpBuffer, DWORD dwSizeBytes);

};


#endif	//_FILE_ASSIST_H_