#ifndef _dlg_h
#define _dlg_h
#include "stdafx.h"
#include "resource.h"
#include "EditListBox.h"
class MainDlg : public CDialogImpl<MainDlg>
{
public:
	enum{IDD = IDD_DIALOG1};

	BEGIN_MSG_MAP(theClass)
		MESSAGE_HANDLER(WM_INITDIALOG,OnInitDialog)
		MESSAGE_HANDLER(WM_CLOSE,OnClose)
	END_MSG_MAP()

	LRESULT OnInitDialog(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL&bHandled)
	{
		CenterWindow();
		mEditLB.SubclassWindow(GetDlgItem(IDC_STATIC));
		return FALSE;
	}


	LRESULT OnClose(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL&bHandled)
	{
		EndDialog(0);
		return FALSE;
	}
private:
	CEditListBox mEditLB;
};

#endif