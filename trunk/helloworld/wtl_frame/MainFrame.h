#ifndef _main_frame_h_1001
#define _main_frame_h_1001
#include "stdafx.h"
#include "resource.h"
template<class T, COLORREF t_crBrushColor>
class CPaintBkgnd : public CMessageMap
{
public:
	CPaintBkgnd(){m_hbrBkgnd = CreateSolidBrush(t_crBrushColor);}
	~CPaintBkgnd(){DeleteObject(m_hbrBkgnd);}
	BEGIN_MSG_MAP(CPaintBkgnd)
		MESSAGE_HANDLER(WM_ERASEBKGND,OnEraseBkgnd)
	END_MSG_MAP()

	LRESULT OnEraseBkgnd(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL&)
	{
		T* pt = static_cast<T*>(this);
		HDC hdc = (HDC)wParam;
		RECT rcClient;

		pt->GetClientRect(&rcClient);
		FillRect(hdc,&rcClient,m_hbrBkgnd);
		return 1;
	}

protected:
	HBRUSH m_hbrBkgnd;
};

class MainFrame : public CFrameWindowImpl<MainFrame>
					//public CPaintBkgnd<MainFrame,RGB(1,1,255)>
{
public:
	
	//typedef CPaintBkgnd<CMyWindow, RGB(1,1,255)> CPaintBkgndBase;

	DECLARE_FRAME_WND_CLASS(NULL,IDR_MENU1)
	BEGIN_MSG_MAP(MainFrame)
		//MESSAGE_HANDLER(WM_PAINT,OnPaint)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(WM_TIMER,OnTimer)
		MESSAGE_HANDLER(WM_ERASEBKGND,OnEraseGnd)
		CHAIN_MSG_MAP(CFrameWindowImpl<MainFrame>)
	END_MSG_MAP()

	LRESULT OnCreate(UINT,WPARAM,LPARAM, BOOL&)
	{
		SetTimer(1,1000);
		CreateSimpleStatusBar();
		//m_status.SubclassWindow(m_hWndStatusBar);
		m_status.SubclassWindow(m_hWndStatusBar);	
		int arrPanes[] = { 0,1,2/*ID_DEFAULT_PANE, IDR_DATE, IDR_TIME*/ };
		m_status.SetPanes(arrPanes, sizeof(arrPanes) / sizeof(int), false);
		return 0;
	}


	// this workaround solves a bug in CMultiPaneStatusBarCtrl
	// (in SetPanes() method) that limits the size of all panes
	// after the default pane to a combined total of 100 pixels  
	void SetPaneWidths(int* arrWidths, int nPanes)
	{
		// find the size of the borders
		int arrBorders[3];
		m_status.GetBorders(arrBorders);

		// calculate right edge of default pane
		arrWidths[0] += arrBorders[2];
		for (int i = 1; i < nPanes; i++)
			arrWidths[0] += arrWidths[i];

		// calculate right edge of remaining panes
		for (int j = 1; j < nPanes; j++)
			arrWidths[j] += arrBorders[2] + arrWidths[j - 1];

		// set the pane widths
		m_status.SetParts(m_status.m_nPanes, arrWidths);
	}
	LRESULT OnDestroy(UINT ,WPARAM ,LPARAM,BOOL&)
	{
		KillTimer(1);
		PostQuitMessage(0);
		return 0;
	}

	LRESULT OnTimer(UINT,WPARAM timerid,LPARAM,BOOL&)
	{
		int id = (int)timerid;
		RedrawWindow();
		return 0;
	}
	LRESULT OnEraseGnd(UINT, WPARAM wParam, LPARAM,BOOL&)
	{
		CDCHandle dc((HDC)wParam);
		CRect rc;
		SYSTEMTIME st;
		CString sTime;

		GetClientRect(rc);
		GetLocalTime(&st);
		sTime.Format(_T("The time is %d:%02d:%02d"),st.wHour,st.wMinute,st.wSecond);

		dc.SaveDC();
		dc.SetBkColor(RGB(255,174,0));
		dc.SetTextColor(RGB(0,0,0));
		dc.ExtTextOut(0,0,ETO_OPAQUE,rc,sTime,sTime.GetLength(),NULL);

		dc.RestoreDC(-1);
		return 0;
	}
protected:

	CMultiPaneStatusBarCtrl m_status;
};		

#endif