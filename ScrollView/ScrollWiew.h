#pragma once
#include "stdafx.h"
//#include <atlscrl.h>
#include "resource.h"
class BmpView : public CScrollWindowImpl<BmpView>
{
protected:
	BEGIN_MSG_MAP(BmpView)
		CHAIN_MSG_MAP(CScrollWindowImpl<BmpView>)
		MSG_WM_ERASEBKGND(OnEraseBkgnd)
	END_MSG_MAP()
public:
	BOOL OnEraseBkgnd(CDCHandle)
	{
		return TRUE;
	}
	void DoPaint(CDCHandle dc)
	{
		RECT rc = { 0,0,m_sizeAll.cx,m_sizeAll.cy };	
		if(bmp.IsNull())
		{
			CBrush br;
			br.CreateSolidBrush(RGB(215,240,215));
			dc.FillRect(&rc,br);
		}
		else
		{
			CDC mem;
			mem.CreateCompatibleDC(dc);
			CBitmap old = mem.SelectBitmap(bmp);
			dc.BitBlt(0,0,rc.right,rc.bottom,mem,0,0,SRCCOPY);
			mem.SelectBitmap(old);
		}
	}
	void SetBmp(HBITMAP bit)
	{
		bmp = bit;
		SIZE sz;
		bmp.GetSize(sz);
		SetScrollSize((int)sz.cx,(int)sz.cy);
	}
private:
	CBitmapHandle bmp;
};

class CMainDlg : public CDialogImpl<CMainDlg>
{
protected:
	BEGIN_MSG_MAP(CMainDlg)
		MSG_WM_INITDIALOG(OnInitDialog)
		MSG_WM_CLOSE(OnClose)
		MSG_WM_GETMINMAXINFO(OnMinMaxInfo)	//限制窗口的大小
		MSG_WM_SIZE(OnSize)
	END_MSG_MAP()
public:
	BOOL OnInitDialog(CWindow,LPARAM)
	{
		CenterWindow();

		//CIcon icon;
		//icon.LoadIcon(IDI_ICON_NTES);
		//SetIcon(icon,TRUE);
		//SetIcon(icon,FALSE);

		CRect rc;
		GetClientRect(&rc);
		bv.Create(m_hWnd,rc,NULL,WS_CHILD|WS_VISIBLE);
		bmp.LoadBitmap(IDB_BMP_ANGLE);
		bv.SetBmp(bmp.m_hBitmap);

		return TRUE;
	}
	void OnClose()
	{
		EndDialog(0);
	}
	void OnMinMaxInfo(LPMINMAXINFO lp)
	{
		lp->ptMinTrackSize = CPoint(300,300);
	}
	void OnSize(UINT,CSize sz)
	{
		bv.MoveWindow(0,0,sz.cx,sz.cy);
	}
private:
	BmpView bv;
	CBitmap bmp;
public:
	enum { IDD = IDD_MAINDLG };
};