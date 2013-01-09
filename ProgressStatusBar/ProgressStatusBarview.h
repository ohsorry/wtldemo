// ProgressStatusBarView.h : interface of the CProgressStatusBarView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROGRESSSTATUSBARVIEW_H__3703AF7F_234E_46E2_88AE_5C5D57D54A88__INCLUDED_)
#define AFX_PROGRESSSTATUSBARVIEW_H__3703AF7F_234E_46E2_88AE_5C5D57D54A88__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "MultiPaneStatusBarEx.h"

LPCSTR DEFAULT_INSTRUCTIONS = "Instructions:\r\n\r\n"
						      "    Type the progress range (from < to),\r\n"
						      "    and the pane in which you want to\r\n"
						      "    see the progress bar (0 to 4), then\r\n"
						      "    click [ Run ] to see the progress bar\r\n"
						      "    running, and an AVI animation next to it.";

class CProgressStatusBarView : public CDialogImpl<CProgressStatusBarView>
{
public:
	CMPSBarWithAll *pParentStatus; 
	enum { IDD = IDD_PROGRESSSTATUSBAR_FORM };

	BOOL PreTranslateMessage(MSG* pMsg)
	{
		return CWindow::IsDialogMessage(pMsg);
	}

	BEGIN_MSG_MAP(CProgressStatusBarView)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_TIMER, OnTimer)
		COMMAND_ID_HANDLER(IDC_RUN, OnRun)		
		COMMAND_ID_HANDLER(IDC_STOP, OnStop)		
	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	RECT m_rctMin;
	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		ATLTRACE("OnInitDialog\n");	
		// TODO: Limit edit boxes to a reasonable length.
		//       Put reasonable defaults in edit boxes.
		CEdit edt;
		edt.Attach(GetDlgItem(IDC_FROM));
		edt.SetLimitText(5);
		edt.SetWindowText("1");
		edt.Detach();

		edt.Attach(GetDlgItem(IDC_TO));
		edt.SetLimitText(7);
		edt.SetWindowText("50");
		edt.Detach();

		edt.Attach(GetDlgItem(IDC_PANE));
		edt.SetLimitText(1);
		edt.SetWindowText("2");
		edt.Detach();

		edt.Attach(GetDlgItem(IDC_INSTRUCTIONS));
		edt.SetWindowText(DEFAULT_INSTRUCTIONS);

		GetWindowRect(&m_rctMin);
		::OffsetRect(&m_rctMin, -m_rctMin.left, -m_rctMin.top);
		SetTimer(1, 80);
		bProgress = false;

		return TRUE;
	}

	LRESULT OnRun(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		ATLTRACE("OnRun\n");	
		// Get values from edit boxes.

		int iFrom = GetDlgItemInt(IDC_FROM);
		m_iTo = GetDlgItemInt(IDC_TO);
		int iPane = GetDlgItemInt(IDC_PANE);

		// Run a few tests.
		CWindow wnd;
		wnd.Attach(GetDlgItem(IDC_INSTRUCTIONS));
		//     bgn < end
		if (iFrom >= m_iTo)
		{
			wnd.SetWindowText("Please note:\r\n\r\n"
						  "    The \"from\" value in the progress\r\n"
						  "    range should be smaller than\r\n"
						  "    the \"to\" value.\r\n\r\n"
						  "    Please try again."
			);
			return 0;
		}
		//     pane in 0,4
		if (iPane > 4 || iPane < 0)
		{
			wnd.SetWindowText("Please note:\r\n\r\n"
						  "    The pane number is the zero-based\r\n"
						  "    index of the status bar pane where\r\n"
						  "    you'd like to see the progress bar.\r\n\r\n"
						  "    Please try a number between 0 and 4.");
			return 0;
		}
		//     progress bar not in use
		// If all is OK, start a status bar.
		if (!pParentStatus->ProgCreate(iPane, iFrom, m_iTo))
		{
			wnd.SetWindowText("Please note:\r\n\r\n"
						  "    The can only be ONE progress bar\r\n"
						  "    running at a certain time.\r\n\r\n"
						  "    Please wait for the end of the current\r\n"
						  "    run before starting another.");
			return 0;
		}
		pParentStatus->ProgSetStep ( 1 );
		bProgress = true;
		wnd.SetWindowText(DEFAULT_INSTRUCTIONS);
		char szAnimFileName[MAX_PATH] = "\0";
        ::GetModuleFileName(_Module.m_hInst, szAnimFileName, MAX_PATH);
        strcpy(::PathFindFileName(szAnimFileName), "THIS.avi");
		pParentStatus->AnimCreate((iPane + 4) % 5, szAnimFileName);
		return 0;
	}
	LRESULT OnStop(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		ATLTRACE("OnStop\n");	
		if (bProgress)
		{
			// Close the progress bar, and mark it as not running.
			pParentStatus->ProgDestroyWindow();
			pParentStatus->AnimDestroyWindow();
			bProgress = false;
		}
		else
		{
			CWindow wnd;
			wnd.Attach(GetDlgItem(IDC_INSTRUCTIONS));
			wnd.SetWindowText("Please note:\r\n\r\n"
						  "    The STOP button is only relevant\r\n"
						  "    when a progress bar is running.\r\n\r\n"
						  "    Please try again while\r\n"
						  "    a progress bar is displayed.");
		}
		return 0;
	}
	LRESULT OnTimer(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		// Check if we have a progress bar.
		if (!bProgress)
			return 0;
		// If so, forward it.
		pParentStatus->ProgStepIt();
		
		// Check if we have arrived to the upper bound.
		int iPosition = pParentStatus->ProgGetPos(); 
		if (iPosition < m_iTo)
			return 0;

		// If so, close the progress bar, and mark it as not running.
		pParentStatus->ProgDestroyWindow();
		pParentStatus->AnimDestroyWindow();
		bProgress = false;
		return 0;
	}
	bool bProgress; 
	int m_iTo;
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROGRESSSTATUSBARVIEW_H__3703AF7F_234E_46E2_88AE_5C5D57D54A88__INCLUDED_)
