#pragma once

#include <string>
#include <vector>
#include "msppt.tlh"

#define LOG(format, ...) {\
	FILE *pf = NULL;\
	pf = fopen("ppt2pic.log", "at");\
	fprintf(pf, format, __VA_ARGS__);\
	fclose(pf);\
	pf=NULL;\
}

class PowerPointTransform
{
public:
	PowerPointTransform(void);
	~PowerPointTransform(void);

	static bool InitCom();
	static void ReleaseCom();

	bool IsValid();
	bool Release();
	void setPowerPoint(char* fileName);
	const wchar_t* powerPoint();

	int getTotalSliderCount();
	int pptExportAll(char* path, 
		char* fileName, 
		char* filterName, 
		int width, int height );

private:
	std::wstring				m_powerPointFile;

public:
	int m_iMagicValid;

};
