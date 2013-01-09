#if !defined(__dlgshadebuttonst_h__)
#define __dlgshadebuttonst_h__

#include "src/ShadeButtonST.h"

class CDlgShadeButtonST : public CAxDialogImpl<CDlgShadeButtonST>
{
public:
	enum { IDD = IDD_SHADED };

	BEGIN_MSG_MAP(CDlgShadeButtonST)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		REFLECT_NOTIFICATIONS()
	END_MSG_MAP()

protected:
	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

private:
	CShadeButtonST	m_btnButtons[9];
};


#endif // !defined(__dlgshadebuttonst_h__)
