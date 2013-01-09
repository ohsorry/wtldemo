// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__B740C89B_5A25_4475_9F16_7CE57F641749__INCLUDED_)
#define AFX_MAINFRM_H__B740C89B_5A25_4475_9F16_7CE57F641749__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CMainFrame : public CFrameWindowImpl<CMainFrame>,
                   public CUpdateUI<CMainFrame>,
                   public CMessageFilter,
                   public CIdleHandler
{
public:
    DECLARE_FRAME_WND_CLASS(NULL, IDR_MAINFRAME)

    BEGIN_UPDATE_UI_MAP(CMainFrame)
        UPDATE_ELEMENT(IDC_START, UPDUI_MENUPOPUP)
        UPDATE_ELEMENT(IDC_STOP, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_TOOLBAR_SHOW,UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_TOOLBAR_HIDE,UPDUI_MENUPOPUP)
    END_UPDATE_UI_MAP()

    BEGIN_MSG_MAP(CMainFrame)
        MESSAGE_HANDLER(WM_CREATE, OnCreate)
        COMMAND_ID_HANDLER(ID_APP_EXIT, OnFileExit)
        COMMAND_ID_HANDLER(ID_FILE_NEW, OnFileNew)
        COMMAND_ID_HANDLER(ID_APP_ABOUT, OnAppAbout)
        COMMAND_ID_HANDLER_EX(IDC_START, OnStart)
        COMMAND_ID_HANDLER_EX(IDC_STOP, OnStop)
		COMMAND_ID_HANDLER(ID_TOOLBAR_SHOW, OnToolbarShow)
		COMMAND_ID_HANDLER(ID_TOOLBAR_HIDE, OnToolbarHide)
        CHAIN_MSG_MAP(CUpdateUI<CMainFrame>)
        CHAIN_MSG_MAP(CFrameWindowImpl<CMainFrame>)
    END_MSG_MAP()

    BOOL PreTranslateMessage(MSG* pMsg);
    BOOL OnIdle();

    LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
    LRESULT OnFileExit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
    LRESULT OnFileNew(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
    LRESULT OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnToolbarShow(UINT ,int id,HWND hwnd,BOOL&);
	LRESULT OnToolbarHide(UINT, int id,HWND hwnd,BOOL&);
    void OnStart(UINT uCode, int nID, HWND hwndCtrl);
    void OnStop(UINT uCode, int nID, HWND hwndCtrl);

protected:
    CWTLClockView m_view;
	CMultiPaneStatusBarCtrl m_status;
private:

	// this workaround solves a bug in CMultiPaneStatusBarCtrl
	// (in SetPanes() method) that limits the size of all panes
	// after the default pane to a combined total of 100 pixels  
	void SetPaneWidths(int* arrWidths, int nPanes)
	{
		// find the size of the borders
		int arrBorders[3];
		m_status.GetBorders(arrBorders);

		// calculate right edge of default pane
		arrWidths[0] += arrBorders[2];
		for (int i = 1; i < nPanes; i++)
			arrWidths[0] += arrWidths[i];

		// calculate right edge of remaining panes
		for (int j = 1; j < nPanes; j++)
			arrWidths[j] += arrBorders[2] + arrWidths[j - 1];

		// set the pane widths
		m_status.SetParts(m_status.m_nPanes, arrWidths);
	}
	// m_hWndStatusBar
	// m_hWndToolBar
	// m_hWndClient
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__B740C89B_5A25_4475_9F16_7CE57F641749__INCLUDED_)
