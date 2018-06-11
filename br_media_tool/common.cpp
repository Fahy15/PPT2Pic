#include "stdafx.h"
#include "common.h"
#include <direct.h>
#include <io.h>
#include <time.h>
#include <sys\timeb.h>
#include <string>



CGSMutex::CGSMutex()
{
#ifdef _WIN32
	InitializeCriticalSection(&m_GSMutex);
#elif _LINUX
	pthread_mutex_init(&m_GSMutex, NULL);
#endif

}



CGSMutex::~CGSMutex()
{
	//Unlock();

#ifdef _WIN32
	DeleteCriticalSection(&m_GSMutex);
#elif _LINUX
	pthread_mutex_destroy(&m_GSMutex);
#endif

}

BOOL CGSMutex::Lock()
{
	BOOL bRet = TRUE;

#ifdef _WIN32
	EnterCriticalSection(&m_GSMutex);
#elif _LINUX
	if (pthread_mutex_lock(&m_GSMutex) != 0)
	{
		bRet = FALSE;
	};
#endif
	return bRet;
}


BOOL CGSMutex::TryLock()
{
	BOOL bRet = TRUE;

#ifdef _WIN32
	bRet = TryEnterCriticalSection(&m_GSMutex);
#elif _LINUX
	if (pthread_mutex_trylock(&m_GSMutex) != 0)
	{
		bRet = FALSE;
	}

#endif
	return bRet;
}


void CGSMutex::Unlock()
{
#ifdef _WIN32
	LeaveCriticalSection(&m_GSMutex);
#elif _LINUX
	pthread_mutex_unlock(&m_GSMutex);
#endif

}


CGSAutoMutex::CGSAutoMutex(CGSMutex *locker) : m_locker(locker)
{

	if (m_locker)
	{
		m_locker->Lock();
	}

}

CGSAutoMutex::~CGSAutoMutex()
{

	if (m_locker)
	{
		m_locker->Unlock();
	}

}


HMODULE g_hModule = NULL;
std::string GetAppPath()
{

	char szPath[MAX_PATH];
	memset(szPath, 0, MAX_PATH);

#ifdef WINCE
	TCHAR szTmp[MAX_PATH];
	memset(szTmp, 0, MAX_PATH);

	DWORD ret = GetModuleFileName(NULL, szTmp, MAX_PATH);
	if (0 == ret)
	{
		return "\\";
	}

	wchar2char(szPath, szTmp, MAX_PATH);
#else
	if (!g_hModule)
	{

#ifdef _DEBUG
		g_hModule = GetModuleHandleA("br_media_tool.exe");
#else
		g_hModule = GetModuleHandleA("br_media_tool.exe");
#endif

	}

	DWORD ret = GetModuleFileNameA(g_hModule, szPath, MAX_PATH);
	if (0 == ret)
	{
		return ".\\";
	}
#endif

	std::string strPath = szPath;
	return strPath.substr(0, strPath.rfind("\\")) + "\\";

}

BOOL GetFileName(std::string& fn, const std::string& path)
{


	for (int i = (int)path.length(); i >= 0; --i)
	{
		if (path[i] == ('\\'))//�Ӻ�ʼ��\Ŀ¼���Ӷ�����ļ���
		{
			for (int j = i + 1; j < (INT)path.length(); ++j)
				fn.push_back(path[j]);

			return TRUE;

		}
	}

	return FALSE;

}

void ReplaceAllChar(std::string& strSrc, char chSrc, char chDest)
{
	std::string::size_type stPos = 0;

	do				//��ͳһ�ָ���Ϊ��/��
	{
		stPos = strSrc.find(chSrc, stPos);
		if (std::string::npos == stPos)
		{
			return;
		}
		else
		{
			strSrc.replace(stPos, 1, std::string(1, chDest));
		}
	} while (1);

	return;
}


int RecursionCreateDir(const char* cPath)
{
	if (NULL == cPath)
	{
		return false;
	}

	std::string strPath = cPath;
	std::string strSubPath;
	std::string::size_type stPos = 0;

#if 0
	do				//��ͳһ�ָ���Ϊ��/��
	{
		stPos = strPath.find('\\', stPos);
		if (std::string::npos == stPos)
		{
			break;
		}
		else
		{
			strPath.replace(stPos, 1, "/");
		}
	} while (1);
#endif

	ReplaceAllChar(strPath, '\\', '/');

	if ('/' != strPath.at(strPath.length() - 1))		//ȷ�����һ��Ŀ¼����
	{
		strPath += "/";
	}

	stPos = 0;
	do				//�ж�Ŀ¼�Ƿ����
	{
		stPos = strPath.find('/', stPos + 1);
		if (std::string::npos == stPos)			//����Ŀ¼���
		{
			return 0;
		}

		strSubPath = strPath.substr(0, stPos);	//ȡǰ��һ�ν����ж�

		if (0 != _access(strSubPath.c_str(), 0))			//������ʱ���д���
		{
#ifdef WINCE
			if (0 != _mkdir(strSubPath.c_str(), NULL))
#else
			if (0 != _mkdir(strSubPath.c_str()))
#endif
			{
				return -1;						//����ʧ��ʱֱ�ӷ���
			}
		}
#if 0
		if ("/" == strPath.at(strPath.length() - 1))
		{
			strPath.erase(strPath.end() - 1);
		}

		if (0 == _access(strPath))
		{
			break;		//���˼�Ŀ¼�Ѿ�����
		}
		else
		{
			strPath = strPath.substr(0, strPath.find_last_of("/"));
		}
#endif
	} while (1);

	return 0;
}