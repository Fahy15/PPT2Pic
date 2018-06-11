#ifndef ANYCHAT_OSDEF_H_INCLUDED
#define ANYCHAT_OSDEF_H_INCLUDED

#if defined (__APPLE__) || defined (__MACH__) || defined (MAC_OS_X) || defined (MAC_OS_CLASSIC)
#define LINUXBASE
#define APPLEOS
#include <objc/objc.h>
#include <corevideo/CVBase.h>
#endif

#if (!TARGET_OS_IPHONE && !TARGET_IPHONE_SIMULATOR) && TARGET_OS_MAC
#define ANYCHAT_OS_MAC
#endif

#pragma once

#include <assert.h>

#if defined(WIN32) || defined(WINCE)
	#pragma warning(disable:4819)
	#include <windows.h>
	#include <windef.h>
	#include <tchar.h>
	#include <wtypes.h>
	//#include "types.h"
#ifndef ASSERT
#	define ASSERT	assert
#endif

#ifndef TRACE
void TRACE(LPCTSTR pszFormat, ...);
#endif


    #define DLL_EXPORT_API extern "C" __declspec(dllexport)
    #define DIRECTORY_SEPARATED_CHAR _T('\\')
	#define AC_DLLMODULE_HANDLE	HMODULE
	#define socklen_t   int
	#define AC_SOCKET_SEND_FLAGS    0

#if defined(WINCE)
#	define AC_LOADLIBRARYEX_FLAGS	0		// wince不支持SearchPath标志
#else
#	define AC_LOADLIBRARYEX_FLAGS	LOAD_WITH_ALTERED_SEARCH_PATH
#endif


#ifdef _INTPTR_T_DEFINED
	typedef intptr_t    INTPTR_T;
	typedef uintptr_t   UINTPTR_T;
#else
	typedef int	INTPTR_T;
	typedef unsigned int   UINTPTR_T;
#endif

	typedef signed char         INT8, *PINT8;
	typedef signed short        INT16, *PINT16;
	typedef signed int          INT32, *PINT32;
	typedef signed __int64      INT64, *PINT64;
	typedef unsigned char       UINT8, *PUINT8;
	typedef unsigned short      UINT16, *PUINT16;
	typedef unsigned int        UINT32, *PUINT32;
	typedef unsigned __int64    UINT64, *PUINT64;

#else
    #include <string.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <errno.h>
    #include <stddef.h>
    #include <sys/times.h>
    #include <wchar.h>
    #include <stdint.h>
	#include "types.h"

#if !defined(OBJC_BOOL_DEFINED)
    typedef long    BOOL;
#endif
    typedef unsigned int UINT;
    typedef double DOUBLE;
    typedef unsigned short WORD;
    typedef unsigned short USHORT;
    typedef short   SHORT;
    typedef unsigned int DWORD;
    typedef unsigned int SOCKET;
    typedef long LONG;
    typedef unsigned char BYTE;
    typedef unsigned char* PBYTE;
    typedef char CHAR;
    typedef unsigned char UCHAR;
    typedef char TCHAR;
    typedef wchar_t WCHAR;
    typedef char* PCHAR;
    typedef const char* LPCTSTR;
    typedef char* LPTSTR;
    typedef int INT;
    typedef unsigned int HANDLE;
    typedef unsigned int HINSTANCE;
	typedef DWORD COLORREF;
	typedef DWORD LRESULT;
    typedef intptr_t    INTPTR_T;
    typedef uintptr_t   UINTPTR_T;

	typedef int8_t		INT8, *PINT8;
	typedef int16_t		INT16, *PINT16;
	typedef int32_t		INT32, *PINT32;
	typedef int64_t		INT64, *PINT64;
	typedef uint8_t		UINT8, *PUINT8;
	typedef uint16_t	UINT16, *PUINT16;
	typedef uint32_t	UINT32, *PUINT32;
	typedef uint64_t	UINT64, *PUINT64;


    #define SOCKADDR_IN sockaddr_in
    #define SOCKADDR    sockaddr
    #define closesocket close
    #define INVALID_SOCKET  (-1)
	#define SD_BOTH	SHUT_RDWR
    #define SOCKET_ERROR (-1)
    #define WSA_INFINITE    ((DWORD)-1L)
    #define _T(x)   x
	#define LPCWSTR		const WCHAR*

    #define WSAEWOULDBLOCK  EWOULDBLOCK
    #define WSATRY_AGAIN    EAGAIN
    #define WSAEINPROGRESS  EINPROGRESS
    #define WSAEINTR        EINTR
    #define WSAENOBUFS      ENOBUFS
    #define WSAENOTSOCK     ENOTSOCK
    #define WSAECONNRESET   ECONNRESET
	#define WSAECONNREFUSED	ECONNREFUSED

