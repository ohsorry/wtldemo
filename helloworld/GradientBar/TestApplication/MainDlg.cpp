// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "MainDlg.h"
#include "UserDlg.h"

LRESULT CMainDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// center the dialog on the screen
	CenterWindow();

	// set icons
	HICON hIcon = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON));
	SetIcon(hIcon, TRUE);
	HICON hIconSmall = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON));
	SetIcon(hIconSmall, FALSE);

	DoDataExchange(false);

	m_ctlUpDown.SetRange(0, 100);

	m_ctlDisplayValue.SetCheck(BST_CHECKED);

	bool checked = m_ctlEnableMarquee.GetCheck() == BST_CHECKED;
	m_gradientBar.EnableMarquee(checked);
	return TRUE;
}

LRESULT CMainDlg::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	EndDialog(IDOK);
	return 0;
}

LRESULT CMainDlg::OnEnChangePercentage(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if (m_ctlEdit.IsWindow())
	{
		CString text;
		m_ctlEdit.GetWindowText(text);
		int val = _ttoi(text);
		m_gradientBar.SetValue(val);
	}
	return 0;
}

LRESULT CMainDlg::OnEnUpdatePercentage(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if (m_ctlEdit.IsWindow())
	{
		CString text;
		m_ctlEdit.GetWindowText(text);
		int val = _ttoi(text);
		if (val > 100)
			m_ctlEdit.SetWindowText(_T("100"));
		else if (val < 0)
			m_ctlEdit.SetWindowText(_T("0"));
	}
	return 0;
}

LRESULT CMainDlg::OnBnClickedDisplayValue(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	bool checked = m_ctlDisplayValue.GetCheck() == BST_CHECKED;
	m_gradientBar.DisplayValue(checked);
	return 0;
}

LRESULT CMainDlg::OnBnClickedEnableMarquee(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	bool checked = m_ctlEnableMarquee.GetCheck() == BST_CHECKED;
	m_gradientBar.EnableMarquee(checked);
	return 0;
}

LRESULT CMainDlg::OnBnClickedButtonRealworld(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	CUserDlg dlg;
	dlg.DoModal();
	return 0;
}

LRESULT CMainDlg::OnBnClickedButtonExit(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	EndDialog(IDOK);
	return 0;
}
