#ifndef _stdafx_h_
#define _stdafx_h_



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

#include <atlwin.h>
#include <atlframe.h>

#include <atlsplit.h>
#include <atlctrls.h>
#include <atlmisc.h>
#include <atlctrlx.h>
#include <atlgdix.h>


#include "CustomTabCtrl.h"
#include "DotNetTabCtrl.h"
#include "SimpleTabCtrls.h"
#include "TabbedFrame.h"
#include "SimpleDlgTabCtrls.h"
//#include "TabbedMDI.h"

#endif