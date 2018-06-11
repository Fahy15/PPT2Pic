#include "stdafx.h"
#include "ListCtrlCellEx.h"

using namespace ListCtrlEx;

//////////////////////////////////////////////////////////////////////////
// ------------ for CInPlaceCombo --------------------//
CInPlaceCombo* CInPlaceCombo::m_pInPlaceCombo = NULL; 

CInPlaceCombo::CInPlaceCombo()
{
	m_iRowIndex = -1;
	m_iColumnIndex = -1;
	m_bESC = FALSE;
}

CInPlaceCombo::~CInPlaceCombo()
{
	if (::IsWindow(m_pInPlaceCombo->m_hWnd))
	{
		SendMessage(WM_CLOSE);
	}
}

BEGIN_MESSAGE_MAP(CInPlaceCombo, CComboBox)
	//{{AFX_MSG_MAP(CInPlaceCombo)
	ON_WM_CREATE()
	ON_WM_KILLFOCUS()
	ON_WM_CHAR()
	ON_CONTROL_REFLECT(CBN_CLOSEUP, OnCloseup)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

int CInPlaceCombo::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CComboBox::OnCreate(lpCreateStruct) == -1)
	{
		return -1;
	}

	CFont* pFont = GetParent()->GetFont();
	SetFont(pFont);
	SetFocus();
	ResetContent(); 
	CStrList::iterator iter=m_DropDownList.begin();
	for (; iter!=m_DropDownList.end(); ++iter)
	{
		AddString(*iter);
	}

	return 0;
}

BOOL CInPlaceCombo::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
		{
			::TranslateMessage(pMsg);
			::DispatchMessage(pMsg);
			return TRUE;				
		}
	}

	return CComboBox::PreTranslateMessage(pMsg);
}

void CInPlaceCombo::OnKillFocus(CWnd* pNewWnd) 
{
	CComboBox::OnKillFocus(pNewWnd);

	CString str;
	GetWindowText(str);
	// Send Notification to parent of ListView ctrl
	LV_DISPINFO dispinfo;
	dispinfo.hdr.hwndFrom = GetParent()->m_hWnd;
	dispinfo.hdr.idFrom = GetDlgCtrlID();
	dispinfo.hdr.code = LVN_ENDLABELEDIT;

	dispinfo.item.mask = LVIF_TEXT;
	dispinfo.item.iItem = m_iRowIndex;
	dispinfo.item.iSubItem = m_iColumnIndex;
	dispinfo.item.pszText = m_bESC ? LPTSTR((LPCTSTR)m_strWindowText) : LPTSTR((LPCTSTR)str);
	dispinfo.item.cchTextMax = m_bESC ? m_strWindowText.GetLength() : str.GetLength();

	GetParent()->SendMessage(WM_NOTIFY, GetParent()->GetDlgCtrlID(), (LPARAM)&dispinfo);
	PostMessage(WM_CLOSE);
}

void CInPlaceCombo::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// If the key is "Esc" set focus back to the list control
	if (nChar == VK_ESCAPE || nChar == VK_RETURN)
	{
		if (nChar == VK_ESCAPE)
		{
			m_bESC = TRUE;
		}
		GetParent()->SetFocus();
		return;
	}

	CComboBox::OnChar(nChar, nRepCnt, nFlags);
}

void CInPlaceCombo::OnCloseup() 
{
	GetParent()->SetFocus();
}

CInPlaceCombo* CInPlaceCombo::GetInstance()
{
	if(m_pInPlaceCombo == NULL)
	{
		m_pInPlaceCombo = new CInPlaceCombo;
	}
	return m_pInPlaceCombo;
}

void CInPlaceCombo::DeleteInstance()
{
	delete m_pInPlaceCombo;
	m_pInPlaceCombo = NULL;
}

