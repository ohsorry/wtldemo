#pragma once

#include "GradientBarCtrl.h"

// CUserDlg dialog

class CUserDlg
	: public CDialogImpl<CUserDlg>
	, public CWinDataExchange<CUserDlg>
{
public:
	enum { IDD = IDD_USERNAME_PASSWORD };

	BEGIN_MSG_MAP(CUserDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
		COMMAND_HANDLER(IDOK, BN_CLICKED, OnBnClickedOk)
		COMMAND_HANDLER(IDCANCEL, BN_CLICKED, OnBnClickedCancel)
		COMMAND_HANDLER(IDC_PASSWORD, EN_CHANGE, OnEnChangePassword)
		COMMAND_HANDLER(IDC_PASSWORD, EN_SETFOCUS, OnEnSetfocusPassword)
		COMMAND_HANDLER(IDC_PASSWORD, EN_KILLFOCUS, OnEnKillfocusPassword)
		COMMAND_HANDLER(IDC_USERNAME, EN_CHANGE, OnEnChangeUsername)
		COMMAND_HANDLER(IDC_PASSWORD_RETYPE, EN_CHANGE, OnEnChangePasswordRetype)
	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnBnClickedOk(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnBnClickedCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnEnChangePassword(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnEnSetfocusPassword(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnEnKillfocusPassword(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnEnChangeUsername(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnEnChangePasswordRetype(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	BEGIN_DDX_MAP(CUserDlg)
        DDX_CONTROL_HANDLE(IDC_USERNAME, m_ctlUsername)
        DDX_CONTROL_HANDLE(IDC_PASSWORD, m_ctlPassword)
        DDX_CONTROL_HANDLE(IDC_PASSWORD_RETYPE, m_ctlRetypePassword)
        DDX_CONTROL_HANDLE(IDOK, m_btnOk)
        DDX_CONTROL(IDC_PASSWORD_STRENGTH, m_ctlPasswordStrength)
    END_DDX_MAP()

private:
	CEdit m_ctlUsername;
	CEdit m_ctlPassword;
	CEdit m_ctlRetypePassword;
	CGradientBarCtrl m_ctlPasswordStrength;
	CButton m_btnOk;

	void UpdateControls();
	bool ShouldEnableOkButton();
};