#ifdef APPLEOS
#   define AC_SOCKET_SEND_FLAGS     0
#else
#   define AC_SOCKET_SEND_FLAGS    MSG_NOSIGNAL
#endif

    #define INVALID_HANDLE_VALUE    0xFFFFFFFF

    #define LPDWORD DWORD*
    #define HWND    void*
    #define HDC     DWORD
    #define CALLBACK
    #define FAR
    #define VOID    void
    #define LPVOID  void*
    #define LPBYTE  unsigned char*
    #define LPCHAR  char*
    #define LPCSTR  const char*
    #define LPCTSTR const char*
    #define TRUE    1
    #define FALSE   0
    #define WPARAM  DWORD
    #define LPARAM  DWORD

    #define PURE    =0
    #define OUT
    #define IN
    #define _cdecl
    #define __cdecl

#ifdef APPLEOS
    #define DLL_EXPORT_API CV_EXPORT
#else
    #define DLL_EXPORT_API extern "C"
#endif

    #define DIRECTORY_SEPARATED_CHAR '/'
	#define AC_DLLMODULE_HANDLE	void*
    #define ASSERT(x)
#ifdef _DEBUG
    #define TRACE(format, ...)	fprintf(stderr, format, ##__VA_ARGS__)
#else
    #define TRACE(format, ...)
#endif
    #define MAX_PATH 256

    #define _snprintf   snprintf
    #define _stricmp    strcasecmp
    #define stricmp     strcasecmp
	#define _tcslen		strlen
	#define _tcscmp     strcmp
	#define _tcscpy     strcpy
	#define _tcschr     strchr
	#define _tcsncpy    strncpy
	#define _sntprintf	_snprintf
	#define _stscanf	sscanf
	#define _tcsrchr    strrchr
	#define _tcscat     strcat
	#define _tcsstr     strstr
	#define _tfopen     fopen
	#define _trename	rename
	#define _vstprintf  vsprintf
	#define _vsnprintf	vsnprintf
	#define _vsntprintf  vsnprintf
	#define _ftprintf   fprintf
	#define _ftscanf    fscanf
	#define _fgetts		fgets
	#define _tstat		stat
	#define _stat		stat
	#define _atoi64     atoll
	#define strtok_s	strtok_r
	#define _getpid		getpid

    DWORD GetTickCount(void);
    DWORD WSAGetLastError(void);
    DWORD GetModuleFileName( char* sModuleName, char* sFileName, int nSize);
	char* GetModuleHandle(const char* sModuleFile);

    #define GetLastError    WSAGetLastError

    #define MAKEWORD(a, b)      ((WORD)(((BYTE)(a)) | ((WORD)((BYTE)(b))) << 8))
    #define MAKELONG(a, b)      ((DWORD)(((WORD)(a)) | ((DWORD)((WORD)(b))) << 16))
    #define LOWORD(l)           ((WORD)(l))
    #define HIWORD(l)           ((WORD)(((DWORD)(l) >> 16) & 0xFFFF))
    #define LOBYTE(w)           ((BYTE)(w))
    #define HIBYTE(w)           ((BYTE)(((WORD)(w) >> 8) & 0xFF))

    #define Sleep(x)    usleep(x*1000)

#endif //_WIN32


#ifndef _WIN32

typedef struct tagBITMAPFILEHEADER {
    WORD bfType;
    DWORD bfSize;
    WORD bfReserved1;
    WORD bfReserved2;
    DWORD bfOffBits;
}BITMAPFILEHEADER;

#if !defined(BITMAPINFOHEADER_DEFINE)
#define BITMAPINFOHEADER_DEFINE
#if defined(APPLEOS)
typedef struct tagBITMAPINFOHEADER {
    uint32_t biSize;
    uint32_t biWidth;
    uint32_t biHeight;
    uint16_t biPlanes;
    uint16_t biBitCount;
    uint32_t biCompression;
    uint32_t biSizeImage;
    uint32_t biXPelsPerMeter;
    uint32_t biYPelsPerMeter;
    uint32_t biClrUsed;
    uint32_t biClrImportant;
}__attribute__((packed)) BITMAPINFOHEADER;
#else
typedef struct tagBITMAPINFOHEADER {
    DWORD biSize;
    LONG biWidth;
    LONG biHeight;
    WORD biPlanes;
    WORD biBitCount;
    DWORD biCompression;
    DWORD biSizeImage;
    LONG biXPelsPerMeter;
    LONG biYPelsPerMeter;
    DWORD biClrUsed;
    DWORD biClrImportant;
}BITMAPINFOHEADER,*LPBITMAPINFOHEADER;
#endif //APPLEOS
#endif

