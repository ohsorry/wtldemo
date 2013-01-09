// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__858AEC70_E6B1_4285_AA86_DCFB1C5C06DE__INCLUDED_)
#define AFX_STDAFX_H__858AEC70_E6B1_4285_AA86_DCFB1C5C06DE__INCLUDED_

// Change these values to use different versions
#define WINVER		0x0500 // FOR GETCOMBOBOXINFO
//#define _WIN32_WINNT	0x0400
#define _WIN32_IE	0x0400
#define _RICHEDIT_VER	0x0100

#include <atlbase.h>
#include <atlapp.h>

extern CAppModule _Module;

#include <atlwin.h>

#include <atlctrls.h> // for CHyperLink
#include <atlctrlx.h> // for CHyperLink
#include <atldlgs.h>  // for CFileDialog

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__858AEC70_E6B1_4285_AA86_DCFB1C5C06DE__INCLUDED_)
