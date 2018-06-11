#ifndef _ANYCHAT_CODECONVERT_INCLUDE_H_
#define _ANYCHAT_CODECONVERT_INCLUDE_H_

#include "osdef.h"

#ifdef CODECONVERT_ICONV
#   include <iconv.h>
#endif

class AC_CodeConvert
{
    public:
        AC_CodeConvert();
        virtual ~AC_CodeConvert();

		// UNICODE to Ansi
		static BOOL Unicode2Ansi(LPCWSTR lpUnicodeStr, CHAR* lpAnsiStr, DWORD dwAnsiBufLen)
		{
			BOOL bSuccess = FALSE;
#if defined(WIN32)
			bSuccess = ::WideCharToMultiByte(CP_ACP, NULL, lpUnicodeStr, (INT)wcslen(lpUnicodeStr), lpAnsiStr, dwAnsiBufLen, NULL, NULL) != 0;
#elif defined(CODECONVERT_ICONV)
			bSuccess = code_convert("UNICODE", "ASCII", (char*)lpUnicodeStr, (INT)wcslen(lpUnicodeStr)*sizeof(WCHAR), lpAnsiStr, dwAnsiBufLen) == 0;
#else
			memcpy(lpAnsiStr, lpUnicodeStr, strlen((PCHAR)lpUnicodeStr)>dwAnsiBufLen ? dwAnsiBufLen : strlen((PCHAR)lpUnicodeStr));
#endif
			return bSuccess;
		}

		// UNICODE to UTF-8
		static BOOL Unicode2UTF8(LPCWSTR lpUnicodeStr, CHAR* lpUTF8Str, DWORD dwUTF8BufLen)
		{
			BOOL bSuccess = FALSE;
#if defined(WIN32)
			bSuccess = ::WideCharToMultiByte( CP_UTF8, 0, lpUnicodeStr, -1, lpUTF8Str, dwUTF8BufLen, NULL, NULL) != 0;
#elif defined(CODECONVERT_ICONV)
			bSuccess = code_convert("UNICODE", "UTF-8", (char*)lpUnicodeStr, (INT)wcslen(lpUnicodeStr)*sizeof(WCHAR), lpUTF8Str, dwUTF8BufLen) == 0;
#else
			memcpy(lpUTF8Str, lpUnicodeStr, strlen((PCHAR)lpUnicodeStr)>dwUTF8BufLen ? dwUTF8BufLen : strlen((PCHAR)lpUnicodeStr));
#endif
			return bSuccess;
		}

		// UNICODE to GBK
		static BOOL Unicode2GBK(LPCWSTR lpUnicodeStr, CHAR* lpGBKStr, DWORD dwGBKBufLen)
		{
			BOOL bSuccess = FALSE;
#if defined(WIN32)
			UINT nCodePage = 936; //GB2312
			bSuccess = ::WideCharToMultiByte(nCodePage, NULL, lpUnicodeStr, (INT)wcslen(lpUnicodeStr), lpGBKStr, dwGBKBufLen, NULL, NULL) != 0;
#elif defined(CODECONVERT_ICONV)
			bSuccess = code_convert("UNICODE", "GB2312", (char*)lpUnicodeStr, (INT)wcslen(lpUnicodeStr)*sizeof(WCHAR), lpGBKStr, dwGBKBufLen) == 0;
#else
			memcpy(lpGBKStr, lpUnicodeStr, strlen((PCHAR)lpUnicodeStr)>dwGBKBufLen ? dwGBKBufLen : strlen((PCHAR)lpUnicodeStr));
#endif
			return bSuccess;
		}

		// Ansi to UNICODE
		static BOOL Ansi2Unicode(CHAR* lpAnsiStr, WCHAR* lpUnicodeStr, DWORD dwBufSize)
		{
			BOOL bSuccess = FALSE;
#if defined(WIN32)
			bSuccess = ::MultiByteToWideChar(CP_ACP, 0, lpAnsiStr, -1, lpUnicodeStr, dwBufSize) != 0;
#elif defined(CODECONVERT_ICONV)
			bSuccess = code_convert("ASCII","UNICODE", (char*)lpAnsiStr, strlen(lpAnsiStr), (char*)lpUnicodeStr, dwBufSize) == 0;
#else
			memcpy(lpUnicodeStr, lpAnsiStr, strlen(lpAnsiStr)>dwBufSize ? dwBufSize : strlen(lpAnsiStr));
#endif
			return bSuccess;
		}

