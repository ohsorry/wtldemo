#pragma  once

#include "stdafx.h"
#include "resource.h"
#include "gdiplus.h"
using namespace Gdiplus;
 
#define TITLE_BAR_HEIGHT 100
// typedef CWinTraits<WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, WS_EX_APPWINDOW | WS_EX_WINDOWEDGE>		CFrameWinTraits;
typedef CWinTraits< WS_CLIPCHILDREN | WS_POPUP|WS_VISIBLE|WS_SYSMENU, WS_EX_WINDOWEDGE>		CMyTraits;
//WS_POPUP|WS_VISIBLE|WS_SYSMENU
class MyWindow : public CWindowImpl<MyWindow,CWindow,CFrameWinTraits>
{
public:
	DECLARE_WND_CLASS("mYwINDOW");

	BEGIN_MSG_MAP(theClass)
		MESSAGE_HANDLER(WM_CREATE,OnCreate)
		MESSAGE_HANDLER(WM_DESTROY,OnDestroy)
		MESSAGE_HANDLER(WM_NCHITTEST,OnNCHitTest)		// 检测是否是标题栏
		MESSAGE_HANDLER(WM_NCPAINT,OnNcPaint)			// 非客户区域绘制
		MESSAGE_HANDLER(WM_NCACTIVATE,OnNcActive)		// 
		MESSAGE_HANDLER(WM_NCCALCSIZE,OnNcCalcSize)	//改变标题栏的大小

		//MESSAGE_HANDLER(WM_NCMOUSEMOVE,OnNcMouseMove)	// 检测鼠标是否停在最大，最小，关闭按钮
		//CHAIN_MSG_MAP( CWindowImpl< MyWindow > )
	END_MSG_MAP()

	LRESULT OnDestroy(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL&handled)
	{
		PostQuitMessage(1);
		return false;
	}

	LRESULT OnNCHitTest(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL&handled)
	{
		RECT WndRc;
		POINT ptMouse;
		GetWindowRect(&WndRc);
		GetCursorPos(&ptMouse);
		ScreenToClient(&ptMouse);
		if(ptMouse.y <= mHeightCap + mHeightTool)
			return HTCAPTION;
		else
			return HTCLIENT; 
	}

	LRESULT OnNcCalcSize(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL&handled)
	{
		/*
		typedef struct _NCCALCSIZE_PARAMS {
		RECT        rgrc[3]; 
		PWINDOWPOS  lppos; 
		} NCCALCSIZE_PARAMS, *LPNCCALCSIZE_PARAMS;  

		·rgrc
		矩形数组。
		第一个矩形包含窗口在移动或改变大小后的新坐标，也就是说，它是建议的新窗口坐标。
		第二个矩形包含了窗口在移动或改变大小前的坐标。
		第三个包含了窗口移动或改变大小前的客户区坐标。
		如果该窗口是子窗口，这些坐标相对于父窗口的客户区。如果窗口是顶层窗口，坐标相对于屏幕原点。

		·lppos
		指向WINDOWPOS结构的指针。该结构包含了对窗口进行移动或改变大小的操作时指定的大小和位置。 
		*/
		LPNCCALCSIZE_PARAMS pSizePtr = (LPNCCALCSIZE_PARAMS)lParam;
		//pSizePtr->rgrc[0].
		DoNcPaint();
		return 0;

	}

	LRESULT OnNcPaint(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL&handled)
	{

		DoNcPaint();
		return false;
	}

	LRESULT OnNcActive(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL&handled)
	{
		DoNcPaint();
		return true;
	}
	
	void DoNcPaint()
	{
		RECT rc,rcTitle;
		GetWindowRect(&rc);
		rcTitle.bottom = 0 + TITLE_BAR_HEIGHT;
		rcTitle.left = 0;
		rcTitle.right = 0 + rc.right - rc.left;
		rcTitle.top = 0;
		CWindowDC dc(m_hWnd);
		//dc.Draw3dRect(0,0,rc.right-rc.left,TITLE_BAR_HEIGHT,RGB(255,0,127),RGB(255,0,127));
		Graphics graphic(dc.m_hDC);
		// fill background
		SolidBrush solidBrush(Color(21, 103, 165));
		graphic.FillRectangle(&solidBrush, 0, 0, rcTitle.right - rcTitle.left, mHeightCap + mHeightTool);

		SolidBrush brush(Color(255,255,255,255));
		FontFamily fontFamily(L"微软雅黑");
		Font font(&fontFamily,12,FontStyleUnderline,UnitPixel);
		PointF pointF(10.0,5.0f);
		graphic.DrawString(L"360安全卫士 8.9",-1,&font,pointF,&brush);
		//graphic.
		//graphic.DrawImage(mImage,0,0);
		BITMAP bitmap;
		HDC hdcMem = CreateCompatibleDC(dc.m_hDC);
		HGDIOBJ oldbitmap = SelectObject(hdcMem,hBitmap);
		GetObject(hBitmap,sizeof(bitmap),&bitmap);
			
		for(int i = 0; i < rcTitle.right - rcTitle.left; i* bitmap.bmWidth > rcTitle.right - rcTitle.left)
		{
			::BitBlt(dc.m_hDC,i * bitmap.bmWidth,mHeightCap,bitmap.bmWidth,bitmap.bmHeight,hdcMem,0,0,SRCCOPY);
			i++;
		}
		

		SelectObject(hdcMem,oldbitmap);
		DeleteDC(hdcMem);


	}

	LRESULT OnNcMouseMove(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL&handled)		
	{
		return false;
	}

	LRESULT OnCreate(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL&handled)
	{
		CenterWindow();
		mHeightCap = GetSystemMetrics (SM_CYCAPTION);
		//GetSystemMetrics()
		//mImage = new Image((wchar_t*)"Skin.bmp");
		hBitmap = (HBITMAP)LoadImage(_Module.GetResourceInstance(),"Skin.bmp",IMAGE_BITMAP,0,0,LR_LOADFROMFILE);

		BITMAP bitmap;
		GetObject(hBitmap,sizeof(bitmap),&bitmap);
		mHeightTool = bitmap.bmHeight;
		return false;
	}

private:
	Gdiplus::Image *mImage;
	HBITMAP hBitmap;

	int mHeightCap;
	int mHeightTool;
};