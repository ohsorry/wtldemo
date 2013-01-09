#include "stdafx.h"
#include "resource.h"
#include "DlgAdvanced.h"

#define IDS_WEBADDR		_T("http://members.tripod.com/~SoftechSoftware/index.html")

CDlgAdvanced::CDlgAdvanced()
{
	m_byCounter = 0;
}

LRESULT CDlgAdvanced::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) 
{
	COLORREF	crBtnColor;

	m_btnAutoRepeat.SubclassWindow(GetDlgItem(IDC_BTNAUTOREPEAT));
	m_btnHyperLink.SubclassWindow(GetDlgItem(IDC_BTNHYPERLINK));
	m_btnCursor.SubclassWindow(GetDlgItem(IDC_BTNCURSOR));
	m_btnFocusRect.SubclassWindow(GetDlgItem(IDC_BTNFOCUSRECT));
	m_chkCheckbox.SubclassWindow(GetDlgItem(IDC_CHECK));
	m_btnBitmap.SubclassWindow(GetDlgItem(IDC_BTNBITMAP));
	
	// Calculate a color effect for hilighting the button
	crBtnColor = ::GetSysColor(COLOR_BTNFACE) + RGB(30, 30, 30);

	// Autorepeat button
	m_btnAutoRepeat.SetIcon(IDI_LAMP2);
	m_btnAutoRepeat.SetColor(CButtonST::BTNST_COLOR_BK_IN, crBtnColor);
	m_btnAutoRepeat.SetAutoRepeat(true, 500);
	// HyperLink button
	m_btnHyperLink.SetIcon(IDI_WEB2);
	m_btnHyperLink.SetColor(CButtonST::BTNST_COLOR_BK_IN, crBtnColor);
	m_btnHyperLink.SetURL(IDS_WEBADDR);
	m_btnHyperLink.SetTooltipText(IDS_WEBADDR);
	// Cursor button
	m_btnCursor.SetIcon(IDI_TOOLS4);
	m_btnCursor.SetColor(CButtonST::BTNST_COLOR_BK_IN, crBtnColor);
	m_btnCursor.SetBtnCursor(IDC_HAND);
	// Focus rectangle always present button
	m_btnFocusRect.SetIcon(IDI_RUN);
	m_btnFocusRect.SetColor(CButtonST::BTNST_COLOR_BK_IN, crBtnColor);
	m_btnFocusRect.SetFlatFocus(TRUE);
	// Checkbox
	m_chkCheckbox.SetIcon(IDI_LEDON, IDI_LEDOFF);
	m_chkCheckbox.SetColor(CButtonST::BTNST_COLOR_BK_IN, crBtnColor);
	// Bitmap button
	m_btnBitmap.SetBitmaps(IDB_PALETTE, RGB(255, 0, 255));
	m_btnBitmap.SetColor(CButtonST::BTNST_COLOR_BK_IN, crBtnColor);

	return TRUE;
}

LRESULT CDlgAdvanced::OnBtnAutorepeat(UINT , int , HWND , BOOL&) 
{
	// A CButtonST with the autorepeat feature enabled
	// will send a BN_CLICKED message every interval (in
	// milliseconds) specified until it is released

	switch (m_byCounter)
		{
		case 0:
			m_btnAutoRepeat.SetIcon(IDI_LAMP1);
			m_byCounter = 1;
			break;
		case 1:
			m_btnAutoRepeat.SetIcon(IDI_LAMP2);
			m_byCounter = 0;
			break;
		}
	return 0;
}
