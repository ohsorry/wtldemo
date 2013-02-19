#ifndef _mainframe_h_
#define _mainframe_h_
#include "stdafx.h"
#include "resource.h"

//#include "MyTreeView.h"

class Mainframe : public CFrameWindowImpl<Mainframe>,
		public CUpdateUI<Mainframe>,
		public CMessageFilter,
		public CIdleHandler
{
public:

	DECLARE_FRAME_WND_CLASS("CC",IDR_MAINFRAME)

	virtual BOOL PreTranslateMessage(MSG* pMsg)
	{
		return false;
	}

	virtual BOOL OnIdle()
	{
		return false;
	}

	BEGIN_UPDATE_UI_MAP(thisClass)
	END_UPDATE_UI_MAP()

	BEGIN_MSG_MAP(theClass)
		MESSAGE_HANDLER(WM_CREATE,OnCreate)
		MESSAGE_HANDLER(WM_DESTROY,OnDestroy)
		//MESSAGE_HANDLER(WM_CLOSE,OnClose)
		CHAIN_MSG_MAP(CFrameWindowImpl<Mainframe>)
		CHAIN_MSG_MAP(CUpdateUI<Mainframe>)
		//REFLECT_NOTIFICATIONS()
		//CHAIN_MSG_MAP_MEMBER(
	END_MSG_MAP()

	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam,BOOL& bHandled);
	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam,BOOL& bHandled);
	LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam,BOOL& bHandled);

private:
	CSplitterWindow mvSpliter;

	CPaneContainer mLeftPane;

	CListViewCtrl mListView;

	CTreeViewCtrl mTreeView;
	
	CImageList mImageList;

	CTabbedChildWindow< CSimpleDotNetTabCtrl <CTabViewTabItem> > m_tabbedChildWindow;

	CListViewCtrl mTabListView;
	CListViewCtrl mTabListView2;
};


#endif