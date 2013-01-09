
#pragma once


class CIconsCursorsDlg : 
   public CThemedDialogImpl<CIconsCursorsDlg>,
   public CDialogResize<CIconsCursorsDlg>
{
public:
   enum { IDD = IDD_ICONSCURSORS };

   CFont m_fontTitle;
   CBitmap m_bmpBlueButton;
   CThemedLabelCtrl m_ctrlTitle;
   CThemedLabelCtrl m_ctrlDescription;
   CThemedButtonCtrl m_ctrlBuildPackage;
   CThemedImageCtrl m_ctrlExpandIcon;
   CThemedShadowStaticCtrl m_ctrlSidebar;
   CThemedShadowStaticCtrl m_ctrlShadow;
   CThemedLabelCtrl m_ctrlSubTitle;
   CThemedLabelCtrl m_ctrlSubDescription;
   CThemedButtonCtrl m_ctrlButton1;
   CThemedButtonCtrl m_ctrlButton2;
   CThemedButtonCtrl m_ctrlButton3;
   CThemedButtonCtrl m_ctrlButton4;
   CThemedIconListCtrl m_ctrlList;
   CThemedLabelCtrl m_ctrlFilename;

   BEGIN_MSG_MAP(CIconsCursorsDlg)
      MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
      NOTIFY_CODE_HANDLER(LVN_ITEMCHANGED, OnItemChanged)
      CHAIN_MSG_MAP( CThemedDialogImpl<CIconsCursorsDlg> )
      CHAIN_MSG_MAP( CDialogResize<CIconsCursorsDlg> )
      REFLECT_NOTIFICATIONS()
   END_MSG_MAP()

   BEGIN_DLGRESIZE_MAP(CIconsCursorsDlg)
      DLGRESIZE_CONTROL(IDC_TITLE, DLSZ_SIZE_X)
      DLGRESIZE_CONTROL(IDC_DESCRIPTION, DLSZ_SIZE_X)
      DLGRESIZE_CONTROL(IDC_SIDEBAR, DLSZ_SIZE_Y)
      DLGRESIZE_CONTROL(IDC_SHADOW, DLSZ_SIZE_X | DLSZ_SIZE_Y)
      DLGRESIZE_CONTROL(IDC_LIST, DLSZ_SIZE_X | DLSZ_SIZE_Y)
      DLGRESIZE_CONTROL(IDC_PACKAGE, DLSZ_MOVE_Y)
      DLGRESIZE_CONTROL(IDC_FILENAME, DLSZ_SIZE_X | DLSZ_MOVE_Y)
   END_DLGRESIZE_MAP()

   LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
   {
      CLogFont lfTitle = GetFont();
      lfTitle.MakeLarger(5);
      lfTitle.MakeBolder();
      m_fontTitle.CreateFontIndirect(&lfTitle);

      m_bmpBlueButton = AtlLoadGdiplusImage(IDB_LEFTBUTTONS, _T("PNG"));

      m_ctrlTitle.SubclassWindow(GetDlgItem(IDC_TITLE));
      m_ctrlTitle.SetTextColor(RGB(40,90,170));

      m_ctrlDescription.SubclassWindow(GetDlgItem(IDC_DESCRIPTION));
      m_ctrlDescription.SetTextStyle(DT_LEFT | DT_TOP | DT_WORDBREAK | DT_NOPREFIX);

      m_ctrlSidebar.SubclassWindow(GetDlgItem(IDC_SIDEBAR));
      m_ctrlSidebar.SetFillBitmap(IDB_LEFTWITHFILL, _T("PNG"));

      m_ctrlSubTitle.SubclassWindow(GetDlgItem(IDC_SUBTITLE));
      m_ctrlSubTitle.SetThemeParent(m_ctrlSidebar);
      m_ctrlSubTitle.SetTextColor(RGB(40,90,170));
      m_ctrlSubTitle.SetTextStyle(DT_LEFT | DT_TOP | DT_END_ELLIPSIS | DT_NOPREFIX);

      m_ctrlSubDescription.SubclassWindow(GetDlgItem(IDC_SUBDESCRIPTION));
      m_ctrlSubDescription.SetThemeParent(m_ctrlSidebar);
      m_ctrlSubDescription.SetTextColor(RGB(40,90,210));
      m_ctrlSubDescription.SetTextStyle(DT_LEFT | DT_TOP | DT_END_ELLIPSIS | DT_NOPREFIX);

      m_ctrlButton1.SubclassWindow(GetDlgItem(IDC_BUTTON1));
      m_ctrlButton1.SetThemeParent(m_ctrlSidebar);
      m_ctrlButton1.SetBitmap(m_bmpBlueButton, CRect(6, 6, 6, 6));

      m_ctrlButton2.SubclassWindow(GetDlgItem(IDC_BUTTON2));
      m_ctrlButton2.SetThemeParent(m_ctrlSidebar);
      m_ctrlButton2.SetBitmap(m_bmpBlueButton, CRect(6, 6, 6, 6));

      m_ctrlButton3.SubclassWindow(GetDlgItem(IDC_BUTTON3));
      m_ctrlButton3.SetThemeParent(m_ctrlSidebar);
      m_ctrlButton3.SetBitmap(m_bmpBlueButton, CRect(6, 6, 6, 6));

      m_ctrlButton4.SubclassWindow(GetDlgItem(IDC_BUTTON4));
      m_ctrlButton4.SetThemeParent(m_ctrlSidebar);
      m_ctrlButton4.SetBitmap(m_bmpBlueButton, CRect(6, 6, 6, 6));

      m_ctrlBuildPackage.SubclassWindow(GetDlgItem(IDC_PACKAGE));
      m_ctrlBuildPackage.SetBitmap(m_bmpBlueButton, CRect(6, 6, 6, 6));
      m_ctrlBuildPackage.SetTextColors(RGB(255,255,255), RGB(255,255,255), RGB(255,255,255), RGB(40,40,40), 4);

      m_ctrlShadow.SubclassWindow(GetDlgItem(IDC_SHADOW));

      static int aDesktopIcons[] = { CSIDL_MYDOCUMENTS, CSIDL_DRIVES, CSIDL_NETWORK, CSIDL_INTERNET, CSIDL_CONTROLS, CSIDL_BITBUCKET };
      static int aStartIcons[]   = { CSIDL_INTERNET, CSIDL_CDBURN_AREA, CSIDL_DESKTOPDIRECTORY };
      static int aFoldersIcons[] = { CSIDL_MYMUSIC, CSIDL_MYPICTURES, CSIDL_MYDOCUMENTS, CSIDL_MYVIDEO, CSIDL_COMMON_APPDATA, CSIDL_COMMON_MUSIC, CSIDL_COMMON_PICTURES, CSIDL_COMMON_VIDEO };
      static int aDriveIcons[]   = { CSIDL_COMMON_ADMINTOOLS, CSIDL_COMMON_APPDATA, CSIDL_COMMON_OEM_LINKS, CSIDL_PRINTHOOD };
      static int aOtherIcons[]   = { CSIDL_NETHOOD, CSIDL_DESKTOP, CSIDL_DRIVES, CSIDL_COMPUTERSNEARME, CSIDL_CONNECTIONS };
      int nIcons = 0, *pIcons = NULL;
      if( m_wCmd == ID_VIEW_ICONS_DESKTOP ) pIcons = aDesktopIcons, nIcons = sizeof(aDesktopIcons) / sizeof(int);
      if( m_wCmd == ID_VIEW_ICONS_START )   pIcons = aStartIcons,   nIcons = sizeof(aStartIcons) / sizeof(int);
      if( m_wCmd == ID_VIEW_ICONS_FOLDERS ) pIcons = aFoldersIcons, nIcons = sizeof(aFoldersIcons) / sizeof(int);
      if( m_wCmd == ID_VIEW_ICONS_DRIVES )  pIcons = aDriveIcons,   nIcons = sizeof(aDriveIcons) / sizeof(int);
      if( m_wCmd == ID_VIEW_ICONS_OTHERS )  pIcons = aOtherIcons,   nIcons = sizeof(aOtherIcons) / sizeof(int);
      m_ctrlList.SubclassWindow(GetDlgItem(IDC_LIST));
      m_ctrlList.PopulateList(pIcons, nIcons);

      m_ctrlFilename.SubclassWindow(GetDlgItem(IDC_FILENAME));
      m_ctrlFilename.SetTextColor(RGB(0,0,0));
      m_ctrlFilename.SetTextStyle(DT_LEFT | DT_VCENTER | DT_END_ELLIPSIS | DT_NOPREFIX);

      ThemedDialog_Init();

      m_ctrlTitle.SetFont(m_fontTitle);
      m_ctrlSubTitle.SetFont(m_fontTitle);

      DlgResize_Init(false, false, 0);

      return TRUE;
   }

   LRESULT OnItemChanged(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)
   {      
      const CThemedIconListCtrl::LISTITEM* pItem = m_ctrlList.GetItem(m_ctrlList.GetSelectedIndex());
      if( pItem != NULL ) {
         m_ctrlFilename.SetWindowText(pItem->szFilename);
         m_ctrlButton1.EnableWindow(TRUE);
         m_ctrlButton2.EnableWindow(TRUE);
      }
      else {
         m_ctrlFilename.SetWindowText(_T(""));
         m_ctrlButton1.EnableWindow(FALSE);
         m_ctrlButton2.EnableWindow(FALSE);
      }
      return 0;
   }
};
