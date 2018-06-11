#ifndef ANYCHAT_MACRODEF_H_INCLUDED
#define ANYCHAT_MACRODEF_H_INCLUDED

#include "errorcode.h"


#if defined(ANYCHAT_NEEDSIGN_EXCEPTION)
	extern BOOL	g_bOccurException;		///< 是否发生异常
#	define ANYCHAT_EXCEPTION_CLEAR		g_bOccurException = FALSE;
#	define ANYCHAT_EXCEPTION_SIGN		g_bOccurException = TRUE;
#else
#	define ANYCHAT_EXCEPTION_CLEAR
#	define ANYCHAT_EXCEPTION_SIGN
#endif //ANYCHAT_NEEDSIGN_EXCEPTION



#define AC_ERROR_BREAK(x)   if((x)!=AC_ERROR_SUCCESS)     break;
#define AC_FALSE_BREAK(x)   if(!(x))    break;
#define AC_RETURNERROR(x,y)   if(x)    return y;
#define AC_SUCCESS(x)   (x) == AC_ERROR_SUCCESS ? TRUE : FALSE
#define AC_CHECKOBJECT_RETURN(x)    if(!(x))    return AC_ERROR_MEMORYFAIL;

#define AC_SAFE_RELEASE(x)  if(x){  (x)->Release();   delete (x);   (x) = NULL;   }
#define AC_SAFE_DELETE_ARRAY(x) if((x)){  delete [](x); (x) = NULL;     }

#define AC_TCHAR_SIZEOF(x)	(sizeof(x)/sizeof(TCHAR))

// 释放资源宏定义
#define ANYCHAT_SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#define ANYCHAT_SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
#define ANYCHAT_SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }
#define ANYCHAT_SAFE_FREE(p)		 { if(p) { free(p);		   (p)=NULL; } }

#ifdef WIN32
#ifdef _DEBUG
	#define ANYCHAT_TRYCATCH_BEGIN
	#define ANYCHAT_TRYCATCH_END(x)
#else
	#define ANYCHAT_TRYCATCH_BEGIN			__try{	ANYCHAT_EXCEPTION_CLEAR;

	#define ANYCHAT_TRYCATCH_NORMALEND		} __except(EXCEPTION_EXECUTE_HANDLER) {		ANYCHAT_EXCEPTION_SIGN;	}
	#define ANYCHAT_TRYCATCH_CRASHEND(x)	} __except(RecordExceptionInfo(GetExceptionInformation(), x)) {		ANYCHAT_EXCEPTION_SIGN;	}

	#ifdef XCrashReport
		#define ANYCHAT_TRYCATCH_END(x)		ANYCHAT_TRYCATCH_CRASHEND(x)
	#else
		#define ANYCHAT_TRYCATCH_END(x)		ANYCHAT_TRYCATCH_NORMALEND
	#endif //XCrashReport
#endif //_DEBUG
#else
	#define ANYCHAT_TRYCATCH_BEGIN          try{	ANYCHAT_EXCEPTION_CLEAR;
	#define ANYCHAT_TRYCATCH_END(x)         }catch(...){	ANYCHAT_EXCEPTION_SIGN;	}
#endif

#ifdef WIN32
#	define ANYCHAT_EXCEPTION_TRY		__try
#	define ANYCHAT_EXCEPTION_CATCH		__except(EXCEPTION_EXECUTE_HANDLER)
#else
#	define ANYCHAT_EXCEPTION_TRY		try
#	define ANYCHAT_EXCEPTION_CATCH		catch(...)
#endif


#if defined( APPLEOS )
#   define BREAK_ON_ERROR(x)   if((x) != noErr)    break;
#else
#   define BREAK_ON_ERROR(x)   if((x) != AC_ERROR_SUCCESS)    break;
#endif

#define ACMAX(a,b) ((a) > (b) ? (a) : (b))
#define ACMAX3(a,b,c) ACMAX(ACMAX(a,b),c)
#define ACMIN(a,b) ((a) > (b) ? (b) : (a))
#define ACMIN3(a,b,c) ACMIN(ACMIN(a,b),c)


#define CONNECTID_MAKE(ip, port, style)	MAKEINT64(ip, MAKELONG((WORD)port, (WORD)style))
#define CONNECTID_GETIP(x)		LODWORD(x)
#define CONNECTID_GETPORT(x)	LOWORD(HIDWORD(x))
#define CONNECTID_GETSTYLE(x)	HIWORD(HIDWORD(x))


#endif // ANYCHAT_MACRODEF_H_INCLUDED
