// ==========================================================================
// MainDlg.cpp : 
// ==========================================================================
#include "stdafx.h"
#include "SCREENTOOL.h"
#include "MainDlg.h"
#include ".\maindlg.h"
#include "AboutDlg.h"
#include "Registry.h"
#include "GetDataDlg.h"

// Static variable initialization
HHOOK CMainDlg::m_hMouseHook = NULL;
HHOOK CMainDlg::m_hKeyboardHook = NULL;
CMainDlg* CMainDlg::m_pThis = NULL;

// Message IDs 
#define WM_TRAY_NOTIFICATION	(WM_USER + 101)
#define WM_SCREENDRAW_DONE		(WM_USER + 102)
#define WM_WINSELECT_DONE		(WM_USER + 103)
#define WM_MOUSEHOOK_CANCEL		(WM_USER + 104)
#define WM_PRINTSCREEN			(WM_USER + 105)

// SHBrowseForFolder callback
static int __stdcall BrowseCallbackProc(HWND, UINT, LPARAM, LPARAM);


BEGIN_MESSAGE_MAP(CMainDlg, CDialog)
	ON_WM_DESTROY()
	ON_WM_CLOSE()
	ON_WM_SIZE()
	ON_WM_SYSCOMMAND()

	ON_CBN_SELCHANGE(IDC_IMAGE_EXT, OnCbnSelchangeImageExt)
	ON_BN_CLICKED(IDC_SCREEN_DRAW, OnBnClickedScreenDraw)
	ON_BN_CLICKED(IDC_WIN_SELECT, OnBnClickedWindowSelect)
	ON_BN_CLICKED(IDC_BROWSE_IMG_PATH, OnBnClickedBrowseImgPath)
	ON_EN_KILLFOCUS(IDC_LINE_WIDTH, OnEnKillfocusLineWidth)

	ON_MESSAGE(WM_SCREENDRAW_DONE, OnDrawComplete)
	ON_MESSAGE(WM_WINSELECT_DONE, OnSelectionComplete)
	ON_MESSAGE(WM_MOUSEHOOK_CANCEL, OnMouseHookCancel)
	ON_MESSAGE(WM_PRINTSCREEN, OnPrintScreen)
	ON_MESSAGE(WM_TRAY_NOTIFICATION, OnTrayNotification)

	ON_COMMAND(ID_SHOW, OnTrayShow)
	ON_COMMAND(ID_SHUTDOWN, OnShutdown)
	ON_COMMAND(ID_SHOW_ABOUT, OnShowAbout)
	ON_COMMAND(ID_DRAW_SEL, OnDrawSelection)
	ON_COMMAND(ID_WIN_SEL, OnWindowSelect)
END_MESSAGE_MAP()

//=================================================
// Construction 
//=================================================
CMainDlg::CMainDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMainDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_hSelectCursor = AfxGetApp()->LoadStandardCursor(IDC_SIZEALL);
	m_bMouseHook = FALSE;
	m_bKeyboardHook = FALSE;
	m_pThis = this;
	m_bDrawing = FALSE;
	m_bSelecting = FALSE;
	m_pDesktopDC = NULL;
	m_iBandPx = 3;
	m_bDitherLast = FALSE;
	m_hHilightWnd = NULL;
	m_bStayInTray = FALSE;
	m_bMinToTray = TRUE;
	m_iNamingOpt = 2;
	m_strImageName = _T("");
	m_hTmpDraw = NULL;

	if(!m_strImagePath.IsEmpty())
		return;

	// Get default image path use desktop for now
	LPITEMIDLIST pidl = 0;
	HRESULT hr = NOERROR;
	if(SUCCEEDED(hr = ::SHGetSpecialFolderLocation(m_hWnd, CSIDL_DESKTOP, &pidl)))
	{
		if(pidl)
		{
			::SHGetPathFromIDList(pidl, m_strImagePath.GetBuffer(MAX_PATH));
			m_strImagePath.ReleaseBuffer();

			// Clean up
			LPMALLOC pMalloc;
			hr = ::SHGetMalloc(&pMalloc);
			pMalloc->Free(pidl);
			pMalloc->Release();
		}
	}

	// Make sure we have a path
	if(m_strImagePath.GetLength() == 0)
		m_strImagePath = _T("C:\\");
	
	// Ensure trailing backslash
	if(m_strImagePath.Right(1) != _T('\\'))
		m_strImagePath += _T('\\');
}