BOOL CInPlaceCombo::ShowComboCtrl(DWORD dwStyle, const CRect &rCellRect, CWnd* pParentWnd, UINT uiResourceID,
								  int iRowIndex, int iColumnIndex, CStrList &lstDropDown, CString strCurSel, int iCurSel )
{
	m_iRowIndex = iRowIndex;
	m_iColumnIndex = iColumnIndex;
	m_bESC = FALSE;

	m_DropDownList.clear();
	m_DropDownList.insert(m_DropDownList.begin(), lstDropDown.begin(), lstDropDown.end());

	BOOL bRetVal = TRUE;

	if (-1 != iCurSel)
	{
		GetLBText(iCurSel, m_strWindowText);
	}
	else if (!strCurSel.IsEmpty()) 
	{
		m_strWindowText = strCurSel;
	}

	if (NULL == m_pInPlaceCombo->m_hWnd) 
	{
		bRetVal = m_pInPlaceCombo->Create(dwStyle, rCellRect, pParentWnd, uiResourceID); 
	}

	if (iCurSel >=0 && iCurSel < this->GetCount())
	{
		SetCurSel(iCurSel);
	}
	else
	{
		SelectString(0, strCurSel);
	}

	return bRetVal;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// --------------- for CInPlaceEdit -------------------//
CInPlaceEdit* CInPlaceEdit::m_pInPlaceEdit=NULL;

CInPlaceEdit::CInPlaceEdit()
{
	m_iRowIndex= -1;
	m_iColumnIndex = -1;
	m_bESC = FALSE;
	m_strValidChars.Empty();
}

CInPlaceEdit::~CInPlaceEdit()
{
	if (::IsWindow(m_pInPlaceEdit->m_hWnd))
	{
		SendMessage(WM_CLOSE);
	}
}

BEGIN_MESSAGE_MAP(CInPlaceEdit, CEdit)
	//{{AFX_MSG_MAP(CInPlaceEdit)
	ON_WM_KILLFOCUS()
	ON_WM_CHAR()
	ON_MESSAGE(WM_PASTE, CInPlaceEdit::OnPaste)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInPlaceEdit message handlers

LRESULT CInPlaceEdit::OnPaste(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	if (!m_strValidChars.IsEmpty())
	{
		CString strFromClipboard;
		// get the text from clipboard
		if(OpenClipboard()) {
			HANDLE l_hData = GetClipboardData(CF_TEXT);
			if(NULL == l_hData) {
				return 1;
			}

			char *l_pBuffer = (char*)GlobalLock(l_hData);
			if(NULL != l_pBuffer) {
				strFromClipboard = l_pBuffer;
			}

			GlobalUnlock(l_hData);
			CloseClipboard();
		}
		// Validate the characters before pasting 
		for(int iCounter_ = 0; iCounter_ < strFromClipboard.GetLength(); iCounter_++)
		{
			if (-1 == m_strValidChars.Find(strFromClipboard.GetAt(iCounter_)))
			{
				return 1;
			}
		}
	}

	//let the individual control handle other processing
	CEdit::Default();	
	return 1;	
}

void CInPlaceEdit::OnKillFocus(CWnd* pNewWnd) 
{
	CEdit::OnKillFocus(pNewWnd);

	CString strEdit;
	GetWindowText(strEdit);

	// Send Notification to parent of edit ctrl
	LV_DISPINFO dispinfo;
	dispinfo.hdr.hwndFrom = GetParent()->m_hWnd;
	dispinfo.hdr.idFrom = GetDlgCtrlID();
	dispinfo.hdr.code = LVN_ENDLABELEDIT;

	dispinfo.item.mask = LVIF_TEXT;
	dispinfo.item.iItem = m_iRowIndex;
	dispinfo.item.iSubItem = m_iColumnIndex; 
	// escape key is down so use old string
	dispinfo.item.pszText = m_bESC ? LPTSTR((LPCTSTR)m_strWindowText) : LPTSTR((LPCTSTR)strEdit);
	dispinfo.item.cchTextMax = m_bESC ? m_strWindowText.GetLength() : strEdit.GetLength();

	GetParent()->SendMessage(WM_NOTIFY, GetParent()->GetDlgCtrlID(), (LPARAM)&dispinfo);
	PostMessage(WM_CLOSE);
}

void CInPlaceEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	if ( (m_strValidChars.IsEmpty()) // no valid chars
		|| ((-1 != m_strValidChars.Find(static_cast<TCHAR> (nChar)))
		|| (nChar == VK_BACK) || (nChar == CTRL_C) || (nChar == CTRL_V) || (nChar == CTRL_X)))
	{
		CEdit::OnChar(nChar, nRepCnt, nFlags);
	}
	else
	{
		MessageBeep(MB_ICONEXCLAMATION);
		return;
	}
}

BOOL CInPlaceEdit::PreTranslateMessage(MSG* pMsg) 
{
	if (WM_KEYDOWN == pMsg->message && (VK_ESCAPE == pMsg->wParam || VK_RETURN == pMsg->wParam))
	{
		if (VK_ESCAPE == pMsg->wParam)
		{
			m_bESC = TRUE;
		}

		GetParent()->SetFocus();
		return TRUE;
	}

	return CEdit::PreTranslateMessage(pMsg);
}

int CInPlaceEdit::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CEdit::OnCreate(lpCreateStruct) == -1)
		return -1;

	// Set the proper font
	CFont* pFont = GetParent()->GetFont();
	SetFont(pFont);

	ShowWindow(SW_SHOW);
	SetWindowText(m_strWindowText);
	SetSel(0, -1);
	SetFocus();

	return 0;
}

