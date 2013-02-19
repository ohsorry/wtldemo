#ifndef _MYDOCKINGWND_H_
#define _MYDOCKINGWND_H_
#include <ExtDockingWindow.h>
class MyDockingWnd : public dockwins::CTitleDockingWindowImpl<MyDockingWnd,
	CWindow,
	dockwins::COutlookLikeTitleDockingWindowTraits>
{
	typedef dockwins::CTitleDockingWindowImpl<MyDockingWnd,CWindow,dockwins::COutlookLikeTitleDockingWindowTraits> baseClass;
public:
	DECLARE_WND_CLASS("Myd")
	BEGIN_MSG_MAP(theClass)
		MESSAGE_HANDLER(WM_CREATE,OnCreate)
		MESSAGE_HANDLER(WM_SIZE, OnSize)
		//MESSAGE_HANDLER(WM_PAINT, OnPaint)
		CHAIN_MSG_MAP(baseClass);
	END_MSG_MAP()
	

	void OnDocked(HDOCKBAR hBar,bool bHorizontal)
	{
		DWORD dwStyle = GetWindowLong(GWL_STYLE)&(~WS_SIZEBOX);
		SetWindowLong( GWL_STYLE, dwStyle);

		baseClass::OnDocked(hBar,bHorizontal);
	}
	void OnUndocked(HDOCKBAR hBar)
	{
		DWORD dwStyle = GetWindowLong(GWL_STYLE) | WS_SIZEBOX;
		SetWindowLong( GWL_STYLE , dwStyle);

		baseClass::OnUndocked(hBar);
	}
	LRESULT OnCreate(UINT uMsg,WPARAM wParam, LPARAM lParam,BOOL&bHandled)
	{
		CRect rcBar(0,0,300,100);
		//GetClientRect(&rcBar);
		mListView.Create(m_hWnd,rcBar,"123",LVS_ALIGNTOP | LVS_ICON | LVS_REPORT | LVS_SHAREIMAGELISTS | WS_CHILD | WS_HSCROLL | WS_VISIBLE | WS_VSCROLL,
			WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR | WS_EX_STATICEDGE);
		mListView.AddColumn("123",0);
		mListView.AddColumn("123",1);
		mListView.AddColumn("123",2);
		//mListView.InsertItem(0,"123");

		//m_hWndClient = mList.m_hWnd;
		UpdateWindow();
		return true;
	}

	LRESULT OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		CPaintDC dc(m_hWnd);
		RECT rc;
		GetClientRect(&rc);
		dc.FillRect(&rc, (HBRUSH)LongToPtr(COLOR_WINDOW + 1));
		dc.SetBkMode(TRANSPARENT);
		int txtLen = GetWindowTextLength()+1;
		TCHAR* pTxt = new TCHAR[txtLen]; 
		GetWindowText(pTxt,txtLen);
		dc.DrawText(pTxt/*_T("SampleDockingWindow")*/,-1,&rc, DT_END_ELLIPSIS | DT_CENTER | DT_VCENTER | DT_SINGLELINE );			
		delete [] pTxt;
		//		dc.DrawText(_T("SampleDockingWindow"),-1,&rc, DT_END_ELLIPSIS | DT_CENTER | DT_VCENTER | DT_SINGLELINE );			
		return 0;
	}

	LRESULT OnSize(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
	{
		if(wParam != SIZE_MINIMIZED )
		{
			RECT rc;
			GetClientRect(&rc);
			::SetWindowPos(mListView.m_hWnd, NULL, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top ,SWP_NOZORDER | SWP_NOACTIVATE);
		}
		bHandled = FALSE;
		return 1;
	}

private:
	CListViewCtrl mListView;

	//CTreeViewCtrl	m_tree;
};


#endif