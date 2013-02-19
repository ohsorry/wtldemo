#include "stdafx.h"

#include "DockingFrame.h"	
#include "TabDocking.h"
#include "MyDockingWnd.h"
#include "resource.h"
#include "SdiView.h"
class MainFrame:public dockwins::CDockingFrameImpl<MainFrame>,
		public CMessageFilter,
		public CIdleHandler

{
public:
	DECLARE_FRAME_WND_CLASS("mY",IDR_MENU1)

	virtual BOOL PreTranslateMessage(MSG* pMsg)
	{
		return false;
	}
	virtual BOOL OnIdle()
	{
		return false;
	}

	BEGIN_MSG_MAP(theClass)
		MESSAGE_HANDLER(WM_CREATE,OnCreate)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
		CHAIN_MSG_MAP(dockwins::CDockingFrameImpl<MainFrame>)
		//CHAIN_MSG_MAP_MEMBER(mDockWnd)
	END_MSG_MAP()

	LRESULT OnCreate(UINT , WPARAM , LPARAM , BOOL& bHandled)
	{
		CMessageLoop*loop = _Module.GetMessageLoop();
		if(loop)
		{
			loop->AddMessageFilter(this);
			loop->AddIdleHandler(this);
		}

		InitializeDockingFrame();

		m_hWndClient = m_view.Create(m_hWnd, rcDefault, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, WS_EX_CLIENTEDGE);
		
		CRect rcBar(0,0,300,100);
		
		mDockWnd.Create(m_hWnd,rcBar,"Mydockingwnd");
		mTabDock1.Create(m_hWnd,rcBar,"DockSite_generic 23\nsadfaslj");
		mTabDock2.Create(m_hWnd,rcBar,"DockSite_generic32\nasldf ");
		DockWindow(mDockWnd,dockwins::CDockingSide(dockwins::CDockingSide::sBottom),0,0,100,100);
		DockWindow(mTabDock1,dockwins::CDockingSide(dockwins::CDockingSide::sRight),0,0,100,100);
		mTabDock2.DockTo(mTabDock1,0);
		return false;
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

public:
	MyDockingWnd mDockWnd;
	CSampleTabDockingWindow mTabDock1,mTabDock2;
	CSDISampleView m_view;
};