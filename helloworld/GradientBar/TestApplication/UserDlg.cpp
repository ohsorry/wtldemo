// UserDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "UserDlg.h"
#include "PasswordStrength.h"

// CUserDlg dialog

LRESULT CUserDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	CenterWindow();
	DoDataExchange(false);
	return TRUE;
}

LRESULT CUserDlg::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	EndDialog(IDCANCEL);
	return TRUE;
}

LRESULT CUserDlg::OnBnClickedOk(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	EndDialog(IDOK);
	return 0;
}

LRESULT CUserDlg::OnBnClickedCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	EndDialog(IDCANCEL);
	return 0;
}

///////////////////////////////////////////////////////////////////////////////
// Password strength display related handlers

// update password strength control
LRESULT CUserDlg::OnEnChangePassword(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	CString password;
	m_ctlPassword.GetWindowText(password);
	CPasswordStrength ps;
	int score = ps.GetScore(password);
	m_ctlPasswordStrength.SetValue(score);

	UpdateControls();
	return 0;
}

// enable passwordstrength marquee when password edit gains focus
LRESULT CUserDlg::OnEnSetfocusPassword(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	m_ctlPasswordStrength.EnableMarquee(true);
	return 0;
}

// disable passwordstrength marquee when password edit looses focus
LRESULT CUserDlg::OnEnKillfocusPassword(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	m_ctlPasswordStrength.EnableMarquee(false);
	return 0;
}


///////////////////////////////////////////////////////////////////////////////
// Handlers and methods to enable/disable OK button

LRESULT CUserDlg::OnEnChangeUsername(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	UpdateControls();
	return 0;
}

LRESULT CUserDlg::OnEnChangePasswordRetype(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	UpdateControls();
	return 0;
}

void CUserDlg::UpdateControls()
{
	m_btnOk.EnableWindow(ShouldEnableOkButton());
}

bool CUserDlg::ShouldEnableOkButton()
{
	if (m_ctlUsername.GetWindowTextLength() == 0)
		return false;
	CString password;
	m_ctlPassword.GetWindowText(password);
	CString retypedPassword;
	m_ctlRetypePassword.GetWindowText(retypedPassword);
	if (password != retypedPassword)
		return false;
	return true;
}
