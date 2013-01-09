//-----------------------------------------------------------------------------
// Project:	FlxWorkshopTk
//-----------------------------------------------------------------------------
// $Author: $
// $Modtime: $
// $Revision: $
// $Workfile: $
//-----------------------------------------------------------------------------
// Copyright © 2001 Dell
//-----------------------------------------------------------------------------
#if !defined(__dlgabout_h__)
#define __dlgabout_h__

#include "src/ButtonST.h"

class CDlgAbout : public CAxDialogImpl<CDlgAbout>
{
public:
	CDlgAbout();

	enum { IDD = IDD_ABOUT };

	BEGIN_MSG_MAP(CMainDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		REFLECT_NOTIFICATIONS()
	END_MSG_MAP()

protected:

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

	CButtonST	m_btnSTLogo;
	CButtonST	m_btnFace;
};

#endif // !defined(__dlgabout_h__)
