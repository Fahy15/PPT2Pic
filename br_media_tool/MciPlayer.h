#include<windows.h>
#include<mmsystem.h>
#include<stdio.h>

#pragma comment(lib,"winmm.lib")

class CMciPlayer
{
public:
	CMciPlayer();
	~CMciPlayer();


	DWORD MCIOpen(const CString &strPath);

	DWORD MCIClose();

	DWORD MCIPlay();

	DWORD MCIPause();

	DWORD MCIGetLength();

private:

	UINT m_DeviceID;

	int m_dLength;

};

