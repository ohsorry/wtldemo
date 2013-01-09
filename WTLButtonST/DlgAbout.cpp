#include "stdafx.h"
#include "resource.h"
#include "DlgAbout.h"

CDlgAbout::CDlgAbout()
{
}

LRESULT CDlgAbout::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) 
{
	m_btnSTLogo.SubclassWindow(GetDlgItem(IDC_BTNSTLOGO));
	m_btnFace.SubclassWindow(GetDlgItem(IDC_BTNFACE));

	// SoftechSoftware logo button
	m_btnSTLogo.SetBitmaps(IDB_STLOGO, RGB(255, 255, 255));
	// Face button
	m_btnFace.SetBitmaps(IDB_FACE, RGB(0, 255, 0));
	
	// Write class version
	CString sVersion;
	sVersion.Format(_T("CButtonST v%s"), CButtonST::GetVersionC());
	::SetWindowText(GetDlgItem(IDC_VERSION), sVersion);

	return TRUE;
}

