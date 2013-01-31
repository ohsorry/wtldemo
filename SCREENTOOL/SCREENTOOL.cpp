// ==========================================================================
// SCREENTOOL.cpp : 
// ==========================================================================
#include "stdafx.h"
#include "SCREENTOOL.h"
#include "MainDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


BEGIN_MESSAGE_MAP(CApp, CWinApp)
	//ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

//=================================================
// CApp construction
//=================================================
CApp::CApp()
{
}

//=================================================
// Overrides
//=================================================
BOOL CApp::InitInstance()
{
	// Allow only 1 instance of this app
	HWND hWnd = NULL;
	if(hWnd = ::FindWindow(NULL, _T("Screen Tool")))
	{
		if((hWnd) && (::IsWindow(hWnd)))
		{
			if(::IsIconic(hWnd))
				::ShowWindow(hWnd, SW_RESTORE);
			::SetForegroundWindow(hWnd);
			::BringWindowToTop(hWnd);
			return FALSE;
		}
	}

	// InitCommonControls() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	InitCommonControls();

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	// Startup GDI+
	GdiplusStartup(&m_GDIPlusToken, &m_GDIPSI, NULL);

	// Create main dialog and show it modal
	CMainDlg dlg;
	dlg.ReadRegistrySettings();
	m_pMainWnd = &dlg;
	dlg.DoModal();
	dlg.WriteRegistrySettings();
	
	// Cleanup GDI+
	GdiplusShutdown(m_GDIPlusToken);
    
	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

//=================================================
// The one and only CApp object
//=================================================
CApp theApp;
