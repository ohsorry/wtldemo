#if !defined(AFX_GENTEXT_H__5E1A6E13_61BD_44CB_AD38_92AECDD09F30__INCLUDED_)
#define AFX_GENTEXT_H__5E1A6E13_61BD_44CB_AD38_92AECDD09F30__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GenText.h : header file
//

#define UNICODE
#ifndef ULONG_PTR
#define ULONG_PTR unsigned long*
#endif
#include "e:\gdiPlugs\gdiplus.h"   ////请修改为你的头文件路径
using namespace Gdiplus;  
#pragma comment(lib, "e:\\\gdiPlugs\\gdiplus.lib") ////请修改为你的.lib文件路径
/////////////////////////////////////////////////////////////////////////////
// GenText window

class GenText : public CStatic
{
// Construction
public:
	GenText();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(GenText)
	//}}AFX_VIRTUAL

// Implementation
public:
	int y;
	virtual ~GenText();

	// Generated message map functions
protected:
	//{{AFX_MSG(GenText)
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	HBITMAP bitmap;
	HDC memDC;
	void PaintBackgound(HDC hdc);
	int x;
	int m_nHeight;
	int m_nWidth;
	GdiplusStartupInput gdiplusStartupInput;
   ULONG_PTR           gdiplusToken;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GENTEXT_H__5E1A6E13_61BD_44CB_AD38_92AECDD09F30__INCLUDED_)
