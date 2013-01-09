#ifndef __REQUIREDDLG_H__
#define __REQUIREDDLG_H__

#include "CCtlColor.h"

// #include "ColorFileDialog.h"
                     

class CRequiredDlg : public CDialogImpl<CRequiredDlg>
                // We're VERY selective about what we'll repaint here (writable edit boxes only).
                , public CCtlColored<CRequiredDlg, FLG_HANDLE_EDIT>  
{
public:
	enum { IDD = IDD_REQUIRED };

	BEGIN_MSG_MAP(CRequiredDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDOK, OnCloseCmd)
		COMMAND_ID_HANDLER(IDCANCEL, OnCloseCmd)
//		COMMAND_ID_HANDLER(ID_FILEOPEN, OnFileOpen);
		CHAIN_MSG_MAP(CCtlColoredThis) 
	END_MSG_MAP()

	// This enum and handle is meant to enable overriding some behavior.
	enum { LightGreen = 0xd0ffd0 };
	CBrush m_LightGreenBrush;	
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
     // Default colors are good enough.
	 //	SetTextColor(::GetSysColor(COLOR_WINDOWTEXT)); 
	 //	SetBkBrush(COLOR_WINDOW);
		CComboBox cb = GetDlgItem(IDC_COUNTRIES);
		cb.AddString("Australia");
		cb.AddString("Britain");
		cb.AddString("Canada");
		cb.AddString("Denmark");
		// This will be used for 'required' fields.
		m_LightGreenBrush.CreateSolidBrush(LightGreen);

	    CenterWindow(GetParent());
		return TRUE;
    }
	// Helper function, to seek empty required fields
	inline bool IsEmptyField(int nID)
	{
		CWindow wnd = GetDlgItem(nID);
		return (0 == wnd.GetWindowTextLength());
	}
	LRESULT OnCloseCmd(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		bool bOk = true;
		if (IDOK == wID)
		{
			// Check that all required fields have been filled.
			if (IsEmptyField(IDC_NAME))
				bOk = false;
			if (bOk && IsEmptyField(IDC_PASSWORD))
				bOk = false;
			if (bOk && IsEmptyField(IDC_COUNTRIES))
				bOk = false;
			if (!bOk)
				MessageBox("Not all required (green background)\nfields have been filled.", 
					        "Required", MB_OK);
		}
		if (bOk) // No required empty fields, and 
		{
			// Don't leak...
			// DeleteObject(m_LightGreenBrush); // done by CBrush destructor.
			EndDialog(wID);   
		}
		return 0;
	}

	// This function puts a green background on any of the relevant controls.
	LRESULT DoHandleCtlColor(UINT uMsg,// One of the WM_CTLCOLOR* messages. 
		                     HDC hdc,  // DC of the control which sent the message.
							 HWND hw)  // Which control sent the message.
	{
		// Get the HWND of the required combo-box and its companion edit box.
		// We'll paint them BOTH.
		CComboBox cb = GetDlgItem(IDC_COUNTRIES);
		COMBOBOXINFO cbi;
		cb.GetComboBoxInfo(&cbi);
		// If the window which got this message is one of the 'required' edit
		// fields, paint it light green
		if (hw == GetDlgItem(IDC_NAME)
		|| 	hw == GetDlgItem(IDC_PASSWORD)
		||  hw == cb
		||  hw == cbi.hwndItem 
		)
		{
			::SetTextColor(hdc, m_TextColor);
			::SetBkColor(hdc, LightGreen);
			return (LRESULT)(HBRUSH) m_LightGreenBrush;
		}
		// Otherwise, do the 'default' thing.
		return (LRESULT) CCtlColoredThis::DoHandleCtlColor(uMsg, hdc, hw);
	}
/*
	LRESULT OnFileOpen(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		CColorFileDialog dlg;
		dlg.SetTextColor(RGB(0XFF,0X80,0X80));
		dlg.SetTextBackGround(RGB(0X00,0XA0,0X00));
		dlg.DoModal();
		return 0;
	}
*/
};     

#endif //__REQUIREDDLG_H__