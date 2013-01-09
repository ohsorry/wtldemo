#include "MainFrame.h"
CAppModule _Module;

int WINAPI WinMain(HINSTANCE hins,HINSTANCE hinsPre, LPSTR lpCL,int nShow)  
{
	_Module.Init(NULL,hins);
	MainFrame mf;
	mf.CreateEx();
	mf.ShowWindow(SW_SHOW);
	mf.UpdateWindow();
	MSG msg;

	while(GetMessage(&msg,NULL,0,0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
	

	_Module.Term();
}