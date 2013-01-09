// MainFrm.cpp : implmentation of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "aboutdlg.h"
#include "WTLClockView.h"
#include "MainFrm.h"
BOOL CMainFrame::PreTranslateMessage(MSG* pMsg)
{
    if(CFrameWindowImpl<CMainFrame>::PreTranslateMessage(pMsg))
        return TRUE;

    return m_view.PreTranslateMessage(pMsg);
}

BOOL CMainFrame::OnIdle()
{
	UIUpdateStatusBar();
    return FALSE;
}
LRESULT CMainFrame::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    m_hWndClient = m_view.Create(m_hWnd, rcDefault, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, WS_EX_CLIENTEDGE);


	CreateSimpleToolBar();
    CreateSimpleStatusBar();
	::ShowWindow(m_hWndToolBar,TRUE);
    // register object for message filtering and idle updates
    CMessageLoop* pLoop = _Module.GetMessageLoop();
    ATLASSERT(pLoop != NULL);
	//    pLoop->AddMessageFilter(this);
    pLoop->AddIdleHandler(this);

    // Set the initial state of the Clock menu items:
    UIEnable ( IDC_START, false );
    UIEnable ( IDC_STOP, true );
	UISetCheck(IDC_START,TRUE);
	// 
	m_status.SubclassWindow(m_hWndStatusBar);
	int arrPanes[] = { 0,1,2};
	m_status.SetPanes(arrPanes, sizeof(arrPanes) / sizeof(int), false);

	// set status bar pane widths using local workaround
	int arrWidths[] = { 0, 90, 60 };
	SetPaneWidths(arrWidths, sizeof(arrWidths) / sizeof(int));

	// set the status bar pane icons
	m_status.SetPaneIcon(0, AtlLoadIconImage(IDI_ICON1, LR_DEFAULTCOLOR));
	m_status.SetPaneIcon(1, AtlLoadIconImage(IDI_ICON2, LR_DEFAULTCOLOR));
	m_status.SetPaneIcon(2, AtlLoadIconImage(IDI_ICON3, LR_DEFAULTCOLOR));
	m_status.SetPaneText(0,"HelloWorld");
	m_status.SetPaneText(1,"1");
	m_status.SetPaneText(2,"2");
    return 0;

}

LRESULT CMainFrame::OnFileExit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    PostMessage(WM_CLOSE);
    return 0;
}

LRESULT CMainFrame::OnFileNew(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    // TODO: add code to initialize document

    return 0;
}

LRESULT CMainFrame::OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    CAboutDlg dlg;
    dlg.DoModal();
    return 0;
}

void CMainFrame::OnStart(UINT uCode, int nID, HWND hwndCtrl)
{
    // Enable Stop and disable Start
    UIEnable ( IDC_START, false );
    UIEnable ( IDC_STOP, true );

    // Tell the view to start its clock.
    m_view.StartClock();
}

void CMainFrame::OnStop(UINT uCode, int nID, HWND hwndCtrl)
{
    // Enable Start and disable Stop
    UIEnable ( IDC_START, true );
    UIEnable ( IDC_STOP, false );

    // Tell the view to stop its clock.
    m_view.StopClock();
}

	LRESULT CMainFrame:: OnToolbarShow(UINT, int id, HWND hwnd,BOOL&) 
	{
		::ShowWindow(m_hWndToolBar,SW_SHOW);
		UpdateLayout();
		UIEnable(ID_TOOLBAR_SHOW,FALSE);
		UIEnable(ID_TOOLBAR_HIDE,TRUE);
		UISetCheck(ID_TOOLBAR_SHOW,TRUE);
		UISetCheck(ID_TOOLBAR_HIDE,FALSE);
		return 0;
	}
	LRESULT CMainFrame:: OnToolbarHide(UINT, int id ,HWND hwnd, BOOL&)
	{
		::ShowWindow(m_hWndToolBar,SW_HIDE);
		UpdateLayout();
		//::HideWindow(m_hWndToolBar);
		UIEnable(ID_TOOLBAR_SHOW,TRUE);
		UIEnable(ID_TOOLBAR_HIDE,FALSE);
		UISetCheck(ID_TOOLBAR_SHOW,FALSE);
		UISetCheck(ID_TOOLBAR_HIDE,TRUE);
		return 0;
	}