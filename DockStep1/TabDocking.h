#ifndef _DOCKING_TAB_H_
#define _DOCKING_TAB_H_

#include "stdafx.h"
#include "atlctrls.h"
#include "DotNetTabCtrl.h"

//#include "CustomTabCtrl.h"
#include "DockingFrame.h"
#include "TabDockingBox.h"

class CSampleTabDockingWindow :
	public dockwins::CBoxedDockingWindowImpl< CSampleTabDockingWindow,CWindow, dockwins::COutlookLikeExBoxedDockingWindowTraits >
{
	typedef CSampleTabDockingWindow    thisClass;
	typedef dockwins::CBoxedDockingWindowImpl<CSampleTabDockingWindow,  CWindow,
		dockwins::COutlookLikeExBoxedDockingWindowTraits>  baseClass;
public:
	DECLARE_WND_CLASS(_T("CSampleTabDockingWindow"))
	BEGIN_MSG_MAP(thisClass)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
		CHAIN_MSG_MAP(baseClass)
	END_MSG_MAP()

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
};

#endif