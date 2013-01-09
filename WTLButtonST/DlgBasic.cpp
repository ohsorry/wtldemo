#include "stdafx.h"
#include "resource.h"
#include "DlgBasic.h"

CDlgBasic::CDlgBasic()
{
}

LRESULT CDlgBasic::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) 
{
	COLORREF	crBtnColor;

	m_btnStandard.SubclassWindow(GetDlgItem(IDC_BTNSTANDARD));
	m_btnHalloween.SubclassWindow(GetDlgItem(IDC_BTNHALLOWEEN));
	m_btnKeyManager.SubclassWindow(GetDlgItem(IDC_BTNKEYMANAGER));
	m_btnDisabled.SubclassWindow(GetDlgItem(IDC_BTNDISABLED));
	m_btnLamp.SubclassWindow(GetDlgItem(IDC_BTNLAMP));
	m_btnZip.SubclassWindow(GetDlgItem(IDC_BTNZIP));
	m_btnJpeg.SubclassWindow(GetDlgItem(IDC_BTNJPEG));
	m_btnCDRom.SubclassWindow(GetDlgItem(IDC_BTNCDROM));
	m_btnTooltip.SubclassWindow(GetDlgItem(IDC_BTNTOOLTIP));
	m_btnSearch.SubclassWindow(GetDlgItem(IDC_BTNSEARCH));
	m_btnBack.SubclassWindow(GetDlgItem(IDC_BTNBACK));
	m_btnNext.SubclassWindow(GetDlgItem(IDC_BTNNEXT));

	// Calculate a color effect for hilighting the button
	crBtnColor = ::GetSysColor(COLOR_BTNFACE) + RGB(30, 30, 30);

	// Standard button
	m_btnStandard.SetIcon(IDI_EOAPP);
	m_btnStandard.SetFlat(FALSE);
	// Halloween button
	m_btnHalloween.SetIcon(IDI_HALLOWEEN2, IDI_HALLOWEEN1);
	// 48x48 icon button
	m_btnKeyManager.SetIcon(IDI_KEYMANAGER);
	m_btnKeyManager.SetColor(CButtonST::BTNST_COLOR_BK_IN, crBtnColor);
	// Disabled button
	m_btnDisabled.SetIcon(IDI_SOUND);
	m_btnDisabled.SetColor(CButtonST::BTNST_COLOR_BK_IN, crBtnColor);
	// Lamp button
	m_btnLamp.SetIcon(IDI_LAMP1, IDI_LAMP2);
	m_btnLamp.DrawBorder(FALSE);
	// Zip button
	m_btnZip.SetIcon(IDI_ZIP2, IDI_ZIP1);
	m_btnZip.SetColor(CButtonST::BTNST_COLOR_BK_IN, crBtnColor);
	// Jpeg button
	m_btnJpeg.SetIcon(IDI_JPEG);
	m_btnJpeg.SetColor(CButtonST::BTNST_COLOR_BK_IN, crBtnColor);
	// CDRom button
	m_btnCDRom.SetIcon(IDI_CDROM);
	m_btnCDRom.DrawBorder(FALSE);
	// Tooltip button
	m_btnTooltip.SetIcon(IDI_BALOON);
	m_btnTooltip.SetColor(CButtonST::BTNST_COLOR_BK_IN, crBtnColor);
	m_btnTooltip.SetTooltipText(_T("This is a tooltip.\r\nMultiline!"));
	// Search button
	m_btnSearch.SetIcon(IDI_SEARCH1, IDI_SEARCH2);
	m_btnSearch.SetColor(CButtonST::BTNST_COLOR_BK_IN, crBtnColor);
	m_btnSearch.SetAlign(CButtonST::ST_ALIGN_VERT);
	// Back button
	m_btnBack.SetIcon(IDI_LEFT2);
	m_btnBack.SetColor(CButtonST::BTNST_COLOR_BK_IN, crBtnColor);
	m_btnBack.SetColor(CButtonST::BTNST_COLOR_FG_IN, RGB(0, 128, 0));
	// Next button
	m_btnNext.SetIcon(IDI_RIGHT2);
	m_btnNext.SetColor(CButtonST::BTNST_COLOR_BK_IN, crBtnColor);
	m_btnNext.SetColor(CButtonST::BTNST_COLOR_FG_IN, RGB(0, 128, 0));
	m_btnNext.SetAlign(CButtonST::ST_ALIGN_HORIZ_RIGHT);

	return TRUE;
} // End of OnInitDialog

LRESULT CDlgBasic::OnBtnDisabled(UINT , int , HWND , BOOL&) 
{
	// Change button text
	m_btnDisabled.SetWindowText(_T("Disabled"));
	// Disable the button
	m_btnDisabled.EnableWindow(false);
	return 0;
}
