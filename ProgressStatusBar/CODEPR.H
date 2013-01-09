// codepr.h : interface of the CUrlDlg class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CODEPR_H__59A85ECB_6EF3_4807_8170_7DE13C2466F2__INCLUDED_)
#define AFX_CODEPR_H__59A85ECB_6EF3_4807_8170_7DE13C2466F2__INCLUDED_

/*

This class shows how you can put a dialog in a pane, and probably, why it's 
not such a good idea.

*/
class CUrlDlg : public CDialogImpl<CUrlDlg>
{
	CHyperLink m_hl;
	CComboBox m_Combo;
public:
	enum { IDD = IDD_URLDLG };

	BEGIN_MSG_MAP(CUrlDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
//      If your pane dialog has child controls, some resizing/moving around might be in order.
//		MESSAGE_HANDLER(WM_SIZE, OnSize)
		COMMAND_CODE_HANDLER(CBN_SELCHANGE, OnSelChange);
	END_MSG_MAP()

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		m_hl.SubclassWindow(GetDlgItem(IDC_CODEPROJECT));
		m_hl.SetHyperLink("http://www.codeproject.com/wtl/MultiPaneStatusBarEx.asp");
		m_Combo.Attach(GetDlgItem(IDC_COMBO1));
		m_Combo.AddString("Article");
		m_Combo.AddString("By the Author");
		m_Combo.SetCurSel(0);
		return TRUE;
	}
	LRESULT OnSelChange(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		if (IDC_COMBO1 == wID)
		{
			int iSel = m_Combo.GetCurSel();
			if (iSel)
				m_hl.SetHyperLink("http://www.codeproject.com/script/Articles/list_articles.asp?userid=306133");
			else
				m_hl.SetHyperLink("http://www.codeproject.com/wtl/MultiPaneStatusBarEx.asp");
		}
		return TRUE;
	}
//	LRESULT OnSize(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
//	{
//      DWORD dwNewWidth = LOWORD(lParam);
//      DWORD dwNewHeigth = HIWORD(lParam);
//		if (SIZE_RESTORED == wParam)
//		{
//		
//		}
//		return TRUE;
//	}

};

#endif // !defined(AFX_CODEPR_H__59A85ECB_6EF3_4807_8170_7DE13C2466F2__INCLUDED_)
