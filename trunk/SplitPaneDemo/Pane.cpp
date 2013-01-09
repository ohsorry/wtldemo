// Pane.cpp : main source file for Pane.exe
//

#include "stdafx.h"

#include <atlframe.h>
#include <atlctrls.h>
#include <atldlgs.h>
#include <atlmisc.h>
#include <atlsplit.h>
#include <atlctrlx.h>

#include "resource.h"

#include "aboutdlg.h"
#include "MainFrm.h"

CAppModule _Module;

int Run(LPTSTR = NULL, int nCmdShow = SW_SHOWDEFAULT)
{
	CMessageLoop theLoop;
	_Module.AddMessageLoop(&theLoop);

	CMainFrame wndMain;

	// size of window to create
	CRect rc = CRect(0,0,380,265);
	if(wndMain.CreateEx(NULL, rc) == NULL)
	{
		ATLTRACE(_T("Main window creation failed!\n"));
		return 0;
	}

	// center and show main window
	wndMain.CenterWindow();
	wndMain.ShowWindow(nCmdShow);

	int nRet = theLoop.Run();

	_Module.RemoveMessageLoop();
	return nRet;
}

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE, LPTSTR lpstrCmdLine, int nCmdShow)
{
	HRESULT hRes = ::CoInitialize(NULL);
	ATLASSERT(SUCCEEDED(hRes));

	::DefWindowProc(NULL, 0, 0, 0L);

	hRes = _Module.Init(NULL, hInstance);
	ATLASSERT(SUCCEEDED(hRes));

	int nRet = Run(lpstrCmdLine, nCmdShow);

	_Module.Term();
	::CoUninitialize();

	return nRet;
}
