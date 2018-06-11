
// br_media_toolDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "br_media_tool.h"
#include "br_media_toolDlg.h"
#include "afxdialogex.h"
#include "common.h"
#include "json.h"
#include <fstream>
#include "fileassist.h"
#include "zip_utils.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#pragma comment(lib, "ppt2pic.lib")
using namespace AnyChat;



// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// Cbr_media_toolDlg �Ի���



Cbr_media_toolDlg::Cbr_media_toolDlg(CWnd* pParent /*=NULL*/)
: CDialogEx(Cbr_media_toolDlg::IDD, pParent)
	, m_editVoiceValue(_T(""))
	, m_editPptValue(_T(""))
	, m_editStartTimeValue(_T(""))
	, m_editEndTimeValue(_T(""))
	, m_staticTotalSecond(_T(""))
	, m_static_current_play(_T(""))
	, m_static_pptInfo(_T(""))
	, m_static_pack_tooltip(_T(""))
	, m_staticSelectResolution(_T(""))
{

	//Gdiplus::GdiplusStartup(&m_pGdiToken, &m_gdiplusStartupInput, NULL);

	//�ݲ���Ҫ�ͷ�
	//Gdiplus::GdiplusShutdown(m_gdiplusToken);

	m_handlePpt = NULL;
	m_i32PptTotalPage = 0;
	m_i32CurrentPlaySecond = 0;
	m_bIsPreview = false;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_bIsNeedInvokeEvent = true;

	m_cstrCurrPicPath = _T("");

	m_i32Resolution = 2;
}

void Cbr_media_toolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_editVoice, m_editVoice);
	DDX_Text(pDX, IDC_editVoice, m_editVoiceValue);
	DDX_Text(pDX, IDC_editPPT, m_editPptValue);
	DDX_Control(pDX, IDC_comboSelectPage, m_comboPptPage);
	DDX_Text(pDX, IDC_editStartTime, m_editStartTimeValue);
	DDX_Text(pDX, IDC_editEndTime, m_editEndTimeValue);
	DDX_Control(pDX, IDC_lstPptConfig, m_lstPptConfig);
	DDX_Control(pDX, IDC_STATIC1, m_picControl);
	DDX_Text(pDX, IDC_STATIC3, m_staticTotalSecond);
	DDX_Control(pDX, IDC_btnPreview, m_btnPreview);
	DDX_Text(pDX, IDC_static_current_play, m_static_current_play);
	DDX_Text(pDX, IDC_static_pptInfo, m_static_pptInfo);
	DDX_Text(pDX, IDC_static_pack_tooltip, m_static_pack_tooltip);
	DDX_Control(pDX, IDC_comboSelectResolution, m_comboSelectResolution);
	DDX_Text(pDX, IDC_static_SelectResolution, m_staticSelectResolution);
	DDX_Control(pDX, IDC_static_current_play, m_staticCtlCurrentPlay);
}

BEGIN_MESSAGE_MAP(Cbr_media_toolDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_btnReset, &Cbr_media_toolDlg::OnBnClickedbtnreset)
	ON_BN_CLICKED(IDC_btnPreview, &Cbr_media_toolDlg::OnBnClickedbtnpreview)
	ON_BN_CLICKED(IDC_btnPack, &Cbr_media_toolDlg::OnBnClickedbtnpack)
	ON_BN_CLICKED(IDC_btnSelectVoiceFile, &Cbr_media_toolDlg::OnBnClickedbtnselectvoicefile)
	ON_BN_CLICKED(IDC_btnSelectPPT, &Cbr_media_toolDlg::OnBnClickedbtnselectppt)
	ON_CBN_SELCHANGE(IDC_comboSelectPage, &Cbr_media_toolDlg::OnCbnSelchangecomboselectpage)
	ON_BN_CLICKED(IDC_btnSavePptItem, &Cbr_media_toolDlg::OnBnClickedbtnsavepptitem)
	ON_WM_TIMER()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_lstPptConfig, &Cbr_media_toolDlg::OnLvnItemchangedlstpptconfig)
	ON_NOTIFY(NM_THEMECHANGED, IDC_lstPptConfig, &Cbr_media_toolDlg::OnNMThemeChangedlstpptconfig)
	ON_WM_ERASEBKGND()
	ON_CBN_SELCHANGE(IDC_comboSelectResolution, &Cbr_media_toolDlg::OnCbnSelchangecomboselectresolution)
END_MESSAGE_MAP()


// Cbr_media_toolDlg ��Ϣ�������

BOOL Cbr_media_toolDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	//ShowWindow(SW_MAXIMIZE);

	CRect cr;
	GetClientRect(&cr);//��ȡ�Ի���ͻ������С   
	ClientToScreen(&cr);//ת��ΪӫĻ����   
	int x = GetSystemMetrics(SM_CXSCREEN);//��ȡӫĻ����Ŀ�ȣ���λΪ����   
	int y = GetSystemMetrics(SM_CYSCREEN);//��ȡӫĻ����ĸ߶ȣ���λΪ����   
	//MoveWindow((x-cr.Width() *2)/2 ,cr.top,cr.Width() *2,cr.Height() *2);//���ô�����ʾ��λ���Լ���С   
	MoveWindow(x/2 - 215, 50, 435, 640);//���ô�����ʾ��λ���Լ���С 

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������

	int iRet = 0;
	iRet = Brppt_InitConvert();
	ASSERT(iRet == 0);
	iRet = Brppt_GetPptHandle(&m_handlePpt);
	ASSERT(iRet == 0);


	//m_lstPptConfig.SetExtendedStyle(
	//	LVS_EX_FLATSB    // ��ƽ������
	//	| LVS_EX_HEADERDRAGDROP    // ���������ק
	//	| LVS_EX_GRIDLINES    // ����������
	//	| LVS_EX_FULLROWSELECT //ȫѡ
	//	);
	//m_lstPptConfig.InsertColumn(0, _T("����ͨ��"), LVCFMT_CENTER, 80);
	//m_lstPptConfig.InsertColumn(1, _T("�ָ�ģʽ(˫���޸�)"), LVCFMT_CENTER, 162);
	//m_lstPptConfig.SetColumnStyle(1, GXLISTCTRL_ITEMSTYLE_COMBO);

	//bind combo box
	int i32Index = 0;
	CString csComboString;
	for (int i = 3; i > 0; i--)
	{
		if (i == 3)
		{
			csComboString.Format(_T("320*240"));
		}
		else if (i == 2)
		{
			csComboString.Format(_T("640*480"));
		}
		else
		{
			csComboString.Format(_T("800*600"));
		}
		
		m_comboSelectResolution.InsertString(i32Index, csComboString.GetBuffer());
		m_comboSelectResolution.SetItemData(i32Index, i);
		i32Index++;
	}
	m_comboSelectResolution.SetCurSel(1);





	m_lstPptConfig.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_lstPptConfig.InsertColumn(0, _T("ҳ��"), 80, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByDigit);
	m_lstPptConfig.InsertColumn(1, _T("��ʼʱ��(˫���༭)"), 140, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByDigit);
	m_lstPptConfig.InsertColumn(2, _T("����ʱ��(˫���༭)"), 140, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByDigit);

	m_lstPptConfig.SetHeaderGradientColor(LISTCTRLHEADCOLOR_BEGIN, LISTCTRLHEADCOLOR_END);
	m_lstPptConfig.SetHeaderHeight(LISTCTRLHEADHEIGHT);
	m_lstPptConfig.SetRowHeigt(LISTCTRLROWHEIGHT);
	m_lstPptConfig.SetSupportSort();

	m_Font1.CreatePointFont(200, _T("Arial"), NULL);
	m_Font2.CreatePointFont(100, _T("Arial"), NULL);
	m_Font3.CreatePointFont(100, _T("Arial"), NULL);
	((CStatic*)GetDlgItem(IDC_static_current_play))->SetFont(&m_Font1);
	((CStatic*)GetDlgItem(IDC_static_pack_tooltip))->SetFont(&m_Font1);
	((CStatic*)GetDlgItem(IDC_static_pptInfo))->SetFont(&m_Font2);
	((CStatic*)GetDlgItem(IDC_STATIC3))->SetFont(&m_Font2);

	((CStatic*)GetDlgItem(IDC_static_SelectResolution))->SetFont(&m_Font3);
	m_staticSelectResolution = _T("ѡ��ֱ���:");
	UpdateData(FALSE);


	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void Cbr_media_toolDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void Cbr_media_toolDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{

		//if (!img.IsNull())
		//{
		//	CRect rect(0, 0, 500, 500);
		//	//if (!img.IsNull()) img.Destroy();
		//	//img.Load(_T("E:\\�����ĵ�����\\1����Ƭ.jpg"));

		//	//m_picControl.GetClientRect(&rect); //���pictrue�ؼ����ڵľ�������
		//	CDC *pDc = m_picControl.GetDC();//���pictrue�ؼ���Dc
		//	SetStretchBltMode(pDc->m_hDC, STRETCH_HALFTONE);
		//	img.StretchBlt(pDc->m_hDC, rect, SRCCOPY);
		//	ReleaseDC(pDc);//�ͷ�picture�ؼ���Dc
		//}

		//Show_picture(m_cstrCurrPicPath);


		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR Cbr_media_toolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void Cbr_media_toolDlg::OnBnClickedbtnreset()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void Cbr_media_toolDlg::OnBnClickedbtnpreview()
{
	//judge isvalid
	if (m_editVoiceValue.GetLength() == 0)
	{
		MessageBox(_T("��ѡ�������ļ�"), _T("����"), MB_OK | MB_ICONERROR);
		return;
	}

	if (m_editPptValue.GetLength() == 0)
	{
		MessageBox(_T("��ѡ��PPT�ļ�"), _T("����"), MB_OK | MB_ICONERROR);
		return;
	}

	//judge isornot pack first
	char szTmp[MAX_PATH] = { 0 };

	memset(szTmp, 0, MAX_PATH);
	CString csAudioPath;
	csAudioPath.Format(_T(".\\br_mediafile\\audio\\"));
	TCHAR* pTmpW = csAudioPath.GetBuffer();
	wchar2char(szTmp, pTmpW, wcslen(pTmpW));

	if (!CFileAssist::IsDirectoryExist(szTmp))
	{
		MessageBox(_T("����ִ�д��"), _T("��ʾ"), MB_OK | MB_ICONINFORMATION);
		return;
	}

	memset(szTmp, 0, MAX_PATH);
	csAudioPath;
	csAudioPath.Format(_T(".\\br_mediafile\\ppt\\"));
	pTmpW = csAudioPath.GetBuffer();
	wchar2char(szTmp, pTmpW, wcslen(pTmpW));
	
	if (!CFileAssist::IsDirectoryExist(szTmp))
	{
		MessageBox(_T("����ִ�д��"), _T("��ʾ"), MB_OK | MB_ICONINFORMATION);
		return;
	}

	if (!m_bIsPreview)
	{
		m_picControl.UnLoad();
		m_picControl.Draw();
		m_picControl.Invalidate();
		m_picControl.UpdateWindow();

		m_bIsPreview = true;
		SetDlgItemTextW(IDC_btnPreview, _T("ֹͣԤ��"));
		m_mciPlayer.MCIPlay();
		SetTimer(1, 1000, NULL);

		//change windows size
		CRect cr;
		GetClientRect(&cr);//��ȡ�Ի���ͻ������С   
		ClientToScreen(&cr);//ת��ΪӫĻ����   
		int x = GetSystemMetrics(SM_CXSCREEN);//��ȡӫĻ����Ŀ�ȣ���λΪ����   
		int y = GetSystemMetrics(SM_CYSCREEN);//��ȡӫĻ����ĸ߶ȣ���λΪ����   
		//MoveWindow((x-cr.Width() *2)/2 ,cr.top,cr.Width() *2,cr.Height() *2);//���ô�����ʾ��λ���Լ���С 


		if (m_i32Resolution == 2)
		{
			m_staticCtlCurrentPlay.MoveWindow(611, 30, 240, 54);
			m_picControl.MoveWindow(422, 91, 508, 396);
			MoveWindow(x / 2 - 540, 50, 1088, 640);//���ô�����ʾ��λ���Լ���С 
		}

		if (m_i32Resolution == 3) //800*600
		{
			m_staticCtlCurrentPlay.MoveWindow(661, 30, 240, 54);
			m_picControl.MoveWindow(422, 91, 508, 396);
			MoveWindow(x / 2 - 620, 50, 1248, 760);//���ô�����ʾ��λ���Լ���С 
		}

		CRect rcPicShow;
		m_picControl.GetWindowRect(rcPicShow);
		ScreenToClient(rcPicShow);

		int tttt = 1;

		if (m_i32Resolution == 1)
		{
			m_staticCtlCurrentPlay.MoveWindow(480, 30, 240, 54);
			m_picControl.MoveWindow(422, 147, 508, 396);
			MoveWindow(x / 2 - 620, 50, 768, 640);//���ô�����ʾ��λ���Լ���С 
		}
	}
	else
	{
		KillTimer(1);
		m_i32CurrentPlaySecond = 0;
		m_bIsPreview = false;
		SetDlgItemTextW(IDC_btnPreview, _T("Ԥ��"));
		m_mciPlayer.MCIClose();
		m_mciPlayer.MCIOpen(m_csVoiceFilePath);

		m_static_current_play.Format(_T("��ǰ���ŵ�%d��"), m_i32CurrentPlaySecond);
		UpdateData(FALSE);

		//change windows size
		CRect cr;
		GetClientRect(&cr);//��ȡ�Ի���ͻ������С   
		ClientToScreen(&cr);//ת��ΪӫĻ����   
		int x = GetSystemMetrics(SM_CXSCREEN);//��ȡӫĻ����Ŀ�ȣ���λΪ����   
		int y = GetSystemMetrics(SM_CYSCREEN);//��ȡӫĻ����ĸ߶ȣ���λΪ����   
		//MoveWindow((x-cr.Width() *2)/2 ,cr.top,cr.Width() *2,cr.Height() *2);//���ô�����ʾ��λ���Լ���С   
		MoveWindow(x / 2 - 215, 50, 435, 640);//���ô�����ʾ��λ���Լ���С 
	}


	//m_picControl.Load(_T(".\\br_mediafile\\ppt\\1.JPG"));
	//m_picControl.Draw();
}


void Cbr_media_toolDlg::OnBnClickedbtnpack()
{
	//judge isvalid
	if (m_editVoiceValue.GetLength() == 0)
	{
		MessageBox(_T("��ѡ�������ļ�"), _T("����"), MB_OK | MB_ICONERROR);
		return;
	}

	if (m_editPptValue.GetLength() == 0)
	{
		MessageBox(_T("��ѡ��PPT�ļ�"), _T("����"), MB_OK | MB_ICONERROR);
		return;
	}

	int iSelResolution = m_comboSelectResolution.GetCurSel();
	int iDataResolution = m_comboPptPage.GetItemData(iSelResolution);
	m_i32Resolution = iDataResolution;

	//save ppt config
	CString csPptAddress;
	CString csStartTime;
	CString csEndTime;
	int i32StartTime;
	int i32EndTime;

	int i32ItemData;
	for (int i = 0; i < m_lstPptConfig.GetItemCount(); i++)
	{
		i32ItemData = m_lstPptConfig.GetItemData(i);
		csPptAddress.Format(_T("ppt/%d.jpg"), i32ItemData);

		csStartTime = m_lstPptConfig.GetItemText(i, 1);
		csEndTime = m_lstPptConfig.GetItemText(i, 2);

		i32StartTime = _ttoi(csStartTime);
		i32EndTime = _ttoi(csEndTime);

		STRUPPTITEM pptItem = { 0 };
		GS_SAFE_COPY(pptItem.ppt_address, MAX_PATH, csPptAddress.GetBuffer());
		pptItem.audio_start = i32StartTime;
		pptItem.audio_end = i32EndTime;

		//if (pptItem.audio_start == 0 && pptItem.audio_end == 0)
		//{
		//	CString cstrMsg;
		//	cstrMsg.Format(_T("����д�� %d ҳ�Ŀ�ʼʱ��ͽ���ʱ�� "), i32ItemData);
		//	MessageBox(cstrMsg.GetBuffer(), _T("����"), MB_OK | MB_ICONERROR);
		//	return;
		//	//continue;
		//}

		if (pptItem.audio_start < 0 || pptItem.audio_start > m_mciPlayer.MCIGetLength() / 1000)
		{
			CString cstrMsg;
			cstrMsg.Format(_T("�� %d ҳ�Ŀ�ʼʱ�䳬����Χ 0 - %d "), i32ItemData, m_mciPlayer.MCIGetLength() / 1000);
			MessageBox(cstrMsg.GetBuffer(), _T("����"), MB_OK | MB_ICONERROR);
			return;
		}

		if (pptItem.audio_end < 0 || pptItem.audio_end > m_mciPlayer.MCIGetLength() / 1000)
		{
			CString cstrMsg;
			cstrMsg.Format(_T("�� %d ҳ�Ľ���ʱ�䳬����Χ 0 - %d "), i32ItemData, m_mciPlayer.MCIGetLength() / 1000);
			MessageBox(cstrMsg.GetBuffer(), _T("����"), MB_OK | MB_ICONERROR);
			return;
		}

		if (pptItem.audio_end < pptItem.audio_start)
		{
			CString cstrMsg;
			cstrMsg.Format(_T("�� %d ҳ�Ľ���ʱ�䲻��С�ڿ�ʼʱ�� "), i32ItemData);
			MessageBox(cstrMsg.GetBuffer(), _T("����"), MB_OK | MB_ICONERROR);
			return;
		}


		//judge is repeat data
		map<int, STRUPPTITEM>::iterator it;
		for (it = m_mapPPT.begin(); it != m_mapPPT.end(); it++)
		{
			if (i32ItemData == it->first)
			{
				continue;
			}
			if (0 == i32StartTime && 0 == i32EndTime)
			{
				continue;
			}

			if ((i32StartTime >= it->second.audio_start && i32StartTime <= it->second.audio_end)
				|| (i32EndTime >= it->second.audio_start && i32EndTime <= it->second.audio_end))
			{
				CString cstrMsg;
				cstrMsg.Format(_T("�� %d ҳ�͵� %d ҳ����ֹʱ�����ص�,\n\n �� %d ҳ�Ŀ�ʼʱ��Ϊ��%d, ����ʱ��Ϊ��%d \n �� %d ҳ�Ŀ�ʼʱ��Ϊ��%d, ����ʱ��Ϊ��%d"),
					it->first, i32ItemData, it->first, it->second.audio_start, it->second.audio_end, i32ItemData, i32StartTime, i32EndTime);
				MessageBox(cstrMsg.GetBuffer(), _T("����"), MB_OK | MB_ICONERROR);
				return;
			}
		}

		m_mapPPT[i32ItemData] = pptItem;
	}

	//tooltip
	m_static_pack_tooltip.Format(_T("���ڴ����..."));
	UpdateData(FALSE);

	char szTmp[MAX_PATH] = { 0 };
	Json::Value root;

	memset(szTmp, 0, MAX_PATH);
	CString csAudioPath;
	csAudioPath.Format(_T("audio/%s"), m_csVoiceFileTitle);
	TCHAR* pTmpW = csAudioPath.GetBuffer();
	wchar2char(szTmp, pTmpW, wcslen(pTmpW));

	root["file_type"] = Json::Value(1);
	root["audio_address"] = Json::Value(szTmp);

	map<int, STRUPPTITEM>::iterator it;
	for (it = m_mapPPT.begin(); it != m_mapPPT.end(); it++)
	{
		Json::Value jsPptItem;

		memset(szTmp, 0, MAX_PATH);
		pTmpW = it->second.ppt_address;
		wchar2char(szTmp, pTmpW, wcslen(pTmpW));

		jsPptItem["ppt_address"] = Json::Value(szTmp);
		jsPptItem["audio_start"] = Json::Value(it->second.audio_start);
		jsPptItem["audio_end"] = Json::Value(it->second.audio_end);

		root["pptlist"].append(jsPptItem);
	}

	Json::StyledWriter styled_writer;
	string strReadme = styled_writer.write(root);

	//delete br_mediafile folder in current dir
	CFileAssist::RemoveDir(".\\br_mediafile\\");

	//delete br_mediafile.zip 
	CFileAssist::RemoveFile(".\\br_mediafile.zip");

	//create folder in current dir
	RecursionCreateDir(".\\br_mediafile\\audio\\");
	RecursionCreateDir(".\\br_mediafile\\ppt\\");


	// copy mp3 file
	memset(szTmp, 0, MAX_PATH);
	pTmpW = m_csVoiceFilePath.GetBuffer();
	wchar2char(szTmp, pTmpW, wcslen(pTmpW));

	string strDestFilePath = ".\\br_mediafile\\audio\\";
	string strDestFileName;
	GetFileName(strDestFileName, szTmp);
	strDestFilePath += strDestFileName;
	CFileAssist::CopySingleFile(szTmp, strDestFilePath.c_str());

	//GET PATH
	string strCurrentPath = GetAppPath();
	strCurrentPath += "\\br_mediafile\\ppt";

	int destWidth, destHeight;
	if (m_i32Resolution == 3)
	{
		destWidth = 800;
		destHeight = 600;
	}
	else if (m_i32Resolution == 2)
	{
		destWidth = 640;
		destHeight = 480;
	}
	else
	{
		destWidth = 320;
		destHeight = 240;
	}

	//convert ppt to jpg fileE:\br_project\br_media_tool\br_media_tool\br_mediafile   E:\\br_project\\br_media_tool\\br_media_tool\\br_mediafile\\ppt
	int iRet = Brppt_PptExportAll(m_handlePpt, (char *)strCurrentPath.c_str(), "", destWidth, destHeight);
	ASSERT(iRet == 0);

	//CString test1 = _T("E:\\br_project\\br_media_tool\\br_media_tool\\br_media_tool\\br_mediafile\\ppt\\3.bmp");
	//m_cximage.Load(test1.GetBuffer());
	//m_cximage.SetJpegQuality(80);
	//m_cximage.Save(_T("E:\\br_project\\br_media_tool\\br_media_tool\\br_media_tool\\br_mediafile\\ppt\\3.jpg"), CXIMAGE_FORMAT_JPG);

	/***********************************************
	* convert bmp to jpg
	* some bmp convert to jpg, cximage.Save failed
	***********************************************/
	//wchar_t szwTmpBmp[MAX_PATH] = {0};
	//wchar_t szwTmpJpg[MAX_PATH] = {0};
	//string strBmp;
	//string strJpg;
	//char szIntTmp[10] = { 0 };
	//for (int i = 1; i <= m_i32PptTotalPage; i ++)
	//{
	//	wmemset(szwTmpBmp, 0, wcslen(szwTmpBmp));
	//	wmemset(szwTmpJpg, 0, wcslen(szwTmpJpg));

	//	strBmp = strCurrentPath;
	//	strBmp += "\\";
	//	itoa(i, szIntTmp, 10);
	//	strBmp += szIntTmp;
	//	strBmp += ".bmp";

	//	strJpg = strCurrentPath;
	//	strJpg += "\\";
	//	strJpg += szIntTmp;
	//	strJpg += ".jpg";

	//	char2wchar(szwTmpBmp, strBmp.c_str(), strBmp.length());

	//	char2wchar(szwTmpJpg, strJpg.c_str(), strJpg.length());

	//	bool bRet = m_cximage.Load(szwTmpBmp);
	//	if (!bRet)
	//	{
	//		continue;
	//	}
	//	m_cximage.SetJpegQuality(100);
	//	bRet = m_cximage.Save(szwTmpJpg, CXIMAGE_FORMAT_JPG);

	//	//delete bmp
	//	CFileAssist::RemoveFile(strBmp.c_str());
	//}

	//write README file
	ofstream fout;
	fout.open(_T(".\\br_mediafile\\config.json"));
	fout << strReadme << flush;
	fout.close();

	//compress folder br_mediafile
	//CZipUtils::compressFolder(".\\br_mediafile\\");
	CZipUtils::compressFolder(".\\br_mediafile\\audio\\", ".\\br_mediafile.zip");
	CZipUtils::compressFolder(".\\br_mediafile\\ppt\\", ".\\br_mediafile.zip");

	string strJsonPath = ".\\br_mediafile\\config.json";
	vector<string> vecCompressFiles;
	vecCompressFiles.push_back(strJsonPath);
	CZipUtils::compressFile(vecCompressFiles, ".\\br_mediafile.zip");

	m_static_pack_tooltip.Format(_T(""));
	UpdateData(FALSE);

	MessageBox(_T("�����ɣ���Ԥ��Ч��"), _T("��ʾ"), MB_OK | MB_ICONINFORMATION);

	
	//root["audio_address"] = Json::Value(123);
}


void Cbr_media_toolDlg::OnBnClickedbtnselectvoicefile()
{
	//wchar_t Filter[] = _T("�����ļ�(*.*)|*.*||");
	wchar_t Filter[] = _T("mp3�ļ�|*.mp3|");
	CFileDialog dlgOpen(TRUE, 0, 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, (LPCTSTR)Filter, NULL);
	if (dlgOpen.DoModal() == IDOK)
	{
		//delete br_mediafile folder in current dir
		CFileAssist::RemoveDir(".\\br_mediafile\\");

		//delete br_mediafile.zip 
		CFileAssist::RemoveFile(".\\br_mediafile.zip");

		m_csVoiceFilePath = dlgOpen.GetPathName();
		CString FileTitle = dlgOpen.GetFileName(); //ȡ��Ҫ�ָ���ļ�����������·������չ����
		m_editVoiceValue = FileTitle;
		m_csVoiceFileTitle = FileTitle;

		m_mciPlayer.MCIClose();
		m_mciPlayer.MCIOpen(m_csVoiceFilePath);
		m_staticTotalSecond.Format(_T("��%d��"), m_mciPlayer.MCIGetLength() / 1000);

		UpdateData(FALSE);
	}
	else return;
}


void Cbr_media_toolDlg::OnBnClickedbtnselectppt()
{
	//wchar_t Filter[] = _T("�����ļ�(*.*)|*.*||");
	wchar_t Filter[] = _T("ppt�ļ�|*.ppt;*.pptx|");
	CFileDialog dlgOpen(TRUE, 0, 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, (LPCTSTR)Filter, NULL);
	if (dlgOpen.DoModal() == IDOK)
	{
		//delete br_mediafile folder in current dir
		CFileAssist::RemoveDir(".\\br_mediafile\\");

		//delete br_mediafile.zip 
		CFileAssist::RemoveFile(".\\br_mediafile.zip");

		//tooltip
		m_static_pptInfo = _T("PPT������...");
		UpdateData(FALSE);

		m_csPptFilePath = dlgOpen.GetPathName();
		CString FileTitle = dlgOpen.GetFileName(); //ȡ��Ҫ�ָ���ļ�����������·������չ����
		m_editPptValue = FileTitle;
		m_csPptFileTitel = FileTitle;


		TCHAR* pPptFilePath = m_csPptFilePath.GetBuffer();
		char szFilePath[MAX_PATH] = {0};
		//wchar2char(szFilePath, pPptFilePath, wcslen(pPptFilePath));
		wchar2char(szFilePath, pPptFilePath, m_csPptFilePath.GetLength());
		
		int iRet = 0;
		iRet = Brppt_SetPowerPoint(m_handlePpt, szFilePath);
		ASSERT(iRet == 0);

		//get ppt total pages
		int count;
		iRet = Brppt_GetTotalSliderCount(m_handlePpt, &count);
		ASSERT(iRet == 0);
		//MessageBox(_T("��ȡppҳ���ɹ�"), _T("����"), MB_OK | MB_ICONERROR);

		m_static_pptInfo.Format(_T("��%dҳ"), count);
		m_i32PptTotalPage = count;
		UpdateData(FALSE);

		//clear combox
		m_comboPptPage.ResetContent();

		//clear list
		m_mapPPT.clear();

		//bind combo box
		int i32Index = 0;
		CString csComboString;
		for (int i = count; i > 0; i --)
		{
			csComboString.Format(_T("%d"), i);
			m_comboPptPage.InsertString(i32Index, csComboString.GetBuffer());
			m_comboPptPage.SetItemData(i32Index, i);
		}
		m_comboPptPage.SetCurSel(0);
		

		m_bIsNeedInvokeEvent = false;
		//clear first
		m_lstPptConfig.DeleteAllItems();
		//insert init data for edit
		CString  cstrSeq;
		for (int i = 0; i < count; i ++)
		{
			cstrSeq.Format(_T("%d"), i + 1);

			m_lstPptConfig.InsertItem(i, cstrSeq.GetBuffer());
			m_lstPptConfig.SetItemData(i , i + 1);
			m_lstPptConfig.SetItemText(i, 1, _T("0"));
			m_lstPptConfig.SetItemText(i, 2, _T("0"));
		}

		UpdateData(FALSE);

		m_bIsNeedInvokeEvent = true;
	}
	else return;
}


void Cbr_media_toolDlg::OnCbnSelchangecomboselectpage()
{
	m_editStartTimeValue = _T("");
	m_editEndTimeValue = _T("");
	UpdateData(FALSE);

	int iSel = m_comboPptPage.GetCurSel();
	int iData = m_comboPptPage.GetItemData(iSel);

	map<int, STRUPPTITEM>::iterator it;
	it = m_mapPPT.find(iData);
	if (it != m_mapPPT.end())
	{
		m_editStartTimeValue.Format(_T("%d"), it->second.audio_start);
		m_editEndTimeValue.Format(_T("%d"), it->second.audio_end);
		UpdateData(FALSE);
	}
}


void Cbr_media_toolDlg::OnBnClickedbtnsavepptitem()
{
	UpdateData(TRUE);

	m_picControl.Invalidate();
	//Show_picture(_T("E:\\�����ĵ�����\\1����Ƭ.jpg"));

	//CRect rect;
	////m_picControl.GetClientRect(&rc);

	////m_picControl.Load(_T("E:\\�����ĵ�����\\1����Ƭ.jpg"));
	////m_picControl.Draw();

	//if (!img.IsNull()) img.Destroy();
	//img.Load(_T("E:\\�����ĵ�����\\1����Ƭ.jpg"));

	//m_picControl.GetClientRect(&rect); //���pictrue�ؼ����ڵľ�������
	//CDC *pDc = m_picControl.GetDC();//���pictrue�ؼ���Dc
	//SetStretchBltMode(pDc->m_hDC, STRETCH_HALFTONE);
	//img.StretchBlt(pDc->m_hDC, rect, SRCCOPY);
	//ReleaseDC(pDc);//�ͷ�picture�ؼ���Dc

	//CDC *dc = m_picControl.GetDC();
	////CPaintDC dc(this);
	//if (!img.IsNull()) img.Destroy();
	//img.Load(_T("E:\\�����ĵ�����\\1����Ƭ.jpg"));
	//if (!img.IsNull()) img.Draw(dc->m_hDC, 0, 0, 100, 100);

//E:\�����ĵ�����\1����Ƭ.jpg

	/*
	CString csPptAddress;
	CString csStartTime;
	CString csEndTime;
	int i32StartTime;
	int i32EndTime;


	int i32ItemData;
	for (int i = 0; i < m_lstPptConfig.GetItemCount(); i++)
	{
		i32ItemData = m_lstPptConfig.GetItemData(i);
		csPptAddress.Format(_T("ppt/%d.jpg"), i32ItemData);

		csStartTime = m_lstPptConfig.GetItemText(i, 1);
		csEndTime = m_lstPptConfig.GetItemText(i, 2);

		i32StartTime = _ttoi(csStartTime);
		i32EndTime = _ttoi(csEndTime);

		STRUPPTITEM pptItem = { 0 };
		GS_SAFE_COPY(pptItem.ppt_address, MAX_PATH, csPptAddress.GetBuffer());
		pptItem.audio_start = i32StartTime;
		pptItem.audio_end = i32EndTime;

		m_mapPPT[i32ItemData] = pptItem;
	}
	*/
	return;

	
	/*
	STRUPPTITEM pptItem = { 0 };
	//pptItem.i32PageIndex = iData;
	GS_SAFE_COPY(pptItem.ppt_address, MAX_PATH, csPptAddress.GetBuffer());
	pptItem.audio_start = i32StartTime;
	pptItem.audio_end = i32EndTime;

	m_mapPPT[iData] = pptItem;


	int iSel = m_comboPptPage.GetCurSel();
	int iData = m_comboPptPage.GetItemData(iSel);
	int i32StartTime = _ttoi(m_editStartTimeValue);
	int i32EndTime = _ttoi(m_editEndTimeValue);

	//judge page isornot repeat
	map<int, STRUPPTITEM>::iterator it;
	for (it = m_mapPPT.begin(); it != m_mapPPT.end(); it ++)
	{
		if (it->first != iData)
		{
			if (i32StartTime >= it->second.audio_start && i32StartTime <= it->second.audio_end)
			{
				CString cstrMsg;
				cstrMsg.Format(_T("��ǰҳ�͵�%dҳ����ֹʱ�����ص�,\n ��%dҳ�Ŀ�ʼʱ��Ϊ��%d, ����ʱ��Ϊ��%d"), it->first, it->first, it->second.audio_start, it->second.audio_end);
				MessageBox(cstrMsg.GetBuffer(), _T("����"), MB_OK | MB_ICONERROR);
				return;
			}

			if (i32EndTime >= it->second.audio_start && i32EndTime <= it->second.audio_end)
			{
				CString cstrMsg;
				cstrMsg.Format(_T("��ǰҳ�͵�%dҳ����ֹʱ�����ص�"), it->first);
				MessageBox(cstrMsg.GetBuffer(), _T("����"), MB_OK | MB_ICONERROR);
				return;
			}
		}
	}


	CString csPptAddress;
	csPptAddress.Format(_T("ppt/%d.jpg"), iData);

	STRUPPTITEM pptItem = { 0 };
	//pptItem.i32PageIndex = iData;
	GS_SAFE_COPY(pptItem.ppt_address, MAX_PATH, csPptAddress.GetBuffer());
	pptItem.audio_start = i32StartTime;
	pptItem.audio_end = i32EndTime;

	m_mapPPT[iData] = pptItem;
	//m_listPPT.push_back(pptItem);

	//insert listctrl

	//judge current page isornot exist
	for (int i = 0; i < m_lstPptConfig.GetItemCount(); i++)
	{
		if (iData == m_lstPptConfig.GetItemData(i))
		{
			m_lstPptConfig.SetItemText(i, 1, m_editStartTimeValue.GetBuffer());
			m_lstPptConfig.SetItemText(i, 2, m_editEndTimeValue.GetBuffer());
			return;
		}
	}



	CString  cstrSeq;
	cstrSeq.Format(_T("%d"), iData);

	int i32ListCount = m_lstPptConfig.GetItemCount();
	m_lstPptConfig.InsertItem(i32ListCount, cstrSeq.GetBuffer());
	m_lstPptConfig.SetItemData(i32ListCount, iData);
	m_lstPptConfig.SetItemText(i32ListCount, 1, m_editStartTimeValue.GetBuffer());
	m_lstPptConfig.SetItemText(i32ListCount, 2, m_editEndTimeValue.GetBuffer());

	*/

}


void Cbr_media_toolDlg::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent)
	{
	case 1:
		PlayPptPicTimer();
		break;
	default:
		break;
	}
	CDialog::OnTimer(nIDEvent);
}