CInPlaceEdit* CInPlaceEdit::GetInstance()
{
	if(m_pInPlaceEdit == NULL)
	{
		m_pInPlaceEdit = new CInPlaceEdit;
	}
	return m_pInPlaceEdit;
}

void CInPlaceEdit::DeleteInstance()
{
	delete m_pInPlaceEdit;
	m_pInPlaceEdit = NULL;
}

BOOL CInPlaceEdit::ShowEditCtrl(DWORD dwStyle, const RECT &rCellRect, CWnd* pParentWnd, 
								UINT uiResourceID, int iRowIndex, int iColumnIndex,
								CString& strValidChars, CString& rstrCurSelection)
{
	m_iRowIndex = iRowIndex;
	m_iColumnIndex = iColumnIndex;
	m_strValidChars = strValidChars;
	m_strWindowText = rstrCurSelection;
	m_bESC = FALSE;

	if (NULL == m_pInPlaceEdit->m_hWnd) 
	{
		return m_pInPlaceEdit->Create(dwStyle, rCellRect, pParentWnd, uiResourceID); 
	}	

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// CListCtrlExSortHead

IMPLEMENT_DYNAMIC(CListCtrlExSortHead, CHeaderCtrl)

CListCtrlExSortHead::CListCtrlExSortHead()
: m_iSortColumn(-1)
,m_bSortDesc(FALSE)
{
	m_Format = "";
	m_Height = 1;
	m_fontHeight = 12;
	m_fontWith = 0;
	m_colorText = RGB(0,0,0);

	m_clrBegin.SetValue(Color::MakeARGB(255, 230,230,230)); //标题默认颜色
	m_clrEnd.SetValue(Color::MakeARGB(255, 240,240,240));	//标题默认颜色

}

CListCtrlExSortHead::~CListCtrlExSortHead()
{

}


BEGIN_MESSAGE_MAP(CListCtrlExSortHead, CHeaderCtrl)
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, &CListCtrlExSortHead::OnNMCustomdraw)
	ON_MESSAGE(HDM_LAYOUT, OnLayout)
	ON_WM_PAINT()
END_MESSAGE_MAP()

void CListCtrlExSortHead::SetGradientColor(ARGB clrBegin, ARGB clrEnd)
{
	m_clrBegin = clrBegin;
	m_clrEnd = clrEnd;
}
LRESULT CListCtrlExSortHead::OnLayout( WPARAM wParam, LPARAM lParam ) 
{ 
	LRESULT lResult = CHeaderCtrl::DefWindowProc(HDM_LAYOUT, 0, lParam); 
	HD_LAYOUT &hdl = *( HD_LAYOUT * ) lParam;  
	RECT *prc = hdl.prc; 
	WINDOWPOS *pwpos = hdl.pwpos; 
	int nHeight = (int)(pwpos->cy * m_Height); //改变高度,m_Height为倍数 
	pwpos->cy = nHeight;
	prc->top = nHeight; 
	return lResult;  
}
// CSortHeadCtrl message handlers
void CListCtrlExSortHead::SetSortArrow( const int nSortColumn, const BOOL bDesc )
{
	if (nSortColumn<0 || nSortColumn>=this->GetItemCount())
	{
		ASSERT(FALSE);
	}
	m_iSortColumn=nSortColumn;
	m_bSortDesc=bDesc;

	// change the item to owner drawn.
	HD_ITEM hditem;

	hditem.mask = HDI_FORMAT;
	VERIFY( GetItem( nSortColumn, &hditem ) );
	hditem.fmt |= HDF_OWNERDRAW;
	VERIFY( SetItem( nSortColumn, &hditem ) );

	// invalidate the header control so it gets redrawn
	Invalidate();
	//UpdateWindow();
}

inline int	 CListCtrlExSortHead::GetCrntSortCol() const
{
	return m_iSortColumn;
}

inline BOOL CListCtrlExSortHead::GetCrntSortDirection() const
{
	return m_bSortDesc;
}

void ListCtrlEx::CListCtrlExSortHead::OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	switch(pNMCD->dwDrawStage)
	{
	case CDDS_PREPAINT:
		*pResult = CDRF_NOTIFYSUBITEMDRAW;          // ask for subitem notifications.
		*pResult = CDRF_DODEFAULT;
		break;
	case CDDS_ITEMPREPAINT:
		*pResult = CDRF_NOTIFYSUBITEMDRAW;          // ask for subitem notifications.
		*pResult = CDRF_DODEFAULT;
		break;
	case CDDS_ITEMPREPAINT|CDDS_SUBITEM:			// recd when CDRF_NOTIFYSUBITEMDRAW is returned in
		*pResult = CDRF_DODEFAULT;
		break;
	default:														// it wasn't a notification that was interesting to us.
		*pResult = CDRF_DODEFAULT;
		break;
	}
	*pResult = 0;
}