//=================================================
// Overrides
//=================================================
BOOL CMainDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	// Add "About..." menu item to system menu.
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if(pSysMenu != NULL)
	{
		CString strAboutMenu = _T("About...");
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// Set max-length on edit boxes
	CEdit* pEdit = NULL;
	VERIFY(pEdit = (CEdit*)GetDlgItem(IDC_IMAGE_NAME));
	pEdit->SetLimitText(50);

	// Fill combo with all supported image formats
	CComboBox* pCombo = NULL;
	VERIFY(pCombo = (CComboBox*)GetDlgItem(IDC_IMAGE_EXT));
	ImageCodecInfo* pImageCodecInfo = NULL;
	UINT nCount = 0; // number of image encoders
	UINT nSize = 0;  // size of the image encoder array in bytes
	GetImageEncodersSize(&nCount, &nSize);
	if(nSize > 0)
	{
		pImageCodecInfo = (ImageCodecInfo*)(malloc(nSize));
		if(pImageCodecInfo != NULL)
		{
			GetImageEncoders(nCount, nSize, pImageCodecInfo);
			for(UINT x = 0; x < nCount; x ++)
			{
				CString strFormat(pImageCodecInfo[x].FormatDescription);
				int iItem = pCombo->AddString(strFormat);
				ASSERT(iItem != -1);
				CLSID* pCLSID = new CLSID;
				ASSERT(pCLSID);
				*pCLSID = pImageCodecInfo[x].Clsid;
				pCombo->SetItemData(iItem, (DWORD_PTR)pCLSID);
				
				// JPEG will be our default image type
				if(strFormat.CompareNoCase(_T("jpeg")) == 0)
				{
					m_clsidImgType = pImageCodecInfo[x].Clsid;
					pCombo->SetCurSel(iItem);
				}
			}
			free(pImageCodecInfo);
		}    
	}

	SetControlValues();

	// Start keyboard hook to capture the print screen key
	ActivateKeyboardHook(TRUE);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

//=================================================
// Message Handlers
//=================================================
void CMainDlg::OnDestroy()
{
	CDialog::OnDestroy();
	GetControlValues();

	// Clean up CLSIDs in combobox
	CComboBox* pCombo = NULL;
	VERIFY(pCombo = (CComboBox*)GetDlgItem(IDC_IMAGE_EXT));
	while(pCombo->GetCount() > 0)
	{
		CLSID* pCLSID = (CLSID*)pCombo->GetItemData(0);
		delete pCLSID;
		pCombo->DeleteString(0);
	}
}

void CMainDlg::OnClose()
{
	// Make sure hooks are cleaned up
	ActivateSelectionHook(0);
	ActivateKeyboardHook(0);

	CDialog::OnClose();
}

void CMainDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// Get state of minimize to tray checkbox
	CButton* pBtn = (CButton*)GetDlgItem(IDC_MIN_TO_TRAY);
	if(!pBtn)
		return;
	if(pBtn->GetCheck())
	{
		if(nType == SIZE_MINIMIZED)
		{
			// Setup tray icon
			memset(&m_NID, 0, sizeof(m_NID));   
			m_NID.cbSize = sizeof(m_NID);
			m_NID.uID = IDR_TRAY_MENU;
			m_NID.hWnd = this->m_hWnd;  
			m_NID.uCallbackMessage = WM_TRAY_NOTIFICATION;
			m_NID.hIcon = m_hIcon;
			strncpy(m_NID.szTip, _T("Screen Tool"), sizeof(m_NID.szTip));   
			m_NID.uFlags = NIF_MESSAGE|NIF_ICON|NIF_TIP;
			if(!Shell_NotifyIcon(NIM_ADD, &m_NID))
			{
				TRACE(_T("Could not create system tray icon\n"));
				return;
			}

			// Hide self
			ShowWindow(SW_HIDE);
		}
		else if(nType == SIZE_RESTORED)
		{
			// Remove tray icon
			m_NID.hIcon = NULL;
			m_NID.uFlags = NIF_ICON;
			::Shell_NotifyIcon(NIM_DELETE, &m_NID);
		}
	}
}

void CMainDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlg;
		dlg.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

LRESULT CMainDlg::OnTrayNotification(WPARAM wParam, LPARAM lParam)
{
	UINT uID = (UINT)wParam; 
    UINT uMsg = (UINT)lParam; 
  
	// Make sure its for us
	if(uID != IDR_TRAY_MENU)
		return 0;

	if(uMsg == WM_LBUTTONDBLCLK)
	{
		// Execute the default command
		ShowWindow(SW_RESTORE);

		return 1;
	}

	if(uMsg == WM_RBUTTONUP)
	{
		// Display the tray menu
		CMenu TrayMenu;
		CMenu* pSubMenu;
		if(!TrayMenu.LoadMenu(IDR_TRAY_MENU))
			return 0;
        if(!(pSubMenu = TrayMenu.GetSubMenu(0))) 
			return 0;

		// Set default menu item
		pSubMenu->SetDefaultItem(ID_SHOW);

		// Display and track the popup menu
        CPoint pos;
		GetCursorPos(&pos);
		SetForegroundWindow();  
		::TrackPopupMenu(pSubMenu->m_hMenu, 0, pos.x, pos.y, 0, m_hWnd, NULL);

		// BUGFIX: See "PRB: Menus for Notification Icons Do Not Work Correctly"
		PostMessage(WM_NULL, 0, 0);

		// Clean up
		TrayMenu.DestroyMenu();

		return 1;
	}

	return 0;
}

void CMainDlg::OnCbnSelchangeImageExt()
{
	CComboBox* pCombo = NULL;
	VERIFY(pCombo = (CComboBox*)GetDlgItem(IDC_IMAGE_EXT));
	int iSel = pCombo->GetCurSel();
	ASSERT(iSel != -1);
	m_clsidImgType = *(CLSID*)pCombo->GetItemData(iSel);
}

