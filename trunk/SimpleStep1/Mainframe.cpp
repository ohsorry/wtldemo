#include "MainFrame.h"


LRESULT Mainframe::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam,BOOL& bHandled)
{
	CenterWindow();
	SetWindowText("Hello Wtl!");
	CMessageLoop *loop  = _Module.GetMessageLoop();
	if(loop)
	{
		loop->AddIdleHandler(this);
		loop->AddMessageFilter(this);
		loop->AddUpdateUI(this);
	}

	//CreateSimpleToolBar();
	//CreateSimpleStatusBar();

	//------------------------vert spliter-----------------------------------
	m_hWndClient = mvSpliter.Create(m_hWnd,rcDefault,"",WS_CHILD|WS_CHILDWINDOW|WS_VISIBLE|WS_CLIPSIBLINGS);
	mvSpliter.SetSplitterPos(200);
	mvSpliter.m_cxyMin = 185;
	//-----------------------------------------------------------

	//


	mLeftPane.Create(mvSpliter.m_hWnd,"Model");
	mLeftPane.SetPaneContainerExtendedStyle(PANECNT_NOCLOSEBUTTON);
	mvSpliter.SetSplitterPane(0,mLeftPane);

#if 0
	mListView.Create(mLeftPane.m_hWnd,rcDefault,"",LVS_ALIGNTOP | LVS_ICON | LVS_REPORT | LVS_SHAREIMAGELISTS | WS_CHILD | WS_HSCROLL | WS_OVERLAPPED | WS_VISIBLE | WS_VSCROLL,
		WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR | WS_EX_STATICEDGE);
	//mvSpliter.SetSplitterPane(0,mLeftPane.m_hWnd);
	mListView.AddColumn("view",0);
	mListView.AddColumn("value",1);
	mListView.AddItem(0,0,"123");
	mListView.AddItem(0,1,"123");

	mLeftPane.SetClient(mListView.m_hWnd);
#endif


	//0000000000000000000000000000
	// CImageList;
	mImageList.Create(16, 16, ILC_COLOR8 | ILC_MASK, 2, 2);
	HICON hIcon = (HICON)::LoadImage(_Module.GetResourceInstance(),MAKEINTRESOURCE(IDI_ITEM),
		IMAGE_ICON,16,16,LR_SHARED);
	int nItemNormal,nItemSelected;
	nItemNormal = mImageList.AddIcon(hIcon);
	nItemSelected = nItemNormal;

	int nIconFolderNormal,nIconFolderSelect;
	hIcon  = (HICON)::LoadImage(_Module.GetResourceInstance(),MAKEINTRESOURCE(IDI_OPEN),
		IMAGE_ICON,16,16,LR_SHARED);
	nIconFolderNormal = mImageList.AddIcon(hIcon);

	hIcon = (HICON)::LoadImage(_Module.GetResourceInstance(),MAKEINTRESOURCE(IDI_CLOSE),
		IMAGE_ICON,16,16,LR_SHARED);
	nIconFolderSelect = mImageList.AddIcon(hIcon);

	mTreeView.Create(mLeftPane,rcDefault,"",TVS_EDITLABELS | TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE,
		WS_EX_CLIENTEDGE | WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR);
	mLeftPane.SetClient(mTreeView.m_hWnd);

	mTreeView.SetImageList(mImageList,TVSIL_NORMAL);

	CTreeItem item = mTreeView.InsertItem("Root",nIconFolderNormal,nIconFolderSelect,TVI_ROOT,NULL);
	CTreeItem folder1 = mTreeView.InsertItem("Folder",nIconFolderNormal,nIconFolderSelect,item,NULL);
	CTreeItem folder2 = mTreeView.InsertItem("Folder",nIconFolderNormal,nIconFolderSelect,item,NULL);

	mTreeView.InsertItem("o1ne item",nItemNormal,nItemSelected,folder1,NULL);
	mTreeView.InsertItem("o2ne item",nItemNormal,nItemSelected,folder1,NULL);
	mTreeView.InsertItem("o3ne item",nItemNormal,nItemSelected,folder1,NULL);
	mTreeView.InsertItem("o4ne item",nItemNormal,nItemSelected,folder1,NULL);
	mTreeView.InsertItem("o5ne item",nItemNormal,nItemSelected,folder1,NULL);

	mTreeView.InsertItem("o1ne item",nItemNormal,nItemSelected,folder2,NULL);
	mTreeView.InsertItem("o2ne item",nItemNormal,nItemSelected,folder2,NULL);
	mTreeView.InsertItem("o3ne item",nItemNormal,nItemSelected,folder2,NULL);
	mTreeView.InsertItem("o4ne item",nItemNormal,nItemSelected,folder2,NULL);
	mTreeView.InsertItem("o5ne item",nItemNormal,nItemSelected,folder2,NULL);

	mTreeView.Expand(item);
	mTreeView.Expand(folder1);

	//
	//CTreeItem item = mTreeView.InsertItem()


	m_tabbedChildWindow.SetReflectNotifications(true);
	m_tabbedChildWindow.SetTabStyles(CTCS_TOOLTIPS | CTCS_DRAGREARRANGE);
	m_tabbedChildWindow.Create(mvSpliter, rcDefault);
	m_tabbedChildWindow.GetTabCtrl().ModifyStyle(CTCS_BOTTOM, (CTCS_SCROLL | CTCS_CLOSEBUTTON));

	mvSpliter.SetSplitterPane(1,m_tabbedChildWindow.m_hWnd);
	mTabListView.Create(m_tabbedChildWindow,rcDefault,"ListView",LVS_ALIGNTOP | LVS_ICON | LVS_REPORT | LVS_SHAREIMAGELISTS | WS_CHILD | WS_HSCROLL | WS_OVERLAPPED | WS_VISIBLE | WS_VSCROLL,
		WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR | WS_EX_STATICEDGE);
	mTabListView.AddColumn("123",0);
	mTabListView.AddColumn("abc",1);

	mTabListView2.Create(m_tabbedChildWindow,rcDefault,"ListView",LVS_ALIGNTOP | LVS_ICON | LVS_REPORT | LVS_SHAREIMAGELISTS | WS_CHILD | WS_HSCROLL | WS_OVERLAPPED | WS_VISIBLE | WS_VSCROLL,
		WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR | WS_EX_STATICEDGE);
	mTabListView2.AddColumn("12d3",0);
	mTabListView2.AddColumn("abcd",1);
	//m_tabbedChildWindow.AddTabWithIcon(mTabListView, _T("List View"), IDI_CLOSE);
	m_tabbedChildWindow.DisplayTab(mTabListView,true,FALSE);
	m_tabbedChildWindow.DisplayTab(mTabListView2,true,FALSE);
	//this->PostMessage(WM_POSTCREATE);
	//
	return false;
}

LRESULT Mainframe::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam,BOOL& bHandled)
{
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->RemoveMessageFilter(this);
	pLoop->RemoveIdleHandler(this);
	pLoop->RemoveUpdateUI(this);

	bHandled = FALSE;
	return false;
}

LRESULT Mainframe::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam,BOOL& bHandled)
{
	PostQuitMessage(0);
	return false;
}