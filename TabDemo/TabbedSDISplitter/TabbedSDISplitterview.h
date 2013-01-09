// TabbedSDISplitterView.h
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TABBEDSDISplitterVIEW_H__3A3EDE40_24E0_4EB6_8AEE_6A03B7E6BD67__INCLUDED_)
#define AFX_TABBEDSDISplitterVIEW_H__3A3EDE40_24E0_4EB6_8AEE_6A03B7E6BD67__INCLUDED_

#pragma once

class CLeftPaneTreeView : public CWindowImpl<CLeftPaneTreeView, CTreeViewCtrlEx>
{
protected:
	WTL::CImageList m_ImageList;

public:
	DECLARE_WND_SUPERCLASS(_T("MyTree"), CTreeViewCtrlEx::GetWndClassName())

	BOOL PreTranslateMessage(MSG* pMsg);

	BEGIN_MSG_MAP(CLeftPaneTreeView)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		REFLECTED_NOTIFY_CODE_HANDLER(TVN_SELCHANGED, OnSelChanged)
		DEFAULT_REFLECTION_HANDLER()
	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnSelChanged(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)
	{
		ATLTRACE(_T("Received reflected TVN_SELCHANGED\n"));
		return 0;
	}

public:
	void LoadDemoNodes(int cx = 16, int cy = 16);
};

class CSomeChildView :
	public CWindowImpl<CSomeChildView, CWindow>,
	public COffscreenDrawRect<CSomeChildView>
{
public:
	DECLARE_FRAME_WND_CLASS(_T("SomeChildView"), IDI_DOCUMENT)

	BEGIN_MSG_MAP(CSomeChildView)
		CHAIN_MSG_MAP(COffscreenDrawRect<CSomeChildView>)
		DEFAULT_REFLECTION_HANDLER()
	END_MSG_MAP()

public:
	void DoPaint(CDCHandle dc, RECT& rcClip);
};

class CSomeSelfDeletingChildView :
	public CWindowImpl<CSomeSelfDeletingChildView, CWindow>,
	public COffscreenDrawRect<CSomeSelfDeletingChildView>
{
public:
	DECLARE_FRAME_WND_CLASS(_T("SomeSelfDeletingChildView"), IDI_DOCUMENT)

	virtual void OnFinalMessage(HWND /*hWnd*/)
	{
		delete this;
	}

	BEGIN_MSG_MAP(CSomeSelfDeletingChildView)
		CHAIN_MSG_MAP(COffscreenDrawRect<CSomeSelfDeletingChildView>)
		MESSAGE_HANDLER(WM_SIZE, OnSize)
		DEFAULT_REFLECTION_HANDLER()
	END_MSG_MAP()

	LRESULT OnSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

public:
	void DoPaint(CDCHandle dc, RECT& rcClip);
};

class CSdiDlgView :
	public CDialogImpl<CSdiDlgView>,
	public CDialogResize<CSdiDlgView>
{
public:
	enum { IDD = IDD_SDIDLG_FORM };

	BOOL PreTranslateMessage(MSG* pMsg);

	BEGIN_MSG_MAP(CSdiDlgView)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_FORWARDMSG, OnForwardMsg)
		CHAIN_MSG_MAP(CDialogResize<CSdiDlgView>)
		DEFAULT_REFLECTION_HANDLER()
	END_MSG_MAP()

    BEGIN_DLGRESIZE_MAP(CSdiDlgView)
		DLGRESIZE_CONTROL(IDC_EDIT1, DLSZ_SIZE_X)
		DLGRESIZE_CONTROL(IDC_BUTTON1, DLSZ_MOVE_X)
		DLGRESIZE_CONTROL(IDC_EDIT2, (DLSZ_SIZE_X | DLSZ_SIZE_Y))
    END_DLGRESIZE_MAP()

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnForwardMsg(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
};

class CMyCustomListView :
	public CWindowImpl<CMyCustomListView, CListViewCtrl>,
	public CCustomDraw<CMyCustomListView>
{
protected:
	typedef CMyCustomListView thisClass;
	typedef CWindowImpl<CMyCustomListView, CListViewCtrl> baseClass;
	typedef CCustomDraw<CMyCustomListView> customDrawClass;

public:
	DECLARE_WND_SUPERCLASS(_T("MyList"), CListViewCtrl::GetWndClassName())

	BOOL PreTranslateMessage(MSG* pMsg) { pMsg; return FALSE; }

	BEGIN_MSG_MAP(CMyCustomListView)
		//MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
		MESSAGE_HANDLER(WM_PRINTCLIENT, OnPaint)
		MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBackground)
		CHAIN_MSG_MAP_ALT(customDrawClass, 1)
		DEFAULT_REFLECTION_HANDLER()
	END_MSG_MAP()

	LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnEraseBackground(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

// Overrides of CCustomDraw to do our custom list view drawing
public:
	DWORD OnPrePaint(int /*idCtrl*/, LPNMCUSTOMDRAW /*lpNMCustomDraw*/);
	DWORD OnItemPrePaint(int /*idCtrl*/, LPNMCUSTOMDRAW /*lpNMCustomDraw*/);
	DWORD OnItemPostPaint(int /*idCtrl*/, LPNMCUSTOMDRAW /*lpNMCustomDraw*/);
};



/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TABBEDSDISplitterVIEW_H__3A3EDE40_24E0_4EB6_8AEE_6A03B7E6BD67__INCLUDED_)
