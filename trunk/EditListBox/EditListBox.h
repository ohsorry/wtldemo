#if !defined(AFX_EDITLISTBOX_H__20010311_8A0B_FC46_5D30_0080AD509054__INCLUDED_)
#define AFX_EDITLISTBOX_H__20010311_8A0B_FC46_5D30_0080AD509054__INCLUDED_

#pragma once

/////////////////////////////////////////////////////////////////////////////
// CEditListBox - Editable List box (ala DevStudio6)
//
// Written by Bjarke Viksoe (bjarke@viksoe.dk)
// Copyright (c) 2001 Bjarke Viksoe.
//
// This code may be used in compiled form in any way you desire. This
// source file may be redistributed by any means PROVIDING it is 
// not sold for profit without the authors written consent, and 
// providing that this notice and the authors name is included. 
//
// This file is provided "as is" with no expressed or implied warranty.
// The author accepts no liability if it causes any damage to you or your
// computer whatsoever. It's free, so don't hassle me about it.
//
// Beware of bugs.
//

#ifndef __cplusplus
#error WTL requires C++ compilation (use a .cpp suffix)
#endif

#ifndef __ATLCTRLS_H__
#error EditListBox.h requires atlctrls.h to be included first
#endif

#ifndef __ATLCTRLX_H__
#error EditListBox.h requires atlctrlx.h to be included first
#endif


// EditListBox Window styles
#define EDLS_NOMOVEBUTTONS   0x00000001
#define EDLS_BROWSE          0x00000002

// EditListBox Notification messages
#define EDLN_FIRST           (0U-1900U)
#define EDLN_LAST            (0U-1950U)

#define EDLN_BROWSE          (EDLN_FIRST-1)
#define EDLN_ITEMCHANGING    (EDLN_FIRST-2)
#define EDLN_ITEMCHANGED     (EDLN_FIRST-3)
#define EDLN_INSERTITEM      (EDLN_FIRST-4)
#define EDLN_DELETEITEM      (EDLN_FIRST-5)
#define EDLN_BEGINLABELEDIT  (EDLN_FIRST-6)
#define EDLN_ENDLABELEDIT    (EDLN_FIRST-7)

typedef struct 
{
	NMHDR hdr;
	int iIndex;
} NMEDITLIST, *PNMEDITLIST;

// Internal EditListBox command messages
#define EDLM_NEW        0x0210
#define EDLM_DELETE     0x0211
#define EDLM_ITEMUP     0x0212
#define EDLM_ITEMDOWN   0x0213

#define WM_EDITLIST_CLEANUP WM_USER+124


