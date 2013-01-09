// stdafx.cpp : source file that includes just the standard includes
//	SimpleTabbedMDIDemo.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

#if (_ATL_VER < 0x0700)
#include <atlimpl.cpp>
#endif //(_ATL_VER < 0x0700)

#if defined(_ATL_MIN_CRT) && (_MSC_VER < 1300)
	// (see TabbedMDI.h)
	RegisterTabbedMDIMessages g_RegisterTabbedMDIMessages;
#endif
