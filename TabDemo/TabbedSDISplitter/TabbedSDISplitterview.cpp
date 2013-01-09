// TabbedSDISplitterView.cpp
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "TabbedSDISplitterView.h"

BOOL CLeftPaneTreeView::PreTranslateMessage(MSG* pMsg)
{
	pMsg;
	return FALSE;
}

LRESULT CLeftPaneTreeView::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// "CTreeViewCtrl::OnCreate()"
	LRESULT lRet = DefWindowProc(uMsg, wParam, lParam);

	// "OnInitialUpdate"
	int cx = 16, cy = 16;
	BOOL bCreate = FALSE;
	bCreate = m_ImageList.Create(cx, cy, ILC_COLOR32 | ILC_MASK, 4, 4);
	if(bCreate)
	{
		LoadDemoNodes(cx, cy);
	}

	bHandled = TRUE;

	return lRet;
}

LRESULT CLeftPaneTreeView::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	BOOL bDestroy = m_ImageList.Destroy();
	bDestroy; //avoid level 4 warning

	// Say that we didn't handle it so that the treeview and anyone else
	//  interested gets to handle the message
	bHandled = FALSE;
	return 0;
}

void CLeftPaneTreeView::LoadDemoNodes(int cx, int cy)
{
	int nIconIndexNormal = -1, nIconIndexSelected = -1;
	HICON hIcon = NULL;

	// NOTE: Don't Load using the LR_LOADTRANSPARENT bit, because the icon
	//  already properly deals with transparency (setting it causes problems).
	//  We will load this as LR_SHARED so that we don't have to do a DeleteIcon.
	hIcon = (HICON)::LoadImage(
				_Module.GetResourceInstance(),
				MAKEINTRESOURCE(IDI_DOCUMENT),
				IMAGE_ICON, cx, cy, LR_SHARED);

	nIconIndexNormal = m_ImageList.AddIcon(hIcon);
	nIconIndexSelected = nIconIndexNormal;


	int nIconFolderIndexNormal = -1, nIconFolderIndexSelected = -1;
	hIcon = (HICON)::LoadImage(
				_Module.GetResourceInstance(),
				MAKEINTRESOURCE(IDI_FOLDER_CLOSED),
				IMAGE_ICON, cx, cy, LR_SHARED);

	nIconFolderIndexNormal = m_ImageList.AddIcon(hIcon);

	hIcon = (HICON)::LoadImage(
				_Module.GetResourceInstance(),
				MAKEINTRESOURCE(IDI_FOLDER_OPEN),
				IMAGE_ICON, cx, cy, LR_SHARED);

	nIconFolderIndexSelected = m_ImageList.AddIcon(hIcon);

	// Hook up the image list to the tree view
	this->SetImageList(m_ImageList, TVSIL_NORMAL);


	CTreeItem tiRoot = this->InsertItem(_T("Root"), nIconFolderIndexNormal, nIconFolderIndexSelected, TVI_ROOT, NULL);

	CTreeItem tiFolder1 = this->InsertItem(_T("Folder"), nIconFolderIndexNormal, nIconFolderIndexSelected, tiRoot, NULL);
	this->InsertItem(_T("Item"), nIconIndexNormal, nIconIndexSelected, tiFolder1, NULL);

	CTreeItem tiFolder2 = this->InsertItem(_T("Folder"), nIconFolderIndexNormal, nIconFolderIndexSelected, tiRoot, NULL);
	this->InsertItem(_T("Item"), nIconIndexNormal, nIconIndexSelected, tiFolder2, NULL);
	this->InsertItem(_T("Item"), nIconIndexNormal, nIconIndexSelected, tiFolder2, NULL);

	tiRoot.Expand();
	tiFolder1.Expand();
	tiFolder2.Expand();
}



void CSomeChildView::DoPaint(CDCHandle dc, RECT& rcClip)
{
	// "EraseBackground"
	dc.FillRect(&rcClip, (HBRUSH)(COLOR_WINDOW+1));

	// "OnPaint"
	CPenHandle hOldPen = dc.SelectStockPen(BLACK_PEN);
	CBrushHandle hOldBrush = dc.SelectStockBrush(HOLLOW_BRUSH);

	int nRectWidth = 10;
	int nRectHeight = 10;

	int nRectColumns = 6;
	int nRectRows = 6;

	RECT rect = {0,0,0,0};

	for(int i=0; i < nRectColumns; ++i)
	{
		rect.left = (i+1)*nRectWidth;
		rect.right = (i+2)*nRectWidth;
		for(int j=0; j < nRectRows; ++j)
		{
			rect.top = (j+1)*nRectHeight;
			rect.bottom = (j+2)*nRectHeight;
			dc.Rectangle(&rect);
		}
	}

	dc.SelectBrush(hOldBrush);
	dc.SelectPen(hOldPen);
}