template <class T, class TBase = CWindow, class TWinTraits = CControlWinTraits>
class ATL_NO_VTABLE CEditListBoxImpl : 
	public CWindowImpl<T, TBase, TWinTraits>
{
public:
	CListBox      m_wndList;
	CContainedWindowT<CEdit> m_wndEdit;
	CButton       m_wndBrowse;
	CImageList    m_wndImages;
	CBitmapButton m_wndButtons[4];
	short         m_nButtons;

	typedef CEditListBoxImpl< T, TBase, TWinTraits > thisClass;
	BEGIN_MSG_MAP(thisClass)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
		MESSAGE_HANDLER(WM_PRINTCLIENT, OnPaint)
		MESSAGE_HANDLER(WM_SIZE, OnSize)
		MESSAGE_HANDLER(WM_ENABLE, OnEnable)
		MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBackground)
		MESSAGE_HANDLER(WM_VKEYTOITEM, OnKeyDown)
		COMMAND_CODE_HANDLER(LBN_DBLCLK, OnDblClick)
		COMMAND_ID_HANDLER(EDLM_NEW, OnNew)
		COMMAND_ID_HANDLER(EDLM_DELETE, OnDelete)
		COMMAND_ID_HANDLER(EDLM_ITEMUP, OnUp)
		COMMAND_ID_HANDLER(EDLM_ITEMDOWN, OnDown)
		MESSAGE_HANDLER(WM_EDITLIST_CLEANUP, OnCleanUp)
		COMMAND_CODE_HANDLER(BN_CLICKED, OnBrowseClick)
		ALT_MSG_MAP(1) // Edit control
		MESSAGE_HANDLER(WM_GETDLGCODE, OnEditGetDlgCode)
		MESSAGE_HANDLER(WM_CHAR, OnEditKey)
		MESSAGE_HANDLER(WM_KILLFOCUS, OnEditLostFocus)
	END_MSG_MAP()

	CEditListBoxImpl() : m_wndEdit(this, 1), m_nButtons(0)
	{
	}

	~CEditListBoxImpl()
	{
		m_wndImages.Destroy();
	}

	// Operations

	BOOL SubclassWindow(HWND hWnd)
	{
		ATLASSERT(m_hWnd==NULL);
		ATLASSERT(::IsWindow(hWnd));
		BOOL bRet = CWindowImpl< CEditListBox, CWindow, CControlWinTraits >::SubclassWindow(hWnd);
		if( bRet ) _Init();
		return bRet;
	}

	int GetSelectedIndex() const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return m_wndList.GetCurSel();
	}

	void SetMaxText(int iMax)
	{
		m_wndEdit.LimitText(iMax);
	}

	BOOL InsertItem(int iIndex, LPCTSTR pstrText)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		ATLASSERT(!::IsBadStringPtr(pstrText,-1));
		int idx = m_wndList.InsertString(iIndex, pstrText);
		if( idx==0 ) m_wndList.SetCurSel(0);
		return idx>=0;
	}

	BOOL SelectItem(int iIndex)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return m_wndList.SetCurSel(iIndex) != LB_ERR;
	}

	int FindItem(LPCTSTR pstrText) const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		ATLASSERT(!::IsBadStringPtr(pstrText,-1));
		return m_wndList.FindString(-1, pstrText);
	}

	BOOL BrowseSelected()
	{
		if( (GetStyle() & EDLS_BROWSE)==0 ) return FALSE;
		if( m_wndList.GetCurSel()==-1 ) return FALSE;
		BOOL bDummy;
		OnBrowseClick(0, 0, 0, bDummy);
		return TRUE;
	}

	void DeleteAllItems()
	{
		m_wndList.ResetContent();
	}

	BOOL DeleteItem(int iIndex)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		ATLASSERT(iIndex>=0);
		return m_wndList.DeleteString(iIndex) != LB_ERR;
	}

	BOOL SetItemText(int iIndex, LPCTSTR pstrText)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		ATLASSERT(iIndex>=0);
		ATLASSERT(!::IsBadStringPtr(pstrText,-1));
		m_wndList.DeleteString(iIndex);
		return m_wndList.InsertString(iIndex, pstrText) != LB_ERR;
	}

	BOOL GetItemText(int iIndex, LPTSTR pstrText, UINT cchMax) const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		ATLASSERT(iIndex>=0);
		ATLASSERT(!::IsBadWritePtr(pstrText, cchMax));
		*pstrText = _T('\0');
		if( m_wndList.GetTextLen(iIndex)>cchMax ) return FALSE;
		return m_wndList.GetText(iIndex, pstrText);
	}

	int GetItemCount() const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return m_wndList.GetCount();
	}

	HWND GetEditCtrl() const
	{
		return m_wndEdit;
	}

	// Implementation

	void _Init()
	{
		DWORD dwStyle = GetStyle();

		RECT rc;
		GetClientRect(&rc);

		m_wndImages.Create(IDB_EDITLISTBOX, 16, 4, RGB(0,128,128));

		m_wndList.Create(m_hWnd, rcDefault, NULL, 
			WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN |
			LBS_NOTIFY | LBS_HASSTRINGS | LBS_WANTKEYBOARDINPUT | WS_VSCROLL,
			WS_EX_CLIENTEDGE);
		ATLASSERT(m_wndList.IsWindow());
		m_wndList.SetFont(AtlGetDefaultGuiFont());

		m_wndEdit.Create(this, 1, m_wndList, &rcDefault, NULL, WS_BORDER | WS_CHILD | ES_AUTOHSCROLL);
		ATLASSERT(m_wndEdit.IsWindow());
		m_wndEdit.SetFont(AtlGetDefaultGuiFont());
		SetMaxText(MAX_PATH);

		m_wndBrowse.Create(m_hWnd, rcDefault, _T("..."), WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
		ATLASSERT(m_wndBrowse.IsWindow());
		m_wndBrowse.SetFont(AtlGetDefaultGuiFont());

		// Resize items
		RECT rcEdit;
		m_wndEdit.GetWindowRect(&rcEdit);
		int nHeight;
		nHeight = max( rcEdit.bottom-rcEdit.top, m_wndList.GetItemHeight(0) );
		nHeight += 4;
		m_wndList.SetItemHeight(0, nHeight);

		// Create navigation buttons
		_AddButton(EDLM_NEW, 0);
		_AddButton(EDLM_DELETE, 1);
		if( (dwStyle & EDLS_NOMOVEBUTTONS)==0 ) {
			_AddButton(EDLM_ITEMUP, 3);
			_AddButton(EDLM_ITEMDOWN, 2);
		}

		// Resize control
		BOOL bDummy;
		OnSize(WM_SIZE, SIZE_RESTORED, 0, bDummy);
	}

	void _AddButton(int iCmd, int iImage)
	{
		ATLASSERT(m_nButtons < sizeof(m_wndButtons)/sizeof(*m_wndButtons));
		m_wndButtons[m_nButtons].Create(m_hWnd, rcDefault, NULL, 0, 0, iCmd);
		m_wndButtons[m_nButtons].SetImageList(m_wndImages);
		m_wndButtons[m_nButtons].SetImages(iImage);
		m_wndButtons[m_nButtons].SetBitmapButtonExtendedStyle(BMPBTN_AUTOSIZE | BMPBTN_HOVER | BMPBTN_AUTO3D_SINGLE | BMPBTN_SHAREIMAGELISTS);
		m_nButtons++;
	}

	LRESULT _SendNotifyMsg(UINT code, int iIndex) const
	{
		NMEDITLIST nm = { 0 };
		nm.hdr.hwndFrom = m_hWnd;
		nm.hdr.idFrom = GetDlgCtrlID();
		nm.hdr.code = code;
		nm.iIndex = iIndex;
		return ::SendMessage(GetParent(), WM_NOTIFY, nm.hdr.idFrom, (LPARAM)&nm);
	}

	void _BeginEdit(int iIndex)
	{
		ATLASSERT(iIndex>=0);
		if( iIndex<0 ) return;
		// Allow owner to cancel action
		if( _SendNotifyMsg(EDLN_BEGINLABELEDIT, iIndex) != 0 ) return;

		// Copy text to Edit control
		int len = m_wndList.GetTextLen(iIndex)+1;
		LPTSTR pstr = (LPTSTR)_alloca(len*sizeof(TCHAR));
		ATLASSERT(pstr);
		m_wndList.GetText(iIndex, pstr);
		m_wndEdit.SetWindowText(pstr);

		RECT rc;
		m_wndList.GetItemRect(iIndex, &rc);
		if( GetStyle() & EDLS_BROWSE ) {
			int cx = rc.bottom - rc.top;
			RECT rcBrowse = { rc.right-cx, rc.top, rc.right, rc.bottom };
			m_wndList.MapWindowPoints(m_hWnd, &rcBrowse);
			m_wndBrowse.SetWindowPos(HWND_TOP, &rcBrowse, SWP_SHOWWINDOW|SWP_NOACTIVATE);
			rc.right -= cx;
		}

		m_wndEdit.SetWindowPos(HWND_TOP, &rc, SWP_SHOWWINDOW);
		m_wndEdit.SetSel(0,-1);
		m_wndEdit.SetFocus();
	}

	// Overridables

	void UpdateLayout()
	{
		RECT rc;
		GetClientRect(&rc);

		RECT rcButtons = rc;
		rcButtons.left = rcButtons.right;
		rcButtons.right -= 2;
		rcButtons.top += 4;
		rcButtons.bottom = rcButtons.top + ::GetSystemMetrics(SM_CYCAPTION) - 8;
		for( int i=m_nButtons-1; i>=0; i-- ) {
			RECT rcButton = rcButtons;
			::GetClientRect(m_wndButtons[i], &rcButton);
			rcButtons.left -= rcButton.right-rcButton.left;
			m_wndButtons[i].SetWindowPos(HWND_TOP, &rcButtons, SWP_NOZORDER|SWP_NOACTIVATE);
			rcButtons.left -= 2;
			rcButtons.right = rcButtons.left;
		}

		rc.top = rc.top + ::GetSystemMetrics(SM_CYCAPTION);
		m_wndList.SetWindowPos(HWND_TOP, &rc, SWP_NOZORDER|SWP_NOACTIVATE);
	}

	void DrawCaption(CDCHandle dc)
	{
		RECT rc;
		GetClientRect(&rc);
		rc.bottom = rc.top + ::GetSystemMetrics(SM_CYCAPTION);

		CBrush brBtnFace;
		// A repaint bug in the WTL CBitmapButton requires us to have the
		// same background color as the IDB_EDITLISTBOX background.
		//brBtnFace.CreateSysColorBrush(COLOR_BTNFACE);
		brBtnFace.CreateSolidBrush(RGB(198,195,198));
		dc.FillRect(&rc, brBtnFace);
		dc.Draw3dRect(&rc, ::GetSysColor(COLOR_BTNSHADOW), ::GetSysColor(COLOR_BTNHIGHLIGHT));

		::InflateRect(&rc, -2, 0);

		dc.SetBkMode(TRANSPARENT);
		dc.SetTextColor(::GetSysColor(COLOR_BTNTEXT));

		HFONT oldFont = dc.SelectFont(AtlGetDefaultGuiFont());

		int len = GetWindowTextLength()+1;
		LPTSTR pstrCaption = (LPTSTR)_alloca(len*sizeof(TCHAR));
		ATLASSERT(pstrCaption);
		GetWindowText(pstrCaption, len);
		dc.DrawText(pstrCaption, -1, &rc, DT_LEFT | DT_SINGLELINE | DT_VCENTER);

		dc.SelectFont(oldFont);
	}

	// Message handlers

	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
	{
		LRESULT lRes = DefWindowProc(uMsg, wParam, lParam);
		_Init();
		return lRes;
	}

	LRESULT OnPaint(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		T* pT = static_cast<T*>(this);
		if(wParam != NULL)
		{
			pT->DrawCaption((HDC)wParam);
		}
		else
		{
			CPaintDC dc(m_hWnd);
			pT->DrawCaption(dc.m_hDC);
		}
		return 0;
	}

	LRESULT OnSize(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		if( wParam != SIZE_MINIMIZED ) {
			T* pT = static_cast<T*>(this);
			pT->UpdateLayout();
		}
		return 0;
	}

	LRESULT OnEnable(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		m_wndList.EnableWindow((BOOL)wParam);
		return 0;
	}

	LRESULT OnEraseBackground(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		return 1; // client area occupied by controls
	}

	LRESULT OnKeyDown(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
	{
		BOOL bDummy;
		switch( LOWORD(wParam) ) {
case VK_INSERT:
	OnNew(0,0,0,bDummy);
	break;
case VK_DELETE:
	OnDelete(0,0,0,bDummy);
	break;
case VK_F2:
	OnDblClick(0,0,0,bDummy);
	break;
		}
		// Always let default handler get a shot at this...
		bHandled = FALSE;
		return 0;
	}

	LRESULT OnDblClick(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		_BeginEdit( m_wndList.GetCurSel() );
		return 0;
	}

	LRESULT OnNew(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		if( m_wndEdit.IsWindowVisible() ) return 0;
		::SetFocus(m_wndList);
		int iIndex = m_wndList.InsertString(m_wndList.GetCount(), _T(""));
		m_wndList.SetCurSel(iIndex);
		if( _SendNotifyMsg(EDLN_INSERTITEM, m_wndList.GetCurSel())==0 ) {
			// Owner accepts insert, so go to edit mode right away...
			_BeginEdit(iIndex);
		}
		else {
			// In case owner cancels the insert and did not change the
			// item text, we delete it again...
			PostMessage(WM_EDITLIST_CLEANUP);
		}
		return 0;
	}

	LRESULT OnDelete(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		if( m_wndEdit.IsWindowVisible() ) return 0;
		int iIndex = m_wndList.GetCurSel();
		if( iIndex==-1 ) return 0;
		if( _SendNotifyMsg(EDLN_DELETEITEM, iIndex)==0 ) {
			m_wndList.DeleteString(iIndex);
		}
		return 0;
	}

	LRESULT OnUp(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		if( m_wndEdit.IsWindowVisible() ) return 0;
		int idxSelected = m_wndList.GetCurSel();
		if( idxSelected==-1 || idxSelected==0 ) return 0;
		int idxOther = idxSelected - 1;
		CComBSTR bstrSelected, bstrOther;
		m_wndList.GetTextBSTR(idxSelected, bstrSelected.m_str);
		m_wndList.GetTextBSTR(idxOther, bstrOther.m_str);
		USES_CONVERSION;
		SetItemText(idxSelected, OLE2CT(bstrOther));
		SetItemText(idxOther, OLE2CT(bstrSelected));
		m_wndList.SetCurSel(idxOther);
		return 0;
	}

	LRESULT OnDown(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		if( m_wndEdit.IsWindowVisible() ) return 0;
		int idxSelected = m_wndList.GetCurSel();
		if( idxSelected==-1 || idxSelected==m_wndList.GetCount()-1 ) return 0;
		int idxOther = idxSelected + 1;
		CComBSTR bstrSelected, bstrOther;
		m_wndList.GetTextBSTR(idxSelected, bstrSelected.m_str);
		m_wndList.GetTextBSTR(idxOther, bstrOther.m_str);
		USES_CONVERSION;
		SetItemText(idxSelected, OLE2CT(bstrOther));
		SetItemText(idxOther, OLE2CT(bstrSelected));
		m_wndList.SetCurSel(idxOther);
		return 0;
	}

	LRESULT OnCleanUp(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		// This is a maintenance message:
		//   Here we remove all empty items.
		for( int i=0; i<m_wndList.GetCount(); i++ ) {
			if( m_wndList.GetTextLen(i)==0 ) {
				m_wndList.DeleteString(i);
				i=-1; // loop again
			}
		}
		return 0;
	}

	LRESULT OnBrowseClick(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		_SendNotifyMsg(EDLN_BROWSE, m_wndList.GetCurSel());
		// We MUST set focus back to the edit control - otherwise
		// the focus logic will be screwed.
		::SetFocus(m_wndEdit);
		return 0;
	}

	// CEdit

	LRESULT OnEditLostFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		// Check if we lost focus to the browse button
		HWND hwndNew = lParam==0 ? ::GetFocus() : (HWND)lParam;
		if( hwndNew==m_wndBrowse ) return DefWindowProc(uMsg, wParam, lParam);

		int iIndex = m_wndList.GetCurSel();
		_SendNotifyMsg(EDLN_ENDLABELEDIT, iIndex);
		// Hide controls
		m_wndEdit.ShowWindow(SW_HIDE);
		m_wndBrowse.ShowWindow(SW_HIDE);
		// Ask parent if it accepts the change
		if( _SendNotifyMsg(EDLN_ITEMCHANGING, iIndex)==0 ) {
			// Owner accepted text change.
			// Copy edit text to selected listview item
			int len = m_wndEdit.GetWindowTextLength()+1;
			LPTSTR pstr = (LPTSTR)_alloca(len*sizeof(TCHAR));
			ATLASSERT(pstr);
			m_wndEdit.GetWindowText(pstr, len);        
			SetItemText(iIndex, pstr);
			// Send "Item Changed" notify message
			_SendNotifyMsg(EDLN_ITEMCHANGED, iIndex);
		}
		m_wndList.SetCurSel(iIndex);
		PostMessage(WM_EDITLIST_CLEANUP);
		bHandled = FALSE; // Windows needs this to disable cursor
		return 0;
	}

	LRESULT OnEditKey(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
	{
		switch( wParam ) {
case VK_ESCAPE:
	{
		// Copy original listview text to edit control.
		// When the edit control looses focus, it will
		// transfer this text back to the list.
		int iIndex = m_wndList.GetCurSel();
		int len = m_wndList.GetTextLen(iIndex)+1;
		LPTSTR pstr = (LPTSTR)_alloca(len*sizeof(TCHAR));
		ATLASSERT(pstr);
		m_wndList.GetText(iIndex, pstr);
		m_wndEdit.SetWindowText(pstr);           
	}
	// FALL THROUGH...
case VK_RETURN:
	{
		m_wndList.SetFocus(); // Causes Edit WM_KILLFOCUS
		return 0;
	}
		}
		bHandled = FALSE;
		return 0;
	}

	LRESULT OnEditGetDlgCode(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
	{
		return DefWindowProc(uMsg, wParam, lParam) | DLGC_WANTALLKEYS;
	}
};

class CEditListBox : public CEditListBoxImpl<CEditListBox>
{
public:
	DECLARE_WND_SUPERCLASS(_T("WTL_EditListBox"), GetWndClassName())
};


#endif // !defined(AFX_EDITLISTBOX_H__20010311_8A0B_FC46_5D30_0080AD509054__INCLUDED_)