void Cbr_media_toolDlg::PlayPptPicTimer()
{
	m_i32CurrentPlaySecond++;
	TRACE("PlayPptPicTimer() %d------------------- \n", m_i32CurrentPlaySecond);

	if (m_i32CurrentPlaySecond >= m_mciPlayer.MCIGetLength() / 1000)
	{
		KillTimer(1);
		TRACE("KillTimer(1);====================== \n");

		m_static_current_play.Format(_T("��ǰ���ŵ� %d ��"), m_i32CurrentPlaySecond);
		UpdateData(FALSE);

		m_i32CurrentPlaySecond = 0;
		m_bIsPreview = false;
		SetDlgItemTextW(IDC_btnPreview, _T("Ԥ��"));

		m_mciPlayer.MCIClose();
		m_mciPlayer.MCIOpen(m_csVoiceFilePath);

		return;
	}

	CString cstrCurrPicPath;
	map<int, STRUPPTITEM>::iterator it;
	for (it = m_mapPPT.begin(); it != m_mapPPT.end(); it++)
	{
		if (m_i32CurrentPlaySecond >= it->second.audio_start && m_i32CurrentPlaySecond <= it->second.audio_end)
		{
			cstrCurrPicPath.Format(_T("./br_mediafile/%s"), it->second.ppt_address);

			//if (!img.IsNull()) img.Destroy();
			//img.Load(cstrCurrPicPath.GetBuffer());

			//m_cstrCurrPicPath = cstrCurrPicPath;
			//m_picControl.Invalidate();

			m_picControl.Load(cstrCurrPicPath.GetBuffer());
			m_picControl.Draw();
		}
	}

	TRACE("��ǰ���ŵ� %d ��------------------- \n", m_i32CurrentPlaySecond);
	m_static_current_play.Format(_T("��ǰ���ŵ� %d ��"), m_i32CurrentPlaySecond);

	UpdateData(FALSE);

	//m_picControl.Load(_T(".\\br_mediafile\\ppt\\1.JPG"));
	//m_picControl.Draw();
}