#define RGB(r,g,b)          ((COLORREF)(((BYTE)(r)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(b))<<16)))
#define GetRValue(rgb)      ((BYTE)(rgb))
#define GetGValue(rgb)      ((BYTE)(((WORD)(rgb)) >> 8))
#define GetBValue(rgb)      ((BYTE)((rgb)>>16))

#define BI_RGB        0L
#define BI_BITFIELDS  3L
#define mmioFOURCC( ch0, ch1, ch2, ch3 ) ( (DWORD)(BYTE)(ch0) | ( (DWORD)(BYTE)(ch1) << 8 ) | ( (DWORD)(BYTE)(ch2) << 16 ) | ( (DWORD)(BYTE)(ch3) << 24 ) )

typedef struct tagRGBQUAD {
    BYTE rgbBlue;
    BYTE rgbGreen;
    BYTE rgbRed;
    BYTE rgbReserved;
}RGBQUAD;

#if !defined(WAVEFORMATEX_DEFINE)
#define WAVEFORMATEX_DEFINE
#if defined(APPLEOS)
typedef struct tagWAVEFORMATEX{
    uint16_t  wFormatTag;
    uint16_t  nChannels;
    uint32_t nSamplesPerSec;
    uint32_t nAvgBytesPerSec;
    uint16_t  nBlockAlign;
    uint16_t  wBitsPerSample;
    uint16_t  cbSize;
}__attribute__((packed)) WAVEFORMATEX,*LPWAVEFORMATEX;
#else
typedef struct tagWAVEFORMATEX{
  WORD  wFormatTag;
  WORD  nChannels;
  DWORD nSamplesPerSec;
  DWORD nAvgBytesPerSec;
  WORD  nBlockAlign;
  WORD  wBitsPerSample;
  WORD  cbSize;
}WAVEFORMATEX,*LPWAVEFORMATEX;
#endif //APPLEOS
#endif

#define WAVE_FORMAT_PCM 1

typedef struct _SYSTEMTIME {
  WORD wYear;
  WORD wMonth;
  WORD wDayOfWeek;
  WORD wDay;
  WORD wHour;
  WORD wMinute;
  WORD wSecond;
  WORD wMilliseconds;
} SYSTEMTIME, *PSYSTEMTIME;

typedef struct _GUID {
	unsigned int  Data1;
	unsigned short Data2;
	unsigned short Data3;
	unsigned char  Data4[ 8 ];
} GUID;

void GetSystemTime(SYSTEMTIME* lpSystemTime);
void GetLocalTime(SYSTEMTIME* lpSystemTime);
char *_strupr_s(char *str, int size);
char *_strlwr_s(char *str, int size);
char * _i64toa(INT64 value, char * outbuf, int size);
#endif


#ifdef APPLEOS
#define STATIC_LINK_LIBRARY
#endif //APPLEOS

#if !defined(WIN32) && !defined(APPLEOS) && !defined(ANDROID)
#define CODECONVERT_ICONV
#endif


#ifndef DIBSIZE
// DIBSIZE calculates the number of bytes required by an image
#define WIDTHBYTES(bits) ((DWORD)(((bits)+31) & (~31)) / 8)
#define DIBWIDTHBYTES(bi) (DWORD)WIDTHBYTES((DWORD)(bi).biWidth * (DWORD)(bi).biBitCount)
#define _DIBSIZE(bi) (DIBWIDTHBYTES(bi) * (DWORD)(bi).biHeight)
#define DIBSIZE(bi) ((bi).biHeight < 0 ? (-1)*(_DIBSIZE(bi)) : _DIBSIZE(bi))
#endif

#define MAKEINT64(low, high) ((UINT64)(((DWORD)(low)) | ((UINT64)((DWORD)(high))) << 32))
#define LODWORD(l)           ((DWORD)(l))
#define HIDWORD(l)           ((DWORD)(((UINT64)(l) >> 32) & 0xFFFFFFFF))


#if ( _MSC_VER >= 1500 )
#	define ANYCHAT_SIN_TYPE	long double
#else
#	define ANYCHAT_SIN_TYPE	INT
#endif


// 检测CPU类型，用于判断是否支持neon指令集
DWORD anychat_cpu_detect(void);

#endif // ANYCHAT_OSDEF_H_INCLUDED
