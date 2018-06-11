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


//************************************
// Method:    Brppt_InitConvert
// FullName:  Brppt_InitConvert
// Access:    public 
// Returns:   BR_PPT2PIC_API bool
// Qualifier: ȫ�ֳ�ʼ��
//************************************
BR_PPT2PIC_API int Brppt_InitConvert();//��ʼ��com

//************************************
// Method:    Brppt_ReleaseConvert
// FullName:  Brppt_ReleaseConvert
// Access:    public 
// Returns:   BR_PPT2PIC_API void
// Qualifier: ȫ�ַ���ʼ��
//************************************
BR_PPT2PIC_API int Brppt_ReleaseConvert();


BR_PPT2PIC_API int Brppt_GetPptHandle(PPT_HANDLE* pHandle);


//************************************
// Method:    Brppt_ReleasePptHandle
// FullName:  Brppt_ReleasePptHandle
// Access:    public 
// Returns:   BR_PPT2PIC_API int
// Qualifier: �ͷ�PPT���
// Parameter: PPT_HANDLE handle
//************************************
BR_PPT2PIC_API int Brppt_ReleasePptHandle(PPT_HANDLE handle);



//************************************
// Method:    Brppt_SetPowerPoint
// FullName:  Brppt_SetPowerPoint
// Access:    public 
// Returns:   BR_PPT2PIC_API int
// Qualifier: ����PPT�ļ���ȫ·��
// Parameter: PPT_HANDLE handle
// Parameter: char * szInFullPath
//************************************
BR_PPT2PIC_API int Brppt_SetPowerPoint(PPT_HANDLE handle, char* szInFullPath);


//************************************
// Method:    Brppt_GetTotalSliderCount
// FullName:  Brppt_GetTotalSliderCount
// Access:    public 
// Returns:   BR_PPT2PIC_API int
// Qualifier:ȡ�õ�ǰpptһ������ҳ
// Parameter: PPT_HANDLE handle
// Parameter: int * i32Count ���ص�ҳ��
//************************************
BR_PPT2PIC_API int Brppt_GetTotalSliderCount(PPT_HANDLE handle, int *i32Count);



//************************************
// Method:    Brppt_PptExportByIndex
// FullName:  Brppt_PptExportByIndex
// Access:    public 
// Returns:   BR_PPT2PIC_API int
// Qualifier:  ������ҳpptΪͼƬ
// Parameter: PPT_HANDLE handle
// Parameter: char * szOutPath Ŀ���ļ�·��
// Parameter: char * szOutFileName Ŀ���ļ�����
// Parameter: int width ת��ͼƬ�Ŀ��
// Parameter: int height ת��ͼƬ�ĸ߶�
// Parameter: int index Ҫת���ڼ�ҳ
//************************************
BR_PPT2PIC_API int Brppt_PptExportByIndex(PPT_HANDLE handle, char* szOutPath, char* szOutFileName, int width, int height, int index);


BR_PPT2PIC_API int Brppt_PptExportAll(PPT_HANDLE handle, char* szOutPath, char* szOutFileName, int width, int height);



#endif 
