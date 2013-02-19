#include "MainFrame.h"

CAppModule _Module;


int run()
{
	CMessageLoop loop;
	_Module.AddMessageLoop(&loop);

	MainFrame wnd;
	wnd.CreateEx();

	wnd.ShowWindow(TRUE);
	wnd.UpdateWindow();

	return loop.Run();
}

int WINAPI WinMain( IN HINSTANCE hInstance, IN HINSTANCE hPrevInstance, IN LPSTR lpCmdLine, IN int nShowCmd )
{
	_Module.Init(0,hInstance);
	int ret = run();
	_Module.Term();

	return ret;
}