// Status.cpp : main source file for Status.exe
//

#include "stdafx.h"

#include <atlframe.h>
#include <atlctrls.h>
#include <atldlgs.h>

#include <atlctrlx.h>
#include <atlmisc.h>

#include "resource.h"

#include "aboutdlg.h"
#include "MainFrm.h"

CAppModule _Module;

int Run(LPTSTR /*lpstrCmdLine*/ = NULL, int nCmdShow = SW_SHOWDEFAULT)
{
	CMessageLoop theLoop;
	_Module.AddMessageLoop(&theLoop);

	CMainFrame wndMain;

	RECT rc = {0, 0, 350, 135};
	if(wndMain.CreateEx(NULL, &rc) == NULL)
	{
		ATLTRACE(_T("Main window creation failed!\n"));
		return 0;
	}

	wndMain.CenterWindow();
	wndMain.ShowWindow(nCmdShow);

	int nRet = theLoop.Run();

	_Module.RemoveMessageLoop();
	return nRet;
}

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR lpstrCmdLine, int nCmdShow)
{
	HRESULT hRes = ::CoInitialize(NULL);
	ATLASSERT(SUCCEEDED(hRes));

	::DefWindowProc(NULL, 0, 0, 0L);

	AtlInitCommonControls(ICC_BAR_CLASSES);	// add flags to support other controls

	hRes = _Module.Init(NULL, hInstance);
	ATLASSERT(SUCCEEDED(hRes));

	int nRet = Run(lpstrCmdLine, nCmdShow);

	_Module.Term();
	::CoUninitialize();

	return nRet;
}
