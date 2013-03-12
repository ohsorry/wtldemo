#ifndef _STDAFX_H_
#define _STDAFX_H_
// Change these values to use different versions
#define WINVER 0x0501
#define _WIN32_WINNT 0x0501
#define _WIN32_IE 0x0600
#define _RICHEDIT_VER 0x0100

#include <atlstr.h> // CString
#include <atltypes.h> // CRect,CPoint,CSize

#include <atlbase.h>
#include <atlapp.h>

extern CAppModule _Module;

#include <atlwin.h>

#define _WTL_NO_WTYPES
#define _WTL_NO_CSTRING
#include <atlmisc.h>

#include <atlscrl.h> 
#include <atlcrack.h>
#endif