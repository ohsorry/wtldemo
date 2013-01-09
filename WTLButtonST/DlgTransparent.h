#if !defined(__dlgtransparent_h__)
#define __dlgtransparent_h__

#include "src/ButtonST.h"

class CBackgroundUtil  
{
public:
	bool SetBitmap(UINT uResourceID = 0);
	CBackgroundUtil();
	virtual ~CBackgroundUtil();

protected:
	bool TileBitmap(CDCHandle pDC, CRect rc);
	bool GetBitmapAndPalette(UINT nIDResource, CBitmap& bitmap, CPalette& pal);
	// For background bitmap
	CBitmap m_BmpPattern;
	CPalette m_BmpPalette;
	int m_nBmpWidth;
	int m_nBmpHeight;
};

class CDlgTransparent : public CAxDialogImpl<CDlgTransparent>, public CBackgroundUtil
{
public:
	CDlgTransparent();

	enum { IDD = IDD_TRANSPARENT };

	BEGIN_MSG_MAP(CMainDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_SETFOCUS, OnSetFocus)
		MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBkgnd)
		REFLECT_NOTIFICATIONS()
	END_MSG_MAP()

protected:

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnSetFocus(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnEraseBkgnd(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

private:
	CButtonST	m_btnLogOff;
	CButtonST	m_btnWorkgroup;
	CButtonST	m_btnOpen;
	CButtonST	m_btnSearch;
	CButtonST	m_btnExplorer;
	CButtonST	m_btnHelp;
	CButtonST	m_btnAbout;
	CButtonST	m_btnCannibal;
	CButtonST	m_chkCheckbox;

	BOOL		m_bFirstTime;
};


#endif // !defined(__dlgtransparent_h__)
