// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__6ED9276C_6FF2_4366_903E_1F9FEE2CD004__INCLUDED_)
#define AFX_STDAFX_H__6ED9276C_6FF2_4366_903E_1F9FEE2CD004__INCLUDED_

// Change these values to use different versions
#define WINVER          0x0500
#define _WIN32_WINNT    0x0500
#define _WIN32_IE       0x0501
#define _RICHEDIT_VER   0x0100

#define _ATL_APARTMENT_THREADED
#define _WTL_USE_CSTRING

#include <atlbase.h>
#include <atlapp.h>
extern CAppModule _Module;
#include <atlcom.h>
#include <atlhost.h>
#include <atlwin.h>
#include <atlctl.h>

#include <atlcrack.h>
#include <atlframe.h>
#include <atlmisc.h>
#include <atldlgs.h>
#include <atlsplit.h>
#include <atlctrls.h>
#include <atlctrlx.h>

#include <shlobj.h>
#include <shlguid.h>

#include <vector>
#include <map>

typedef std::map<CLIPFORMAT, CString> CClipFormatNameMap;


#define countof(x) (sizeof(x)/(sizeof(x)[0]))

#if _ATL_VER < 0x0700
#undef BEGIN_MSG_MAP
#define BEGIN_MSG_MAP(x) BEGIN_MSG_MAP_EX(x)
#endif

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__6ED9276C_6FF2_4366_903E_1F9FEE2CD004__INCLUDED_)
