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

	if (nSize>nLen)
	{
		nSize=nLen;
	}
	nSize=WideCharToMultiByte(
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

inline LPWSTR char2wchar(LPWSTR dst,LPCSTR src,int len)
{
	int nResult=0;

	nResult = MultiByteToWideChar(
		CP_ACP,    
		MB_PRECOMPOSED,
		src,
		(int) strlen(src)+1,
		NULL,
		0);

	nResult = MultiByteToWideChar(
		CP_ACP,    
		0,
		src,
		(int) strlen(src)+1,
		dst,
		len);

	return nResult==0 ? NULL:dst;

}