// IconPackSample.cpp
//

#include "stdafx.h"

#include <atlframe.h>
#include <atlctrls.h>
#include <atldlgs.h>

#include "resource.h"

#include "MainFrm.h"

CAppModule _Module;


int Run(LPTSTR /*lpstrCmdLine*/ = NULL, int nCmdShow = SW_SHOWDEFAULT)
{
   CMessageLoop theLoop;
   _Module.AddMessageLoop(&theLoop);

   CMainFrame wndMain;
   if( wndMain.CreateEx() == NULL ) {
      ATLTRACE(_T("Main window creation failed!\n"));
      return 0;
   }
   wndMain.ShowWindow(nCmdShow);

   int nRet = theLoop.Run();

   _Module.RemoveMessageLoop();
   return nRet;
}


// While Windows Vista provides SHGetStockIconInfo(), we'll have to look for an alternative
// on Windows XP. Here we use the Shell Namespace APIs.
HICON ExtractStockIcon(HWND hWnd, int csidl, int cxyIcon, LPTSTR pstrPath, int& iIndex)
{
   ATLASSERT(pstrPath);
   // First, let's get the PIDL for the special folder.
   HICON hIconLarge = NULL;
   LPITEMIDLIST pidlFull = NULL;
   HRESULT hr = ::SHGetFolderLocation(hWnd, csidl, NULL, 0, &pidlFull);
   if( FAILED(hr) ) return NULL;
   // Now bind to the Shell Folder's parent.
   CComPtr<IShellFolder> spItem;
   LPCITEMIDLIST pidlItem = NULL;
   hr = ::SHBindToParent(pidlFull, IID_IShellFolder, (LPVOID*) &spItem, &pidlItem);
   if( SUCCEEDED(hr) ) {
      // We can ask the Folder for the item's IExtractIcon interface to obtain
      // the requested icon.
      CComPtr<IExtractIcon> spEI;
      hr = spItem->GetUIObjectOf(hWnd, 1, &pidlItem, IID_IExtractIcon, NULL, (LPVOID*) &spEI);
      if( SUCCEEDED(hr) ) {
         UINT dwFlags = 0;
         hr = spEI->GetIconLocation(GIL_OPENICON, pstrPath, MAX_PATH, &iIndex, &dwFlags);
         if( FAILED(hr) ) hr = spEI->GetIconLocation(0, pstrPath, MAX_PATH, &iIndex, &dwFlags);
         if( SUCCEEDED(hr) ) {
            HICON hIconSmall = NULL;
            spEI->Extract(pstrPath, iIndex, &hIconLarge, &hIconSmall, cxyIcon);
            ::DestroyIcon(hIconSmall);
         }
      }
   }
   ::CoTaskMemFree(pidlFull);
   return hIconLarge;
}


int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR lpstrCmdLine, int nCmdShow)
{
   HRESULT hRes = ::CoInitialize(NULL);
   ATLASSERT(SUCCEEDED(hRes));

   ::DefWindowProc(NULL, 0, 0, 0L);

   AtlInitCommonControls(ICC_LISTVIEW_CLASSES | ICC_BAR_CLASSES | ICC_PROGRESS_CLASS);

   hRes = _Module.Init(NULL, hInstance);
   ATLASSERT(SUCCEEDED(hRes));

   int nRet = Run(lpstrCmdLine, nCmdShow);

   _Module.Term();
   ::CoUninitialize();

   return nRet;
}

