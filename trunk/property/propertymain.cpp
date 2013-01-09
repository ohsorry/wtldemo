#include "stdafx.h"
#include "MyDialog.h"
CAppModule _Module;

int WINAPI WinMain(HINSTANCE hins,HINSTANCE hinsPre, LPSTR lpCL,int nShow)  
{
	_Module.Init(NULL,hins);
	MyDialog d;
	d.DoModal();
	//while(GetMessage(&msg,NULL,0,0))
	//{
	//	TranslateMessage(&msg);
	//	DispatchMessage(&msg);
	//}
	//return msg.wParam;


	_Module.Term();
}