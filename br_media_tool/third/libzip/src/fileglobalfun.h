#ifndef _GLOBALFUN_INCLUDE__
#define _GLOBALFUN_INCLUDE__

#include "osdef.h"
#include <vector>
#include <string>

/**
 *	全局操作函数
 */

class CFileGlobalFunc
{
	CFileGlobalFunc();
	virtual ~CFileGlobalFunc();

public:
	// 转换文件名格式为操作系统支持的格式
	static CHAR* ConvertFileNameOSSupport(CHAR* szSrcFileName, BOOL bSrcUTF8, CHAR* szTarFileName, DWORD dwSize);
	// 判断文件是否存在
	static BOOL IsFileExist(CHAR* lpFilePath, BOOL bUTF8Encode = FALSE);

	// 判断目录是否存在
	static BOOL IsDirectoryExist(CHAR* lpPathName, BOOL bUTF8Encode = FALSE);

	// 获取文件长度
	static DWORD GetFileLength(CHAR* lpFilePath, BOOL bUTF8Encode=FALSE);
	// 获取文件长度(64bit)
	static INT64 GetFileLength64(CHAR* lpFilePath, BOOL bUTF8Encode = FALSE);

	// 删除文件
	static void RemoveFile(CHAR* lpFilePath, BOOL bUTF8Encode = FALSE);

	// 文件改名
	static int RenameFile(CHAR* lpSrcFile, CHAR* lpDesFile, BOOL bUTF8Encode = FALSE);

	// 拷贝文件
	static BOOL CopyFile(CHAR* lpSrcFile, CHAR* lpDesFile, BOOL bUTF8Encode = FALSE);

	// 创建一个空文件，即长度为零字节的文件
	static BOOL CreateEmptyFile(CHAR* lpFilePath, BOOL bUTF8Encode = FALSE);

	// 组件注册
	static void RegisterComponent(CHAR* lpFileName);

	// 判断一个组件是否需要注册
	static BOOL IsFileNeedRegister(CHAR* strFilePath);

	// 计算一段缓冲区的CRC16校验值
	static unsigned short crc16(unsigned char *buff, int bufflen);

	// 获取文件版本信息
	static BOOL GetLocalFileVersion(CHAR* szPathName,CHAR* strVersion, DWORD dwSize);

	// 确保路径存在，如果目录不存在就创建目录,可以创建多层次的目录
	static void MakeSureDirectory(CHAR* lpszDirName, BOOL bUTF8Encode = FALSE);

	// 调整磁盘，使得路径可用
	static BOOL AdjustDiskAndCreateDirectory(CHAR* lpPathName, DWORD dwSize);

	// 文件AES加解密
	static DWORD FileAesEncDec(const CHAR* lpPassword, BOOL bEncrypt, const CHAR* lpSrcFile, const CHAR* lpDesFile, BOOL bUTF8Encode=FALSE);

#if defined(WIN32)
	// 从注册表中读取字符串类型数据
	static BOOL RegReadStringValue(CHAR* lpKeyName, CHAR* lpValueName, BYTE* lpOutBuf, DWORD& dwBufSize);
	// 将字符串类型数据写入到注册表中
	static BOOL RegWriteStringValue(CHAR* lpKeyName, CHAR* lpValueName, BYTE* lpStrBuf, DWORD dwBufSize);
	// 从注册表中读取DWORD类型数据
	static BOOL RegReadDWordValue(CHAR* lpKeyName, CHAR* lpValueName, DWORD& dwValue);
	// 将DWORD类型数据写入到注册表中
	static BOOL RegWriteDWordValue(CHAR* lpKeyName, CHAR* lpValueName, DWORD dwValue);
#endif

	// 枚举目录下单级所有文件, 失败返回非0值
	static DWORD EnumFiles(CHAR* lpFolderPath, std::vector<std::string>& vtrFiles);
};





#endif//_GLOBALFUN_INCLUDE__
