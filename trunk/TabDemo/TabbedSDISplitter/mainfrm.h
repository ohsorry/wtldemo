// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__E73C44B1_1D57_4EE3_A5D8_872E1558D245__INCLUDED_)
#define AFX_MAINFRM_H__E73C44B1_1D57_4EE3_A5D8_872E1558D245__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CMainFrame : public CFrameWindowImpl<CMainFrame>, public CUpdateUI<CMainFrame>,
		public CMessageFilter, public CIdleHandler
{
public:
	DECLARE_FRAME_WND_CLASS(NULL, IDR_MAINFRAME)

	CSplitterWindow m_splitter;

	CPaneContainer m_paneTree;
	CLeftPaneTreeView m_viewTree;

	CTabbedChildWindow< CDotNetTabCtrl<CTabViewTabItem> > m_tabbedChildWindow;
	CMyCustomListView m_view1;
	CSdiDlgView m_view2;
	CSomeChildView m_view3;

	CCommandBarCtrl m_CmdBar;

public:
	enum
	{
		WM_POSTCREATE = WM_APP + 1,
	};

// Constructors
public:

// Message Handling
public:

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnIdle();

	BEGIN_UPDATE_UI_MAP(CMainFrame)
		UPDATE_ELEMENT(ID_VIEW_TOOLBAR, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_VIEW_STATUS_BAR, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_VIEW_TREE, UPDUI_MENUPOPUP)
	END_UPDATE_UI_MAP()

	BEGIN_MSG_MAP(CMainFrame)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(WM_POSTCREATE, OnPostCreate)
		MESSAGE_HANDLER(WM_SETTINGCHANGE, OnSettingChange)
		MESSAGE_HANDLER(WM_SYSCOLORCHANGE, OnSettingChange)
		COMMAND_ID_HANDLER(ID_PANE_CLOSE, OnViewTree)
		COMMAND_ID_HANDLER(ID_APP_EXIT, OnFileExit)
		COMMAND_ID_HANDLER(ID_FILE_NEW, OnFileNew)
		COMMAND_ID_HANDLER(ID_VIEW_TOOLBAR, OnViewToolBar)
		COMMAND_ID_HANDLER(ID_VIEW_STATUS_BAR, OnViewStatusBar)
		COMMAND_ID_HANDLER(ID_VIEW_TREE, OnViewTree)
		COMMAND_ID_HANDLER(ID_APP_ABOUT, OnAppAbout)
		CHAIN_COMMANDS_MEMBER(m_tabbedChildWindow)
		CHAIN_MSG_MAP(CUpdateUI<CMainFrame>)
		CHAIN_MSG_MAP(CFrameWindowImpl<CMainFrame>)
		// Be sure to have DEFAULT_REFLECTION_HANDLER in the message
		// map for child windows
		REFLECT_NOTIFICATIONS()
	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnPostCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnSettingChange(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnFileExit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnFileNew(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnViewToolBar(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnViewStatusBar(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnViewTree(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__E73C44B1_1D57_4EE3_A5D8_872E1558D245__INCLUDED_)
