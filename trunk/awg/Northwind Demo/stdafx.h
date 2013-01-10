// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#pragma once

// Change these values to use different versions
#define WINVER			0x0600
#define _WIN32_WINNT	0x0600
#define _WIN32_IE		0x0600
#define _RICHEDIT_VER	0x0100

#define CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include <atlbase.h>
#include <atlapp.h>

extern CAppModule _Module;

#include <atlwin.h>

#include <atlframe.h>
#include <atlstr.h>
#include <atlctrls.h>
#include <atlctrlx.h>
#include <atldlgs.h>
#include <atlctrlw.h>
#include <atlddx.h>
#include <atlcrack.h>
#include <atlscrl.h>
#include <comutil.h>
#include <comdef.h>
#include <atlcoll.h>
#include <atlgdi.h>
#include "../atlgdix.h"
#define _ATL_TMP_NO_CSTRING
#include <atlmisc.h>
#include <atlsplit.h>

//#pragma comment(lib,"comsupp.lib")

#import "msado15.dll" rename("EOF","adEOF")