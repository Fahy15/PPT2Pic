
// br_media_tool.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// Cbr_media_toolApp: 
// �йش����ʵ�֣������ br_media_tool.cpp
//

class Cbr_media_toolApp : public CWinApp
{
public:
	Cbr_media_toolApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern Cbr_media_toolApp theApp;