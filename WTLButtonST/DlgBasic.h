#if !defined(__dlgbasic_h__)
#define __dlgbasic_h__

#include "src/ButtonST.h"

class CDlgBasic : public CAxDialogImpl<CDlgBasic>
{
public:
	CDlgBasic();

	enum { IDD = IDD_BASIC };

	BEGIN_MSG_MAP(CMainDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_HANDLER(IDC_BTNDISABLED, BN_CLICKED, OnBtnDisabled)
		REFLECT_NOTIFICATIONS()
	END_MSG_MAP()

protected:

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnBtnDisabled(UINT , int , HWND , BOOL&);

	CButtonST	m_btnStandard;
	CButtonST	m_btnHalloween;
	CButtonST	m_btnKeyManager;
	CButtonST	m_btnDisabled;
	CButtonST	m_btnLamp;
	CButtonST	m_btnZip;
	CButtonST	m_btnJpeg;
	CButtonST	m_btnCDRom;
	CButtonST	m_btnTooltip;
	CButtonST	m_btnSearch;
	CButtonST	m_btnBack;
	CButtonST	m_btnNext;
};



#endif // !defined(__dlgbasic_h__)
