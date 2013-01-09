
#pragma once


class CSystemSettingsDlg : 
   public CThemedDialogImpl<CSystemSettingsDlg>,
   public CDialogResize<CSystemSettingsDlg>
{
public:
   enum { IDD = IDD_SYSTEMSETTINGS };

   CFont m_fontTitle;
   CFont m_fontHeader;
   CThemedLabelCtrl m_ctrlTitle;
   CThemedLabelCtrl m_ctrlDescription;
   CThemedLabelCtrl m_ctrlHeader1;
   CThemedLabelCtrl m_ctrlHeader2;
   CThemedCheckboxCtrl m_ctrlButton1;
   CThemedCheckboxCtrl m_ctrlButton2;
   CThemedCheckboxCtrl m_ctrlButton3;
   CThemedCheckboxCtrl m_ctrlButton4;

   BEGIN_MSG_MAP(CSystemSettingsDlg)
      MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
      CHAIN_MSG_MAP( CThemedDialogImpl<CSystemSettingsDlg> )
      CHAIN_MSG_MAP( CDialogResize<CSystemSettingsDlg> )
      REFLECT_NOTIFICATIONS()
   END_MSG_MAP()

   BEGIN_DLGRESIZE_MAP(CSystemSettingsDlg)
      DLGRESIZE_CONTROL(IDC_TITLE, DLSZ_SIZE_X)
      DLGRESIZE_CONTROL(IDC_DESCRIPTION, DLSZ_SIZE_X)
   END_DLGRESIZE_MAP()

   LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
   {
      CLogFont lfTitle = GetFont();
      lfTitle.MakeLarger(5);
      lfTitle.MakeBolder();
      m_fontTitle.CreateFontIndirect(&lfTitle);

      CLogFont lfHeader = GetFont();
      lfHeader.MakeBolder();
      m_fontHeader.CreateFontIndirect(&lfHeader);

      m_ctrlTitle.SubclassWindow(GetDlgItem(IDC_TITLE));
      m_ctrlTitle.SetTextColor(RGB(40,90,170));

      m_ctrlDescription.SubclassWindow(GetDlgItem(IDC_DESCRIPTION));
      m_ctrlDescription.SetTextStyle(DT_LEFT | DT_TOP | DT_WORDBREAK | DT_NOPREFIX);

      m_ctrlHeader1.SubclassWindow(GetDlgItem(IDC_HEADER1));

      m_ctrlHeader2.SubclassWindow(GetDlgItem(IDC_HEADER2));

      m_ctrlButton1.SubclassWindow(GetDlgItem(IDC_BUTTON1));

      m_ctrlButton2.SubclassWindow(GetDlgItem(IDC_BUTTON2));

      m_ctrlButton3.SubclassWindow(GetDlgItem(IDC_BUTTON3));

      m_ctrlButton4.SubclassWindow(GetDlgItem(IDC_BUTTON4));

      ThemedDialog_Init();

      m_ctrlTitle.SetFont(m_fontTitle);
      m_ctrlHeader1.SetFont(m_fontHeader);
      m_ctrlHeader2.SetFont(m_fontHeader);

      DlgResize_Init(false, false, 0);

      return TRUE;
   }
};

