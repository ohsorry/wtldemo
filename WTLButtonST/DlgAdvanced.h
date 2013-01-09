#if !defined(__dlgadvanced_h__)
#define __dlgadvanced_h__

#include "src/ButtonST.h"

class CDlgAdvanced : public CAxDialogImpl<CDlgAdvanced>
{
public:
	CDlgAdvanced();

	enum { IDD = IDD_ADVANCED };

	BEGIN_MSG_MAP(CMainDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_HANDLER(IDC_BTNAUTOREPEAT, BN_CLICKED, OnBtnAutorepeat)
		REFLECT_NOTIFICATIONS()
	END_MSG_MAP()

protected:

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnBtnAutorepeat(UINT , int , HWND , BOOL&);

	CButtonST	m_btnAutoRepeat;
	CButtonST	m_btnHyperLink;
	CButtonST	m_btnCursor;
	CButtonST	m_btnFocusRect;
	CButtonST	m_chkCheckbox;
	CButtonST	m_btnBitmap;

	BYTE		m_byCounter;
};


#endif // !defined(__dlgadvanced_h__)
