#ifndef _GLOBALFUN_INCLUDE__
#define _GLOBALFUN_INCLUDE__

#include "osdef.h"
#include <vector>
#include <string>

/**
 *	ȫ�ֲ�������
 */

class CFileGlobalFunc
{
	CFileGlobalFunc();
	virtual ~CFileGlobalFunc();

public:
	// ת���ļ�����ʽΪ����ϵͳ֧�ֵĸ�ʽ
	static CHAR* ConvertFileNameOSSupport(CHAR* szSrcFileName, BOOL bSrcUTF8, CHAR* szTarFileName, DWORD dwSize);
	// �ж��ļ��Ƿ����
	static BOOL IsFileExist(CHAR* lpFilePath, BOOL bUTF8Encode = FALSE);

	// �ж�Ŀ¼�Ƿ����
	static BOOL IsDirectoryExist(CHAR* lpPathName, BOOL bUTF8Encode = FALSE);

	// ��ȡ�ļ�����
	static DWORD GetFileLength(CHAR* lpFilePath, BOOL bUTF8Encode=FALSE);
	// ��ȡ�ļ�����(64bit)
	static INT64 GetFileLength64(CHAR* lpFilePath, BOOL bUTF8Encode = FALSE);

	// ɾ���ļ�
	static void RemoveFile(CHAR* lpFilePath, BOOL bUTF8Encode = FALSE);

	// �ļ�����
	static int RenameFile(CHAR* lpSrcFile, CHAR* lpDesFile, BOOL bUTF8Encode = FALSE);

	// �����ļ�
	static BOOL CopyFile(CHAR* lpSrcFile, CHAR* lpDesFile, BOOL bUTF8Encode = FALSE);

	// ����һ�����ļ���������Ϊ���ֽڵ��ļ�
	static BOOL CreateEmptyFile(CHAR* lpFilePath, BOOL bUTF8Encode = FALSE);

	// ���ע��
	static void RegisterComponent(CHAR* lpFileName);

	// �ж�һ������Ƿ���Ҫע��
	static BOOL IsFileNeedRegister(CHAR* strFilePath);

	// ����һ�λ�������CRC16У��ֵ
	static unsigned short crc16(unsigned char *buff, int bufflen);

	// ��ȡ�ļ��汾��Ϣ
	static BOOL GetLocalFileVersion(CHAR* szPathName,CHAR* strVersion, DWORD dwSize);

	// ȷ��·�����ڣ����Ŀ¼�����ھʹ���Ŀ¼,���Դ������ε�Ŀ¼
	static void MakeSureDirectory(CHAR* lpszDirName, BOOL bUTF8Encode = FALSE);

	// �������̣�ʹ��·������
	static BOOL AdjustDiskAndCreateDirectory(CHAR* lpPathName, DWORD dwSize);

	// �ļ�AES�ӽ���
	static DWORD FileAesEncDec(const CHAR* lpPassword, BOOL bEncrypt, const CHAR* lpSrcFile, const CHAR* lpDesFile, BOOL bUTF8Encode=FALSE);

#if defined(WIN32)
	// ��ע����ж�ȡ�ַ�����������
	static BOOL RegReadStringValue(CHAR* lpKeyName, CHAR* lpValueName, BYTE* lpOutBuf, DWORD& dwBufSize);
	// ���ַ�����������д�뵽ע�����
	static BOOL RegWriteStringValue(CHAR* lpKeyName, CHAR* lpValueName, BYTE* lpStrBuf, DWORD dwBufSize);
	// ��ע����ж�ȡDWORD��������
	static BOOL RegReadDWordValue(CHAR* lpKeyName, CHAR* lpValueName, DWORD& dwValue);
	// ��DWORD��������д�뵽ע�����
	static BOOL RegWriteDWordValue(CHAR* lpKeyName, CHAR* lpValueName, DWORD dwValue);
#endif

	// ö��Ŀ¼�µ��������ļ�, ʧ�ܷ��ط�0ֵ
	static DWORD EnumFiles(CHAR* lpFolderPath, std::vector<std::string>& vtrFiles);
};





#endif//_GLOBALFUN_INCLUDE__
