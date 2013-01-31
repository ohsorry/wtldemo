// GenText.cpp : implementation file
//

#include "stdafx.h"

#include "GDIAPP.h"
#include "GenText.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// GenText

GenText::GenText()
{
	 GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
}

GenText::~GenText()
{
 GdiplusShutdown(gdiplusToken);
}


BEGIN_MESSAGE_MAP(GenText, CStatic)
	//{{AFX_MSG_MAP(GenText)
	ON_WM_PAINT()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// GenText message handlers

void GenText::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	BitBlt(dc.m_hDC ,0,0,m_nWidth,m_nHeight,memDC,0,0,SRCCOPY);
//TransparentBlt(dc.m_hDC ,x,y,m_nWidth,m_nHeight,memDC,0,0,m_nWidth,m_nHeight,RGB(0,0,0));	
	//dc.Rectangle (x,y,m_nWidth,m_nHeight);
}

int GenText::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CStatic::OnCreate(lpCreateStruct) == -1)
		return -1;
	x=lpCreateStruct->x;
	y=lpCreateStruct->y;
	m_nWidth=lpCreateStruct->cx ;
	m_nHeight=lpCreateStruct->cy;

	CClientDC dc(this);
	    Graphics graphics(dc.m_hDC );
	SolidBrush brush(Color(255,50,50,150));
	graphics.FillRectangle(&brush,x,y,m_nWidth,m_nHeight);
		
	memDC=CreateCompatibleDC(dc.m_hDC );
	bitmap=CreateCompatibleBitmap(dc.m_hDC ,420,200);
	SelectObject(memDC,bitmap);
	::BitBlt (memDC,0,0,420,200,dc.m_hDC ,0,0,SRCCOPY);
	PaintBackgound(memDC);
	// TODO: Add your specialized creation code here
	
	return 0;
}

void GenText::PaintBackgound(HDC hdc)
{
	Graphics graphics(hdc);
	graphics.SetSmoothingMode(SmoothingModeAntiAlias);
	graphics.SetInterpolationMode(InterpolationModeHighQualityBicubic);

	FontFamily fontFamily(L"Arial");
	StringFormat strformat;
	wchar_t pszbuf[]=L"Happy New Year!";

	GraphicsPath path;
	path.AddString(pszbuf,wcslen(pszbuf),&fontFamily,
		FontStyleRegular,48,Point(10,10),&strformat);

	Pen pen(Color(155,215,215,215),3);

	graphics.DrawPath(&pen,&path);
	//SolidBrush brush(Color(255,128,0,255));
	LinearGradientBrush linGrBrush(
		Point(0,0),Point(0,90),
		Color(255,255,255,255),
		Color(255,30,120,195));

	LinearGradientBrush linGrBrushW(
		Point(0,0),Point(0,90),
		Color(255,255,255,255),
		Color(5,1,1,1));
/**/
   for(int i=1; i<8; i+=1)
    {
        Pen pen(Color(32, 0, 12, 12), i);
        pen.SetLineJoin(LineJoinRound);
        graphics.DrawPath(&pen, &path);
    } 
	
	graphics.FillPath(&linGrBrush,&path);
	graphics.FillPath(&linGrBrushW,&path);
}
