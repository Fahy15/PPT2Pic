
#pragma once
#include <string>

using namespace std;

#define GSThread			HANDLE				//�߳�����
#define GSProcessMutex		HANDLE				//������
#define GSMutex				CRITICAL_SECTION	//�߳���
#define GSRwmutex			CRITICAL_SECTION	//��д��
#define GSCond				HANDLE				//��������
#define GSSem				HANDLE				//�ź���

std::string GetAppPath();

class CGSMutex
{
public:
	CGSMutex();
	~CGSMutex(void);
	BOOL	Lock();		//�������ɹ�����TRUE��ʧ�ܷ���FALSE  
	void	Unlock();	//�������ɹ�����TRUE,ʧ�ܷ���FALSE
	BOOL	TryLock();	//�������ļ������ɹ�����TRUE,ʧ�ܷ���FALSE

public:
	GSMutex m_GSMutex;
};


class  CGSAutoMutex
{

public:
	CGSAutoMutex(CGSMutex *locker); //����
	~CGSAutoMutex(void); //����

private:
	CGSMutex *m_locker;

};


inline LPSTR wchar2char(LPSTR dst,const LPWSTR src,int nLen)
{
	int nSize = WideCharToMultiByte( 
		CP_ACP,  
		NULL,   
		src, 
		-1,   
		NULL,  
		0,    
		NULL,  
		FALSE);  

	//if (nSize>nLen)
	//{
	//	nSize=nLen;
	//}
	nSize=WideCharToMultiByte( // �ٵó�char*�ִ�
		CP_ACP, 
		NULL, 
		src, 
		-1,   
		dst,   
		nSize,  
		NULL, 
		FALSE);

	if (nSize>0)
	{
		return dst;
	}
	else
	{
		return NULL;
	}
}

BOOL GetFileName(std::string& fn, const std::string& path);

//charתunicode
inline LPWSTR char2wchar(LPWSTR dst,LPCSTR src,int len)
{

	//	TCHAR wszSrcFile[MAX_PATH];
	//WCHAR *tszDestFile;
	int nResult=0;

	nResult = MultiByteToWideChar(
		CP_ACP,    
		MB_PRECOMPOSED,
		src,
		(int) strlen(src)+1,
		NULL,
		0);

	//dst=new WCHAR[nResult];
	nResult = MultiByteToWideChar(
		CP_ACP,    
		0,
		src,
		(int) strlen(src)+1,
		dst,
		len);

	return nResult==0 ? NULL:dst;

}

void ReplaceAllChar(std::string& strSrc, char chSrc, char chDest);

int RecursionCreateDir(const char* cPath);

#define GS_SAFE_COPY(strDest, strDestLen, strSource)	_tcsncpy_s(strDest, strDestLen, strSource, _TRUNCATE)