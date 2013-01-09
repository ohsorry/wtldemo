// MainFrm.cpp : implmentation of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "aboutdlg.h"
#include "TabbedSdiSplitterView.h"
#include "MainFrm.h"

BOOL CMainFrame::PreTranslateMessage(MSG* pMsg)
{
	if(CFrameWindowImpl<CMainFrame>::PreTranslateMessage(pMsg))
		return TRUE;

	//return m_view.PreTranslateMessage(pMsg);

	HWND hWndFocus = ::GetFocus();

	if(m_tabbedChildWindow.IsChild(hWndFocus))
	{
		if(m_tabbedChildWindow.PreTranslateMessage(pMsg))
		{
			return TRUE;
		}
	}
	else if(m_paneTree.IsChild(hWndFocus))
	{
		if(m_viewTree.PreTranslateMessage(pMsg))
		{
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CMainFrame::OnIdle()
{
	UIUpdateToolBar();
	return FALSE;
}

LRESULT CMainFrame::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// create command bar window
	HWND hWndCmdBar = m_CmdBar.Create(m_hWnd, rcDefault, NULL, ATL_SIMPLE_CMDBAR_PANE_STYLE);
	// attach menu
	m_CmdBar.AttachMenu(GetMenu());
	// load command bar images
	m_CmdBar.LoadImages(IDR_MAINFRAME);
	// remove old menu
	SetMenu(NULL);

	HWND hWndToolBar = CreateSimpleToolBarCtrl(m_hWnd, IDR_MAINFRAME, FALSE, ATL_SIMPLE_TOOLBAR_PANE_STYLE);

	CreateSimpleReBar(ATL_SIMPLE_REBAR_NOBORDER_STYLE);
	AddSimpleReBarBand(hWndCmdBar);
	AddSimpleReBarBand(hWndToolBar, NULL, TRUE);

	CreateSimpleStatusBar();

	// register object for message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->AddMessageFilter(this);
	pLoop->AddIdleHandler(this);

	UIAddToolBar(hWndToolBar);
	UISetCheck(ID_VIEW_TOOLBAR, 1);
	UISetCheck(ID_VIEW_STATUS_BAR, 1);
	UISetCheck(ID_VIEW_TREE, 1);


	m_hWndClient = m_splitter.Create(m_hWnd, rcDefault, NULL,
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);

	m_splitter.SetSplitterExtendedStyle(0);
	//SetSplitterPos works best in a "Post-Create" step
	//m_splitter.SetSplitterPos(200);

	m_paneTree.Create(m_splitter, _T("Tree"));

	// If you plan on having any tab views needing
	// notifications to be reflected (such as if you
	// have a custom draw list view), be sure to include
	// the following line.  If you know you won't need
	// notifications to be reflected, it can cut down
	// on the message traffic if you don't.
	// Also beware that reflecting notifications back
	// to some common controls (especially a tree view)
	// can cause visual anomolies unless you superclass
	// the control and add "DEFAULT_REFLECTION_HANDLER()"
	// to the message map.
	m_tabbedChildWindow.SetReflectNotifications(true);

	m_tabbedChildWindow.SetTabStyles(CTCS_TOOLTIPS | CTCS_DRAGREARRANGE);
	m_tabbedChildWindow.Create(m_splitter, rcDefault);
	//m_tabbedChildWindow.GetTabCtrl().ModifyStyle(CTCS_BOTTOM, (CTCS_SCROLL | CTCS_CLOSEBUTTON));

	m_viewTree.Create(m_paneTree, rcDefault, NULL,
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | TVS_HASLINES | TVS_HASBUTTONS | TVS_LINESATROOT | TVS_SHOWSELALWAYS,
		WS_EX_CLIENTEDGE);
	m_paneTree.SetClient(m_viewTree);

	m_view1.Create(m_tabbedChildWindow, rcDefault, NULL,
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | LVS_REPORT,
		0);
	m_view2.Create(m_tabbedChildWindow);
	m_view3.Create(m_tabbedChildWindow, rcDefault, _T("Other View"),
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
		0);

	m_view1.InsertColumn(0, _T("Column"), LVCFMT_LEFT, 200, 0);

	LVITEM lvItem = {0};
	lvItem.mask = LVIF_TEXT;
	lvItem.iItem = 0;
	lvItem.pszText = _T("Item 1");
	m_view1.InsertItem(&lvItem);

	lvItem.iItem = 1;
	lvItem.pszText = _T("Item 2");
	m_view1.InsertItem(&lvItem);

	lvItem.iItem = 2;
	lvItem.pszText = _T("Item 3");
	m_view1.InsertItem(&lvItem);

	m_tabbedChildWindow.AddTabWithIcon(m_view1, _T("List View"), IDI_DOCUMENT);

	// You can also have the view windows provide an icon
	// (such as by using DECLARE_FRAME_WND_CLASS instead of DECLARE_WND_CLASS)
	// and window/frame caption text (provided in Create or CreateEx call),
	// and calling "DisplayTab"
	m_tabbedChildWindow.DisplayTab(m_view2, TRUE, TRUE);
	m_tabbedChildWindow.DisplayTab(m_view3, TRUE, TRUE);

	m_splitter.SetSplitterPanes(m_paneTree, m_tabbedChildWindow);

	this->PostMessage(WM_POSTCREATE);

	return 0;
}

LRESULT CMainFrame::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->RemoveMessageFilter(this);
	pLoop->RemoveIdleHandler(this);

	bHandled = FALSE;
	return 0;
}

LRESULT CMainFrame::OnPostCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	//SetSplitterPos works best after the default WM_CREATE has been handled
	m_splitter.SetSplitterPos(200);

	return 0;
}

