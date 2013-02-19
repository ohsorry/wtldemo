#ifndef _generic_h_
#define _generic_h_
class DockSiteWnd : public dockwins::CDockingSiteImpl<DockSiteWnd>
{
	typedef dockwins::CDockingSiteImpl<DockSiteWnd> baseClass;

public:
	DECLARE_WND_CLASS(_T("CDockSiteSampleWnd"))
	BEGIN_MSG_MAP(CDockSiteSampleWnd)
		MESSAGE_HANDLER(WM_CREATE,OnCreate)
		CHAIN_MSG_MAP(baseClass)
	END_MSG_MAP()
	LRESULT OnCreate(UINT uMsg,WPARAM wParam, LPARAM lParam,BOOL&bHandled)
	{
		CRect rcBar(0,0,300,100);
		GetClientRect(&rcBar);
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
private:
	CListViewCtrl mListView;
};


#endif