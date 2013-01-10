#include "stdafx.h"
#include "Dlg.h"
CAppModule _Module;

int WINAPI WinMain( IN HINSTANCE hInstance, IN HINSTANCE hPrevInstance, IN LPSTR lpCmdLine, IN int nShowCmd )
{
	_Module.Init(NULL,hInstance);

	MainDlg dlg;
	dlg.DoModal();

	_Module.Term();
	return 0;
}