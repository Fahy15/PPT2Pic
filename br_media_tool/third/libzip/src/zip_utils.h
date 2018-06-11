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


// 连接底层库
/*
#ifdef _WINDOWS_
#	pragma comment(lib, "zip.lib")
#	pragma comment(lib, "zlib.lib")
#endif*/

#pragma comment(lib, "zip.lib")
#pragma comment(lib, "zlibstatic.lib")


using namespace libzippp;

// zip文件的操作
class CZipUtils
{
public:
	CZipUtils();
	virtual ~CZipUtils();

public:
	// 文件抽取
	//static bool extractPacket(const char *lpszSrc, const char *lpszDes = NULL);
	// 压缩文件(文件夹)
	static bool compressFolder(const char *lpszSrc, const char *lpszDes = NULL);
	// 压缩文件夹的递归函数
	static bool compressSubFolder(ZipArchive &hz, const char *lpszSrc, const char *lpszDes);
	// 压缩文件
	static bool compressFile(const std::vector<std::string> &vtrFile, const char *lpszDes);

public:
	// GBK转UTF8
	//static CHAR * _GBK2UTF8(CHAR* lpGbkStr, CHAR* lpUtf8Str, DWORD dwBufSize);
	//// UTF8转GBK
	//static CHAR * _UTF82GBK(CHAR* lpUtf8Str, CHAR* lpGbkStr, DWORD dwBufSize);
};

#endif // _ANYCHAT_SERVERCLUSTERS_ZIPUTILS_H