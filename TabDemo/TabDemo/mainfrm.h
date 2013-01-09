// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__053AD660_0AE2_11D6_8BF1_00500477589F__INCLUDED_)
#define AFX_MAINFRM_H__053AD660_0AE2_11D6_8BF1_00500477589F__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "SimpleTabCtrls.h"

class CMainFrame : public CTabbedMDIFrameWindowImpl<CMainFrame>, public CUpdateUI<CMainFrame>,
		public CMessageFilter, public CIdleHandler
{
protected:
	typedef CTabbedMDIFrameWindowImpl<CMainFrame> baseClass;
	typedef CMainFrame thisClass;

protected:
	CTabbedPopupFrame<CDotNetTabCtrl<CTabViewTabItem> > m_OutputFrame;

	CPlainTextView m_LogView;
	CPlainTextView m_ErrorsView;
	CPlainTextView m_WarningsView;

public:
	DECLARE_FRAME_WND_CLASS(NULL, IDR_MAINFRAME)

	CTabbedMDICommandBarCtrl m_CmdBar;

	virtual BOOL PreTranslateMessage(MSG* pMsg)
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
		else if(m_OutputFrame.IsWindow() && m_OutputFrame.IsChild(hWndFocus))
		{
			if(m_OutputFrame.PreTranslateMessage(pMsg))
			{
				return TRUE;
			}
		}


		if(!bCalledBaseClass)
		{
			// If the base class hasn't already had a shot at doing
			// PreTranslateMessage (because the main frame or an
			// MDI child didn't have focus), call it now
			if(baseClass::PreTranslateMessage(pMsg))
				return TRUE;
		}

		return FALSE;
	}

	virtual BOOL OnIdle()
	{
		UIUpdateToolBar();
		return FALSE;
	}

	BEGIN_UPDATE_UI_MAP(CMainFrame)
		UPDATE_ELEMENT(ID_VIEW_TOOLBAR, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_VIEW_STATUS_BAR, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_VIEW_OUTPUT_WINDOW, UPDUI_MENUPOPUP)
	END_UPDATE_UI_MAP()

	BEGIN_MSG_MAP(CMainFrame)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(WM_SETTINGCHANGE, OnSettingChange)
		MESSAGE_HANDLER(WM_SYSCOLORCHANGE, OnSettingChange)
		COMMAND_ID_HANDLER(ID_APP_EXIT, OnFileExit)
		COMMAND_ID_HANDLER(ID_FILE_NEW, OnFileNew)
		COMMAND_ID_HANDLER(ID_VIEW_TOOLBAR, OnViewToolBar)
		COMMAND_ID_HANDLER(ID_VIEW_STATUS_BAR, OnViewStatusBar)
		COMMAND_ID_HANDLER(ID_VIEW_OUTPUT_WINDOW, OnViewOutputWindow)
		COMMAND_ID_HANDLER(ID_APP_ABOUT, OnAppAbout)
		COMMAND_ID_HANDLER(ID_WINDOW_CASCADE, OnWindowCascade)
		COMMAND_ID_HANDLER(ID_WINDOW_TILE_HORZ, OnWindowTile)
		COMMAND_ID_HANDLER(ID_WINDOW_ARRANGE, OnWindowArrangeIcons)
		CHAIN_MDI_CHILD_COMMANDS()
		CHAIN_MSG_MAP(CUpdateUI<CMainFrame>)
		CHAIN_MSG_MAP(CTabbedMDIFrameWindowImpl<CMainFrame>)
	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

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

		CreateMDIClient();

		// If you want to adjust the scroll "speed", here's how you could do it:
		//TTabCtrl& TabCtrl = m_tabbedClient.GetTabOwner().GetTabCtrl();
		//TabCtrl.SetScrollDelta(10);
		//TabCtrl.SetScrollRepeat(TTabCtrl::ectcScrollRepeat_Slow);

		// If you want to use the MDI child's document icon, uncomment the following:
		//m_tabbedClient.UseMDIChildIcon(TRUE);

		// Uncomment the following line if you don't want the document icon
		//  and frame caption buttons to show up for maximized MDI children
		//m_CmdBar.UseMaxChildDocIconAndFrameCaptionButtons(false);
		m_CmdBar.SetMDIClient(m_hWndMDIClient);

		UIAddToolBar(hWndToolBar);
		UISetCheck(ID_VIEW_TOOLBAR, 1);
		UISetCheck(ID_VIEW_STATUS_BAR, 1);
		UISetCheck(ID_VIEW_OUTPUT_WINDOW, 1);

		// register object for message filtering and idle updates
		CMessageLoop* pLoop = _Module.GetMessageLoop();
		ATLASSERT(pLoop != NULL);
		pLoop->AddMessageFilter(this);
		pLoop->AddIdleHandler(this);


		// m_OutputFrame
		{
			RECT rcClientArea;
			::GetClientRect(m_hWndClient, &rcClientArea);

			RECT rcOutputFrame = {rcClientArea.left + 5, rcClientArea.bottom - 250, rcClientArea.right - 5, rcClientArea.bottom - 5};

			//If the frame/view is a popup, transform client coords. to screen coords.
			::MapWindowPoints(m_hWndClient, NULL, (LPPOINT)&rcOutputFrame, 2);

			m_OutputFrame.SetCloseCommand(ID_VIEW_OUTPUT_WINDOW);
			m_OutputFrame.CreateEx(m_hWnd, rcOutputFrame);

			// You can let the tabbed frame figure out a reasonable tab height,
			// or you can set it yourself
			//m_OutputFrame.SetTabAreaHeight(24);

			m_LogView.Create(m_OutputFrame, rcDefault, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_HSCROLL | WS_VSCROLL | ES_AUTOHSCROLL | ES_AUTOVSCROLL | ES_MULTILINE | ES_NOHIDESEL, WS_EX_CLIENTEDGE);
			m_ErrorsView.Create(m_OutputFrame, rcDefault, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_HSCROLL | WS_VSCROLL | ES_AUTOHSCROLL | ES_AUTOVSCROLL | ES_MULTILINE | ES_NOHIDESEL, WS_EX_CLIENTEDGE);
			m_WarningsView.Create(m_OutputFrame, rcDefault, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_HSCROLL | WS_VSCROLL | ES_AUTOHSCROLL | ES_AUTOVSCROLL | ES_MULTILINE | ES_NOHIDESEL, WS_EX_CLIENTEDGE);

			m_OutputFrame.AddTabWithIcon(m_LogView, _T("Log"), MAKEINTRESOURCE(IDI_LOG));
			m_OutputFrame.AddTabWithIcon(m_ErrorsView, _T("Errors"), MAKEINTRESOURCE(IDI_ERRORS));
			m_OutputFrame.AddTabWithIcon(m_WarningsView, _T("Warnings"), MAKEINTRESOURCE(IDI_WARNINGS));
		}

		return 0;
	}

	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
	{
		CMessageLoop* pLoop = _Module.GetMessageLoop();
		ATLASSERT(pLoop != NULL);
		pLoop->RemoveMessageFilter(this);
		pLoop->RemoveIdleHandler(this);

		bHandled = FALSE;
		return 0;
	}

	LRESULT OnSettingChange(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
	{
		// System settings or metrics have changed.  Propogate this message
		// to all the child windows so they can update themselves as appropriate.
		this->SendMessageToDescendants(uMsg, wParam, lParam, TRUE);

		return 0;
	}

	LRESULT OnFileExit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		PostMessage(WM_CLOSE);
		return 0;
	}

	LRESULT OnFileNew(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		CHtmlFrame* pChild = new CHtmlFrame;
		pChild->CreateEx(m_hWndClient);

		pChild->SetCommandBarCtrlForContextMenu(&m_CmdBar);

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

	LRESULT OnViewOutputWindow(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		BOOL bVisible = !m_OutputFrame.IsWindowVisible();
		m_OutputFrame.ShowWindow(bVisible ? SW_SHOWNOACTIVATE : SW_HIDE);
		UISetCheck(ID_VIEW_OUTPUT_WINDOW, bVisible);
		UpdateLayout();
		return 0;
	}

	LRESULT OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		CAboutDlg dlg;
		dlg.DoModal();
		return 0;
	}

	LRESULT OnWindowCascade(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		MDICascade();
		return 0;
	}

	LRESULT OnWindowTile(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		MDITile();
		return 0;
	}

	LRESULT OnWindowArrangeIcons(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		MDIIconArrange();
		return 0;
	}
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__053AD660_0AE2_11D6_8BF1_00500477589F__INCLUDED_)
