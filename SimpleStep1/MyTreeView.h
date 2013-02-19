#ifndef _my_tree_view_h_
#define _my_tree_view_h_
#include "stdafx.h"

class MyTreeView : public CWindowImpl<MyTreeView, CTreeViewCtrlEx>
{
public:
	//DECLARE_WND_SUPERCLASS("mYtREEVIEW",CTreeViewCtrl::GetWndClassName());
	BEGIN_MSG_MAP(MyTreeView)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		CHAIN_MSG_MAP(CWindowImpl< MyTreeView,CTreeViewCtrlEx > )
	END_MSG_MAP()
	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{

	}
};

#endif