void CMainDlg::OnBnClickedScreenDraw()
{
	// Minimize self
	ShowWindow(SW_MINIMIZE);

	// Turn on mouse hook
	ActivateSelectionHook(TRUE);
}

void CMainDlg::OnBnClickedWindowSelect()
{
	m_bSelecting = TRUE;
	m_hHilightWnd = NULL;

	// Minimize self
	ShowWindow(SW_MINIMIZE);

	// Turn on mouse hook
	ActivateSelectionHook(TRUE);
}

void CMainDlg::OnBnClickedBrowseImgPath()
{
	// Set a pointer to the shell's IMalloc interface
	LPMALLOC pMalloc;
    if(!(SUCCEEDED(::SHGetMalloc(&pMalloc))))
		return;
	
	CWnd* pWnd = GetDlgItem(IDC_IMAGE_PATH);
	ASSERT(pWnd);
	CString strOldPath = _T("");
	pWnd->GetWindowText(strOldPath);

	// Fill out a browse info structure
	BROWSEINFO bi;
	memset(&bi, 0, sizeof(bi));
	bi.hwndOwner = m_hWnd;
	bi.ulFlags = BIF_RETURNONLYFSDIRS|BIF_DONTGOBELOWDOMAIN;
	CString strTitle = _T("Select Image Directoy");
	bi.lpszTitle = strTitle;
	bi.lpfn = BrowseCallbackProc;
	bi.lParam = (LPARAM)pWnd;

	// Show the folder selection dialog
	LPITEMIDLIST pidl = ::SHBrowseForFolder(&bi); 
	if(pidl != NULL)
    {
		CString strNewPath = _T("");
		::SHGetPathFromIDList(pidl, strNewPath.GetBuffer(255));
		strNewPath.ReleaseBuffer();
		if(strNewPath.Right(1) != _T("\\"))
			strNewPath += _T("\\");
		if(strOldPath.CompareNoCase(strNewPath) != 0)
			pWnd->SetWindowText(strNewPath);
		m_strImagePath = strNewPath;

		pMalloc->Free(pidl);
    }
	pMalloc->Release();
}

void CMainDlg::OnEnKillfocusLineWidth()
{
	CEdit* pEdit = NULL;
	VERIFY(pEdit = (CEdit*)GetDlgItem(IDC_LINE_WIDTH));
	CString strVal;
	pEdit->GetWindowText(strVal);
	int iVal = _tstoi(strVal);
	if((iVal < 1) || (iVal > 15))
	{
		MessageBox(_T("Line Width must be between 1 and 15"), 
			_T("Screen Tool"), MB_ICONERROR);
		pEdit->SetSel(0, -1);
		pEdit->SetFocus();
	}
	m_iBandPx = iVal;
}

LRESULT CMainDlg::OnDrawComplete(WPARAM wParam, LPARAM lParam)
{
	USES_CONVERSION;

	// Remove mouse hook
	ActivateSelectionHook(FALSE);
	
	// Get screenshot rect
	CRect rScreenshot(m_ptFirst, m_ptLast);
	rScreenshot.NormalizeRect();

	// Force repaint of screenshot area
	/*HWND hDesktop = ::GetDesktopWindow();
	if((hDesktop) && (::IsWindow(hDesktop)))
	{
		::InvalidateRect(hDesktop, NULL, TRUE);
		::UpdateWindow(hDesktop);
		::RedrawWindow(hDesktop, NULL, NULL, RDW_FRAME|
			RDW_INVALIDATE|RDW_UPDATENOW|RDW_ALLCHILDREN);
	}

	// Grab screenshot 
	HBITMAP hScreenshotBmp = GetScreenshot(::GetDesktopWindow(), &rScreenshot);
	*/

	// Crop out selection area from saved ddb of entire desktop
	HBITMAP hScreenshotBmp = CropDDB(m_hTmpDraw, rScreenshot);
	::DeleteObject(m_hTmpDraw);
	m_hTmpDraw = NULL;

	// Check if we were launched from tray menu
	if(!m_bStayInTray) 
	{
		// Uncheck button
		CButton* pBtn = (CButton*)GetDlgItem(IDC_SCREEN_DRAW);
		ASSERT(pBtn);
		pBtn->SetCheck(0);

		// Restore self
		ShowWindow(SW_RESTORE);
	}
	m_bStayInTray = FALSE;

	// Do not continue if aquiring screenshot failed
	if(!hScreenshotBmp)
		return -1;
	
	// Create file name
	CString strFile = GetOutputFileName();

	// Create full file path/name
	CString strFilePath = m_strImagePath;
	strFilePath += strFile;
	
	// Save screenshot as jpeg
	SaveImageAs(hScreenshotBmp, strFilePath);

	// Clean up dib
	::DeleteObject(hScreenshotBmp);

	return 0;
}

