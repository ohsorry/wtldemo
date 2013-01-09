
#pragma once


class CIconPackagesDlg : 
   public CThemedDialogImpl<CIconPackagesDlg>,
   public CDialogResize<CIconPackagesDlg>
{
public:
   enum { IDD = IDD_ICONPACKAGES };

   CFont m_fontTitle;
   CBitmap m_bmpBlueButton;
   CThemedLabelCtrl m_ctrlTitle;
   CThemedLabelCtrl m_ctrlDescription;
   CThemedHyperlinkCtrl m_ctrlFilterCategory;
   CThemedHyperlinkCtrl m_ctrlFilterOptions;
   CThemedHyperlinkCtrl m_ctrlFilterFeatured;
   CThemedImageCtrl m_ctrlExpandIcon;
   CThemedShadowStaticCtrl m_ctrlSidebar;
   CThemedShadowStaticCtrl m_ctrlPreview;
   CThemedLabelCtrl m_ctrlSubTitle;
   CThemedLabelCtrl m_ctrlSubDescription;
   CThemedButtonCtrl m_ctrlButton1;
   CThemedButtonCtrl m_ctrlButton2;
   CThemedButtonCtrl m_ctrlButton3;
   CThemedButtonCtrl m_ctrlButton4;
   CThemedRowListBoxCtrl m_ctrlList;

   BEGIN_MSG_MAP(CIconPackagesDlg)
      MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
      COMMAND_HANDLER(IDC_LIST, LBN_SELCHANGE, OnListSelChanged)
      CHAIN_MSG_MAP( CThemedDialogImpl<CIconPackagesDlg> )
      CHAIN_MSG_MAP( CDialogResize<CIconPackagesDlg> )
      REFLECT_NOTIFICATIONS()
   END_MSG_MAP()

   BEGIN_DLGRESIZE_MAP(CIconPackagesDlg)
      DLGRESIZE_CONTROL(IDC_TITLE, DLSZ_SIZE_X)
      DLGRESIZE_CONTROL(IDC_DESCRIPTION, DLSZ_SIZE_X)
      DLGRESIZE_CONTROL(IDC_LIST, DLSZ_SIZE_X)
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

      m_ctrlPreview.SubclassWindow(GetDlgItem(IDC_PREVIEW));
      m_ctrlPreview.SetFillBitmap(IDB_PREVIEW_VISTA, _T("PNG"));
      m_ctrlPreview.SetInset(CSize(6, 6));

      m_ctrlFilterCategory.SubclassWindow(GetDlgItem(IDC_FILTER_CATEGORY));

      m_ctrlFilterOptions.SubclassWindow(GetDlgItem(IDC_FILTER_OPTIONS));

      m_ctrlFilterFeatured.SubclassWindow(GetDlgItem(IDC_FILTER_FEATURED));

      m_ctrlExpandIcon.SubclassWindow(GetDlgItem(IDC_EXPANDICON));
      m_ctrlExpandIcon.SetBitmap(IDB_ICON1, _T("PNG"));
      m_ctrlExpandIcon.SizeToFit();

      m_ctrlList.SubclassWindow(GetDlgItem(IDC_LIST));
      CThemedRowListBoxCtrl::LISTITEM Item[] = {
         { _T("Windows Default Icons"), _T("Microsoft") },
         { _T("Random Package"), _T("Various") },
         { _T("Cool Flavor Vista"), _T("bviksoe") },
         { _T("Jacko Flavor Vista"), _T("mjackson") },
         { _T("Franko Flavor Vista"), _T("janedoe") },
      };
      for( int i = 0; i < sizeof(Item) / sizeof(Item[0]); i++ ) m_ctrlList.AddItem(Item[i]);
      m_ctrlList.SetCurSel(0);

      ThemedDialog_Init();

      m_ctrlTitle.SetFont(m_fontTitle);
      m_ctrlSubTitle.SetFont(m_fontTitle);

      DlgResize_Init(false, false, 0);

      return TRUE;
   }

   LRESULT OnListSelChanged(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
   {
      int iItem = m_ctrlList.GetCurSel();
      m_ctrlPreview.SetFillBitmap(iItem == 0 ? IDB_PREVIEW_VISTA : IDB_PREVIEW_RANDOM, _T("PNG"), 600);
      return 0;
   }
};

