// MainFrm.h : interface of the CMainFrame class
//
// Copyright 2002 by SoftGee.  This is free software.  You may use it however
// you want.  THIS SOFTWARE IS OFFERED AS-IS WITH NO WARRANTIES OF ANY KIND.
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__BBA5DFCA_6C1A_11D6_B657_0048548B09C5__INCLUDED_)
#define AFX_MAINFRM_H__BBA5DFCA_6C1A_11D6_B657_0048548B09C5__INCLUDED_

#pragma once

class CMainFrame : public CFrameWindowImpl<CMainFrame>
{
public:
	DECLARE_FRAME_WND_CLASS(NULL, IDR_MAINFRAME)

	CSplitterWindow m_vSplit;
	CHorSplitterWindow m_hzSplit;

	CPaneContainer m_lPane;
	CPaneContainer m_tPane;
	CPaneContainer m_bPane;

	CEdit m_edit;

	BEGIN_MSG_MAP(CMainFrame)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		COMMAND_ID_HANDLER(ID_PANE_CLOSE, OnPaneClose)
		COMMAND_ID_HANDLER(ID_APP_EXIT, OnFileExit)
		COMMAND_ID_HANDLER(ID_APP_ABOUT, OnAppAbout)
		CHAIN_MSG_MAP(CFrameWindowImpl<CMainFrame>)
	END_MSG_MAP()

	LRESULT OnCreate(UINT, WPARAM, LPARAM, BOOL&)
	{
		CreateSimpleToolBar();

		// set flat toolbar style
		CToolBarCtrl tool = m_hWndToolBar;
		tool.SetStyle(tool.GetStyle() | TBSTYLE_FLAT);

		CreateSimpleStatusBar();

		m_hWndClient = CreateClient();

		return 0;
	}

	HWND CreateClient()
	{
// vertical splitter setup
		// client rect for vertical splitter
		CRect rcVert;
		GetClientRect(&rcVert);

		// create the vertical splitter
		m_vSplit.Create(m_hWnd, rcVert, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);

		// set the vertical splitter parameters
		m_vSplit.m_cxyMin = 35; // minimum size
		m_vSplit.SetSplitterPos(85); // from left
		m_vSplit.m_bFullDrag = false; // ghost bar enabled

// horizontal splitter setup
		// client rect for horizontal splitter
		CRect rcHorz;
		GetClientRect(&rcHorz);

		// create the horizontal splitter. Note that vSplit is parent of hzSplit
		m_hzSplit.Create(m_vSplit.m_hWnd, rcHorz, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);

		// set the horizontal splitter parameters
		m_hzSplit.m_cxyMin = 35; // minimum size
		m_hzSplit.SetSplitterPos(100); // from top
		m_hzSplit.m_bFullDrag = false; // ghost bar enabled

		// add the horizontal splitter to right pane of vertical splitter
		m_vSplit.SetSplitterPane(1, m_hzSplit);

// left pane container setup
		// create the left container
		m_lPane.Create(m_vSplit.m_hWnd);

		// add container to left pane (0) of vertical splitter
		m_vSplit.SetSplitterPane(0, m_lPane);

		// set the left pane title
		m_lPane.SetTitle("Left Pane");

// top pane container setup
		// create the top container.  Note use of hzSplit as parent
		m_tPane.Create(m_hzSplit.m_hWnd);

		// add container to top pane (0) of horizontal splitter
		m_hzSplit.SetSplitterPane(0, m_tPane);

		// set the top pane title
		m_tPane.SetTitle("Top Pane -- no Close button");

		// remove the close button from the top container
		m_tPane.SetPaneContainerExtendedStyle(PANECNT_NOCLOSEBUTTON);

// bottom pane container setup
		// create the bottom container.  Note hzSplit is parent
		m_bPane.Create(m_hzSplit.m_hWnd);

		// add container to bottom pane (1) of horizontal splitter
		m_hzSplit.SetSplitterPane(1, m_bPane);

		// set the bottom container's header style to vertical
		m_bPane.SetPaneContainerExtendedStyle(PANECNT_VERTICAL);

		// create an edit. Note that m_bPane is the parent. Set
		// the edit's font and place text in it
		m_edit.Create(m_bPane.m_hWnd, rcDefault, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, WS_EX_CLIENTEDGE);
		m_edit.SetFont((HFONT)GetStockObject(DEFAULT_GUI_FONT), TRUE);
		m_edit.SetWindowText(" Bottom Pane -- with vertical header and edit as child");

		// assign the edit to the bottom container
		m_bPane.SetClient(m_edit.m_hWnd);

		return m_vSplit.m_hWnd;
	}

	LRESULT OnPaneClose(WORD, WORD, HWND hWndCtl, BOOL&)
	{
		// hide the container whose Close button was clicked. Use 
		// DestroyWindow(hWndCtl) instead if you want to totally 
		// remove the container instead of just hiding it
		::ShowWindow(hWndCtl, SW_HIDE);

		// find the container's parent splitter
		HWND hWnd = ::GetParent(hWndCtl);
		CSplitterWindow* pWnd;
		pWnd = (CSplitterWindow*)::GetWindowLong(hWnd, GWL_ID);

		// take the container that was Closed out of the splitter.
		// Use SetSplitterPane(nPane, NULL) if you want to stay in
		// multipane mode instead of changing to single pane mode
		int nCount = pWnd->m_nPanesCount;
		for(int nPane = 0; nPane < nCount; nPane++)
		{
			if (hWndCtl == pWnd->m_hWndPane[nPane])
			{
				pWnd->SetSinglePaneMode(nCount - nPane - 1);
				break;
			}
		}

		return 0;
	}

	LRESULT OnFileExit(WORD, WORD, HWND, BOOL&)
	{
		PostMessage(WM_CLOSE);
		return 0;
	}

	LRESULT OnAppAbout(WORD, WORD, HWND, BOOL&)
	{
		CAboutDlg dlg;
		dlg.DoModal();
		return 0;
	}
};

#endif // !defined(AFX_MAINFRM_H__BBA5DFCA_6C1A_11D6_B657_0048548B09C5__INCLUDED_)
