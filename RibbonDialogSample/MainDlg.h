// MainDlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

// +++ STEP 1 +++
#include "RibbonTabDialog.h"

// +++ STEP 2 +++
class CMainDlg :	public CRibbonTabDialogImpl<CMainDlg>,
					public CDialogResize<CMainDlg>,
					public CUpdateUI<CMainDlg>,
					public CIdleHandler
{
private:
	CStatic	m_static;
	CFont	m_font;

public:
	enum { IDD = IDD_MAINDLG };

	virtual BOOL OnIdle() {	return FALSE; }

    BEGIN_DLGRESIZE_MAP(CReminderListDlg)
		DLGRESIZE_CONTROL(IDOK, DLSZ_MOVE_X | DLSZ_MOVE_Y)
		DLGRESIZE_CONTROL(IDCANCEL, DLSZ_MOVE_X | DLSZ_MOVE_Y)
		DLGRESIZE_CONTROL(ID_APP_ABOUT, DLSZ_MOVE_X | DLSZ_MOVE_Y)
		DLGRESIZE_CONTROL(IDC_EDIT1, DLSZ_SIZE_X)
		DLGRESIZE_CONTROL(IDC_LIST1, DLSZ_SIZE_X | DLSZ_SIZE_Y)
    END_DLGRESIZE_MAP()

	BEGIN_UPDATE_UI_MAP(CMainDlg)
	END_UPDATE_UI_MAP()

	BEGIN_MSG_MAP_EX(CMainDlg)
// +++ STEP 3 +++
		CHAIN_MSG_MAP(CRibbonTabDialogImpl<CMainDlg>)

		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		COMMAND_ID_HANDLER(ID_APP_ABOUT, OnAppAbout)
		COMMAND_ID_HANDLER(IDOK, OnOK)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)

// +++ STEP 5 +++
		COMMAND_ID_HANDLER(IDS_TAB1SUBITEM1, OnTab1Subitem1Selected)
		MSG_WM_COMMAND(OnCommand)

		CHAIN_MSG_MAP(CDialogResize<CMainDlg>)
	END_MSG_MAP()

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		// center the dialog on the screen
		CenterWindow();

		// set icons
		HICON hIcon = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME),  IMAGE_ICON, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON), LR_DEFAULTCOLOR);
		SetIcon(hIcon, TRUE);
		HICON hIconSmall = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
		SetIcon(hIconSmall, FALSE);

		// register object for idle updates
		CMessageLoop* pLoop = _Module.GetMessageLoop();
		ATLASSERT(pLoop != NULL);
		pLoop->AddIdleHandler(this);

		UIAddChildWindowContainer(m_hWnd);

		DlgResize_Init();

		// Create a big, bold font...
		CLogFont lf;
		lf.SetMessageBoxFont();
		lf.lfWeight = FW_BOLD;
		lf.MakeLarger(4);
		m_font.CreateFontIndirect(&lf);

		// ...and attach it to the static control
		m_static.Attach(GetDlgItem(IDC_STATIC1));
		m_static.SendMessage(WM_SETFONT, (WPARAM) m_font.m_hFont);

// +++ STEP 4 +++
		// ---------------------------------------------------------------------------
		// Look how easy! :D
		// ---------------------------------------------------------------------------
		// Add tabs
		AddTab(IDS_TAB1);
		AddSubTab(IDS_TAB1SUBITEM1, IDI_ICON2);
		AddSubTab(IDS_TAB1SUBITEM2, IDI_ICON1);

		AddTab(IDS_TAB2);
		AddSubTab(IDS_TAB2SUBITEM1, IDI_ICON4);
		AddSubTab(IDS_TAB2SUBITEM2, IDI_ICON6);
		AddSubTab(IDS_TAB2SUBITEM3, IDI_ICON7);
		// ---------------------------------------------------------------------------

		return TRUE;
	}

	void OnCommand(UINT uNotifyCode, int nID, CWindow wndCtl)
	{
		SetMsgHandled(FALSE);
		
		if ( ((uNotifyCode == 0) || (uNotifyCode == 1)) && ((nID >= IDS_TAB1) && (nID <= IDS_TAB2SUBITEM3)) )
		{
			CString str;
			str.LoadString(nID);

			// Strip out the ampersands
			str.Replace(_T("&"), _T(""));
			
			m_static.SetWindowText(str);
		}
	}

	LRESULT OnTab1Subitem1Selected(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		CString str(_T("Hello there!"));
		m_static.SetWindowText(str);
		return 0;
	}

	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		// unregister idle updates
		CMessageLoop* pLoop = _Module.GetMessageLoop();
		ATLASSERT(pLoop != NULL);
		pLoop->RemoveIdleHandler(this);

		return 0;
	}

	LRESULT OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		CAboutDlg dlg;
		dlg.DoModal();
		return 0;
	}

	LRESULT OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		// TODO: Add validation code 
		CloseDialog(wID);
		return 0;
	}

	LRESULT OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		CloseDialog(wID);
		return 0;
	}

	void CloseDialog(int nVal)
	{
		DestroyWindow();
		::PostQuitMessage(nVal);
	}
};
