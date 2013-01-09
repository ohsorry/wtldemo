//
//	Simple test application to demonstrate a Custom Control
//
//	Written by J Brown 8/11/2001.
//	Freeware.
//
#include <windows.h>
#include <tchar.h>
#include "CustCtrl.h"
#include "resource.h"

BOOL CALLBACK DlgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
	case WM_INITDIALOG:
		SetDlgItemText(hwnd, IDC_CUSTOM1, _T("A custom control") );
		return TRUE;

	case WM_CLOSE:
		EndDialog(hwnd, 0);
		return TRUE;

	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDCANCEL: case IDOK:
			EndDialog(hwnd, 0);
			return TRUE;
		}

		return FALSE;
	}

	return FALSE;
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR lpCmdLine, int nShowCmd)
{
	InitCustomControl();

	DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), 0, DlgProc);
	return 0;
}