LRESULT CMainDlg::OnSelectionComplete(WPARAM wParam, LPARAM lParam)
{
	// Remove mouse hook
	ActivateSelectionHook(FALSE);

	// Grab screenshot 
	HBITMAP hScreenshotBmp = NULL;
	if((m_hHilightWnd) && (::IsWindow(m_hHilightWnd)))
		hScreenshotBmp = GetScreenshot(m_hHilightWnd, NULL);
		
	// Check if we were launched from tray menu
	if(!m_bStayInTray) 
	{
		// Uncheck button
		CButton* pBtn = (CButton*)GetDlgItem(IDC_WIN_SELECT);
		ASSERT(pBtn);
		pBtn->SetCheck(0);

		// Restore self
		ShowWindow(SW_RESTORE);
	}
	m_bStayInTray = FALSE;

	// Do not continue if aquiring screenshot failed
	if(!hScreenshotBmp)
		return -1;
	
	// Create file name
	CString strFile = GetOutputFileName();

	// Create full file path/name
	CString strFilePath = m_strImagePath;
	strFilePath += strFile;

	// Save screenshot as jpeg
	SaveImageAs(hScreenshotBmp, strFilePath);

	// Clean up dib
	::DeleteObject(hScreenshotBmp);

	return 0;
}

LRESULT CMainDlg::OnMouseHookCancel(WPARAM wParam, LPARAM lParam)
{
	// Remove mouse hook
	ActivateSelectionHook(FALSE);

	// Check if we were launched from tray menu
	if(!m_bStayInTray) 
	{
		// Uncheck buttons
		CButton* pBtn = NULL;
		VERIFY(pBtn = (CButton*)GetDlgItem(IDC_SCREEN_DRAW));
		pBtn->SetCheck(0);
		VERIFY(pBtn = (CButton*)GetDlgItem(IDC_WIN_SELECT));
		pBtn->SetCheck(0);

		// Restore self
		ShowWindow(SW_RESTORE);
	}
	m_bStayInTray = FALSE;

	return 0;
}

LRESULT CMainDlg::OnPrintScreen(WPARAM wParam, LPARAM lParam)
{
	BOOL bAltDown = (BOOL)wParam;

	HWND hWnd = NULL;
	if(bAltDown)
		hWnd = ::GetForegroundWindow();
	else
		hWnd = ::GetDesktopWindow();
	
	// Grab screenshot 
	HBITMAP hScreenshotBmp = GetScreenshot(hWnd, NULL);
	ASSERT(hScreenshotBmp);

	// Create file name
	CString strFile = GetOutputFileName();

	// Create full file path/name
	CString strFilePath = m_strImagePath;
	strFilePath += strFile;

	SaveImageAs(hScreenshotBmp, strFilePath);

	// Clean up dib
	::DeleteObject(hScreenshotBmp);
	
	return 0;
}

//=================================================
// Tray Menu Item Handlers
//=================================================
void CMainDlg::OnTrayShow()
{
	// Remove tray icon
	m_NID.hIcon = NULL;
	m_NID.uFlags = NIF_ICON;
	::Shell_NotifyIcon(NIM_DELETE, &m_NID);

	// Restore window 
	ShowWindow(SW_RESTORE);
}

void CMainDlg::OnShutdown()
{
	// Remove tray icon
	m_NID.hIcon = NULL;
	m_NID.uFlags = NIF_ICON;
	::Shell_NotifyIcon(NIM_DELETE, &m_NID);

	// Shutdown app
	PostMessage(WM_CLOSE);
}

void CMainDlg::OnShowAbout()
{
	CAboutDlg dlg;
	dlg.DoModal();
}

void CMainDlg::OnDrawSelection()
{
	m_bStayInTray = TRUE;

	// Turn on mouse hook
	ActivateSelectionHook(TRUE);
}

void CMainDlg::OnWindowSelect()
{
	m_bStayInTray = TRUE;
	m_bSelecting = TRUE;
	m_hHilightWnd = NULL;

	// Turn on mouse hook
	ActivateSelectionHook(TRUE);
}

//=================================================
// Mouse Hook Notifications
//=================================================
void CMainDlg::OnMouseHookLButtonDown(UINT nFlags, CPoint point)
{
	// Check mode flag
	if(m_bSelecting)
		return;

	// Not selecting and hook is active .. must be drawing

	// Store start point
	m_ptFirst = point;
	m_ptLast = point;	

	// Initialize sizing members
	m_bDitherLast = FALSE;

	// Set drawing flag
	m_bDrawing = TRUE;
}

void CMainDlg::OnMouseHookMouseMove(UINT nFlags, CPoint point)
{
	// TODO : figure out while cursor is not changing 
	// Change cursor
	::SetCursor(m_hSelectCursor);

	// Check selecting flag
	if(m_bSelecting)
	{
		// Get window under mouse
		HWND hFound = ::WindowFromPoint(point);
		if(m_hHilightWnd != hFound)
		{
			HWND hOld = m_hHilightWnd;
			m_hHilightWnd = hFound;

			// Undraw old
			if((hOld) && (::IsWindow(hOld)))
				RefreshWindow(hOld);

			// Draw new
			HighlightWindow(m_hHilightWnd);
		}
	}

	// Check drawing flag
	if(m_bDrawing)
	{
		// Draw 
		DrawFocusRect(point);

		// Save current point
		m_ptLast = point;
	}
}