LRESULT CSomeSelfDeletingChildView::OnSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	Invalidate();
	return 0;
}

void CSomeSelfDeletingChildView::DoPaint(CDCHandle dc, RECT& rcClip)
{
	// "EraseBackground"
	dc.FillRect(&rcClip, (HBRUSH)(COLOR_WINDOW+1));

	CRect rcClient;
	this->GetClientRect(&rcClient);

	CString sText(_T("Hello World"));
	dc.DrawText(sText, sText.GetLength(), &rcClient, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

BOOL CSdiDlgView::PreTranslateMessage(MSG* pMsg)
{
	return IsDialogMessage(pMsg);
}

LRESULT CSdiDlgView::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	this->SetWindowText(_T("Dialog View"));

	HICON hIcon = (HICON)::LoadImage(
				_Module.GetResourceInstance(),
				MAKEINTRESOURCE(IDR_MAINFRAME),
				IMAGE_ICON, 16, 16, LR_SHARED);
	this->SetIcon(hIcon, ICON_SMALL);

	DlgResize_Init(false, true, WS_CLIPCHILDREN);
	return 0;
}

LRESULT CSdiDlgView::OnForwardMsg(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
	LPMSG pMsg = (LPMSG)lParam;

	return this->PreTranslateMessage(pMsg);
}

LRESULT CMyCustomListView::OnPaint(UINT uMsg, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	if( wParam != NULL )
	{
		CMemDC memdc((HDC)wParam, NULL);

		memdc.FillSolidRect(&memdc.m_rc, ::GetSysColor(COLOR_WINDOW));
		this->DefWindowProc( uMsg, (WPARAM)memdc.m_hDC, 0);
	}
	else
	{
		CPaintDC dc(m_hWnd);
		CMemDC memdc(dc.m_hDC, &dc.m_ps.rcPaint);

		memdc.FillSolidRect(&dc.m_ps.rcPaint, ::GetSysColor(COLOR_WINDOW));
		this->DefWindowProc( uMsg, (WPARAM)memdc.m_hDC, 0);
	}
	return 0;
}

LRESULT CMyCustomListView::OnEraseBackground(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// Erase the background in OnPaint
	return 1;
}

DWORD CMyCustomListView::OnPrePaint(int /*idCtrl*/, LPNMCUSTOMDRAW /*lpNMCustomDraw*/)
{
	return CDRF_NOTIFYITEMDRAW;
}

DWORD CMyCustomListView::OnItemPrePaint(int /*idCtrl*/, LPNMCUSTOMDRAW /*lpNMCustomDraw*/)
{
	return CDRF_NOTIFYPOSTPAINT;
}

DWORD CMyCustomListView::OnItemPostPaint(int /*idCtrl*/, LPNMCUSTOMDRAW lpNMCustomDraw)
{
	CDCHandle dc( lpNMCustomDraw->hdc );

	int nItem = (int)lpNMCustomDraw->dwItemSpec;

	RECT rcItem;
	this->GetItemRect(nItem, &rcItem, LVIR_BOUNDS);

	CPen penOutline;
	if(this->GetItemState(nItem, LVIS_SELECTED) == LVIS_SELECTED)
	{
		penOutline.CreatePen(PS_SOLID, 1, ::GetSysColor(COLOR_HIGHLIGHT));
	}
	else 
	{
		penOutline.CreatePen(PS_SOLID, 1, ::GetSysColor(COLOR_GRAYTEXT));
	}

	CPenHandle penOld = dc.SelectPen(penOutline);

	dc.MoveTo(rcItem.left, rcItem.top);
	dc.LineTo(rcItem.left, rcItem.bottom-1);
	dc.LineTo(rcItem.right-1, rcItem.bottom-1);
	dc.LineTo(rcItem.right-1, rcItem.top);
	dc.LineTo(rcItem.left, rcItem.top);

	dc.SelectPen(penOld);

	return CDRF_DODEFAULT;
}

