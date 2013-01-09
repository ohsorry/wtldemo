// MainFrm.cpp : implmentation of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "PlainTextView.h"

#include "aboutdlg.h"
#include "HtmlView.h"
#include "HtmlFrame.h"
#include "MainFrm.h"

BOOL CMainFrame::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg == NULL)
	{
		return FALSE;
	}

	bool bCalledBaseClass = false;

	if(pMsg->hwnd == m_hWnd || pMsg->hwnd == m_hWndMDIClient)
	{
		// Message is sent directly to main frame or
		// to the MDIClient window
		if(baseClass::PreTranslateMessage(pMsg))
			return TRUE;

		bCalledBaseClass = true;
	}

	HWND hWndFocus = ::GetFocus();
	HWND hWndMDIActive = this->MDIGetActive();
	
	if((hWndMDIActive == hWndFocus) || (::IsChild(hWndMDIActive, hWndFocus)))
	{
		// Message is sent to Active MDI child frame
		// or a descendant
		// NOTE: IsChild checks if the window is a direct child or a descendant

		if(baseClass::PreTranslateMessage(pMsg))
			return TRUE;

		bCalledBaseClass = true;

		if(hWndMDIActive != NULL)
		{
			return (BOOL)::SendMessage(hWndMDIActive, WM_FORWARDMSG, 0, (LPARAM)pMsg);
		}
	}
	else
	{
		for(_PaneWindowIter iter=m_PaneWindows.begin(); iter!=m_PaneWindows.end(); iter++)
		{
			CTabbedAutoHideDockingWindow* pPaneWindow = *iter;
			if(pPaneWindow->IsChild(hWndFocus))
			{
				if(pPaneWindow->PreTranslateMessage(pMsg))
				{
					return TRUE;
				}
			}
		}
	}

	if(!bCalledBaseClass)
	{
		// If the base class hasn't already had a shot at doing
		// PreTranslateMessage (because the main frame or an
		// MDI child didn't have focus), call it now
		if(baseClass::PreTranslateMessage(pMsg))
			return TRUE;

		// Give active MDI child a chance.
		if (hWndMDIActive != NULL)
		{
			return (BOOL)::SendMessage(hWndMDIActive, WM_FORWARDMSG, 0, (LPARAM)pMsg);
		}
	}

	return FALSE;
}

BOOL CMainFrame::OnIdle()
{
	UIUpdateToolBar();
	int id=ID_VIEW_PANEFIRST;
	for(_PaneWindowIter iter=m_PaneWindows.begin(); iter!=m_PaneWindows.end(); id++, iter++)
	{
		if((*iter)->IsWindow())
		{
			UISetCheck(id,(*iter)->IsWindowVisible());
		}
	}
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

	CreateMDIClient();

	// If you want to only show MDI tabs when there's more than one window, uncomment the following
	//CTabbedMDIClient<TTabCtrl>::TTabOwner& tabOwner = m_tabbedClient.GetTabOwner();
	//tabOwner.SetMinTabCountForVisibleTabs(2);

	// If you want to adjust the scroll "speed", here's how you could do it:
	//TTabCtrl& tabControl = tabOwner.GetTabCtrl();
	//tabControl.SetScrollDelta(10);
	//tabControl.SetScrollRepeat(TTabCtrl::ectcScrollRepeat_Slow);

	// If you want to use the MDI child's document icon, uncomment the following:
	//m_tabbedClient.UseMDIChildIcon(TRUE);

	// If you want to only show MDI tabs when the MDI children
	// are maximized, uncomment the following:
	//m_tabbedClient.HideMDITabsWhenMDIChildNotMaximized(TRUE);

	m_tabbedClient.SetTabOwnerParent(m_hWnd);
	BOOL bSubclass = m_tabbedClient.SubclassWindow(m_hWndMDIClient);
	bSubclass; // avoid warning on level 4
	//m_CmdBar.UseMaxChildDocIconAndFrameCaptionButtons(true);
	m_CmdBar.UseMaxChildDocIconAndFrameCaptionButtons(false);
	m_CmdBar.SetMDIClient(m_hWndMDIClient);

	UIAddToolBar(hWndToolBar);
	UISetCheck(ID_VIEW_TOOLBAR, 1);
	UISetCheck(ID_VIEW_STATUS_BAR, 1);

	// register object for message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->AddMessageFilter(this);
	pLoop->AddIdleHandler(this);


	InitializeDockingFrame();

	InitializeDefaultPanes();

	PostMessage(CWM_INITIALIZE);

	return 0;
}