void CMainDlg::OnMouseHookLButtonUp(UINT nFlags, CPoint point)
{
	if(m_bSelecting)
	{
		// Get window under mouse
		HWND hFound = ::WindowFromPoint(point);
		if(m_hHilightWnd != hFound)
		{
			HWND hOld = m_hHilightWnd;
			m_hHilightWnd = hFound;

			// Undraw old
			if((hOld) && (::IsWindow(hOld)))
				RefreshWindow(hOld);
		}
		else
		{
			if((m_hHilightWnd) && (::IsWindow(m_hHilightWnd)))
				RefreshWindow(m_hHilightWnd);
		}

		m_bSelecting = FALSE;

		// We dont want to block the hook so post a message back to ourselves and get out
		PostMessage(WM_WINSELECT_DONE, (WPARAM)0, (LPARAM)0);
	}

	// Check drawing flag
	if(m_bDrawing)
	{
		// Clean up selection box
		DrawFocusRect(point, TRUE);

		m_bDrawing = FALSE;

		// We dont want to block the hook so post a message back to ourselves and get out
		PostMessage(WM_SCREENDRAW_DONE, (WPARAM)0, (LPARAM)0);
	}
}

void CMainDlg::OnMouseHookRButtonDown(UINT nFlags, CPoint point)
{
	// Cancel any drawing or selection operations
	if(m_bSelecting)
	{
		RefreshWindow(m_hHilightWnd);
		m_bSelecting = FALSE;
	}
	if(m_bDrawing)
	{
		DrawFocusRect(point, TRUE);
		m_bDrawing = FALSE;
	}
	PostMessage(WM_MOUSEHOOK_CANCEL, (WPARAM)0, (LPARAM)0);
}

//=================================================
// Helper Functions
//=================================================
CString CMainDlg::GetOutputFileName()
{
	CString strFile = _T("");
	
	GetControlValues();

	switch(m_iNamingOpt) 
	{
		case 0: // Use entered prefix
		{
			strFile = m_strImageName;
			break;
		}
		case 1: // Always ask for prefix
		{
			CGetDataDlg dlg;
			dlg.SetTitle(_T("Enter File Prefix"));
			if(dlg.DoModal() == IDOK)
				strFile = dlg.GetData();
			break;
		}
		case 2: // No prefix
			break;
	}
	if(strFile.GetLength() > 0)
		strFile += _T("_");

	// Add time stamp
	COleDateTime dtNow = COleDateTime::GetCurrentTime();
	CString strDT = dtNow.Format(_T("%d-%m-%y_%H-%M-%S"));
	strFile += strDT;

	// Add file extension
	//strFile += _T(".jpg");
	CComboBox* pCombo = NULL;
	VERIFY(pCombo = (CComboBox*)GetDlgItem(IDC_IMAGE_EXT));
    strFile += _T(".");
	CString strExt = _T("");
	pCombo->GetWindowText(strExt);
	strFile += strExt;

	return strFile;
}

BOOL CMainDlg::ActivateSelectionHook(BOOL bActivate)
{
	// Check if already active
	if(m_bMouseHook == bActivate)
		return TRUE;

	if(bActivate)
	{
		// Get desktop window handle
		HWND hDesktop = ::GetDesktopWindow();
		ASSERT(hDesktop);

		// We just called minimize so refresh desktop
		::InvalidateRect(hDesktop, NULL, TRUE);
		::UpdateWindow(hDesktop);
	
		// Grab screenshot of entire desktop (before ruber band is drawn)
		m_hTmpDraw = GetScreenshot(hDesktop);
		if(!m_hTmpDraw)
			return FALSE;
	
		// Set mouse hook
		m_hMouseHook = ::SetWindowsHookEx(WH_MOUSE_LL, 
			(HOOKPROC)LowLevelMouseProc, GetModuleHandle(NULL), 0);
		if(!m_hMouseHook)
			return FALSE;
		
		// Get desktop DC
		m_pDesktopDC = CDC::FromHandle(::GetDCEx(hDesktop, 0, DCX_WINDOW));
		ASSERT(m_pDesktopDC);
	}
	else
	{
		// Release screen DC
		if(m_pDesktopDC != NULL)
		{
			::ReleaseDC(::GetDesktopWindow(), m_pDesktopDC->GetSafeHdc());
			m_pDesktopDC = NULL;
		}

		// Release hook
		if(m_hMouseHook)
		{
			::UnhookWindowsHookEx(m_hMouseHook);
			m_hMouseHook = NULL;
		}
	}

	m_bMouseHook = bActivate;

	return TRUE;
}

BOOL CMainDlg::ActivateKeyboardHook(BOOL bActivate)
{
	// Check if already active
	if(m_bKeyboardHook == bActivate)
		return TRUE;

	if(bActivate)
	{
		// Set keyboard hook
		m_hKeyboardHook = ::SetWindowsHookEx(WH_KEYBOARD_LL, 
			(HOOKPROC)LowLevelKeyboardProc, GetModuleHandle(NULL), 0);
		if(!m_hKeyboardHook)
			return FALSE;
	}
	else
	{
		// Release hook
		if(m_hKeyboardHook)
		{
			::UnhookWindowsHookEx(m_hKeyboardHook);
			m_hKeyboardHook = NULL;
		}
	}

	m_bKeyboardHook = bActivate;

	return TRUE;
}

