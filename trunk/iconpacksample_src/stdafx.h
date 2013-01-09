// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

//#define WINVER         0x0600
//#define _WIN32_WINNT   0x0600
//#define _WIN32_IE      0x0600
//#define _RICHEDIT_VER  0x0100

#define WINVER         0x0501
#define _WIN32_WINNT   0x0501
#define _WIN32_IE      0x0501
#define _RICHEDIT_VER  0x0100

#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NON_CONFORMING_SWPRINTFS

#include <atlbase.h>
#include <atlapp.h>

extern CAppModule _Module;

#include <atlcom.h>
#include <atlhost.h>
#include <atlwin.h>
#include <atlctl.h>
#include <atlctrls.h>
#include <atlctrlx.h>
#include <atlmisc.h>
#include <atltheme.h>

#include <commctrl.h>

#include "Thread.h"

#include "ImageHelpers.h"

extern HICON ExtractStockIcon(HWND hWnd, int csidl, int cxyIcon, LPTSTR pstrPath, int& iIndex);

