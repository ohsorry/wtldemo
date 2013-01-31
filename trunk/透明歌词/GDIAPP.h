// GDIAPP.h : main header file for the GDIAPP application
//

#if !defined(AFX_GDIAPP_H__69A570C5_F71E_498E_91D4_577AD94775F9__INCLUDED_)
#define AFX_GDIAPP_H__69A570C5_F71E_498E_91D4_577AD94775F9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CGDIAPPApp:
// See GDIAPP.cpp for the implementation of this class
//

class CGDIAPPApp : public CWinApp
{
public:
	CGDIAPPApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGDIAPPApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CGDIAPPApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GDIAPP_H__69A570C5_F71E_498E_91D4_577AD94775F9__INCLUDED_)