void CMainDlg::DrawFocusRect(CPoint pt, BOOL bRemoveRect)
{
	ASSERT(m_pDesktopDC != NULL);

	// Get frame sizes
	CSize szFrame(m_iBandPx, m_iBandPx);
	CSize szLast = szFrame;
	if(bRemoveRect)
		szFrame.cx = szFrame.cy = 0;

	// Create brushes
	//CBrush* pWhiteBrush = CBrush::FromHandle
	//	((HBRUSH)::GetStockObject(WHITE_BRUSH));
	CBrush* pWhiteBrush = CBrush::FromHandle
		((HBRUSH)::CreateSolidBrush(RGB(255,255,255)));
	CBrush* pDitherBrush = CDC::GetHalftoneBrush();
	CBrush* pBrush = pWhiteBrush;

	// Get rect coorinates of last and current box
	CRect rNow(m_ptFirst, pt);
	rNow.NormalizeRect();
	CRect rLast(m_ptFirst, m_ptLast);
	rLast.NormalizeRect();

	// Draw the drag frame
	m_pDesktopDC->DrawDragRect(&rNow, szFrame, &rLast, szLast,
		pBrush, m_bDitherLast ? pDitherBrush : pWhiteBrush);

	// Save for next frame
	m_bDitherLast = (pBrush == pDitherBrush);
}

HBITMAP CMainDlg::GetScreenshot(HWND hWnd, CRect* pRect/*=NULL*/)
{
	// Get DC
	HDC hDC = ::GetWindowDC(hWnd);
	HDC hCaptureDC = ::CreateCompatibleDC(hDC);
	int nBPP = ::GetDeviceCaps(hDC, BITSPIXEL);

	// If no rect was supplied grab entire window
	CRect rCapture;
	if(pRect)
		rCapture.CopyRect(pRect);
	else
		::GetWindowRect(hWnd, rCapture);

	// Create device independent bitmap
	BITMAPINFO bmiCapture = {
		sizeof(BITMAPINFOHEADER), 
		rCapture.Width(), -rCapture.Height(), 
		1, nBPP, BI_RGB, 0, 0, 0, 0, 0,
	};
	LPBYTE lpCapture;
	HBITMAP hbmCapture = CreateDIBSection(hDC, &bmiCapture,
		DIB_PAL_COLORS, (LPVOID*)&lpCapture, NULL, 0);
	if(!hbmCapture)
	{
		::DeleteDC(hCaptureDC);
		::DeleteDC(hDC);
		return NULL;
	}
	
	// Grab the image
	int nCapture = ::SaveDC(hCaptureDC);
	::SelectObject(hCaptureDC, hbmCapture);
	if(pRect)
		::BitBlt(hCaptureDC, 0, 0, rCapture.Width(), rCapture.Height(), hDC, rCapture.left, rCapture.top, SRCCOPY);
	else
		::BitBlt(hCaptureDC, 0, 0, rCapture.Width(), rCapture.Height(), hDC, 0, 0, SRCCOPY);
	::RestoreDC(hCaptureDC, nCapture);
	::DeleteDC(hCaptureDC);
	::DeleteDC(hDC);

	// Return the bitmap handle
	return hbmCapture;
}

HBITMAP CMainDlg::CropDDB(HBITMAP hSrcBmp, CRect rCrop)
{
	// Load source image into DC
	HDC hDesktopDC = ::GetWindowDC(::GetDesktopWindow());
	HDC hSrcDC = ::CreateCompatibleDC(hDesktopDC);
	if(!hSrcDC)
		return NULL;
	::SelectObject(hSrcDC, hSrcBmp);

	// Create DC for target bmp
	HDC hCropDC = ::CreateCompatibleDC(hDesktopDC);
	if(!hCropDC)
	{
		::DeleteDC(hSrcDC);
		return NULL;
	}

	// Create device independent bitmap
	int nBPP = ::GetDeviceCaps(hSrcDC, BITSPIXEL);
	BITMAPINFO bmiCrop = {
		sizeof(BITMAPINFOHEADER), 
		rCrop.Width(), -rCrop.Height(), 
		1, nBPP, BI_RGB, 0, 0, 0, 0, 0,
	};
	LPBYTE lpCrop;
	HBITMAP hbmCrop = CreateDIBSection(hCropDC, &bmiCrop,
		DIB_PAL_COLORS, (LPVOID*)&lpCrop, NULL, 0);
	if(!hbmCrop)
	{
		::DeleteDC(hSrcDC);
		::DeleteDC(hCropDC);
		return NULL;
	}

	// Crop the image
	::SelectObject(hCropDC, hbmCrop);
	::BitBlt(hCropDC, 0, 0, rCrop.Width(), rCrop.Height(), hSrcDC, rCrop.left, rCrop.top, SRCCOPY);
	::DeleteDC(hSrcDC);
	::DeleteDC(hCropDC);

	// Return the bitmap 
	return hbmCrop;
}

