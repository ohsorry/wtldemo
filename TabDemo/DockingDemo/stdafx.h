// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__99FC9CAD_4832_49BC_A30D_F8FA14DDBB4F__INCLUDED_)
#define AFX_STDAFX_H__99FC9CAD_4832_49BC_A30D_F8FA14DDBB4F__INCLUDED_

// Change these values to use different versions
#define WINVER		0x0500
#define _WIN32_WINNT	0x0500
#define _WIN32_IE	0x0501
#define _RICHEDIT_VER	0x0100

#ifdef _DEBUG
	// When debugging, turn on the CRT's debugging facilities
	// for checking for memory leaks
	// (we call _CrtSetDbgFlag in _tWinMain)
	#define _CRTDBG_MAP_ALLOC
#endif

// This is required for hosting browser in ATL7
#define _ATL_DLL

#include <atlbase.h>
#if _ATL_VER >= 0x0700
	#include <atlcoll.h>
	#include <atlstr.h>
	#include <atltypes.h>
	#define _WTL_NO_CSTRING
	#define _WTL_NO_WTYPES
#else
	#define _WTL_USE_CSTRING
#endif
#include <atlapp.h>

extern CAppModule _Module;

#include <atlmisc.h>
#include <atlcom.h>
#include <atlhost.h>
#include <atlwin.h>
#include <atlctl.h>

#include <atlframe.h>
#include <atlctrls.h>
#include <atldlgs.h>
#include <atlctrlw.h>

#include "atlgdix.h"

#include "CustomTabCtrl.h"
#include "DotNetTabCtrl.h"
#include "SimpleTabCtrls.h"
//#include "SimpleDlgTabCtrls.h"
#include "TabbedFrame.h"
//#include "TabbedMDISave.h"
#include "ListViewNoFlicker.h"

#define _TABBEDMDI_MESSAGES_EXTERN_REGISTER
#define _TABBEDMDI_MESSAGES_NO_WARN_ATL_MIN_CRT
#include "TabbedMDI.h"

#include <DWAutoHide.h>

#include <MsHtml.h>

#ifdef _DEBUG
	// When debugging, turn on the CRT's debugging facilities
	// for checking for memory leaks
	// (we call _CrtSetDbgFlag in _tWinMain)
	#define _CRTDBG_MAP_ALLOC
	#include <stdlib.h>
	#include <crtdbg.h>
#endif

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__99FC9CAD_4832_49BC_A30D_F8FA14DDBB4F__INCLUDED_)
