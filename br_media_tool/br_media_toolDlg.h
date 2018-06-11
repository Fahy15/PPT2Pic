
// br_media_toolDlg.h : 头文件
//

#pragma once
#include "ppt2pic.h"
#include "afxwin.h"
#include "afxcmn.h"
#include <map>
#include "PictureEx.h"
#include "MciPlayer.h"
#include "ListCtrlEx.h"

#include "ximage.h" 
#pragma comment(lib, "cximage.lib")
//#pragma comment(lib, "cximagecrtd.lib")
#pragma comment(lib, "jasper.lib")
#pragma comment(lib, "jbig.lib")
#pragma comment(lib, "Jpeg.lib")
#pragma comment(lib, "mng.lib")
#pragma comment(lib, "png.lib")
#pragma comment(lib, "tiff.lib")
#pragma comment(lib, "zlib.lib")
#pragma comment(lib, "libdcr.lib")
#pragma comment(lib, "libpsd.lib")

using namespace std;
using namespace ListCtrlEx;

typedef struct _StruPptItem
{
	//int i32PageIndex;
	TCHAR ppt_address[MAX_PATH];
	int audio_start;
	int audio_end;
}STRUPPTITEM, *STRUPPTITEMPTR;


// Cbr_media_toolDlg 对话框
class Cbr_media_toolDlg : public CDialogEx
{
// 构造
public:
	Cbr_media_toolDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_BR_MEDIA_TOOL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedbtnreset();
	afx_msg void OnBnClickedbtnpreview();
	afx_msg void OnBnClickedbtnpack();
	afx_msg void OnBnClickedbtnselectvoicefile();
	afx_msg void OnBnClickedbtnselectppt();
	afx_msg void OnCbnSelchangecomboselectpage();
	afx_msg void OnBnClickedbtnsavepptitem();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

private:
	void PlayPptPicTimer();

	CString m_csVoiceFilePath;     //语音文件路径 
	CString m_csVoiceFileTitle;   //语音文件名

	CString m_csPptFilePath;     //ppt文件路径 
	CString m_csPptFileTitel;    //ppt文件名

	PPT_HANDLE m_handlePpt;
	int m_i32PptTotalPage;

	map<int, STRUPPTITEM> m_mapPPT;
	CMciPlayer m_mciPlayer;

	int m_i32CurrentPlaySecond;
	bool m_bIsPreview;

	GdiplusStartupInput m_gdiplusStartupInput;
	ULONG_PTR m_pGdiToken;

	bool m_bIsNeedInvokeEvent;

	CFont m_Font1;
	CFont m_Font2;
	CFont m_Font3;

	int m_i32Resolution;  // 1:image 2:640*480 3:800*600

	CImage img;

	CString m_cstrCurrPicPath;

	CxImage m_cximage;
public:
	CEdit m_editVoice;
	CString m_editVoiceValue;
	CString m_editPptValue;
	CComboBox m_comboPptPage;
	CString m_editStartTimeValue;
	CString m_editEndTimeValue;
	CListCtrlEx m_lstPptConfig;
	CPictureEx m_picControl;
	CString m_staticTotalSecond;
	CButton m_btnPreview;
	CString m_static_current_play;
	CString m_static_pptInfo;
	CString m_static_pack_tooltip;
	afx_msg void OnLvnItemchangedlstpptconfig(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMThemeChangedlstpptconfig(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	bool Show_picture(CString imgPath);
	CComboBox m_comboSelectResolution;
	CString m_staticSelectResolution;
	CStatic m_staticCtlCurrentPlay;
	afx_msg void OnCbnSelchangecomboselectresolution();
	afx_msg void OnBnClickedCheck1();
};