void Cbr_media_toolDlg::OnLvnItemchangedlstpptconfig(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO:  �ڴ���ӿؼ�֪ͨ����������

	if (m_bIsNeedInvokeEvent)
	{
		int i32Item = pNMLV->iItem;
		int i32SubItem = pNMLV->iSubItem;
		uint ui32Changed = pNMLV->uChanged;
	}

	*pResult = 0;
}


void Cbr_media_toolDlg::OnNMThemeChangedlstpptconfig(NMHDR *pNMHDR, LRESULT *pResult)
{
	// �ù���Ҫ��ʹ�� Windows XP ����߰汾��
	// ���� _WIN32_WINNT ���� >= 0x0501��
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
}


BOOL Cbr_media_toolDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ

	
	//Show_picture(m_cstrCurrPicPath);
	//if (!img.IsNull())
	//{
	//	CRect rect(0, 0, 500, 500);
	//	//m_picControl.GetClientRect(&rc);

	//	//m_picControl.Load(_T("E:\\�����ĵ�����\\1����Ƭ.jpg"));
	//	//m_picControl.Draw();

	//	//if (!img.IsNull()) img.Destroy();
	//	//img.Load(_T("E:\\�����ĵ�����\\1����Ƭ.jpg"));

	//	//m_picControl.GetClientRect(&rect); //���pictrue�ؼ����ڵľ�������
	//	CDC *pDc = m_picControl.GetDC();//���pictrue�ؼ���Dc
	//	SetStretchBltMode(pDc->m_hDC, STRETCH_HALFTONE);
	//	img.StretchBlt(pDc->m_hDC, rect, SRCCOPY);
	//	ReleaseDC(pDc);//�ͷ�picture�ؼ���Dc
	//}

	//return TRUE;

	return CDialogEx::OnEraseBkgnd(pDC);
}
bool Cbr_media_toolDlg::Show_picture(CString imgPath)
{
	if (imgPath.IsEmpty())
	{
		return true;
	}

	int destWidth, destHeight;
	if (m_i32Resolution == 3)
	{
		destWidth = 800;
		destHeight = 600;
	}
	else if (m_i32Resolution == 2)
	{
		destWidth = 640;
		destHeight = 480;
	}
	else
	{
		destWidth = 320;
		destHeight = 240;
	}
	CRect rect(0, 0, destWidth, destHeight);//���������  
	CRect rect1;
	CImage image; //����ͼƬ��  
	image.Load(imgPath);
	int height, width;
	height = image.GetHeight();
	width = image.GetWidth();

	//m_picControl.GetClientRect(&rect); //���pictrue�ؼ����ڵľ�������  
	CDC *pDc = m_picControl.GetDC();//���pictrue�ؼ���Dc  
	SetStretchBltMode(pDc->m_hDC, STRETCH_HALFTONE);

	//if (width <= rect.Width() && height <= rect.Width()) //СͼƬ��������  
	//{
	//	rect1 = CRect(rect.TopLeft(), CSize(width, height));
	//	image.StretchBlt(pDc->m_hDC, rect1, SRCCOPY); //��ͼƬ����Picture�ؼ���ʾ�ľ�������  
	//	return TRUE;
	//}
	//else
	{
		//float xScale = (float)rect.Width() / (float)width;
		//float yScale = (float)rect.Height() / (float)height;
		//float ScaleIndex = (xScale >= yScale ? xScale : yScale);
		//rect1 = CRect(rect.TopLeft(), CSize((int)width*ScaleIndex, (int)height*ScaleIndex));

		image.StretchBlt(pDc->m_hDC, rect, SRCCOPY); //��ͼƬ����Picture�ؼ���ʾ�ľ�������  
	}
	ReleaseDC(pDc);//�ͷ�picture�ؼ���Dc  

	if (!image.IsNull())
	{
		image.Destroy();
	}
	return TRUE;
}

void Cbr_media_toolDlg::OnCbnSelchangecomboselectresolution()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������


	int iSelResolution = m_comboSelectResolution.GetCurSel();
	int iDataResolution = m_comboPptPage.GetItemData(iSelResolution);

	if (m_i32Resolution != iDataResolution)
	{
		//delete br_mediafile folder in current dir
		CFileAssist::RemoveDir(".\\br_mediafile\\");

		//delete br_mediafile.zip 
		CFileAssist::RemoveFile(".\\br_mediafile.zip");

		m_i32Resolution = iDataResolution;
	}
}