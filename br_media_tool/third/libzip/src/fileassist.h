#ifndef _FILE_ASSIST_H_
#define _FILE_ASSIST_H_

#include <fileglobalfun.h>
#include <vector>
#include <string>
#include <algorithm>	// STL�㷨ͷ�ļ�

// �ж��ַ����Ƿ��ǡ�Ŀ¼�ָ����β��
#define	STR_END_IS_SEPARATED(lpPath) (strlen(lpPath)>0 && lpPath[strlen(lpPath)-1]==DIRECTORY_SEPARATED_CHAR)

// ����ַ����ǲ����ļ��зָ����β�ľ������
#define STR_END_ADD_SEPARATED(lpStr) {				\
			if (!STR_END_IS_SEPARATED(lpStr)){		\
				char szSeparated[10] = { 0 };		\
				_snprintf(szSeparated, sizeof(szSeparated), "%c", DIRECTORY_SEPARATED_CHAR);	\
				strcat(lpStr, szSeparated);		\
			}										\
		}

// ·��ĩβ����ַ������Զ���ȫ�ļ��зָ����
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
	// �ж��ļ��Ƿ����
	static BOOL IsFileExist(const char * lpFilePath);
	// �ж�Ŀ¼�Ƿ����
	static BOOL IsDirectoryExist(const char * lpPathName);

	// ɾ���ļ�
	static void RemoveFile(const char * lpFilePath);
	// ɾ���ļ���(ɾ���ǿ��ļ���), bOnlyInternal == true ��ֻ����ڲ���������ͬ�ļ���һ��ɾ��
	static BOOL RemoveDir(const char * lpPathName, BOOL bOnlyInternal = FALSE);
	// ɾ���ļ�������������ļ����ܱ������ļ��г���, vtrProtected:�ܱ����ļ������������������е��ļ��в���
	static BOOL RemoveFileOfDir(const char * lpPathName, const std::vector<std::string> &vtrProtected);

	// �����ļ���
	static BOOL MakeDirectory(const char * lpPathName);
	// ����һ��·������·���ĵ�һ�����ж��Ƿ���ڣ������ھʹ�����
	static bool MakeIntegrityPath(const char* lpIntegrityPath);

	// �����ļ�
	static bool CopySingleFile(const char * lpFileSrc, const char * lpFileDest);
	// �����ļ����������е��ļ���ָ��·��(Ŀ��·��������ڣ����򷵻�false)
	static bool CopyFileOfDir(const char * lpDirSrc, const char * lpDirDest);
	// �����ļ����������е��ļ���ָ��·��(Ŀ��·��������ڣ����򷵻�false), vtrProtected:�ܱ����ļ������������������е��ļ��в���
	static bool CopyFileOfDir(const char * lpDirSrc, const char * lpDirDest, const std::vector<std::string> &vtrProtected);

	// �ļ�������(�Կյ��ļ���Ҳ��Ч)
	static bool RenameFile(const char * lpSrcFile, const char * lpDesFile);
	// �ļ���������
	static bool RenameDir(const char * lpSrcDir, const char * lpDestDir);
	static bool RenameDirEx(const char * lpSrcDir, const char * lpDestDir);	// ���������з�ʽ

	// ����Ŀ���ļ��У����ҳ����й��˷�Χ���ļ�(ֻ����һ��Ŀ¼)
	static std::vector<std::string> FindFileForDir(const char * lpDestDir, const char *lpszFilespec);
	// ����Ŀ���ļ��У����ҳ����й��˷�Χ���ļ���(ֻ����һ��Ŀ¼)
	static std::vector<std::string> FindDirForDir(const char * lpDestDir, const char *lpszFilespec);

public:	// ��������
	// �����ַ���(���ַ������ԡ�mark��Ϊ��Ǽ���)
	static std::vector<std::string> catString(const char * lpszSrc, char mark);
	// ��ȡ�ļ��ĵ�ǰ·����or ��ȡ�ļ��е��ϼ�Ŀ¼��,ע�⣺ֱ���ַ���������������Ч���ж�
	static char * getNearbyPath(const char * lpSrc, char * lpBuffer, DWORD dwSizeInBytes);
	// ��ȡ���һ��Ŀ¼(������ļ����ͻ�ȡ���ļ�����)
	static char * getLastDirName(const char * lpSrc, char * lpBuffer, DWORD dwSizeBytes);
	// ���·��ת����·��(�Դ�..���ַ�����������ƴ��)
	static char * relativePath2AbsolutePath(const char * lpSrc, char * lpBuffer, DWORD dwSizeBytes);

};


#endif	//_FILE_ASSIST_H_