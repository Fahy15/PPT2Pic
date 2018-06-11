// ppt2pic.cpp : 定义 DLL 应用程序的导出函数。
//
#include "ppt2pic.h"
#include "PowerPointTransform.h"
int g_iIsComInited = 0; 

BR_PPT2PIC_API int Brppt_InitConvert()
{
	if (0 == g_iIsComInited)
	{
		PowerPointTransform::InitCom();
		g_iIsComInited = 1;
	}
	
	return BR_PPT_SUCCESS;	
}

BR_PPT2PIC_API int Brppt_ReleaseConvert()
{
	if (0 == g_iIsComInited)
	{
		return BR_PPT_INITCONVERT_NOT_INIT;
	}

	PowerPointTransform::ReleaseCom();
	g_iIsComInited = 0;
	return BR_PPT_SUCCESS;
}


BR_PPT2PIC_API int Brppt_GetPptHandle(PPT_HANDLE* pHandle)
{
	PowerPointTransform* pPPT = new PowerPointTransform();
	*pHandle = (PPT_HANDLE)pPPT;
	
	return BR_PPT_SUCCESS;
}


BR_PPT2PIC_API int Brppt_ReleasePptHandle(PPT_HANDLE handle)
{
	PowerPointTransform *pPPT = (PowerPointTransform*)handle;
	if (!pPPT->IsValid())
	{
		return BR_PPT_HANDLE_NOT_VALID;
	}

	pPPT->Release();
	delete pPPT;

	return BR_PPT_SUCCESS;
}



BR_PPT2PIC_API int Brppt_SetPowerPoint(PPT_HANDLE handle, char* szInFullPath)
{
	PowerPointTransform *pPPT = (PowerPointTransform*)handle;
	if (!pPPT->IsValid())
	{
		return BR_PPT_HANDLE_NOT_VALID;
	}

	pPPT->setPowerPoint(szInFullPath);

	return BR_PPT_SUCCESS;
}


BR_PPT2PIC_API int Brppt_GetTotalSliderCount(PPT_HANDLE handle, int *i32Count)
{
	PowerPointTransform *pPPT = (PowerPointTransform*)handle;
	if (!pPPT->IsValid())
	{
		return BR_PPT_HANDLE_NOT_VALID;
	}

	*i32Count = pPPT->getTotalSliderCount();
	return BR_PPT_SUCCESS;
}

BR_PPT2PIC_API int Brppt_PptExportAll(PPT_HANDLE handle, char* szOutPath, char* szOutFileName, int width, int height)
{
	PowerPointTransform *pPPT = (PowerPointTransform*)handle;
	if (!pPPT->IsValid())
	{
		return BR_PPT_HANDLE_NOT_VALID;
	}

	pPPT->pptExportAll(szOutPath, szOutFileName, "JPG", width, height);

	return BR_PPT_SUCCESS;
}


