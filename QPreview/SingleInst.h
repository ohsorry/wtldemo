/************************************************************************
模快名:		仿360安全卫士界面
功能:		尚未对模块功能进行描述
完成日期:	2010-10-20
作者:		杨伟峰 Yang Weifeng(Email: 11259500@qq.com)

本代码可以自由使用，但因使用本代码造成的后果，本人不承担任何责任
************************************************************************/

#pragma once

class CSingleInst
{
public:
	CSingleInst(LPCTSTR lpszGuid);
	~CSingleInst(void);

	BOOL	AppStart();	//	return IsFirstInstance
	void	AppEnd();

	void	InitCompleted(HWND hMainWnd);
	BOOL	WaitForInitCompleted(HWND &hMainWnd, DWORD dwMillisecond = INFINITE);

	void	InitCompleted(LPVOID pData = NULL, UINT uSize = 0);
	BOOL	WaitForInitCompleted(LPVOID pData = NULL, UINT uSize = 0, DWORD dwMillisecond = INFINITE);

protected:
	CString	m_strGuid;
	CString	m_strGuidMapFile;
	
	HANDLE	m_hInstMutex;
	HANDLE	m_hMapFile;
};

//CSingleInst singleInst("Test1");
//
//BOOL CMainClientProcess::InitInstance()
//{
//	if(!CProcessBase::InitInstance())
//	{
//		return FALSE;
//	}
//
//	BOOL bRunning = !singleInst.AppStart();
//	if(bRunning)
//	{
//		HWND hWnd;
//		singleInst.WaitForInitCompleted(&hWnd, sizeof(HWND));
//
//		ShowWindow(hWnd, SW_SHOW);
//		SendMessage(hWnd, WM_SYSCOMMAND, SC_RESTORE, NULL);
//		return FALSE;
//	}
//	else
//	{
//		this->m_pMainWnd = CWndInterface::CreateMainFrame();
//		this->m_pMainWnd->ShowWindow(SW_SHOW);
//		this->m_pMainWnd->UpdateWindow();
//
//		theMainWnd = this->m_pMainWnd->GetSafeHwnd();
//
//		singleInst.InitCompleted(&theMainWnd, sizeof(theMainWnd));
//	}
//
//	return TRUE;
//}
//
//int CMainClientProcess::ExitInstance()
//{
//	singleInst.AppEnd();
//	return CProcessBase::ExitInstance();
//}

// AppStart,InitComplete, WaitForInitCompleted必须在同一线程中使用
