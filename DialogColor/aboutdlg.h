#ifndef __ABOUTDLG_H__
#define __ABOUTDLG_H__


#include "CCtlColor.h"

class CAboutDlg : public CDialogImpl<CAboutDlg>
                , public CCtlColored<CAboutDlg>  // Add this line
{
public:
	enum { IDD = IDD_ABOUTBOX };

	BEGIN_MSG_MAP(CAboutDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDOK, OnCloseCmd)
		COMMAND_ID_HANDLER(IDCANCEL, OnCloseCmd)
		CHAIN_MSG_MAP(CCtlColored<CAboutDlg>) // Add this line
	END_MSG_MAP()

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		static int iColorSet = 0;
		switch (iColorSet)
		{
			case 0:
       			// Tooltip colors
				SetTextColor(::GetSysColor(COLOR_INFOTEXT)); 
				SetBkBrush(COLOR_INFOBK);
				break;
			case 1:
				// Light green on dark green, like an old monochrome machine (XT).
				SetTextColor(0x70ff70); 
				SetTextBackGround(0x007000); 
				break;
			case 2:
       			// Default dialog colors, with no theme bug.
				SetTextColor(::GetSysColor(COLOR_BTNTEXT)); 
				SetBkBrush(COLOR_BTNFACE);
				break;
			default:
				// 'Regular window' (SDI, MDI) colors.
				break;
		}
		++iColorSet %= 4; // Different color next time
	    CenterWindow(GetParent());
		return TRUE;
    }

	LRESULT OnCloseCmd(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		EndDialog(wID);   
		return 0;
	}
	
};     

#endif //__ABOUTDLG_H__