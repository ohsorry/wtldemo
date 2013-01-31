// ==========================================================================
// SCREENTOOL.h : Main Application Header File
//
// Known Issues:  The application sporadically crashes at startup when 
//                compiled in release mode.  For now the workaround is 
//                only distribute debug builds.
//
// TODO:          Populate About dialog
// 
// Ideas:         Maybe do owner-draw tray menu
// ==========================================================================
#pragma once
#ifndef __AFXWIN_H__
#error include 'stdafx.h' before including this file for PCH
#endif
#include "resource.h"	


class CApp : public CWinApp
{
public:
	CApp();

protected:
	// GDI+
	GDIPSI		m_GDIPSI;
	ULONG_PTR	m_GDIPlusToken;

public:
	// Overrides
	virtual BOOL InitInstance();

	// Messages
	DECLARE_MESSAGE_MAP()
};

extern CApp theApp;