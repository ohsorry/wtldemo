#ifndef _dialog_about_h_
#define _dialog_about_h_
#include "atl_inc.h"
#include "resource.h"
class AboutDialog : public CDialogImpl<AboutDialog>
{
public:
	enum{
		IDD=IDD_DIALOG,
	};
	BEGIN_MSG_MAP(AboutDialog)
		MESSAGE_HANDLER(WM_INITDIALOG,OnInitDialog)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
		COMMAND_ID_HANDLER(ID_OK,OnOk)
		COMMAND_ID_HANDLER(ID_CANCEL,OnCancel)
	END_MSG_MAP()
		LRESULT OnInitDialog(UINT ,WPARAM,LPARAM ,BOOL &)
		{
			CenterWindow();
			return false;
		}
		LRESULT OnClose(UINT,WPARAM,LPARAM , BOOL &)
		{
			EndDialog(0);
			return false;
		}
		LRESULT OnOk(UINT notify,int id,HWND hwnd,BOOL &)
		{
			EndDialog(1);
			return 0;
		}

		LRESULT OnCancel(UINT notify,int id,HWND hwnd,BOOL &)
		{
			EndDialog(2);
			return 0;
		}
};
#endif