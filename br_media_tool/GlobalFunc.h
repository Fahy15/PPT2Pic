
#pragma once
#include "Singleton.h"
#define _GLIBCXX_USE_WSTRING
#include <string>
#include <list>
#include <vector>


using namespace std;
#define SAFE_DELETE_POINTER(p) if(p!=NULL) {delete p; p=NULL;}
#define SAFE_DELETE_POINTER_ARRAY(p) if(p!=NULL) {delete[] p; p=NULL;}

class CGlobalFunc:public Singleton<CGlobalFunc>
{
	DECLARE_SINGLETON(CGlobalFunc)
private:
	CGlobalFunc();
	~CGlobalFunc();
public:	
	bool GetExePath(TCHAR *szPath);


	Image* GetPngImage(UINT resID);

	//设置CComboBox默认选择
	bool SetComboxDefaultSel(CComboBox *pComboCtrl, DWORD nDefaultVal);


	BOOL IfValidIP(DWORD dwIp);

	//检查是否有非法字符
	bool Path_CheckIllegalChar(const CString &strInput);

	TCHAR*	MyI2A(INT32 iSrc, TCHAR* szDst);
	INT32	MyA2I(TCHAR* szSrc);


	bool NotInList(vector<int> &lstData, int nID);
	bool NotInList(list<int> &lstData, int nID);


	bool CheckChinese(TCHAR* szInput);

	
private:
	//CSplashDlg	m_splashDlg;
};

#define MYI2A(INT_V,STR_V) INSTANCE(CGlobalFunc)->MyI2A(INT_V,STR_V)
#define MYA2I(STR_V) INSTANCE(CGlobalFunc)->MyA2I(STR_V)

const DWORD ColorOfBackGround = Color::MakeARGB(255,255,255,255);

const DWORD ColorOfTree = RGB(248,248,248);


//定义消息返回类型
