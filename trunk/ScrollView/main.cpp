#include "CusWin.h"
#pragma comment(lib, "GdiPlus.lib")
CAppModule _Module;
#include "GdiPlusHelper.h"
int WINAPI WinMain( IN HINSTANCE hInstance, IN HINSTANCE hPrevInstance, IN LPSTR lpCmdLine, IN int nShowCmd )
{
	MSG msg;	
	GdiPlusHelper::GdiPlusAutoInit autoInit;

	_Module.Init(NULL,hInstance);
	
	CusWin win;
	win.Create(NULL,CWindow::rcDefault,"CusWin",WS_OVERLAPPEDWINDOW);

	win.ShowWindow(TRUE);
	win.UpdateWindow();

	while(GetMessage(&msg,0,0,0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	_Module.Term();
}