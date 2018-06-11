#include "stdafx.h"
#include "GlobalFunc.h"
#include "common.h"
#define _GLIBCXX_USE_WSTRING
#include <string>


CGlobalFunc::CGlobalFunc()
{
	
}
CGlobalFunc::~CGlobalFunc()
{

}


bool CGlobalFunc::GetExePath(TCHAR* szPath)
{
	HMODULE hm = GetModuleHandle(_T("HMCUI.DLL"));
	int iPathlen = GetModuleFileName(hm, szPath, MAX_PATH);
	if (iPathlen > 0)
	{
		while(TRUE)
		{
			if(szPath[iPathlen--]==_T('\\'))
				break;
		}
		szPath[++iPathlen]=   0x0;
		return true;
	}
	else if (iPathlen == ERROR_INSUFFICIENT_BUFFER)
	{
	}
	return false;
}

Image *CGlobalFunc::GetPngImage(UINT resID)
{
	Image *pimg = NULL;

	HGLOBAL         hGlobal = NULL;
	HRSRC         hSource = NULL;
	LPVOID         lpVoid  = NULL;
	int             nSize   = 0;

	hSource = FindResource(AfxGetResourceHandle(), MAKEINTRESOURCE(resID), _T("PNG")); // 导入自定义资源时的 类型“PNG”

	if(hSource == NULL)
		return pimg;

	hGlobal = LoadResource(AfxGetResourceHandle(), hSource);
	if(hGlobal == NULL)
	{    
		FreeResource(hGlobal);       
		return pimg;        
	}

	lpVoid = LockResource(hGlobal);
	if(lpVoid == NULL)
		goto Exit;

	nSize = (UINT)SizeofResource(AfxGetResourceHandle(), hSource);

	HGLOBAL hGlobal2 = GlobalAlloc(GMEM_MOVEABLE, nSize);
	if(hGlobal2 == NULL)
	{
		goto Exit;
	}

	void* pData = GlobalLock(hGlobal2);
	memcpy(pData, (void *)hGlobal, nSize);
	GlobalUnlock(hGlobal2);

	IStream* pStream = NULL;
	if(CreateStreamOnHGlobal(hGlobal2, TRUE, &pStream) == S_OK)
	{
		pimg = Image::FromStream(pStream, FALSE);
		pStream->Release();
	}
	FreeResource(hGlobal2); 
Exit:
	UnlockResource(hGlobal);    
	FreeResource(hGlobal); 
	return pimg;
}

bool CGlobalFunc::SetComboxDefaultSel(CComboBox *pComboCtrl, DWORD nDefaultVal)
{
	if (pComboCtrl == NULL)
		return false;
	int iCount = pComboCtrl->GetCount();
	for (int i=0; i<iCount; i++)
	{
		if (nDefaultVal == pComboCtrl->GetItemData(i))
		{
			pComboCtrl->SetCurSel(i);
			return true;
		}
	}
	return false;
}


TCHAR* CGlobalFunc::MyI2A(int iSrc, TCHAR* szDst)
{
#ifdef _UNICODE
	szDst =  _itow(iSrc, szDst, 10);
#else
	szDst =  itoa(iSrc, szDst, 10);
#endif
	return szDst;
}
INT32	CGlobalFunc::MyA2I(TCHAR* szSrc)
{
#ifdef _UNICODE
	return _wtoi(szSrc);
#else
	return atoi(szSrc);
#endif
}

// 合法IP判断
BOOL CGlobalFunc::IfValidIP(DWORD dwIp)
{
	// 全0 或 全 1 都是不合法IP
	if ( 0 == dwIp)
	{
		return FALSE;
	}
	if ( 4294967295 == dwIp ) //255.255.255.255
	{
		return FALSE;
	}

	// 尾段为0或255也是不合法的IP
	DWORD iFour = LOBYTE(LOWORD(dwIp));
	if ( 0== iFour || 255 == iFour)
	{
		return FALSE;
	}

	return TRUE;
}

//路径,检查是否有非法字符
bool CGlobalFunc::Path_CheckIllegalChar(const CString &strInput)
{
	TCHAR szIllegal[] = {_T("/\\:*?\"<>|@￥#%&~ 《》")};
	int ilen = _tcslen(szIllegal);
	for (int i=0; i<ilen; i++)
	{
		if (strInput.Find(szIllegal[i])>=0)
		{
			return true;
		}
	}
	return false;
}

//判断数据是否存在列表中

bool CGlobalFunc::NotInList(vector<int> &lstData, int nID)
{
	if (lstData.size() == 0)
	{
		return true;
	}

	vector<int>::iterator it = std::find(lstData.begin(), lstData.end(), nID);
	if (it != lstData.end()) // 找到了
	{
		return false;
	}
	return true;
}
bool CGlobalFunc::NotInList(list<int> &lstData, int nID)
{
	if (lstData.size() == 0)
	{
		return true;
	}

	list<int>::iterator it = std::find(lstData.begin(), lstData.end(), nID);
	if (it != lstData.end()) // 找到了
	{
		return false;
	}
	return true;
}


bool CGlobalFunc::CheckChinese(TCHAR* szInput)
{
	int iLen = _tcslen(szInput);
	for(int i=0; i < iLen; i++)
	{
		if(IsDBCSLeadByte(szInput[i]))
		{
			return true;
		}
	}
	return false;

}