int CMainDlg::GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
   UINT  num = 0;          // number of image encoders
   UINT  size = 0;         // size of the image encoder array in bytes

   ImageCodecInfo* pImageCodecInfo = NULL;

   GetImageEncodersSize(&num, &size);
   if(size == 0)
      return -1;  // Failure

   pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
   if(pImageCodecInfo == NULL)
      return -1;  // Failure

   GetImageEncoders(num, size, pImageCodecInfo);

   for(UINT j = 0; j < num; ++j)
   {
      if( wcscmp(pImageCodecInfo[j].MimeType, format) == 0 )
      {
         *pClsid = pImageCodecInfo[j].Clsid;
         free(pImageCodecInfo);
         return j;  // Success
      }    
   }

   free(pImageCodecInfo);
   return -1;  // Failure
}

BOOL CMainDlg::SaveImageAs(HBITMAP hBmp, CString strFile)
{
	USES_CONVERSION;

	// Create GDI+ image 
	Bitmap* pScreenShot = new Bitmap(hBmp, (HPALETTE)NULL);

	// Save the image
	BOOL bRes = (pScreenShot->Save(T2W(strFile), &m_clsidImgType, NULL) == Ok);

	// Clean up GDI+ image
	delete pScreenShot;

	return bRes;
}
/*
BOOL CMainDlg::SaveImageAsJpeg(HBITMAP hBmp, CString strFile)
{
	USES_CONVERSION;

	// Create GDI+ image 
	Bitmap* pScreenShot = new Bitmap(hBmp, (HPALETTE)NULL);

	// Setup GDI+ image encoder
	CLSID imageCLSID;
	ULONG uQuality = 99;
	EncoderParameters ep;
	ep.Count = 1;
	ep.Parameter[0].NumberOfValues = 1;
	ep.Parameter[0].Guid = EncoderQuality;
	ep.Parameter[0].Type  = EncoderParameterValueTypeLong;
	ep.Parameter[0].Value = &uQuality;
	GetEncoderClsid(L"image/jpeg", &imageCLSID);

	// Save the image
	BOOL bRes = (pScreenShot->Save(T2W(strFile), &imageCLSID, &ep) == Ok);

	// Clean up GDI+ image
	delete pScreenShot;

	return bRes;
}
*/
void CMainDlg::UpdateToolTip(CString strToolTip)
{
	m_NID.uFlags = NIF_TIP;
	_tcscpy(m_NID.szTip, strToolTip);
	::Shell_NotifyIcon(NIM_MODIFY, &m_NID);
}

void CMainDlg::HighlightWindow(HWND hWnd)
{
	// Get coordinates
	RECT rect;   
	::GetWindowRect(hWnd, &rect);

	// Setup to draw
	HDC hDC = ::GetWindowDC(hWnd);
	if(!hDC)
		return;
	HPEN hPen = ::CreatePen(PS_SOLID, m_iBandPx, RGB(255, 0, 0));
	HGDIOBJ hPrevPen = ::SelectObject(hDC, hPen);
	HGDIOBJ hPrevBrush = ::SelectObject(hDC, ::GetStockObject(HOLLOW_BRUSH));

	// Draw highlight
	::Rectangle(hDC, 0, 0, (rect.right - rect.left), (rect.bottom - rect.top));

	// Clean up
	::SelectObject(hDC, hPrevPen);
	::SelectObject(hDC, hPrevBrush);
	::DeleteObject(hPen);
	::ReleaseDC(hWnd, hDC);
}

void CMainDlg::RefreshWindow(HWND hWnd)
{
	::InvalidateRect(hWnd, NULL, TRUE);
	::UpdateWindow(hWnd);
	::RedrawWindow(hWnd, NULL, NULL, RDW_FRAME|
		RDW_INVALIDATE|RDW_UPDATENOW|RDW_ALLCHILDREN);
}

//=================================================
// Hook Callback
//=================================================
LRESULT CALLBACK CMainDlg::LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if(nCode < 0) 
		return ::CallNextHookEx(m_hMouseHook, nCode, wParam, lParam);

	MSLLHOOKSTRUCT* pMS = (MSLLHOOKSTRUCT*)lParam;
	ASSERT(pMS);
	CPoint point(pMS->pt);

	switch(wParam)
	{
		case WM_MOUSEMOVE:
			m_pThis->OnMouseHookMouseMove(0, point);
			break;
		case WM_LBUTTONDOWN:
			m_pThis->OnMouseHookLButtonDown(0, point);
			break;
		case WM_LBUTTONUP:
			m_pThis->OnMouseHookLButtonUp(0, point);
			break;
		case WM_RBUTTONDOWN:
			m_pThis->OnMouseHookRButtonDown(0, point);
			break;
	}

	return ::CallNextHookEx(m_hMouseHook, nCode, wParam, lParam);
}

