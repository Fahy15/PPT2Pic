#ifndef _BR_PPT2PIC_H_
#define _BR_PPT2PIC_H_

#define BR_PPT2PIC_EXPORTS

#ifdef BR_PPT2PIC_EXPORTS
#define BR_PPT2PIC_API extern "C" __declspec(dllexport)
#else
#define BR_PPT2PIC_API extern "C" __declspec(dllimport)
#endif 


typedef void *PPT_HANDLE; 


#define BR_PPT_SUCCESS 					        0	//�����ɹ�
#define BR_PPT_FILE_NOT_EXIST			        1	//�ļ�������
#define BR_PPT_UNKNOWN_ERROR				    2	//δ֪����
#define BR_PPT_HANDLE_NOT_VALID                 3   //�����Ч
#define BR_PPT_INITCONVERT_NOT_INIT             4   //δ��ʼ��


BR_PPT2PIC_API int Brppt_InitConvert();

BR_PPT2PIC_API int Brppt_ReleaseConvert();


BR_PPT2PIC_API int Brppt_GetPptHandle(PPT_HANDLE* pHandle);

BR_PPT2PIC_API int Brppt_ReleasePptHandle(PPT_HANDLE handle);

BR_PPT2PIC_API int Brppt_SetPowerPoint(PPT_HANDLE handle, char* szInFullPath);
BR_PPT2PIC_API int Brppt_GetTotalSliderCount(PPT_HANDLE handle, int *i32Count);
BR_PPT2PIC_API int Brppt_PptExportAll(PPT_HANDLE handle, char* szOutPath, char* szOutFileName, int width, int height);



#endif 
