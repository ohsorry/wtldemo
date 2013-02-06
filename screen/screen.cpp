#include "MainForm.h"
#pragma comment( lib, "gdiplus.lib" )
#include "gdiplus.h"
CAppModule _Module;

int WINAPI WinMain( IN HINSTANCE hInstance, IN HINSTANCE hPrevInstance, IN LPSTR lpCmdLine, IN int nShowCmd )
{
	_Module.Init(NULL,hInstance);

	ULONG_PTR gdiplusToken;
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	MainForm mywnd;
	mywnd.Create(NULL,CWindow::rcDefault,"MyWidnow");
	mywnd.ShowWindow(TRUE);
	mywnd.UpdateWindow();

	MSG uMsg;
	while(GetMessage(&uMsg,0,0,0))
	{
		TranslateMessage(&uMsg);
		DispatchMessage(&uMsg);
	}
	Gdiplus::GdiplusShutdown(gdiplusToken);
	_Module.Term();
	return 0;
}