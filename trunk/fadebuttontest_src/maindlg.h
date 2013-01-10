// maindlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINDLG_H__52346C70_0C78_4656_BB93_EC3D8B2ABC3C__INCLUDED_)
#define AFX_MAINDLG_H__52346C70_0C78_4656_BB93_EC3D8B2ABC3C__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "FadeButton.h"
#include "DlgTabCtrl.h"

#include "Views.h"


class CMainDlg : public CDialogImpl<CMainDlg>
{
public:
   enum { IDD = IDD_MAINDLG };

   CFadeButtonCtrl m_ctrlPrev;
   CFadeButtonCtrl m_ctrlNext;
   CFadeButtonCtrl m_ctrlCancel;
   CFadeButtonCtrl m_ctrlAbout;
   CDialogTabCtrl m_ctrlTab;

   CView1 m_view1;
   CView2 m_view2;
   CView3 m_view3;

   BEGIN_MSG_MAP(CMainDlg)
      MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
      COMMAND_ID_HANDLER(ID_APP_ABOUT, OnAppAbout)
      COMMAND_ID_HANDLER(IDOK, OnOK)
      COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
      COMMAND_ID_HANDLER(IDC_BUTTON1, OnPrev);
      COMMAND_ID_HANDLER(IDC_BUTTON2, OnNext);
      REFLECT_NOTIFICATIONS()
   END_MSG_MAP()

   LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
   {
      // Center the dialog on the screen
      CenterWindow();

      // Set icons
      HICON hIcon = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), 
         IMAGE_ICON, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON), LR_DEFAULTCOLOR);
      SetIcon(hIcon, TRUE);
      HICON hIconSmall = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), 
         IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
      SetIcon(hIconSmall, FALSE);

      // Subclass buttons
      m_ctrlPrev.SubclassWindow(GetDlgItem(IDC_BUTTON1));
      m_ctrlPrev.SetImages(IDB_BACK_1, IDB_BACK_2);
      m_ctrlNext.SubclassWindow(GetDlgItem(IDC_BUTTON2));
      m_ctrlNext.SetImages(IDB_FORWARD_1, IDB_FORWARD_2);
      m_ctrlCancel.SubclassWindow(GetDlgItem(IDCANCEL));
      m_ctrlCancel.SetImages(IDB_CANCEL_1, IDB_CANCEL_2);
      m_ctrlAbout.SubclassWindow(GetDlgItem(ID_APP_ABOUT));
      m_ctrlAbout.SetImages(IDB_ABOUT_1, IDB_ABOUT_2);
      m_ctrlAbout.SetToolTipText(_T("About this app"));
      m_ctrlAbout.SetTextColor(RGB(0,0,200));

      // Create tab container
      m_ctrlTab.SubclassWindow(GetDlgItem(IDC_TAB));
      m_view1.Create(m_hWnd);
      m_view2.Create(m_hWnd);
      m_view3.Create(m_hWnd);

      TCITEM tci = { 0 };
      tci.mask = TCIF_TEXT;
      tci.pszText = _T("View 1");
      m_ctrlTab.InsertItem(0, &tci, m_view1);
      tci.pszText = _T("View 2");
      m_ctrlTab.InsertItem(1, &tci, m_view2);
      tci.pszText = _T("View 3");
      m_ctrlTab.InsertItem(2, &tci, m_view3);
      m_ctrlTab.SetCurSel(0);

      return TRUE;
   }
   LRESULT OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
   {
      CSimpleDialog<IDD_ABOUTBOX, FALSE> dlg;
      dlg.DoModal();
      return 0;
   }
   LRESULT OnPrev(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
   {
      m_ctrlTab.SetCurSel( m_ctrlTab.GetCurSel()-1 );
      return 0;
   }
   LRESULT OnNext(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
   {
      m_ctrlTab.SetCurSel( m_ctrlTab.GetCurSel()+1 );
      return 0;
   }
   LRESULT OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
   {
      // TODO: Add validation code 
      EndDialog(wID);
      return 0;
   }
   LRESULT OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
   {
      EndDialog(wID);
      return 0;
   }
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINDLG_H__52346C70_0C78_4656_BB93_EC3D8B2ABC3C__INCLUDED_)
