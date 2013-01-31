// GDIAPPDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GDIAPP.h"
#include "GDIAPPDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGDIAPPDlg dialog

CGDIAPPDlg::CGDIAPPDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGDIAPPDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGDIAPPDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bBack=false;
	m_kind=cx=0;
 GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
}

void CGDIAPPDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGDIAPPDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CGDIAPPDlg, CDialog)
	//{{AFX_MSG_MAP(CGDIAPPDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_MENU_ABOUT, OnMenuAbout)
	ON_COMMAND(ID_MENU_Exit, OnMENUExit)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGDIAPPDlg message handlers

BOOL CGDIAPPDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
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

	UpdateDisplay();
	SetTimer(1,50,NULL);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CGDIAPPDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CGDIAPPDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CGDIAPPDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

int CGDIAPPDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
    hFuncInst = LoadLibrary("User32.DLL"); 
	BOOL bRet=FALSE;
	if(hFuncInst) 
		UpdateLayeredWindow=(MYFUNC)GetProcAddress(hFuncInst, "UpdateLayeredWindow");
	else
	{
		AfxMessageBox("User32.dll ERROR!");
		exit(0);
	}

	// Initialize GDI+.
	m_Blend.BlendOp=0; //theonlyBlendOpdefinedinWindows2000
	m_Blend.BlendFlags=0; //nothingelseisspecial...
	m_Blend.AlphaFormat=1; //...
	m_Blend.SourceConstantAlpha=255;//AC_SRC_ALPHA

	// TODO: Add your specialized creation code here
	
	return 0;
}

BOOL CGDIAPPDlg::UpdateDisplay(int Transparent)
{
	HDC hdcTemp=GetDC()->m_hDC;
	m_hdcMemory=CreateCompatibleDC(hdcTemp);
	HBITMAP hBitMap=CreateCompatibleBitmap(hdcTemp,755,350);
	SelectObject(m_hdcMemory,hBitMap);
	if(Transparent<0||Transparent>100)	Transparent=100;

	m_Blend.SourceConstantAlpha=int(Transparent*0.5);//1~255
	HDC hdcScreen=::GetDC (m_hWnd);
	RECT rct;
	GetWindowRect(&rct);
	POINT ptWinPos={rct.left,rct.top};

	Graphics graph(m_hdcMemory);

	Graphics graphics(m_hdcMemory);

	graphics.SetSmoothingMode(SmoothingModeAntiAlias);
	graphics.SetInterpolationMode(InterpolationModeHighQualityBicubic);

	FontFamily fontFamily(L"Arial Black");
	StringFormat strformat;
	wchar_t pszbuf[][80]={{L"Hello everyone!"},
	{L"Happy New Year!"},
	{L"I wish you will lead a  happy life!"},
	{L"Love you!"},
	{L"Thanks all the people on CP!"}
	};

	GraphicsPath path;
	path.AddString(pszbuf[m_kind],wcslen(pszbuf[m_kind]),&fontFamily,
		FontStyleRegular,38,Point(10,10),&strformat);

	Pen pen(Color(155,215,215,215),3);

	graphics.DrawPath(&pen,&path);
	

	
	LinearGradientBrush linGrBrush(
		Point(0,0),Point(0,90),
		Color(255,255,255,255),
		Color(255,30,120,195));

	LinearGradientBrush linGrBrushW(
		Point(0,10),Point(0,60),
		Color(255,255,255,255),
		Color(15,1,1,1));
/**/
   for(int i=1; i<9; i+=1)
    {
        Pen pen(Color(62, 0, 2, 2), i);
        pen.SetLineJoin(LineJoinRound);
        graphics.DrawPath(&pen, &path);
    } 
   
   SolidBrush brush(Color(25,228,228,228));
   Pen pen1(Color(155,223,223,223));
   Pen pen2(Color(55,223,223,223));	 
   Image image(L"ly.png");
   if(m_bBack)
   {
	   graphics.FillRectangle(&brush,3,5,750,90);
	   graphics.DrawRectangle(&pen1,2,6,751,91);
	   graphics.DrawRectangle(&pen2,1,5,753,93);

	   graphics.DrawImage(&image,600,5);
   }

	graphics.FillPath(&linGrBrush,&path);
	graphics.FillPath(&linGrBrushW,&path);
	

	SIZE sizeWindow={755,350};
	POINT ptSrc={0,0};
	DWORD dwExStyle=GetWindowLong(m_hWnd,GWL_EXSTYLE);
	if((dwExStyle&0x80000)!=0x80000)
		SetWindowLong(m_hWnd,GWL_EXSTYLE,dwExStyle^0x80000);

	BOOL bRet=FALSE;
	bRet= UpdateLayeredWindow( m_hWnd,hdcScreen,&ptWinPos,
				&sizeWindow,m_hdcMemory,&ptSrc,0,&m_Blend,2);
	graph.ReleaseHDC(m_hdcMemory);
	::ReleaseDC(m_hWnd,hdcScreen);
	hdcScreen=NULL;
	::ReleaseDC(m_hWnd,hdcTemp);
	hdcTemp=NULL;
	DeleteObject(hBitMap);
	DeleteDC(m_hdcMemory);
	m_hdcMemory=NULL;
	return bRet;
}

void CGDIAPPDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
//	UpdateDisplay();
	cx+=1;
	if(cx>20) { 

		m_kind++;
			m_bBack=false;UpdateDisplay();cx=0;  }
    		if(m_kind>3)
			m_kind=0;

	CDialog::OnTimer(nIDEvent);
}

void CGDIAPPDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	SendMessage(WM_SYSCOMMAND,0xF012,0);  
	m_bBack=false;
	CDialog::OnLButtonDown(nFlags, point);
}

void CGDIAPPDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	m_bBack=true;
	cx=0;
	UpdateDisplay();
	CDialog::OnMouseMove(nFlags, point);
}

void CGDIAPPDlg::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	// TODO: Add your message handler code here
	if (point.x == -1 && point.y == -1)
	{
		CRect rect;
		GetClientRect(rect);
		ClientToScreen(rect);

		point = rect.TopLeft();
		point.Offset(5, 5);
	}

	CMenu menu;
	VERIFY(menu.LoadMenu(IDR_MENU1));

	CMenu* pPopup = menu.GetSubMenu(0);
	ASSERT(pPopup != NULL);
	CWnd* pWndPopupOwner = this;

	while (pWndPopupOwner->GetStyle() & WS_CHILD)
		pWndPopupOwner = pWndPopupOwner->GetParent();

	pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y,
		pWndPopupOwner);
}

void CGDIAPPDlg::OnMenuAbout() 
{
	// TODO: Add your command handler code here
	CAboutDlg dlg;
	dlg.DoModal ();
}

void CGDIAPPDlg::OnMENUExit() 
{
	// TODO: Add your command handler code here
	PostQuitMessage(0);
}

void CGDIAPPDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	GdiplusShutdown(gdiplusToken);
	// TODO: Add your message handler code here
	
}
