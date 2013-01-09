#include "stdafx.h"
#include "Resource.h"
#include "MyTabCtrl.h"
#include "DlgBasic.h"
#include "DlgAdvanced.h"
#include "DlgTransparent.h"
#include "DlgShadeButtonST.h"
#include "DlgAbout.h"

CMyTabCtrl::CMyTabCtrl()
{
	::ZeroMemory(&m_tabPages, sizeof(m_tabPages));
	m_tabPages[0]=new CDlgBasic;
	m_tabPages[1]=new CDlgAdvanced;
	m_tabPages[2]=new CDlgTransparent;
	m_tabPages[3]=new CDlgShadeButtonST;
	m_tabPages[4]=new CDlgAbout;

	m_nNumberOfPages=5;
}

CMyTabCtrl::~CMyTabCtrl()
{
	delete ((CDlgBasic *)m_tabPages[0]);
	delete ((CDlgAdvanced *)m_tabPages[1]);
	delete ((CDlgTransparent *)m_tabPages[2]);
	delete ((CDlgShadeButtonST *)m_tabPages[3]);
	delete ((CDlgAbout *)m_tabPages[4]);
}

void CMyTabCtrl::Init()
{
	m_tabCurrent=0;

	((CDlgBasic *)m_tabPages[0])->Create(*this);
	((CDlgAdvanced *)m_tabPages[1])->Create(*this);
	((CDlgTransparent *)m_tabPages[2])->Create(*this);
	((CDlgShadeButtonST *)m_tabPages[3])->Create(*this);
	((CDlgAbout *)m_tabPages[4])->Create(*this);

	m_tabPages[0]->ShowWindow(SW_SHOW);
	m_tabPages[1]->ShowWindow(SW_HIDE);
	m_tabPages[2]->ShowWindow(SW_HIDE);
	m_tabPages[3]->ShowWindow(SW_HIDE);
	m_tabPages[4]->ShowWindow(SW_HIDE);

	SetRectangle();
}

void CMyTabCtrl::SetRectangle()
{
	CRect tabRect, itemRect;
	int nX, nY, nXc, nYc;

	GetClientRect(&tabRect);
	GetItemRect(0, &itemRect);

	nX=itemRect.left;
	nY=itemRect.bottom+1;
	nXc=tabRect.right-itemRect.left-1;
	nYc=tabRect.bottom-nY-1;

	m_tabPages[0]->SetWindowPos(HWND_TOP, nX, nY, nXc, nYc, SWP_SHOWWINDOW);
	for(int nCount=1; nCount < m_nNumberOfPages; nCount++){
		m_tabPages[nCount]->SetWindowPos(HWND_TOP, nX, nY, nXc, nYc, SWP_HIDEWINDOW);
	}
}

LRESULT CMyTabCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	DefWindowProc();

	if(m_tabCurrent != GetCurFocus()){
		m_tabPages[m_tabCurrent]->ShowWindow(SW_HIDE);
		m_tabCurrent = GetCurFocus();
		m_tabPages[m_tabCurrent]->ShowWindow(SW_SHOW);
		m_tabPages[m_tabCurrent]->SetFocus();
	}
	return 0;
}

