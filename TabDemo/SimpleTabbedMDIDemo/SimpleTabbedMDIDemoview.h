// SimpleTabbedMDIDemoView.h : interface of the CSimpleTabbedMDIDemoView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_SIMPLETABBEDMDIDEMOVIEW_H__053AD665_0AE2_11D6_8BF1_00500477589F__INCLUDED_)
#define AFX_SIMPLETABBEDMDIDEMOVIEW_H__053AD665_0AE2_11D6_8BF1_00500477589F__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CSimpleTabbedMDIDemoView :
	public CWindowImpl<CSimpleTabbedMDIDemoView, CEdit>,
	public CEditCommands<CSimpleTabbedMDIDemoView>
{
public:
	DECLARE_WND_SUPERCLASS(NULL, CEdit::GetWndClassName())

	BOOL PreTranslateMessage(MSG* pMsg)
	{
		pMsg;
		return FALSE;
	}

	BEGIN_MSG_MAP(CSimpleTabbedMDIDemoView)
		CHAIN_MSG_MAP_ALT(CEditCommands<CSimpleTabbedMDIDemoView>, 1)
	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SIMPLETABBEDMDIDEMOVIEW_H__053AD665_0AE2_11D6_8BF1_00500477589F__INCLUDED_)
