#ifndef _BR_PPT2PIC_H_
#define _BR_PPT2PIC_H_

#define BR_PPT2PIC_EXPORTS

#ifdef BR_PPT2PIC_EXPORTS
#define BR_PPT2PIC_API extern "C" __declspec(dllexport)
#else
#define BR_PPT2PIC_API extern "C" __declspec(dllimport)
#endif 


typedef void *PPT_HANDLE; 


#define BR_PPT_SUCCESS 					        0	//操作成功
#define BR_PPT_FILE_NOT_EXIST			        1	//文件不存在
#define BR_PPT_UNKNOWN_ERROR				    2	//未知错误
#define BR_PPT_HANDLE_NOT_VALID                 3   //句柄无效
#define BR_PPT_INITCONVERT_NOT_INIT             4   //未初始化


//************************************
// Method:    Brppt_InitConvert
// FullName:  Brppt_InitConvert
// Access:    public 
// Returns:   BR_PPT2PIC_API bool
// Qualifier: 全局初始化
//************************************
BR_PPT2PIC_API int Brppt_InitConvert();//初始化com

//************************************
// Method:    Brppt_ReleaseConvert
// FullName:  Brppt_ReleaseConvert
// Access:    public 
// Returns:   BR_PPT2PIC_API void
// Qualifier: 全局反初始化
//************************************
BR_PPT2PIC_API int Brppt_ReleaseConvert();


BR_PPT2PIC_API int Brppt_GetPptHandle(PPT_HANDLE* pHandle);


//************************************
// Method:    Brppt_ReleasePptHandle
// FullName:  Brppt_ReleasePptHandle
// Access:    public 
// Returns:   BR_PPT2PIC_API int
// Qualifier: 释放PPT句柄
// Parameter: PPT_HANDLE handle
//************************************
BR_PPT2PIC_API int Brppt_ReleasePptHandle(PPT_HANDLE handle);



//************************************
// Method:    Brppt_SetPowerPoint
// FullName:  Brppt_SetPowerPoint
// Access:    public 
// Returns:   BR_PPT2PIC_API int
// Qualifier: 设置PPT文件的全路径
// Parameter: PPT_HANDLE handle
// Parameter: char * szInFullPath
//************************************
BR_PPT2PIC_API int Brppt_SetPowerPoint(PPT_HANDLE handle, char* szInFullPath);


//************************************
// Method:    Brppt_GetTotalSliderCount
// FullName:  Brppt_GetTotalSliderCount
// Access:    public 
// Returns:   BR_PPT2PIC_API int
// Qualifier:取得当前ppt一共多少页
// Parameter: PPT_HANDLE handle
// Parameter: int * i32Count 返回的页数
//************************************
BR_PPT2PIC_API int Brppt_GetTotalSliderCount(PPT_HANDLE handle, int *i32Count);



//************************************
// Method:    Brppt_PptExportByIndex
// FullName:  Brppt_PptExportByIndex
// Access:    public 
// Returns:   BR_PPT2PIC_API int
// Qualifier:  导出单页ppt为图片
// Parameter: PPT_HANDLE handle
// Parameter: char * szOutPath 目标文件路径
// Parameter: char * szOutFileName 目标文件名称
// Parameter: int width 转换图片的宽度
// Parameter: int height 转换图片的高度
// Parameter: int index 要转换第几页
//************************************
BR_PPT2PIC_API int Brppt_PptExportByIndex(PPT_HANDLE handle, char* szOutPath, char* szOutFileName, int width, int height, int index);


BR_PPT2PIC_API int Brppt_PptExportAll(PPT_HANDLE handle, char* szOutPath, char* szOutFileName, int width, int height);



#endif 