LRESULT CMainFrame::OnSettingChange(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
	// System settings or metrics have changed.  Propogate this message
	// to all the child windows so they can update themselves as appropriate.
	this->SendMessageToDescendants(uMsg, wParam, lParam, TRUE);

	return 0;
}

LRESULT CMainFrame::OnFileExit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	PostMessage(WM_CLOSE);
	return 0;
}

LRESULT CMainFrame::OnFileNew(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	static int nCount = 1;
	CString sText;
	sText.Format(_T("View %d"), nCount++);

	CSomeSelfDeletingChildView* pChild = new CSomeSelfDeletingChildView;
	pChild->Create(m_tabbedChildWindow, rcDefault, sText);
	m_tabbedChildWindow.DisplayTab(pChild->m_hWnd, TRUE, TRUE);

	return 0;
}

LRESULT CMainFrame::OnViewToolBar(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	static BOOL bVisible = TRUE;	// initially visible
	bVisible = !bVisible;
	CReBarCtrl rebar = m_hWndToolBar;
	int nBandIndex = rebar.IdToIndex(ATL_IDW_BAND_FIRST + 1);	// toolbar is 2nd added band
	rebar.ShowBand(nBandIndex, bVisible);
	UISetCheck(ID_VIEW_TOOLBAR, bVisible);
	UpdateLayout();
	return 0;
}

LRESULT CMainFrame::OnViewStatusBar(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	BOOL bVisible = !::IsWindowVisible(m_hWndStatusBar);
	::ShowWindow(m_hWndStatusBar, bVisible ? SW_SHOWNOACTIVATE : SW_HIDE);
	UISetCheck(ID_VIEW_STATUS_BAR, bVisible);
	UpdateLayout();
	return 0;
}

LRESULT CMainFrame::OnViewTree(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	BOOL bVisible = !::IsWindowVisible(m_paneTree);
	::ShowWindow(m_paneTree, bVisible ? SW_SHOWNOACTIVATE : SW_HIDE);
	UISetCheck(ID_VIEW_TREE, bVisible);
	m_splitter.SetSinglePaneMode(bVisible ? SPLIT_PANE_NONE : SPLIT_PANE_RIGHT);
	UpdateLayout();
	return 0;
}

LRESULT CMainFrame::OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CAboutDlg dlg;
	dlg.DoModal();
	return 0;
}
