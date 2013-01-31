// ==========================================================================
// AboutDlg.cpp :
// ==========================================================================
#include "stdafx.h"
#include "AboutDlg.h"
#include ".\aboutdlg.h"

#pragma comment (lib, "Version.lib") // For VerQueryValue()

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

IMPLEMENT_DYNAMIC(CAboutDlg, CDialog)

//=================================================
// Construction / Destruction
//=================================================
CAboutDlg::CAboutDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAboutDlg::IDD, pParent)
{
}

CAboutDlg::~CAboutDlg()
{
}

//=================================================
// Overrides
//=================================================
BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// Load the icon
	HICON hIcon = NULL;
	VERIFY(hIcon = AfxGetApp()->LoadIcon(IDI_ABOUT));
	SetIcon(hIcon, TRUE);
	SetIcon(hIcon, FALSE);

	// Make window semi-transparent
	ModifyStyleEx(0, WS_EX_LAYERED);
	::SetLayeredWindowAttributes(m_hWnd, 
		RGB(255,0,255),	215, LWA_COLORKEY|LWA_ALPHA);

	// Append the version info to the title bar
	CString strCaption = _T("Screen Tool");
	CString strVersion = _T("");
	if(GetVersionInfo(strVersion))
	{
		CString strVer;
		VERIFY(strVer.LoadString(IDS_VERSION));
		strCaption += _T(" ");
		strCaption += strVer;
		strCaption += _T(": ");
		strCaption += strVersion;
	}
	SetWindowText(strCaption);
	

	return FALSE;
}


//=================================================
// Helper Functions
//=================================================
BOOL CAboutDlg::GetVersionInfo(CString& strVersion)
{
	HINSTANCE hResInst = AfxGetResourceHandle();
	ASSERT(hResInst);
	HRSRC hRsc = ::FindResource(hResInst, 
		MAKEINTRESOURCE(VS_VERSION_INFO), RT_VERSION);
	if(!hRsc)
		return FALSE;
	HGLOBAL hGlobal = ::LoadResource(hResInst, hRsc); 
    if(!hGlobal)
		return FALSE;
	LPVOID pVersionInfo = ::LockResource(hGlobal);  
	if(pVersionInfo)
	{
        DWORD dwLen = 0;
		DWORD dwLangID = 0;
        LPVOID pBuff = NULL;
		TCHAR strFileEntry[256] = {_T('\0')};
		_stprintf(strFileEntry, _T("\\VarFileInfo\\Translation"));
		BOOL bRetVal = ::VerQueryValue(pVersionInfo, 
			strFileEntry, &pBuff, (UINT*)&dwLen);
		if(bRetVal && dwLen==4) 
        {
			memcpy(&dwLangID, pBuff, 4);            
			_stprintf(strFileEntry, _T("\\StringFileInfo\\%02X%02X%02X%02X\\%s"),
				(dwLangID & 0xFF00)>>8,dwLangID & 0xFF,(dwLangID & 0xFF000000)>>24, 
				(dwLangID & 0xFF0000)>>16, _T("FileVersion"));            
		}
		else 
		{
			_stprintf(strFileEntry, _T("\\StringFileInfo\\%04X04B0\\%s"), 
				::GetUserDefaultLangID(), _T("FileVersion"));
		}
		if(::VerQueryValue(pVersionInfo, strFileEntry, &pBuff, (UINT*)&dwLen))
		{
			strVersion = (LPTSTR)pBuff;
		}
	}

	::UnlockResource(hGlobal);  
	::FreeResource(hGlobal);

	return TRUE;
}



