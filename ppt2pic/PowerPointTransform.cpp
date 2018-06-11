//#include <afxdisp.h>
//#include <Windows.h>
#include <comdef.h>
#include <io.h>

#include "PowerPointTransform.h"
#include "common.h"

#define  VALID_MAGIC 12345

bool PowerPointTransform::InitCom()
{
	HRESULT hr = CoInitialize(0);

	if (FAILED(hr))
	{
		_com_error e(hr);
		MessageBox(NULL, e.ErrorMessage(), L"com初始化失败", MB_OK);  
		return false;
	}

	return true;
}

void PowerPointTransform::ReleaseCom()
{
	CoUninitialize();
}

PowerPointTransform::PowerPointTransform(void)
{
	m_iMagicValid = VALID_MAGIC;
}

PowerPointTransform::~PowerPointTransform(void)
{
}


bool PowerPointTransform::IsValid()
{
	if (m_iMagicValid == VALID_MAGIC)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool PowerPointTransform::Release()
{
	return true;
}


void PowerPointTransform::setPowerPoint( char *fileName )
{
	wchar_t szwFileName[MAX_PATH] = {0};
	char2wchar(szwFileName, fileName, strlen(fileName));
	m_powerPointFile = szwFileName;
}


int PowerPointTransform::pptExportAll( char* path, 
									char* fileName, 
									char* filterName, 
									int width, int height)
{
	wchar_t szwPath[MAX_PATH] = {0};
	wchar_t szwfileName[MAX_PATH] = {0};
	wchar_t szwfilterName[MAX_PATH] = {0};

	char2wchar(szwPath, path, strlen(path));
	char2wchar(szwfileName, fileName, strlen(fileName));
	char2wchar(szwfilterName, filterName, strlen(filterName));


	wchar_t uprFilterName[MAX_PATH] = {0};
	wcscpy_s(uprFilterName, _countof(uprFilterName), szwfilterName);
	_wcsupr_s(uprFilterName, _countof(uprFilterName));
	if ( wcscmp(uprFilterName, L"JPG") !=0
		&& wcscmp(uprFilterName, L"PNG") !=0 
		&& wcscmp(uprFilterName, L"TIF") !=0 
		&& wcscmp(uprFilterName, L"BMP") !=0
		&& wcscmp(uprFilterName, L"GIF") !=0
		)
	{
		return false;
	}

	DWORD begin = GetTickCount();

	MSPpt::_ApplicationPtr m_application;
	HRESULT hr = m_application.CreateInstance(__uuidof(MSPpt::Application));

	if (FAILED(hr))
	{
		_com_error e(hr);
		MessageBox(NULL, e.ErrorMessage(), L"错误提示", MB_OK);   
		return -1;
	}

	MSPpt::PresentationsPtr presentations = m_application->GetPresentations();
	try
	{
		MSPpt::_PresentationPtr myPresentation = presentations->Open(m_powerPointFile.c_str(), Office::msoCTrue, Office::msoFalse,Office::msoFalse);
		if (NULL == myPresentation)
		{
			return -1;
		}

		MSPpt::SlidesPtr sliders = myPresentation->GetSlides();
		int count = sliders->GetCount();
		for (int i = 1; i <= count; ++i)
		{
			_variant_t index = long(i);
			MSPpt::_SlidePtr slide = sliders->Item(index);

			wchar_t tempPath[MAX_PATH]={0};
			swprintf_s(tempPath, _countof(tempPath), L"%s\\%s%d.%s", szwPath, szwfileName, i, _wcslwr_s(uprFilterName,sizeof(uprFilterName)));
			slide->Export(tempPath, uprFilterName, width, height);
		}

		myPresentation->Close();
		m_application->Quit();

	}
	catch (_com_error& e)
	{
		MessageBox(0, e.Description(), e.ErrorMessage(), 0);
		return -1;
	}

	DWORD end = GetTickCount() - begin;

	return 0;
}

const wchar_t* PowerPointTransform::powerPoint()
{
	return m_powerPointFile.c_str();
}


int PowerPointTransform::getTotalSliderCount()
{
	MSPpt::_ApplicationPtr m_application;
	HRESULT hr = m_application.CreateInstance(__uuidof(MSPpt::Application));

	if (FAILED(hr))
	{
		_com_error e(hr);
		MessageBox(NULL, e.ErrorMessage(), L"错误提示", MB_OK);  
		return -1;
	}

	try
	{
	MSPpt::PresentationsPtr presentations = m_application->GetPresentations();

		MSPpt::_PresentationPtr myPresentation = presentations->Open(m_powerPointFile.c_str(), Office::msoCTrue, Office::msoFalse,Office::msoFalse);
		if (NULL == myPresentation)
		{
			return -1;
		}

		MSPpt::SlidesPtr sliders = myPresentation->GetSlides();
		long count = sliders->GetCount();

		myPresentation->Close();
		m_application->Quit();
		return count;
	}
	catch (_com_error& e)
	{
		MessageBox(0, e.Description(), e.ErrorMessage(), 0);
	}

	return -1;
}