LRESULT CMainFrame::OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	//bool bSafeToClose = m_tabbedClient.SaveAllModified(true, true);
	bool bSafeToClose = true;
	if(!bSafeToClose)
	{
		// Someone "cancelled".  Don't let DefWindowProc, or
		// anyone else have WM_CLOSE (the one sent to main frame)
		bHandled = TRUE;
		return 0;
	}

	this->UninitializeDefaultPanes();

	m_tabbedClient.UnsubclassWindow();

	bHandled = FALSE;
	return 0;
}

LRESULT CMainFrame::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->RemoveMessageFilter(this);
	pLoop->RemoveIdleHandler(this);

	// NOTE: the pane windows will delete themselves,
	//  so we just need to remove them from out list
	m_PaneWindows.erase(m_PaneWindows.begin(),m_PaneWindows.end());

	for(_PaneWindowIconsIter iter=m_PaneWindowIcons.begin(); iter!=m_PaneWindowIcons.end(); iter++)
	{
		::DestroyIcon(*iter);
		*iter = NULL;
	}
	m_PaneWindowIcons.erase(m_PaneWindowIcons.begin(), m_PaneWindowIcons.end());

	bHandled = FALSE;
	return 0;
}

LRESULT CMainFrame::OnSettingChange(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
	// Call the base class handler first
	BOOL bBaseHandled = FALSE;
	baseClass::OnSettingChange(uMsg,wParam,lParam,bBaseHandled);

	// Then broadcast to every descendant
	// (NOTE: If there are other top level windows,
	// they need to handle WM_SETTINGCHANGE themselves)
	this->SendMessageToDescendants(uMsg, wParam, lParam, TRUE);

	return 0;
}

LRESULT CMainFrame::OnSysColorChange(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// If baseClass defines OnSysColorChange, uncomment following code,
	// and removed "bHandled = FALSE;"
	bHandled = FALSE;

	//// Call the base class handler first
	//BOOL bBaseHandled = FALSE;
	//baseClass::OnSysColorChange(uMsg,wParam,lParam,bBaseHandled);

	// Then broadcast to every descendant
	// (NOTE: If there are other top level windows,
	// they need to handle WM_SETTINGCHANGE themselves)
	this->SendMessageToDescendants(uMsg, wParam, lParam, TRUE);

	return 0;
}

LRESULT CMainFrame::OnQueryEndSession(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	m_tabbedClient.CloseAll();
	return 0;
}

LRESULT CMainFrame::OnInitialize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	sstate::CDockWndMgr mgrDockWnds;

	for(_PaneWindowIter iter=m_PaneWindows.begin(); iter!=m_PaneWindows.end(); iter++)
	{
		mgrDockWnds.Add(sstate::CDockingWindowStateAdapter<CTabbedAutoHideDockingWindow>(*(*iter)));
	}

	// NOTE: If you want to match "nCmdShow" from the Run call, pass that to here
	//  and use instead of SW_SHOWDEFAULT.  Or you can force something like SW_SHOWMAXIMIZED.
	m_stateMgr.Initialize(_T("SOFTWARE\\WTL Docking Window\\DockingDemo"),m_hWnd, SW_SHOWDEFAULT);
	m_stateMgr.Add(sstate::CRebarStateAdapter(m_hWndToolBar));
	m_stateMgr.Add(sstate::CToggleWindowAdapter(m_hWndStatusBar));
	m_stateMgr.Add(mgrDockWnds);
	m_stateMgr.Restore();
	UpdateLayout();
	return 0;
}

LRESULT CMainFrame::OnFileExit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	PostMessage(WM_CLOSE);
	return 0;
}

LRESULT CMainFrame::OnFileNew(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CHtmlFrame* pChild = new CHtmlFrame;
	pChild->CreateEx(m_hWndClient);

	pChild->SetCommandBarCtrlForContextMenu(&m_CmdBar);

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

LRESULT CMainFrame::OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CAboutDlg dlg;
	dlg.DoModal();
	return 0;
}

LRESULT CMainFrame::OnWindowCascade(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	MDICascade();
	return 0;
}

LRESULT CMainFrame::OnWindowTile(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	MDITile();
	return 0;
}

LRESULT CMainFrame::OnWindowArrangeIcons(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	MDIIconArrange();
	return 0;
}

