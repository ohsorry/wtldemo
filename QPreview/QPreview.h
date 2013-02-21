/************************************************************************
模快名:		仿360安全卫士界面
功能:		尚未对模块功能进行描述
完成日期:	2010-10-20
作者:		杨伟峰 Yang Weifeng(Email: 11259500@qq.com)

本代码可以自由使用，但因使用本代码造成的后果，本人不承担任何责任
************************************************************************/

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号
#include "SingleInst.h"

// CQPreview:
// 有关此类的实现，请参阅 QPreview.cpp
class CQPreview : public CWinApp
{
public:
	CQPreview();

	DECLARE_MESSAGE_MAP()

private:
	virtual BOOL InitInstance();
	virtual int  ExitInstance();

	ULONG_PTR	m_GdiplusToken;
	CSingleInst	m_siSingleInst;
};

extern CQPreview theApp;