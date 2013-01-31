// maindlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINDLG_H__7E8EF4A6_08D5_4524_B840_D3E479423D34__INCLUDED_)
#define AFX_MAINDLG_H__7E8EF4A6_08D5_4524_B840_D3E479423D34__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "ListViewMarquee.h"


class CMainDlg : public CDialogImpl<CMainDlg>
{
public:
   enum { IDD = IDD_MAINDLG };

   CComboBox m_ctrlView;
   CImageList m_Images;
   CListViewCtrl m_ctrlList;
   CListViewMarquee m_ListMarquee;
   CSimpleArray<CString> m_aImageNames;

   BEGIN_MSG_MAP(CMainDlg)
      MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
      COMMAND_ID_HANDLER(ID_APP_ABOUT, OnAppAbout)
      COMMAND_ID_HANDLER(IDOK, OnOK)
      COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
      COMMAND_CODE_HANDLER(CBN_SELCHANGE, OnSelChange)
      CHAIN_MSG_MAP_ALT_MEMBER( m_ListMarquee, 1 )
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

      m_ctrlView = GetDlgItem(IDC_TYPE);
      m_ctrlView.AddString("Report");
      m_ctrlView.AddString("Icon");
      m_ctrlView.AddString("List");
      m_ctrlView.SetCurSel(1);

      m_Images.Create(16, 16, 0, 200, 200);
      _PopulateImages();

      m_ctrlList = GetDlgItem(IDC_LIST1);
      _PopulateList();

      m_ListMarquee.Install(m_ctrlList);

      return TRUE;
   }

   LRESULT OnSelChange(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
   {
      switch( m_ctrlView.GetCurSel() ) {
      case 0: m_ctrlList.SetView(LVS_REPORT); break;
      case 1: m_ctrlList.SetView(LVS_ICON); break;
      case 2: m_ctrlList.SetView(LVS_LIST); break;
      }
      return 0;
   }

   LRESULT OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
   {
      CSimpleDialog<IDD_ABOUTBOX, FALSE> dlg;
      dlg.DoModal();
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

   void _PopulateImages()
   {
      CFindFile ff;
      for( BOOL bRes = ff.FindFile("data\\*.ico"); bRes; bRes = ff.FindNextFile() ) {
         HICON hIcon = (HICON) ::LoadImage(NULL, ff.GetFilePath(), IMAGE_ICON, 16, 16, LR_LOADFROMFILE);
         m_Images.AddIcon(hIcon);
         CString sTitle = ff.GetFileTitle();
         m_aImageNames.Add(sTitle);
      }
      ff.Close();
   }
   void _PopulateList()
   {
      m_ctrlList.SetImageList(m_Images, LVSIL_NORMAL);
      m_ctrlList.SetImageList(m_Images, LVSIL_SMALL);
      m_ctrlList.SetSelectedColumn(0);
      m_ctrlList.SetIconSpacing(80, 60);
      m_ctrlList.SetView(LVS_REPORT);
      m_ctrlList.AddColumn("Name", 0);
      m_ctrlList.AddColumn("Region", 1);
      m_ctrlList.AddColumn("Long/lat", 2);
      m_ctrlList.AddColumn("Airport", 3);
      // City,CountryRegion,Long,Lat,Airport,Dial Prefix,Area Code,TZ Index
      FILE* f = ::fopen("data\\citylist.csv", "r");
      ATLASSERT(f);
      char szLine[800] = { 0 };
      ::fgets(szLine, 800, f);
      int iIndex = 0;
      while( !feof(f) ) {
         ::fgets(szLine, 800, f);
         char szCity[200] = { 0 };
         char szCountry[100] = { 0 };
         char szLong[40] = { 0 };
         char szLat[40] = { 0 };
         char szAir[40] = { 0 };
         sscanf(szLine, "%[^,],%[^,],%[^,],%[^,],%[^,]", szCity, szCountry, szLong, szLat, szAir);
         char szLongLat[50] = { 0 };
         sprintf(szLongLat, "%s/%s", szLong, szLat);
         char szAbbrev[3] = { 0 };
         szAbbrev[0] = (char) tolower(szCountry[0]);
         szAbbrev[1] = (char) tolower(szCountry[1]);
         int iImage = _FindImage(szAbbrev);
         szAbbrev[1] = (char) tolower(szCountry[2]);
         if( iImage == -1 ) iImage = _FindImage(szAbbrev);
         if( iImage == -1 ) iImage = _FindImage("checkered");
         int iItem = m_ctrlList.InsertItem(iIndex++, szCity, iImage);
         m_ctrlList.SetItemText(iItem, 1, szCountry);
         m_ctrlList.SetItemText(iItem, 2, szLongLat);
         m_ctrlList.SetItemText(iItem, 3, szAir);
      }
      ::fclose(f);
      m_ctrlList.SetColumnWidth(0, 150);
      m_ctrlList.SetColumnWidth(1, 120);
      m_ctrlList.SetColumnWidth(2, 80);
      m_ctrlList.SetColumnWidth(3, 60);
      m_ctrlList.SetView(LVS_ICON);
   }
   int _FindImage(LPCTSTR pstrName) const
   {
      for( int i = 0; i < m_aImageNames.GetSize(); i++ ) {
         if( m_aImageNames[i] == pstrName ) return i;
      }
      return -1;
   }
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINDLG_H__7E8EF4A6_08D5_4524_B840_D3E479423D34__INCLUDED_)
