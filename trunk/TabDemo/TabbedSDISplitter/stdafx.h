// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__22B3BB57_2EBA_4731_8159_984014EFF2E4__INCLUDED_)
#define AFX_STDAFX_H__22B3BB57_2EBA_4731_8159_984014EFF2E4__INCLUDED_

// Change these values to use different versions
#define WINVER		0x0400
//#define _WIN32_WINNT	0x0400
#define _WIN32_IE	0x0400
#define _RICHEDIT_VER	0x0100

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
#include <atlwin.h>

#include <atlframe.h>
#include <atlctrls.h>
#include <atldlgs.h>
#include <atlctrlw.h>
#include <atlctrlx.h>
#include <atlsplit.h>

#include "atlgdix.h"

#include "CustomTabCtrl.h"
#include "DotNetTabCtrl.h"
//#include "SimpleTabCtrls.h"
//#include "SimpleDlgTabCtrls.h"
#include "TabbedFrame.h"
//#include "TabbedMDI.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__22B3BB57_2EBA_4731_8159_984014EFF2E4__INCLUDED_)
