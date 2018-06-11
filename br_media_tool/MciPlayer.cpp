#include "stdafx.h"
#include "MciPlayer.h"
#include <assert.h>

CMciPlayer::CMciPlayer()
{
	m_DeviceID = 0;
	m_dLength = 0;
}

CMciPlayer::~CMciPlayer()
{
}

DWORD CMciPlayer::MCIOpen(const CString &strPath)
{
	MCI_OPEN_PARMS mciOP;
	DWORD dwReturn;

	assert(m_DeviceID == 0);
	mciOP.lpstrDeviceType = NULL;
	mciOP.lpstrElementName = strPath;
	dwReturn = mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_WAIT | MCI_OPEN_SHAREABLE,
		(DWORD)(LPVOID)&mciOP);
	if (dwReturn == 0)
	{
		m_DeviceID = mciOP.wDeviceID;
		this->MCIGetLength();
	}
	else
		m_DeviceID = 0;
	return dwReturn;
}
//关闭MCI设备
DWORD CMciPlayer::MCIClose()
{
	MCI_GENERIC_PARMS mciGP;
	DWORD dwReturn;
	dwReturn = mciSendCommand(m_DeviceID, MCI_CLOSE, MCI_NOTIFY | MCI_WAIT,(DWORD)(LPVOID)&mciGP);
	m_DeviceID = NULL;
	return dwReturn;
}
//播放
DWORD CMciPlayer::MCIPlay()
{
	MCI_PLAY_PARMS mciPP;
	return mciSendCommand(m_DeviceID, MCI_PLAY, MCI_NOTIFY,(DWORD)(LPVOID)&mciPP);
}

DWORD CMciPlayer::MCIPause()
{
	return mciSendCommand(m_DeviceID, MCI_PAUSE, NULL, NULL);
}

DWORD CMciPlayer::MCIGetLength()
{
	DWORD dwReturn;
	MCI_STATUS_PARMS mciStatusParms;

	mciStatusParms.dwItem = MCI_STATUS_LENGTH; 
	dwReturn = mciSendCommand(m_DeviceID, MCI_STATUS, MCI_WAIT | MCI_STATUS_ITEM, (DWORD)(LPVOID)&mciStatusParms);//关键,取得长度

	m_dLength = mciStatusParms.dwReturn;
	return m_dLength;
}