LRESULT CALLBACK CMainDlg::LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if(nCode < 0) 
		return ::CallNextHookEx(m_hKeyboardHook, nCode, wParam, lParam);

	KBDLLHOOKSTRUCT* pKS = (KBDLLHOOKSTRUCT*)lParam;
	ASSERT(pKS);

	TRACE2("vkCode=%0X\nflags=%0X\n\n", pKS->vkCode, pKS->flags);

	if((wParam == WM_KEYDOWN) || (wParam == WM_SYSKEYDOWN))
	{
		if(pKS->vkCode == VK_SNAPSHOT)
		{
			if(pKS->flags & LLKHF_ALTDOWN)
			{
				m_pThis->PostMessage(WM_PRINTSCREEN, 1, 0);
			}
			else
			{
				m_pThis->PostMessage(WM_PRINTSCREEN, 0, 0);
			}
			return 1;
		}
	}

	return ::CallNextHookEx(m_hKeyboardHook, nCode, wParam, lParam);
}

//=================================================
// Callback for shell directory browser
//=================================================
static int __stdcall BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
	TCHAR strInitDir[MAX_PATH] = {NULL};
    if(uMsg == BFFM_INITIALIZED)
    {
		CString strInitDir = _T("");
		CWnd* pWnd = (CWnd*)lpData;
		pWnd->GetWindowText(strInitDir);
		::SendMessage(hwnd, BFFM_SETSELECTION, TRUE, 
			(LPARAM)strInitDir.GetBuffer(255));
		strInitDir.ReleaseBuffer();
    }
	return 0;
}

//=================================================
// Persistence
//=================================================
void CMainDlg::ReadRegistrySettings()
{
	CRegistry reg(HKEY_CURRENT_USER, _T("Software\\TOOLS\\SCREENTOOL"));
	m_bMinToTray = reg.GetRegInt(_T("MinToTray"), m_bMinToTray);
	m_iBandPx = reg.GetRegInt(_T("LineWidth"), m_iBandPx);
	reg.GetRegStr(_T("ImagePath"), m_strImagePath.GetBuffer(255), m_strImagePath);
	m_strImagePath.ReleaseBuffer();
	reg.GetRegStr(_T("ImageName"), m_strImageName.GetBuffer(255), m_strImageName);
	m_strImageName.ReleaseBuffer();
	m_iNamingOpt = reg.GetRegInt(_T("NamingOption"), m_iNamingOpt);
}

void CMainDlg::WriteRegistrySettings()
{
	CRegistry reg(HKEY_CURRENT_USER, _T("Software\\TOOLS\\SCREENTOOL"));
	reg.SetRegInt(_T("MinToTray"), m_bMinToTray);
	reg.SetRegInt(_T("LineWidth"), m_iBandPx);
	reg.SetRegStr(_T("ImagePath"), (LPTSTR)(LPCTSTR)m_strImagePath);
	reg.SetRegStr(_T("ImageName"), (LPTSTR)(LPCTSTR)m_strImageName);
	reg.SetRegInt(_T("NamingOption"), m_iNamingOpt);
}

void CMainDlg::GetControlValues()
{	
	CWnd* pWnd = NULL;
	VERIFY(pWnd = (CEdit*)GetDlgItem(IDC_LINE_WIDTH));
	CString strTemp;
	pWnd->GetWindowText(strTemp);
	m_iBandPx = _tstoi(strTemp);
	VERIFY(pWnd = (CEdit*)GetDlgItem(IDC_IMAGE_PATH));
	pWnd->GetWindowText(m_strImagePath);
	if(m_strImagePath.Right(1) != _T('\\'))
		m_strImagePath += _T('\\');
	VERIFY(pWnd = GetDlgItem(IDC_IMAGE_NAME));
	pWnd->GetWindowText(m_strImageName);

	CButton* pBtn = (CButton*)GetDlgItem(IDC_MIN_TO_TRAY);
	ASSERT(pBtn);
	m_bMinToTray = pBtn->GetCheck();

	if(((CButton*)GetDlgItem(IDC_OPT_IMGNAME_1))->GetCheck() == 1)
		m_iNamingOpt = 0;
	else if(((CButton*)GetDlgItem(IDC_OPT_IMGNAME_2))->GetCheck() == 1)
		m_iNamingOpt = 1;
	else
		m_iNamingOpt = 2;
}

void CMainDlg::SetControlValues()
{
	CEdit* pEdit = NULL;
	VERIFY(pEdit = (CEdit*)GetDlgItem(IDC_LINE_WIDTH));
	CString strVal;
	strVal.Format(_T("%d"), m_iBandPx);
	pEdit->SetWindowText(strVal);
	VERIFY(pEdit = (CEdit*)GetDlgItem(IDC_IMAGE_PATH));
	pEdit->SetWindowText(m_strImagePath);
	VERIFY(pEdit = (CEdit*)GetDlgItem(IDC_IMAGE_NAME));
	pEdit->SetWindowText(m_strImageName);
	CButton* pBtn = NULL;
	VERIFY(pBtn = (CButton*)GetDlgItem(IDC_MIN_TO_TRAY));
	pBtn->SetCheck(m_bMinToTray);
	switch(m_iNamingOpt) {
		case 0: ((CButton*)GetDlgItem(IDC_OPT_IMGNAME_1))->SetCheck(1);  break;
		case 1: ((CButton*)GetDlgItem(IDC_OPT_IMGNAME_2))->SetCheck(1);  break;
		case 2: ((CButton*)GetDlgItem(IDC_OPT_IMGNAME_3))->SetCheck(1);  break;
	}
}






