#include "stdafx.h"
#include "resource.h"
#include "PropertyList.h"
class MyDialog : public CDialogImpl<MyDialog>
{
private:
	CPropertyListCtrl mProperty;
public:
	enum{IDD=IDD_DIALOG1};
	BEGIN_MSG_MAP(MyDialog)
		MESSAGE_HANDLER(WM_INITDIALOG,OnInit)
		//CHAIN_MSG_MAP(CDialogImpl<MyDialog>)
		MESSAGE_HANDLER(WM_CLOSE,OnClose)
		REFLECT_NOTIFICATIONS()
	END_MSG_MAP()

	LRESULT OnInit(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL &handled)
	{
		CenterWindow();
		RECT rc = {0,0,200,400};
		mProperty.Create(m_hWnd,&rc);
		mProperty.ShowWindow(TRUE);
		//mProperty.SubclassWindow(GetDlgItem(IDC_LIST1));
		mProperty.SetExtendedListStyle(PLS_EX_CATEGORIZED);
		HPROPERTY hAppearance = mProperty.AddItem( PropCreateCategory(_T("Appearance"), 1234) );
		HPROPERTY hName = mProperty.AddItem( PropCreateSimple(_T("Name"), _T("bjarke")) );
		mProperty.AddItem( PropCreateSimple(_T("X"), 123L) );
		mProperty.AddItem( PropCreateSimple(_T("Y"), 456L) );
		mProperty.AddItem( PropCreateCategory(_T("Behaviour")) );
		mProperty.AddItem( PropCreateSimple(_T("Enabled"), false) );
		mProperty.AddItem( PropCreateFileName(_T("Picture"), _T("C:\\Temp\\Test.bmp")) );
		//CCategoryProperty cat(L"aa",1);
		//mProperty.AddItem(&cat);
		//mProperty.
		return FALSE;
	}

	LRESULT OnClose(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL &handled)
	{
		//CenterWindow();
		EndDialog(1);
		return FALSE;
	}
};