// maindlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINDLG_H__ACC08E7B_D3AD_4403_A0EE_D8F158CF9440__INCLUDED_)
#define AFX_MAINDLG_H__ACC08E7B_D3AD_4403_A0EE_D8F158CF9440__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "CCtlColor.h"
#include "aboutdlg.h"
#include "REQUIRED.H"

class CMainDlg : public CDialogImpl<CMainDlg>
               , public CCtlColored<CMainDlg, ~0>  
{
public:
	enum { IDD = IDD_MAINDLG };


	BEGIN_MSG_MAP(CMainDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(ID_APP_ABOUT, OnAppAbout) 
		COMMAND_ID_HANDLER(ID_REQUIRED, OnRequired) 
		COMMAND_ID_HANDLER(IDOK, OnOK)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
		COMMAND_RANGE_HANDLER(IDC_CHECK1, IDC_CHECK4, OnCheck);
		CHAIN_MSG_MAP(CCtlColoredThis) 
	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)
	CHyperLink m_chl;
	CHyperLink m_chl2;
	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		// center the dialog on the screen
		CenterWindow();

		SetTextBackGround(0xFFf0d0);  // Lightish kind of blue
		SetTextColor(RGB(0Xa0, 0x10, 0x10)); // Kind of red

		m_chl.SubclassWindow(GetDlgItem(IDC_ARTICLE));
		m_chl.SetHyperLink("http://www.codeproject.com/wtl/ChangeColorWtlDlg.asp");

		m_chl2.SubclassWindow(GetDlgItem(IDC_AUTHOR));
		m_chl2.SetHyperLink("http://www.codeproject.com/script/Articles/list_articles.asp?userid=306133");

		CComboBox ccb = GetDlgItem(IDC_COMBO1);
		ccb.AddString("String 1");
		ccb.AddString("String 2");
		ccb.AddString("String 3");
		ccb.AddString("String 4");
		ccb.SetCurSel(2);
	
		CListBox clb = GetDlgItem(IDC_LIST1);
		clb.AddString("String  1");
		clb.AddString("String  2");
		clb.AddString("String  3");
		clb.AddString("String  4");
		clb.AddString("String  5");
		clb.AddString("String  6");
		clb.AddString("String  7");
		clb.AddString("String  8");
		clb.AddString("String  9");
		clb.AddString("String 10");
		clb.AddString("String 11");
		clb.AddString("String 12");
		clb.SetCurSel(1);  // InitializeFlatSB didn't help here.

		CEdit edt = GetDlgItem(IDC_EDIT1);
		edt.SetWindowText(
			"READ ONLY\r\n\r\n"
			"The WM_CTLCOLORLISTBOX message is sent to the parent window\r\n"
			"of a list box before the system draws the list box. \r\n"
			"By responding to this message, the parent window can set the\r\n"
			"text and background colors of the list box by using the specified\r\n"
			" display device context handle.\r\n"
			"A window receives this message through its WindowProc function." 
			);

		edt.Detach();
		edt.Attach(GetDlgItem(IDC_EDIT2));
		edt.SetWindowText(
			"By default, the DefWindowProc function selects the default \r\n"
			"system colors for the list box.\r\n"
			"The system does not automatically destroy the returned brush.\r\n"
			"It is the application's responsibility to destroy the brush\r\n"
			"when it is no longer needed.\r\n"
			"The WM_CTLCOLORLISTBOX message is never sent between threads.\r\n"
			"It is sent only within one thread.\r\n"
			"If a dialog box procedure handles this message, it should cast\r\n"
			"the desired return value to a BOOL and return the value directly.\r\n"
			"If the dialog box procedure returns FALSE, then default message\r\n"
			"handling is performed. The DWL_MSGRESULT value set\r\n"
			"by the SetWindowLong function is ignored."
	);

		CButton btn = GetDlgItem(IDC_CHECK1);
		btn.SetCheck(1);
		btn.Detach();

		btn = GetDlgItem(IDC_CHECK2);
		btn.SetCheck(1);
		btn.Detach();

		btn = GetDlgItem(IDC_CHECK3);
		btn.SetCheck(1);
		btn.Detach();

		btn = GetDlgItem(IDC_CHECK4);
		btn.SetCheck(1);
		btn.Detach();

		CTrackBarCtrl Slider = GetDlgItem(IDC_SLIDER1);
		Slider.SetRange(0, 8);

		// set icons
		HICON hIcon = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), 
			IMAGE_ICON, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON), LR_DEFAULTCOLOR);
		SetIcon(hIcon, TRUE);
		HICON hIconSmall = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), 
			IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
		SetIcon(hIconSmall, FALSE);

		return TRUE;
	}

	LRESULT OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		CAboutDlg dlg;
		dlg.DoModal();
		return 0;
	}
	LRESULT OnRequired(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		CRequiredDlg dlg;
		dlg.DoModal();
		return 0;
	}


	LRESULT OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		// TODO: Add validation code 
		EndDialog(wID);
		return 0;
	}

	LRESULT OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		EndDialog(wID);
		return 0;
	}
	
	LRESULT OnCheck(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& /*bHandled*/)
	{
		CButton btn = hWndCtl;
		int iCheck = btn.GetCheck();
		UCHAR wFlag = 0;
		switch (wID)
		{
			case IDC_CHECK1: 
				wFlag = FLG_HANDLE_DLG;
				break;
			case IDC_CHECK2: 
				wFlag = FLG_HANDLE_STATIC;
				break;
			case IDC_CHECK3: 
				wFlag = FLG_HANDLE_EDIT;
				break;
			case IDC_CHECK4: 
				wFlag = FLG_HANDLE_SCROLLBAR;
				break;
			default:
				return 0;
		}
		// ^= toggles bits from m_Flags which are on in wFlag.
		m_Flags ^= wFlag;
		// Repaint the dialog.
		Invalidate();
		// The track bar control does not repaint unless it's given the focus.
		CTrackBarCtrl tbc =  GetDlgItem(IDC_SLIDER1);
		tbc.SetFocus();
		// Return the focus to wherever the user clicked.
		CWindow wnd = hWndCtl;
		wnd.SetFocus();
		return 0;
	}
	// Overrides
	// This function overrides the default behavior of CCtlColoredThis::DoHandleCtlColor,
	// to mark read-only edit controls.
	LRESULT DoHandleCtlColor(UINT uMsg,// One of the WM_CTLCOLOR* messages. 
		                     HDC hdc,  // DC of the control which sent the message.
							 HWND hw)  // Which control sent the message.
	{
		bool bHandled = false;
		if (WM_CTLCOLORSTATIC == uMsg)
		{
			CEdit wnd = hw;
			DWORD dwStyle = wnd.GetStyle();
			if (ES_READONLY  & dwStyle)
			{
				// ::SetTextColor(hdc, ClrComplement(m_TextColor));   
				::SetTextColor(hdc, ClrRotateRight(m_TextColor));   
				// ::SetTextColor(hdc, ClrRotateLeft(m_TextColor));   
				if (CLR_INVALID != m_clrBackGround)
					::SetBkColor(hdc, m_clrBackGround);
				::SelectObject(hdc, m_brshBackGround);
				bHandled = TRUE;
			}
		}
		if (bHandled)
			return (LRESULT) m_brshBackGround;
		return CCtlColoredThis::DoHandleCtlColor(uMsg, hdc, hw);
	}

};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINDLG_H__ACC08E7B_D3AD_4403_A0EE_D8F158CF9440__INCLUDED_)