		// UTF-8 to UNICODE
		static BOOL UTF82Unicode(CHAR* lpUtf8Str, WCHAR* lpUnicodeStr, DWORD dwBufSize)
		{
			BOOL bSuccess = FALSE;
#if defined(WIN32)
			bSuccess = ::MultiByteToWideChar(CP_UTF8, 0, lpUtf8Str, -1, lpUnicodeStr, dwBufSize) != 0;
#elif defined(CODECONVERT_ICONV)
			bSuccess = code_convert("UTF-8","UNICODE", (char*)lpUtf8Str, strlen(lpUtf8Str), (char*)lpUnicodeStr, dwBufSize) == 0;
#else
			memcpy(lpUnicodeStr, lpUtf8Str, strlen(lpUtf8Str)>dwBufSize ? dwBufSize : strlen(lpUtf8Str));
#endif
			return bSuccess;
		}

		// GBK to UNICODE
		static BOOL GBK2Unicode(CHAR* lpGbkStr, WCHAR* lpUnicodeStr, DWORD dwBufSize)
		{
			BOOL bSuccess = FALSE;
#if defined(WIN32)
			UINT nCodePage = 936; //GB2312
			bSuccess = ::MultiByteToWideChar(nCodePage, 0, lpGbkStr, -1, lpUnicodeStr, dwBufSize) != 0;
#elif defined(CODECONVERT_ICONV)
			bSuccess = code_convert("GB2312","UNICODE", (char*)lpGbkStr, strlen(lpGbkStr), (char*)lpUnicodeStr, dwBufSize) == 0;
#else
			memcpy(lpUnicodeStr, lpGbkStr, strlen(lpGbkStr)>dwBufSize ? dwBufSize : strlen(lpGbkStr));
#endif
			return bSuccess;
		}

		// GBK to UTF8
		static BOOL GBK2UTF8(CHAR* lpGbkStr, CHAR* lpUtf8Str, DWORD dwBufSize)
		{
			BOOL bSuccess = FALSE;
			WCHAR szUnicodeBuf[8192] = {0};
			do
			{
				if(!GBK2Unicode(lpGbkStr, szUnicodeBuf, sizeof(szUnicodeBuf)))
					break;
				if(!Unicode2UTF8(szUnicodeBuf, lpUtf8Str, dwBufSize))
					break;
				bSuccess = TRUE;
			} while (FALSE);
			return bSuccess;
		}

		// UTF8 to GBK
		static BOOL UTF82GBK(CHAR* lpUtf8Str, CHAR* lpGbkStr, DWORD dwBufSize)
		{
			BOOL bSuccess = FALSE;
			WCHAR szUnicodeBuf[8192] = {0};
			do
			{
				if(!UTF82Unicode(lpUtf8Str, szUnicodeBuf, sizeof(szUnicodeBuf)))
					break;
				if(!Unicode2GBK(szUnicodeBuf, lpGbkStr, dwBufSize))
					break;
				bSuccess = TRUE;
			} while (FALSE);
			return bSuccess;
		}


    private:

#ifdef CODECONVERT_ICONV
		static int code_convert(const char *from_charset, const char *to_charset,char *inbuf,int inlen,char *outbuf,int outlen)
		{
			int rc;
			char **pin = &inbuf;
			char **pout = &outbuf;
			iconv_t cd = iconv_open(to_charset,from_charset);
			if (cd==0)
				return -1;
			memset(outbuf,0,outlen);
			size_t insize = inlen;
			size_t outsize = outlen;
			rc = iconv(cd, pin, &insize, pout, &outsize);
			iconv_close(cd);
			return rc;
		}
#endif
};

#endif // _ANYCHAT_CODECONVERT_INCLUDE_H_
