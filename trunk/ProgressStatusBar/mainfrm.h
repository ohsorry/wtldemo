// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__980B9012_8875_479F_9D25_53D6B95B2597__INCLUDED_)
#define AFX_MAINFRM_H__980B9012_8875_479F_9D25_53D6B95B2597__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000		

#include "MultiPaneStatusBarEx.h"
#include "codepr.h"

#pragma comment ( lib,  "shlwapi.lib" )

class CMainFrame : public CFrameWindowImpl<CMainFrame>, public CUpdateUI<CMainFrame>,
		public CMessageFilter, public CIdleHandler
{
public:
	DECLARE_FRAME_WND_CLASS(NULL, IDR_MAINFRAME)

	CProgressStatusBarView m_view;

	CCommandBarCtrl m_CmdBar;

	virtual BOOL PreTranslateMessage(MSG* pMsg)
	{
		if(CFrameWindowImpl<CMainFrame>::PreTranslateMessage(pMsg))
			return TRUE;

		return m_view.PreTranslateMessage(pMsg);
	}

	virtual BOOL OnIdle()
	{
		UIUpdateToolBar();
		return FALSE;
	}

	BEGIN_UPDATE_UI_MAP(CMainFrame)
		UPDATE_ELEMENT(ID_VIEW_TOOLBAR, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_VIEW_STATUS_BAR, UPDUI_MENUPOPUP)
	END_UPDATE_UI_MAP()

	BEGIN_MSG_MAP(CMainFrame)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		COMMAND_ID_HANDLER(ID_APP_EXIT, OnFileExit)
		COMMAND_ID_HANDLER(ID_FILE_NEW, OnFileNew)
		COMMAND_ID_HANDLER(ID_VIEW_TOOLBAR, OnViewToolBar)
		COMMAND_ID_HANDLER(ID_VIEW_STATUS_BAR, OnViewStatusBar)
		COMMAND_ID_HANDLER(ID_APP_ABOUT, OnAppAbout)
		CHAIN_MSG_MAP(CUpdateUI<CMainFrame>)
		CHAIN_MSG_MAP(CFrameWindowImpl<CMainFrame>)
	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

    CMPSBarWithAll m_sBar;
	CUrlDlg m_CodeProWtl;

	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
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
		// Make sure the status bar is multi-pane, divide as necessary.
		m_sBar.SubclassWindow(m_hWndStatusBar);
		int arrParts[] = 
		{ 
			ID_DEFAULT_PANE, 
			ID_PANE_1,
			ID_PANE_2,
			ID_PANE_3,
			ID_PANE_4
		};
		m_sBar.SetPanes(arrParts, sizeof(arrParts) / sizeof(arrParts[0]), false);
		m_sBar.SetText(1, " Distorted icon. ");
		m_CodeProWtl.Create(m_sBar.m_hWnd);
		m_sBar.AddAny(2, m_CodeProWtl.m_hWnd);
		m_sBar.SetIcon(0, AtlLoadIconImage(IDR_MAINFRAME, 
			                               LR_DEFAULTCOLOR,
										   ::GetSystemMetrics(SM_CXSMICON) * 2,
										   ::GetSystemMetrics(SM_CYSMICON)));
		m_sBar.SetIcon(4, AtlLoadIconImage(IDI_IC_BITMAP, 
			                               LR_DEFAULTCOLOR,
										   ::GetSystemMetrics(SM_CXSMICON) * 3,
										   ::GetSystemMetrics(SM_CYSMICON)));
		m_sBar.SetIcon(1, AtlLoadIconImage(IDI_IC_BITMAP2,
			                               0,
										   ::GetSystemMetrics(SM_CXSMICON) * 6,
										   ::GetSystemMetrics(SM_CYSMICON)));
        // Load one of four bitmaps on pane #4.			
		SYSTEMTIME st;
		GetLocalTime(&st);
		switch (st.wSecond % 4)
		{
		case 0:
			m_sBar.SetBitmap(3, IDB_BITMAP1);
			break;
		case 1:
			m_sBar.SetBitmap(3, IDB_BITMAP2);
			break;
		case 2:
			m_sBar.SetBitmap(3, IDB_BITMAP3);
			break;
		default:
			m_sBar.SetBitmap(3, IDB_BITMAP4);
			break;
		}

    	m_hWndClient = m_view.Create(m_hWnd);
		m_view.pParentStatus = &m_sBar;	// Allow the view to update the status bar.

		UIAddToolBar(hWndToolBar);
		UISetCheck(ID_VIEW_TOOLBAR, 1);
		UISetCheck(ID_VIEW_STATUS_BAR, 1);

		// register object for message filtering and idle updates
		CMessageLoop* pLoop = _Module.GetMessageLoop();
		ATLASSERT(pLoop != NULL);
		pLoop->AddMessageFilter(this);
		pLoop->AddIdleHandler(this);

		return 0;
	}

	LRESULT OnFileExit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		PostMessage(WM_CLOSE);
		return 0;
	}

	LRESULT OnFileNew(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		// TODO: add code to initialize document

		return 0;
	}

	LRESULT OnViewToolBar(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
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

	LRESULT OnViewStatusBar(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		BOOL bVisible = !::IsWindowVisible(m_hWndStatusBar);
		::ShowWindow(m_hWndStatusBar, bVisible ? SW_SHOWNOACTIVATE : SW_HIDE);
		UISetCheck(ID_VIEW_STATUS_BAR, bVisible);
		UpdateLayout();
		return 0;
	}

	LRESULT OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		CAboutDlg dlg;
		dlg.DoModal();
		return 0;
	}
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__980B9012_8875_479F_9D25_53D6B95B2597__INCLUDED_)
