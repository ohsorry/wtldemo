// ChildFrm.h : interface of the CChildFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHILDFRM_H__053AD664_0AE2_11D6_8BF1_00500477589F__INCLUDED_)
#define AFX_CHILDFRM_H__053AD664_0AE2_11D6_8BF1_00500477589F__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#define WM_POSTCREATE WM_USER + 1

class CChildFrame : public CTabbedMDIChildWindowImpl<CChildFrame>
{
public:
	DECLARE_FRAME_WND_CLASS(NULL, IDR_MDICHILD)

	CSimpleTabbedMDIDemoView m_view;

	virtual void OnFinalMessage(HWND /*hWnd*/)
	{
		delete this;
	}

	BEGIN_MSG_MAP(CChildFrame)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_POSTCREATE, OnPostCreate)
		MESSAGE_HANDLER(WM_FORWARDMSG, OnForwardMsg)
		CHAIN_MSG_MAP(CTabbedMDIChildWindowImpl<CChildFrame>)
	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
	{
		m_hWndClient = m_view.Create(m_hWnd, rcDefault, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_HSCROLL | WS_VSCROLL | ES_AUTOHSCROLL | ES_AUTOVSCROLL | ES_MULTILINE | ES_NOHIDESEL, WS_EX_CLIENTEDGE);

		this->PostMessage(WM_POSTCREATE);

		bHandled = FALSE;
		return 1;
	}

	LRESULT OnPostCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		static int nCount=0;

		CString sToolTip;
		sToolTip.Format(_T("C:\\This is\\a test\\of the tooltip %d\\Document"), nCount++);
		this->SetTabToolTip(sToolTip);

		CMenuHandle menu = this->GetSystemMenu(FALSE);
		if(menu.IsMenu())
		{
			// To change the context menu when clicking on the tab corresponding
			//  to this window in the tabbed MDI, you can simply modify the 
			//  system menu for the window here.

			// Add menu items:
			//menu.AppendMenu(MF_SEPARATOR);
			//menu.AppendMenu((MF_ENABLED | MF_STRING), ID_VIEW_SOURCE, _T("View Source"));

			// Delete menu items:
			//menu.DeleteMenu(SC_SIZE, MF_BYCOMMAND);
			//menu.DeleteMenu(SC_MOVE, MF_BYCOMMAND);
			//menu.DeleteMenu(SC_MINIMIZE, MF_BYCOMMAND);
			//menu.DeleteMenu(SC_RESTORE, MF_BYCOMMAND);

			if(0 != menu.GetMenuString(SC_NEXTWINDOW, NULL, 0, MF_BYCOMMAND))
			{
				menu.DeleteMenu(SC_NEXTWINDOW, MF_BYCOMMAND);

				int nLastItemIndex = menu.GetMenuItemCount()-1;
				int nId = menu.GetMenuItemID(nLastItemIndex);
				if(SC_SEPARATOR == nId || 0 == nId)
				{
					menu.DeleteMenu(nLastItemIndex, MF_BYPOSITION);
				}
			}
		}

		return 0;
	}

	LRESULT OnForwardMsg(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
	{
		LPMSG pMsg = (LPMSG)lParam;

		if(CTabbedMDIChildWindowImpl<CChildFrame>::PreTranslateMessage(pMsg))
			return TRUE;

		return m_view.PreTranslateMessage(pMsg);
	}
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHILDFRM_H__053AD664_0AE2_11D6_8BF1_00500477589F__INCLUDED_)