void ListCtrlEx::CListCtrlExSortHead::OnPaint()
{
	CPaintDC dc(this); // device context for painting	

	Graphics grap(dc.GetSafeHdc());
	// 不为绘图消息调用 CHeaderCtrl::OnPaint()
	int nItem; 
	nItem = GetItemCount();//得到有几个单元 
	for(int i = 0; i<nItem;i ++) 
	{ 
		CRect tRect;
		GetItemRect(i,&tRect);//得到Item的尺寸
		CRect nRect(tRect);//拷贝尺寸到新的容器中 
		nRect.left++;//留出分割线的地方 
		//绘制立体背景 

		{
			CRect rectClient = tRect;			
			LinearGradientBrush linGrBrush(Point(tRect.left, tRect.top),
				Point(tRect.left, tRect.bottom), m_clrBegin, m_clrEnd);
				//Color(255,230,230,230),Color(255,230,230,230));
			rectClient.DeflateRect(1,1,1,1);
			grap.FillRectangle(&linGrBrush, tRect.left, tRect.top, tRect.Width(), tRect.Height());

			Pen pen(Color(255,200,200,200));
			grap.DrawLine(&pen, rectClient.left, rectClient.bottom, rectClient.right, rectClient.bottom);
			grap.DrawLine(&pen, rectClient.right, rectClient.top, rectClient.right, rectClient.bottom);
		}		

		dc.SetBkMode(TRANSPARENT);
		CFont nFont ,* nOldFont; 
		dc.SetTextColor(m_colorText);
		nFont.CreateFont(m_fontHeight,m_fontWith,0,0,0,FALSE,FALSE,0,0,0,0,0,0,_TEXT("宋体"));//创建字体 
		nOldFont = dc.SelectObject(&nFont);
		UINT nFormat = 1;
		if (m_Format[i]=='0')
		{
			nFormat = DT_LEFT;
			tRect.left+=3;
		}
		else if (m_Format[i]=='1')
		{
			nFormat = DT_CENTER;
		}
		else if (m_Format[i]=='2')
		{
			nFormat = DT_RIGHT;
			tRect.right-=3;
		}
		//将文字显示在一个适合的高度位置
		TEXTMETRIC metric;
		dc.GetTextMetrics(&metric);
		int ofst = 0;
		ofst = tRect.Height() - metric.tmHeight;
		tRect.OffsetRect(0,ofst/2);
		dc.DrawText(m_HChar[i],&tRect,nFormat);
		dc.SelectObject(nOldFont); 
		nFont.DeleteObject(); //释放字体 

		//绘制排序的箭头
		if (i == static_cast<UINT>(m_iSortColumn))
		{
			CDC *pDC = &dc;
			CRect rcIcon(tRect);
			int offset = pDC->GetTextExtent(_T(" "), 1).cx*2;
			Point pt[3];
			if (!m_bSortDesc)
			{
				pt[0].X = rcIcon.right - 10;
				pt[0].Y = rcIcon.top + 4;
				pt[1].X = rcIcon.right - 5;
				pt[1].Y = rcIcon.top + 9;
				pt[2].X = rcIcon.right - 15;
				pt[2].Y = rcIcon.top + 9;
			}else
			{			
				pt[0].X = rcIcon.right - 5;
				pt[0].Y = rcIcon.top + 4;
				pt[1].X = rcIcon.right - 15;
				pt[1].Y = rcIcon.top + 4;
				pt[2].X = rcIcon.right - 10;
				pt[2].Y = rcIcon.top + 9;
			}		
			Pen pen(Color(255,0,0,0));
			grap.DrawPolygon(&pen, pt, 3);
		}
	} 
	//画头部剩余部分(没有标题的那部分)
	CRect rtRect;
	CRect clientRect;
	GetItemRect(nItem - 1,rtRect);
	GetClientRect(clientRect); //当前(CHeaderCtrl)控件的大小
	rtRect.left = rtRect.right+1;
	rtRect.right = clientRect.right;
	CRect nRect(rtRect);
	//绘制立体背景 
	{
		CRect rectClient = nRect;
		LinearGradientBrush linGrBrush(Point(rectClient.left, rectClient.top),	
			Point(rectClient.left, rectClient.bottom),
			m_clrBegin,
			m_clrEnd);

		rectClient.DeflateRect(1,1,1,1);
		grap.FillRectangle(&linGrBrush, rectClient.left, rectClient.top, rectClient.Width(), rectClient.Height());
	}
	

}
