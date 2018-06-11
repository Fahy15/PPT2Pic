#ifndef _ANYCHAT_SERVERCLUSTERS_ZIPUTILS_H
#define _ANYCHAT_SERVERCLUSTERS_ZIPUTILS_H

#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>

#include "zip.h"
#include "zlib.h"
#include "zconf.h"
#include "zipconf.h"
#include "libzippp.h"


// ���ӵײ��
/*
#ifdef _WINDOWS_
#	pragma comment(lib, "zip.lib")
#	pragma comment(lib, "zlib.lib")
#endif*/

#pragma comment(lib, "zip.lib")
#pragma comment(lib, "zlibstatic.lib")


using namespace libzippp;

// zip�ļ��Ĳ���
class CZipUtils
{
public:
	CZipUtils();
	virtual ~CZipUtils();

public:
	// �ļ���ȡ
	//static bool extractPacket(const char *lpszSrc, const char *lpszDes = NULL);
	// ѹ���ļ�(�ļ���)
	static bool compressFolder(const char *lpszSrc, const char *lpszDes = NULL);
	// ѹ���ļ��еĵݹ麯��
	static bool compressSubFolder(ZipArchive &hz, const char *lpszSrc, const char *lpszDes);
	// ѹ���ļ�
	static bool compressFile(const std::vector<std::string> &vtrFile, const char *lpszDes);

public:
	// GBKתUTF8
	//static CHAR * _GBK2UTF8(CHAR* lpGbkStr, CHAR* lpUtf8Str, DWORD dwBufSize);
	//// UTF8תGBK
	//static CHAR * _UTF82GBK(CHAR* lpUtf8Str, CHAR* lpGbkStr, DWORD dwBufSize);
};

#endif // _ANYCHAT_SERVERCLUSTERS_ZIPUTILS_H