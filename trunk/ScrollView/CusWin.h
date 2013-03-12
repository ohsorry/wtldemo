#ifndef _CUS_WIN_H_
#define _CUS_WIN_H_
#include "stdafx.h"
#include "GdiPlusHelper.h"
#include "winuser.h"
#include "ScrollWiew.h"
typedef CWinTraits<WS_POPUPWINDOW, WS_EX_TRANSPARENT /*| WS_EX_LAYERED*/ | WS_EX_TOPMOST | WS_EX_TOOLWINDOW> CCusWinTraits;
//class CusWin : public CWindowImpl<CusWin,CWindow,CCusWinTraits>
	class CusWin : public CScrollWindowImpl<CusWin>
{
public:
	DECLARE_WND_CLASS("cuswin")

	BEGIN_MSG_MAP(THECLASS)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_DESTROY,OnDes)
		MESSAGE_HANDLER(WM_PAINT,OnPaint)
		MESSAGE_HANDLER(WM_MOUSEMOVE,OnMouseMove)
	END_MSG_MAP()

	LRESULT OnCreate(UINT , WPARAM , LPARAM , BOOL& )
	{
		//ModifyStyle(WS_VISIBLE | WS_BORDER | WS_CAPTION | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 0);
		const BYTE AlphaPercent = 200;
		//::SetLayeredWindowAttributes(m_hWnd, RGB(255,255,255), AlphaPercent, LWA_COLORKEY | LWA_ALPHA);

		mXinlang = mTianqi = NULL;
		mXinlang = Gdiplus::Image::FromFile(L"D:\\weather\\sina_icon.png",TRUE);
		mTianqi = Gdiplus::Image::FromFile(L"D:\\weather\\na.png",TRUE);
		mBackGnd = Gdiplus::Image::FromFile(L"D:\\weather\\bg_rain.jpg",TRUE);
		// 
		//Draw();
		//Invalidate(TRUE);

		SetWindowPos(HWND_TOP,0,0,mBackGnd->GetWidth(),mBackGnd->GetHeight(),SWP_SHOWWINDOW);
		//SetScrollRange(SB_HORZ,200,mBackGnd->GetWidth());
		//SetScrollRange(SB_VERT,200,mBackGnd->GetHeight());
		//CenterWindow();
		return false;
	}

	LRESULT OnDes(UINT , WPARAM , LPARAM , BOOL& )
	{
		delete mBackGnd;
		delete mTianqi;
		PostQuitMessage(1);
		return false;
	}

	LRESULT OnPaint(UINT , WPARAM , LPARAM , BOOL& )
	{
		CRect rc;
		GetClientRect(&rc);

		CPaintDC dc(m_hWnd);
		//Gdiplus::Graphics graphics2(dc.m_hDC);

		//CClientDC dc(m_hWnd);
		CDC dcmem;
		dcmem.CreateCompatibleDC(dc);
		CBitmap bitmap;
		bitmap.CreateCompatibleBitmap(dc,rc.Width(),rc.Height());
		HBITMAP hBitOld = dcmem.SelectBitmap(bitmap);
		CBrush brush;
		brush.CreateSolidBrush(RGB(127,127,127));

		dcmem.FillRect(rc,brush);
		Gdiplus::Graphics graphics(dcmem.m_hDC);

		Gdiplus::ImageAttributes imAtt;
		imAtt.SetColorKey(
			Gdiplus::Color(250, 250, 250),
			Gdiplus::Color(250, 250, 250),
			Gdiplus::ColorAdjustTypeBitmap);

		graphics.DrawImage(mBackGnd,Gdiplus::Rect(0,0,mBackGnd->GetWidth(),mBackGnd->GetHeight()),0,0,mBackGnd->GetWidth(),mBackGnd->GetHeight(),Gdiplus::UnitPixel,&imAtt);
		graphics.DrawImage(mXinlang,0,0);
		graphics.DrawImage(mTianqi,50,50);
	
		Gdiplus::Pen pen(Gdiplus::Color::Red,1);
		graphics.DrawRectangle(&pen,10,10,200,200);

		//dc.TransparentBlt(0,0,rc.Width(),rc.Height(),dcmem,0,0,rc.Width(),rc.Height(),RGB(255,255,255));
		dc.BitBlt(0,0,rc.Width(),rc.Height(),dcmem,0,0,SRCCOPY);
		//graphics2.Dc
		dcmem.SelectBitmap(hBitOld);

		return false;
	}

	LRESULT OnMouseMove(UINT , WPARAM , LPARAM , BOOL& )
	{
		CMainDlg dlg;
		dlg.DoModal();
		return false;
	}


	void Draw()
	{
		CClientDC dc(m_hWnd);
		Gdiplus::Graphics graphics(dc.m_hDC);		
		graphics.DrawImage(mXinlang,0,0);
	}

private:
	Gdiplus::Image* mXinlang;
	Gdiplus::Image* mTianqi;

	Gdiplus::Image* mBackGnd;
};

#endif