void CMainFrame::InitializeDefaultPanes(void)
{
	CRect rcClient;
	this->GetClientRect(&rcClient);

	CRect rcFloat(0,0,400,200);
	CRect rcDock(0,0,150,rcClient.Width()-200);

	CImageList ilIcons;
	ilIcons.Create(IDB_MSDEV_TAB_ICONS, 16, 4, RGB(0,255,0));

	HWND hWndFirst = 
	this->CreatePlainTextOutputPane(m_TaskListView,      _T("Task List"),      ilIcons.ExtractIcon(6),  rcFloat, rcDock, NULL);
	this->CreatePlainTextOutputPane(m_CommandWindowView, _T("Command Window"), ilIcons.ExtractIcon(10), rcFloat, rcDock, hWndFirst);
	this->CreatePlainTextOutputPane(m_OutputView,        _T("Output"),         ilIcons.ExtractIcon(3),  rcFloat, rcDock, hWndFirst);
	this->CreatePlainTextOutputPane(m_FindResultsView,   _T("Find Results 1"), ilIcons.ExtractIcon(11), rcFloat, rcDock, hWndFirst);
	this->CreateDialogPane(_T("Test Dialog"),  rcFloat, rcDock, hWndFirst);

}

void CMainFrame::UninitializeDefaultPanes(void)
{
	m_TaskListView.SendMessage(WM_CLOSE);
	m_CommandWindowView.SendMessage(WM_CLOSE);
	m_OutputView.SendMessage(WM_CLOSE);
	m_FindResultsView.SendMessage(WM_CLOSE);
	m_TestDialog.SendMessage(WM_CLOSE);

	if(m_TaskListView.IsWindow())
	{
		m_TaskListView.DestroyWindow();
	}
	if(m_CommandWindowView.IsWindow())
	{
		m_CommandWindowView.DestroyWindow();
	}
	if(m_OutputView.IsWindow())
	{
		m_OutputView.DestroyWindow();
	}
	if(m_FindResultsView.IsWindow())
	{
		m_FindResultsView.DestroyWindow();
	}
	if(m_TestDialog.IsWindow())
	{
		m_TestDialog.DestroyWindow();
	}
}

HWND CMainFrame::CreatePlainTextOutputPane(CPlainTextView& view, LPCTSTR sName, HICON hIcon, CRect& rcFloat, CRect& rcDock, HWND hDockTo)
{
	HWND hWndPane = NULL;

	// Task List
	CTabbedAutoHideDockingWindow* pPaneWindow = CTabbedAutoHideDockingWindow::CreateInstance();
	if(pPaneWindow)
	{
		DWORD dwStyle=WS_OVERLAPPEDWINDOW | WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
		hWndPane = pPaneWindow->Create(m_hWnd,rcFloat,sName,dwStyle);
		DockWindow(
			*pPaneWindow,
			dockwins::CDockingSide(dockwins::CDockingSide::sBottom),
			0 /*nBar*/,
			float(0.0)/*fPctPos*/,
			rcDock.Width() /* nWidth*/,
			rcDock.Height() /* nHeight*/);

		view.Create(hWndPane, rcDefault, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_HSCROLL | WS_VSCROLL | ES_AUTOHSCROLL | ES_AUTOVSCROLL | ES_MULTILINE | ES_NOHIDESEL, WS_EX_CLIENTEDGE);
		view.SetIcon(hIcon, ICON_SMALL);

		m_PaneWindowIcons.insert(m_PaneWindowIcons.end(), hIcon);
		m_PaneWindows.insert(m_PaneWindows.end(), pPaneWindow);

		pPaneWindow->SetReflectNotifications(false);
		pPaneWindow->SetClient(view);

		if(hDockTo)
		{
			pPaneWindow->DockTo(hDockTo, (int)m_PaneWindows.size());
		}	

	}

	return hWndPane;
}

HWND CMainFrame::CreateDialogPane(LPCTSTR sName, CRect& rcFloat, CRect& rcDock, HWND hDockTo)
{
	HWND hWndPane = NULL;

	// Task List
	CTabbedAutoHideDockingWindow* pPaneWindow = CTabbedAutoHideDockingWindow::CreateInstance();
	if(pPaneWindow)
	{
		DWORD dwStyle=WS_OVERLAPPEDWINDOW | WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
		hWndPane = pPaneWindow->Create(m_hWnd,rcFloat,sName,dwStyle);
		DockWindow(
			*pPaneWindow,
			dockwins::CDockingSide(dockwins::CDockingSide::sBottom),
			0 /*nBar*/,
			float(0.0)/*fPctPos*/,
			rcDock.Width() /* nWidth*/,
			rcDock.Height() /* nHeight*/);

		m_TestDialog.Create(hWndPane);
		//m_TestDialog.SetIcon(hIcon, ICON_SMALL);

		//m_PaneWindowIcons.insert(m_PaneWindowIcons.end(), hIcon);
		m_PaneWindows.insert(m_PaneWindows.end(), pPaneWindow);

		pPaneWindow->SetReflectNotifications(false);
		pPaneWindow->SetClient(m_TestDialog);

		if(hDockTo)
		{
			pPaneWindow->DockTo(hDockTo, (int)m_PaneWindows.size());
		}	

	}

	return hWndPane;
}
