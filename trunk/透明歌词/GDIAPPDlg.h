// GDIAPPDlg.h : header file
//

#if !defined(AFX_GDIAPPDLG_H__2F8FB528_3F75_4056_901F_27465A22B1B6__INCLUDED_)
#define AFX_GDIAPPDLG_H__2F8FB528_3F75_4056_901F_27465A22B1B6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "stdafx.h"
/////////////////////////////////////////////////////////////////////////////
// CGDIAPPDlg dialog
#define UNICODE
#ifndef ULONG_PTR
#define ULONG_PTR unsigned long*
#endif
#include "gdiplus.h"   ////Modify your path
using namespace Gdiplus;  
#pragma comment(lib, "gdiplus.lib") //Modify your lib path

class CGDIAPPDlg : public CDialog
{
// Construction
public:
	int m_kind;
	int cx;
	BOOL UpdateDisplay(int Transparent=255);
	CGDIAPPDlg(CWnd* pParent = NULL);	// standard constructor
	HINSTANCE hFuncInst ;
	typedef BOOL (WINAPI *MYFUNC)(HWND,HDC,POINT*,SIZE*,HDC,POINT*,COLORREF,BLENDFUNCTION*,DWORD);          
	MYFUNC UpdateLayeredWindow;
	BLENDFUNCTION m_Blend;
	HDC m_hdcMemory;
// Dialog Data
	//{{AFX_DATA(CGDIAPPDlg)
	enum { IDD = IDD_GDIAPP_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGDIAPPDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CGDIAPPDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnMenuAbout();
	afx_msg void OnMENUExit();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	BOOL m_bBack;
	GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR           gdiplusToken;
};




//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GDIAPPDLG_H__2F8FB528_3F75_4056_901F_27465A22B1B6__INCLUDED_)
