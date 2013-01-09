// WTLClipSpy.cpp : main source file for WTLClipSpy.exe
//

#include "stdafx.h"
#include "resource.h"
#include "aboutdlg.h"
#include "MainFrm.h"

CAppModule _Module;

CMainFrame* g_pMainWnd = NULL;

int Run(LPTSTR /*lpstrCmdLine*/ = NULL, int nCmdShow = SW_SHOWDEFAULT)
{
    CMessageLoop theLoop;
    _Module.AddMessageLoop(&theLoop);

    CMainFrame wndMain;
    g_pMainWnd = &wndMain;

    if(wndMain.CreateEx() == NULL)
    {
        ATLTRACE(_T("Main window creation failed!\n"));
        return 0;
    }

    wndMain.ShowWindow(nCmdShow);

    int nRet = theLoop.Run();

    _Module.RemoveMessageLoop();
    return nRet;
}

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR lpstrCmdLine, int nCmdShow)
{
    // Init COM with OleInitialize() so drag 'n drop will work.
    HRESULT hRes = OleInitialize(NULL);
    ATLASSERT(SUCCEEDED(hRes));

    // Init the common controls, then load the rich edit DLL.
    AtlInitCommonControls ( ICC_WIN95_CLASSES );
    LoadLibrary ( CRichEditCtrl::GetLibraryName() );

    hRes = _Module.Init(NULL, hInstance);
    ATLASSERT(SUCCEEDED(hRes));

    AtlAxWinInit();

    int nRet = Run(lpstrCmdLine, nCmdShow);

    _Module.Term();
    OleUninitialize();

    return nRet;
}
