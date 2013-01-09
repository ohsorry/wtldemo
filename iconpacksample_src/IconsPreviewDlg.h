
#pragma once


class CIconsPreviewDlg : 
   public CThemedDialogImpl<CIconsPreviewDlg>,
   public CDialogResize<CIconsPreviewDlg>
{
public:
   enum { IDD = IDD_ICONSPREVIEW };

   CFont m_fontTitle;
   CThemedLabelCtrl m_ctrlTitle;
   CThemedLabelCtrl m_ctrlDescription;
   CThemedShadowStaticCtrl m_ctrlShadow;
   CThemedIconListCtrl m_ctrlPreview;
   CThemedLabelCtrl m_ctrlFilename;
   CThemedRowListBoxCtrl m_ctrlList;

   BEGIN_MSG_MAP(CIconsPreviewDlg)
      MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
      NOTIFY_CODE_HANDLER(LVN_ITEMCHANGED, OnItemChanged)
      CHAIN_MSG_MAP( CThemedDialogImpl<CIconsPreviewDlg> )
      CHAIN_MSG_MAP( CDialogResize<CIconsPreviewDlg> )
      REFLECT_NOTIFICATIONS()
   END_MSG_MAP()

   BEGIN_DLGRESIZE_MAP(CIconsPreviewDlg)
      DLGRESIZE_CONTROL(IDC_TITLE, DLSZ_SIZE_X)
      DLGRESIZE_CONTROL(IDC_DESCRIPTION, DLSZ_SIZE_X)
      DLGRESIZE_CONTROL(IDC_SHADOW, DLSZ_SIZE_X | DLSZ_SIZE_Y)
      DLGRESIZE_CONTROL(IDC_PREVIEW, DLSZ_SIZE_X | DLSZ_SIZE_Y)
      DLGRESIZE_CONTROL(IDC_LIST, DLSZ_SIZE_X | DLSZ_MOVE_Y)
      DLGRESIZE_CONTROL(IDC_FILENAME, DLSZ_SIZE_X | DLSZ_MOVE_Y)
   END_DLGRESIZE_MAP()

   LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
   {
      CLogFont lfTitle = GetFont();
      lfTitle.MakeLarger(5);
      lfTitle.MakeBolder();
      m_fontTitle.CreateFontIndirect(&lfTitle);

      m_ctrlTitle.SubclassWindow(GetDlgItem(IDC_TITLE));
      m_ctrlTitle.SetTextColor(RGB(40,90,170));

      m_ctrlDescription.SubclassWindow(GetDlgItem(IDC_DESCRIPTION));
      m_ctrlDescription.SetTextStyle(DT_LEFT | DT_TOP | DT_WORDBREAK | DT_NOPREFIX);

      m_ctrlShadow.SubclassWindow(GetDlgItem(IDC_SHADOW));

      static int aIcons[] = { CSIDL_MYDOCUMENTS, CSIDL_DRIVES, CSIDL_NETWORK, CSIDL_INTERNET, CSIDL_CONTROLS, CSIDL_BITBUCKET };
      m_ctrlPreview.SubclassWindow(GetDlgItem(IDC_PREVIEW));
      m_ctrlPreview.PopulateList(aIcons, sizeof(aIcons) / sizeof(int));

      m_ctrlFilename.SubclassWindow(GetDlgItem(IDC_FILENAME));
      m_ctrlFilename.SetTextColor(RGB(0,0,0));
      m_ctrlFilename.SetTextStyle(DT_LEFT | DT_VCENTER | DT_END_ELLIPSIS | DT_NOPREFIX);

      m_ctrlList.SubclassWindow(GetDlgItem(IDC_LIST));
      CThemedRowListBoxCtrl::LISTITEM Item[] = {
         { _T("Cool Flavor Vista"), _T("bviksoe") },
         { _T("Jacko Flavor Vista"), _T("mjackson") },
         { _T("Franko Flavor Vista"), _T("janedoe") },
      };
      for( int i = 0; i < sizeof(Item) / sizeof(Item[0]); i++ ) m_ctrlList.AddItem(Item[i]);
      m_ctrlList.SetCurSel(0);

      ThemedDialog_Init();

      m_ctrlTitle.SetFont(m_fontTitle);

      DlgResize_Init(false, false, 0);

      return TRUE;
   }

   LRESULT OnItemChanged(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)
   {      
      const CThemedIconListCtrl::LISTITEM* pItem = m_ctrlPreview.GetItem(m_ctrlPreview.GetSelectedIndex());
      if( pItem != NULL ) m_ctrlFilename.SetWindowText(pItem->szFilename);
      else m_ctrlFilename.SetWindowText(_T(""));
      return 0;
   }
};
