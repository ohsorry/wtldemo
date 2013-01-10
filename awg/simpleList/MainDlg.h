#ifndef _maindlg_h_
#define _maindlg_h_
#include "stdafx.h"
#include "resource.h"
#include "MyListView.h"

class MainDlg:public CDialogImpl< MainDlg >
{
public:
	enum{IDD = IDD_DIALOG1,};

	BEGIN_MSG_MAP(baseclass)

		MESSAGE_HANDLER(WM_INITDIALOG,OnInitDlg)
		MESSAGE_HANDLER(WM_CLOSE,OnCloseDlg)
		//CHAIN_MSG_MAP(CDialogImpl< MainDlg >)
		//CHAIN_MSG_MAP_MEMBER(m_listView)
	END_MSG_MAP()

	LRESULT OnInitDlg(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL &bHandled)
	{
		CenterWindow();
#if 1

		RECT rc = {10,10,200,400};
		GetClientRect(&rc);
		RECT rc1 = {rc.left,rc.top,rc.right/2,rc.bottom/2};
		// LVS_ALIGNTOP | LVS_ICON | LVS_REPORT | LVS_SHAREIMAGELISTS | WS_CHILD | WS_HSCROLL | WS_OVERLAPPED | WS_VISIBLE | WS_VSCROLL
		mListView.Create(m_hWnd,&rc1,NULL,LVS_ICON | LVS_REPORT | WS_CHILD |WS_OVERLAPPED | WS_VISIBLE , WS_EX_CLIENTEDGE);
		mListView.AddColumn("He",0);
		mListView.AddColumn("sexy",1);
		mListView.AddItem(0,0,"sfdas");
		mListView.AddItem(0,1,"sfdas22");

		mListView.AddItem(1,0,"sfdas");
		mListView.AddItem(1,1,"sfdas22");

		mListView.AddItem(2,0,"sfdas");
		mListView.AddItem(2,1,"sfdas22");
		//mListView.SetBkColor(COLORREF(0x002200));
		mListView.SetColumnWidth(0,50);

		RECT rc2 = {rc.right/2+5,rc.top,rc.right,rc.bottom/2};
		DWORD treeview_type = TVS_EDITLABELS | TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE;
		DWORD treeview_ex_type = WS_EX_CLIENTEDGE | WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR;
		mTreeView.Create(m_hWnd,&rc2,NULL,treeview_type,treeview_ex_type);

		//
		HIMAGELIST hImageList=ImageList_Create(16,16,ILC_COLOR32,0,3);
#if 0
		HBITMAP hBitMap=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP1));       // 
		ImageList_Add(hImageList,hBitMap,NULL);
		DeleteObject(hBitMap);
#endif		//
		ImageList_AddIcon(hImageList,LoadIcon(NULL,IDC_WAIT));
		ImageList_AddIcon(hImageList,LoadIcon(NULL,IDC_IBEAM));
		ImageList_AddIcon(hImageList,LoadIcon(NULL,IDC_ARROW));
		//mTreeView.SetImageList(hImageList);

		 TreeView_SetImageList(mTreeView.m_hWnd, hImageList, TVSIL_NORMAL);

		//imglist.
		HTREEITEM itemRoot = mTreeView.InsertItem("root",0,1,NULL,NULL);
		HTREEITEM item = mTreeView.InsertItem("sub1",0,1,itemRoot,NULL);
		item = mTreeView.InsertItem("sub1",0,2,itemRoot,item);
		//mTreeView.setimage
		RECT r = {rc.left,rc.bottom/2,rc.right,rc.bottom};
		m_listView.Create(m_hWnd,&r,NULL,  
			WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS |   
			WS_CLIPCHILDREN | LVS_REPORT, WS_EX_CLIENTEDGE);  
		// fill in the headers  
		m_listView.AddColumn(_T("Symbol"), 0);  
		m_listView.AddColumn(_T("Company     "), 1);  
		m_listView.AddColumn(_T("Price     "), 2);              
		m_listView.AddItem(0,0,"VOD");  
		m_listView.AddItem(0,1,"Vodaphone Airtouch");  
		m_listView.AddItem(0,2,"252.25");  
		m_listView.AddItem(1,0,"LAT");  
		m_listView.AddItem(1,1,"Lattice Group");  
		m_listView.AddItem(1,2,"149.9");  
		m_listView.AddItem(2,0,"CLA");  
		m_listView.AddItem(2,1,"Claims Direct");  
		m_listView.AddItem(2,2,"132.50");  

#endif
		// 
		return false;
	}

	LRESULT OnCloseDlg(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL &bHandled)
	{
		EndDialog(0);
		return false;
	}
private:
	CListViewCtrl mListView;
	CTreeViewCtrl mTreeView;

	MyListView m_listView;
};

#endif