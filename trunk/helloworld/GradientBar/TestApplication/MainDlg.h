// MainDlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include "GradientBarCtrl.h"

class CMainDlg
	: public CDialogImpl<CMainDlg>
	, public CWinDataExchange<CMainDlg>
{
public:
	enum { IDD = IDD_MAINDLG };

	BEGIN_MSG_MAP(CMainDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
		COMMAND_HANDLER(IDC_PERCENTAGE, EN_CHANGE, OnEnChangePercentage)
		COMMAND_HANDLER(IDC_PERCENTAGE, EN_UPDATE, OnEnUpdatePercentage)
		COMMAND_HANDLER(IDC_DISPLAY_VALUE, BN_CLICKED, OnBnClickedDisplayValue)
		COMMAND_HANDLER(IDC_SHOW_MARQUEE, BN_CLICKED, OnBnClickedEnableMarquee)
		COMMAND_HANDLER(IDC_BUTTON_EXIT, BN_CLICKED, OnBnClickedButtonExit)
		COMMAND_HANDLER(IDC_BUTTON_REALWORLD, BN_CLICKED, OnBnClickedButtonRealworld)
	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnEnChangePercentage(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnEnUpdatePercentage(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnBnClickedDisplayValue(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnBnClickedEnableMarquee(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnBnClickedButtonRealworld(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnBnClickedButtonExit(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	BEGIN_DDX_MAP(CMainDlg)
        DDX_CONTROL_HANDLE(IDC_PERCENTAGE, m_ctlEdit)
        DDX_CONTROL_HANDLE(IDC_SPIN, m_ctlUpDown)
        DDX_CONTROL(IDC_GRADIENT, m_gradientBar)
        DDX_CONTROL_HANDLE(IDC_DISPLAY_VALUE, m_ctlDisplayValue)
        DDX_CONTROL_HANDLE(IDC_SHOW_MARQUEE, m_ctlEnableMarquee)
    END_DDX_MAP()

private:
	CEdit m_ctlEdit;
	CUpDownCtrl m_ctlUpDown;
	CGradientBarCtrl m_gradientBar;
	CButton m_ctlDisplayValue;
	CButton m_ctlEnableMarquee;
};
