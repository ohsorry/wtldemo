// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__C996E99B_CBF8_11D6_B6AE_0048548B09C5__INCLUDED_)
#define AFX_MAINFRM_H__C996E99B_CBF8_11D6_B6AE_0048548B09C5__INCLUDED_

#pragma once

class CMainFrame : public CFrameWindowImpl<CMainFrame>
{
public:
	DECLARE_FRAME_WND_CLASS(NULL, IDR_MAINFRAME)

	CMultiPaneStatusBarCtrl m_status;

	BEGIN_MSG_MAP(CMainFrame)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_TIMER, OnTimer)
		COMMAND_ID_HANDLER(ID_APP_EXIT, OnFileExit)
		COMMAND_ID_HANDLER(ID_APP_ABOUT, OnAppAbout)
		CHAIN_MSG_MAP(CFrameWindowImpl<CMainFrame>)
	END_MSG_MAP()

	LRESULT OnCreate(UINT, WPARAM, LPARAM, BOOL& bHandled)
	{
		CreateSimpleStatusBar();

		// subclass the status bar as multipane
		m_status.SubclassWindow(m_hWndStatusBar);

		// set status bar panes
		int arrPanes[] = { ID_DEFAULT_PANE, IDR_DATE, IDR_TIME };
		m_status.SetPanes(arrPanes, sizeof(arrPanes) / sizeof(int), false);

		// set status bar pane widths using local workaround
		int arrWidths[] = { 0, 90, 60 };
		SetPaneWidths(arrWidths, sizeof(arrWidths) / sizeof(int));

		// set the status bar pane icons
		m_status.SetPaneIcon(ID_DEFAULT_PANE, AtlLoadIconImage(IDR_DEFAULT, LR_DEFAULTCOLOR));
		m_status.SetPaneIcon(IDR_DATE, AtlLoadIconImage(IDR_DATE, LR_DEFAULTCOLOR));
		m_status.SetPaneIcon(IDR_TIME, AtlLoadIconImage(IDR_TIME, LR_DEFAULTCOLOR));

		// initialize date/time and start a 1 second timer
		OnTimer(0, 0, 0, bHandled);
		SetTimer(1, 1000);

		return 0;
	}

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

	LRESULT OnTimer(UINT, WPARAM, LPARAM, BOOL&)
	{
		CString str;

		// get the current date and time
		SYSTEMTIME st;
		::GetLocalTime(&st);

		// Pane 1: Display the date
		str.Format("%i/%i/%i", st.wMonth, st.wDay, st.wYear);
		m_status.SetPaneText(IDR_DATE, str);

		// Pane 2: Display the time
		str.Format("%i:%02i", st.wHour, st.wMinute);
		m_status.SetPaneText(IDR_TIME, str);

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


#endif // !defined(AFX_MAINFRM_H__C996E99B_CBF8_11D6_B6AE_0048548B09C5__INCLUDED_)
