#include "MainFrame.h"

CAppModule _Module;

int Run()
{
	CMessageLoop loop;
	_Module.AddMessageLoop(&loop);
	Mainframe frame;
	frame.CreateEx(NULL,CWindow::rcDefault);
	frame.ShowWindow(TRUE);
	frame.UpdateWindow();
	int ret = loop.Run();	
	return ret;
	
}

int WINAPI WinMain( IN HINSTANCE hInstance, IN HINSTANCE hPrevInstance, IN LPSTR lpCmdLine, IN int nShowCmd )
{
	_Module.Init(0,hInstance);	
	int n = Run();
	_Module.Term();
	return n;
}