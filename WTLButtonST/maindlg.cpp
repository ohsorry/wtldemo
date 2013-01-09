#include "stdafx.h"
#include "resource.h"
#include "maindlg.h"

#define IDS_MAILADDR	_T("mailto:davide_calabro@yahoo.com")
#define IDS_WEBADDR		_T("http://members.tripod.com/~SoftechSoftware/index.html")

LRESULT CMainDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// center the dialog on the screen
	CenterWindow();

	// set icons
	HICON hIcon = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), 
		IMAGE_ICON, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON), LR_DEFAULTCOLOR);
	SetIcon(hIcon, TRUE);
	HICON hIconSmall = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), 
		IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
	SetIcon(hIconSmall, FALSE);

	COLORREF	crBtnColor;

	// Calculate a color effect for hilighting the button
	crBtnColor = ::GetSysColor(COLOR_BTNFACE) + RGB(30, 30, 30);

	// Button close
	m_btnClose.SubclassWindow(GetDlgItem(IDCANCEL));
	m_btnClose.SetIcon(IDI_CANCEL1, IDI_CANCEL4);
	m_btnClose.SetColor(CButtonST::BTNST_COLOR_BK_IN, crBtnColor);
	m_btnClose.SetTooltipText(_T("Close the application"));

	TCITEM tci;
	tci.mask = TCIF_TEXT;

	m_tabMyTabCtrl.SubclassWindow(GetDlgItem(IDC_TAB));
	tci.pszText = _T("Basic features");
	m_tabMyTabCtrl.InsertItem(0, &tci);
	tci.pszText = _T("Advanced features");
	m_tabMyTabCtrl.InsertItem(1, &tci);
	tci.pszText = _T("Transparent effect");
	m_tabMyTabCtrl.InsertItem(2, &tci);
	tci.pszText = _T("CShadeButtonST");
	m_tabMyTabCtrl.InsertItem(3, &tci);
	tci.pszText = _T("About");
	m_tabMyTabCtrl.InsertItem(4, &tci);
	m_tabMyTabCtrl.Init();

	m_hyperlink1.SubclassWindow(GetDlgItem(IDC_HOMEPAGELINK));
	m_hyperlink1.SetHyperLink(IDS_WEBADDR);
	m_hyperlink2.SubclassWindow(GetDlgItem(IDC_EMAILLINK));
	m_hyperlink2.SetHyperLink(IDS_MAILADDR);

	return TRUE;
}

