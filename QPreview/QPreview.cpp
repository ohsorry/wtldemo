/************************************************************************
模快名:		仿360安全卫士界面
功能:		尚未对模块功能进行描述
完成日期:	2010-10-20
作者:		杨伟峰 Yang Weifeng(Email: 11259500@qq.com)

本代码可以自由使用，但因使用本代码造成的后果，本人不承担任何责任
************************************************************************/

#include "stdafx.h"
#include "QPreview.h"

#include "WndPreview.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CQPreview

BEGIN_MESSAGE_MAP(CQPreview, CWinApp)
END_MESSAGE_MAP()


// 唯一的一个 CQPreview 对象

CQPreview theApp;

// CQPreview 构造

CQPreview::CQPreview()
// 标使程序单例的GUID
:m_siSingleInst(_T("{B758ACA3-0D85-4518-B852-722B9460F6F2}"))
{
}

// CQPreview 初始化

BOOL CQPreview::InitInstance()
{
	// 判断进程中是否已经运行有本程序的实例
	BOOL bRunning = !m_siSingleInst.AppStart();
	if(bRunning)
	{
		// 如果已经存在其它已经运行实例，激活该实例
		HWND hMainWnd;
		m_siSingleInst.WaitForInitCompleted(&hMainWnd, sizeof(HWND));

		SendMessage(hMainWnd, WM_SYSCOMMAND,SC_RESTORE ,NULL);
		SetForegroundWindow(hMainWnd);

		return FALSE;
	}
	else
	{
		CWinApp::InitInstance();

		// 初始化GDI+运行环境
		GdiplusStartupInput gdiplusStartupInput;
		GdiplusStartup(&m_GdiplusToken, &gdiplusStartupInput, NULL);

		CWnd * pMainWnd = new CWndPreview();

		// 创建主窗口，程序退出时CDlgMainFrame会自动释放
		LPCTSTR lpszClassName=AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW,
			::LoadCursor(NULL,MAKEINTRESOURCE(IDC_ARROW)));
		// WS_EX_APPWINDOW可以让子窗口显示在任务栏中
		m_pMainWnd = pMainWnd;
		BOOL bResult = pMainWnd->CreateEx(WS_EX_APPWINDOW, lpszClassName, NULL, 
			WS_POPUP|WS_VISIBLE, 0, 0, 0, 0, NULL, 0);   
		if(!bResult)
		{
			AfxMessageBox(_T("窗口初始化失败！"));
			return FALSE;
		}
		pMainWnd->UpdateWindow();
		pMainWnd->ShowWindow(SW_SHOW);
		// 保存当前进程主窗口，以便进程实例进行激活
		HWND hMainWnd = m_pMainWnd->GetSafeHwnd();
		m_siSingleInst.InitCompleted(&hMainWnd, sizeof(hMainWnd));

		return TRUE;
	}
}

int  CQPreview::ExitInstance()
{
	// 释放界面资源
	CImageInfo::Instance()->Release();

	// 清理 gdi+ 环境
	GdiplusShutdown(m_GdiplusToken);

	// 清理程序单例资源
	m_siSingleInst.AppEnd();

	return CWinApp::ExitInstance();
}
