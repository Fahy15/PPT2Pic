#ifndef _ANYCHAT_GUID_INCLUDED__H_
#define _ANYCHAT_GUID_INCLUDED__H_

#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <assert.h>
#include <sys/stat.h>
#include <signal.h>
#include <setjmp.h>
#include <time.h>

#include "osdef.h"

#if defined(WIN32)
#	include <ObjBase.h>
#else
#	include <uuid/uuid.h>			// need link -luuid
#endif


inline bool operator<( const GUID & lhs, const GUID & rhs )
{
	return ( memcmp( &lhs, &rhs, sizeof(GUID) ) > 0 ? true : false );
}

#if !defined(WIN32)
inline BOOL operator== (const GUID& guid1 , const GUID& guid2)
{
	return !memcmp(&guid1, &guid2, sizeof(GUID));
}

inline BOOL operator!= (const GUID& guid1 , const GUID& guid2)
{
	return memcmp(&guid1, &guid2, sizeof(GUID));
}
#endif

class CGuidUtils
{
public:
	CGuidUtils() {}
	virtual ~CGuidUtils() {}

public:
	// 产生GUID
	static GUID CreateGuid(void)
	{
		GUID guid = {0};
#ifdef WIN32
		if(S_OK !=::CoCreateGuid(&guid))
		{
			ASSERT(0);
		}
#else
		uuid_generate(reinterpret_cast<unsigned char *>(&guid));
#endif
		return guid;
	}

	// GUID转换为字符串
    static CHAR* GuidToString(const GUID &guid, CHAR* lpStr, DWORD dwSize)
	{
		_snprintf(lpStr, dwSize, "%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X" ,
			guid.Data1,  guid.Data2,   guid.Data3 ,
			guid.Data4[0],   guid.Data4[1],
			guid.Data4[2],   guid.Data4[3],
			guid.Data4[4],   guid.Data4[5],
			guid.Data4[6],   guid.Data4[7] );
		return lpStr;
	}

	// 字符串转换为GUID
	static GUID StringToGuid(const CHAR* lpStr)
	{
		GUID guid = {0};
		int b[8] = {0};
		sscanf(lpStr, "%08X-%4hX-%4hX-%02X%02X-%02X%02X%02X%02X%02X%02X",
				&guid.Data1, &guid.Data2, &guid.Data3,
				&b[0], &b[1], &b[2], &b[3], &b[4], &b[5], &b[6], &b[7]);
		for (int i=0; i<8; i++)
			guid.Data4[i] = (unsigned char)b[i];
		return guid;
	}

	// 判断GUID是否为空
	static BOOL GuidIsNull(const GUID &guid)
	{
		GUID guid_null = {0};
		return guid == guid_null;
	}




};

#endif // _ANYCHAT_GUID_INCLUDED__H_
