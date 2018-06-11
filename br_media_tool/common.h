
#pragma once
#include <string>

using namespace std;

#define GSThread			HANDLE				//线程类型
#define GSProcessMutex		HANDLE				//进程锁
#define GSMutex				CRITICAL_SECTION	//线程锁
#define GSRwmutex			CRITICAL_SECTION	//读写锁
#define GSCond				HANDLE				//条件变量
#define GSSem				HANDLE				//信号量

std::string GetAppPath();

class CGSMutex
{
public:
	CGSMutex();
	~CGSMutex(void);
	BOOL	Lock();		//加锁，成功返回TRUE，失败返回FALSE  
	void	Unlock();	//解锁，成功返回TRUE,失败返回FALSE
	BOOL	TryLock();	//非阻塞的加锁，成功返回TRUE,失败返回FALSE

public:
	GSMutex m_GSMutex;
};


class  CGSAutoMutex
{

public:
	CGSAutoMutex(CGSMutex *locker); //加锁
	~CGSAutoMutex(void); //解锁

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
	nSize=WideCharToMultiByte( // 再得出char*字串
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

//char转unicode
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