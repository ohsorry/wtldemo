
#pragma once


class CAboutDlg : 
   public CThemedDialogImpl<CAboutDlg>,
   public CDialogResize<CAboutDlg>
{
public:
   enum { IDD = IDD_ABOUT };

   CThemedImageCtrl m_ctrlImage;
   CThemedLabelCtrl m_ctrlTitle;
   CThemedHyperlinkCtrl m_ctrlLink;

   BEGIN_MSG_MAP(CAboutDlg)
      MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
      COMMAND_ID_HANDLER(IDC_LINK, OnWebLink)
      CHAIN_MSG_MAP( CThemedDialogImpl<CAboutDlg> )
      CHAIN_MSG_MAP( CDialogResize<CAboutDlg> )
      REFLECT_NOTIFICATIONS()
   END_MSG_MAP()

   BEGIN_DLGRESIZE_MAP(CIconPackagesDlg)
      DLGRESIZE_CONTROL(IDC_IMAGE, DLSZ_CENTER_X)
      DLGRESIZE_CONTROL(IDC_DESCRIPTION, DLSZ_CENTER_X | DLSZ_SIZE_X)
      DLGRESIZE_CONTROL(IDC_LINK, DLSZ_CENTER_X | DLSZ_SIZE_X)
   END_DLGRESIZE_MAP()

   LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
   {
      m_ctrlImage.SubclassWindow(GetDlgItem(IDC_IMAGE));
      m_ctrlImage.SetBitmap(IDB_LOGO, _T("PNG"));
      m_ctrlImage.SizeToFit();

      m_ctrlTitle.SubclassWindow(GetDlgItem(IDC_DESCRIPTION));
      m_ctrlTitle.SetTextColor(RGB(0,0,0));
      m_ctrlTitle.SetTextStyle(DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX);

      m_ctrlLink.SubclassWindow(GetDlgItem(IDC_LINK));

      ThemedDialog_Init();

      DlgResize_Init(false, false, 0);

      return TRUE;
   }

   LRESULT OnWebLink(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
   {
      ::ShellExecute(m_hWnd, _T("open"), _T("http://www.viksoe.dk/code/"), NULL, NULL, SW_SHOWNORMAL);
      return 0;
   }
};

