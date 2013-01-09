
#pragma once


//////////////////////////////////////////////////////////////////////
// Themed Icon List control
//

class CThemedIconListCtrl : 
   public CWindowImpl<CThemedIconListCtrl, CListViewCtrl>, 
   public CCustomDraw< CThemedIconListCtrl >
{
public:
   CFontHandle m_fontNormal;
   CBitmap m_bmpSelection;
   CImageList m_Images;

   enum { CXY_ICON = 64 };
   enum { CXY_ITEM = 100 };
   enum { WM_APP_ADDITEM = WM_APP + 400 };

   typedef struct tagLISTITEM
   {
      HICON hIcon;
      TCHAR szTitle[MAX_PATH];
      TCHAR szFilename[MAX_PATH + 10];
   } LISTITEM;

   // Populate Thread

   class CPopulateThread : public CThreadImpl<CPopulateThread>
   {
   public:
      CWindow m_wndList;
      CSimpleValArray<int> m_aList;

      DWORD Run()
      {
         ::OleInitialize(NULL);
         ::Sleep(200L);
         for( int i = 0; i < m_aList.GetSize(); i++ ) {
            LISTITEM* pItem = new LISTITEM;
            ::ZeroMemory(pItem, sizeof(LISTITEM));
            int iIndex = 0;
            TCHAR szFilename[MAX_PATH] = { 0 };
            pItem->hIcon = ExtractStockIcon(m_wndList, m_aList[i], CXY_ICON, szFilename, iIndex);
            if( pItem->hIcon == NULL ) {
               delete pItem;
               continue;
            }
            ::wsprintf(pItem->szFilename, _T("%s,%d"), szFilename, iIndex);
            ::wsprintf(pItem->szTitle, _T("#%d"), abs(iIndex));
            if( m_wndList.IsWindow() ) m_wndList.PostMessage(WM_APP_ADDITEM, 0, reinterpret_cast<LPARAM>(pItem));
         }
         ::OleUninitialize();
         return 0;
      }
   };

   CPopulateThread m_threadPopulate;

   // Operations

   BOOL SubclassWindow(HWND hWnd)
   {
      ATLASSERT(m_hWnd==NULL);
      ATLASSERT(::IsWindow(hWnd));
      BOOL bRet = CWindowImpl<CThemedIconListCtrl, CListViewCtrl>::SubclassWindow(hWnd);
      if( bRet ) Init();
      return bRet;
   }

   void Init()
   {
      m_bmpSelection = AtlLoadGdiplusImage(IDB_SELECTION, _T("PNG"));
      // Create a dummy ImageList control to set the item size in the list
      m_Images.Create(CXY_ITEM, CXY_ITEM, ILC_COLOR32, 1, 0);
      SetImageList(m_Images, LVSIL_NORMAL);
      AddColumn(_T(""), 0);
      // Switch to report view and set colours
      SetView(LV_VIEW_ICON);
      SetBkColor(RGB(255,255,255));
      SetTextBkColor(RGB(255,255,255));
      SetIconSpacing(CXY_ITEM + 80, CXY_ITEM + 26);
   }

   BOOL PopulateList(const int* pIcons, int nIcons)
   {
      ATLASSERT(::IsWindow(m_hWnd));
      ATLASSERT(m_threadPopulate.m_aList.GetSize()==0);
      // Spin off the work in a separate thread. We'll need to copy
      // all the arguments to a safe location first.
      m_threadPopulate.Stop();
      m_threadPopulate.m_wndList = m_hWnd;
      for( int i = 0; i < nIcons; i++ ) m_threadPopulate.m_aList.Add(pIcons[i]);
      return m_threadPopulate.Start();
   }

   const LISTITEM* GetItem(int iIndex)
   {
      if( iIndex < 0 || iIndex >= GetItemCount() ) return NULL;
      return reinterpret_cast<const LISTITEM*>(GetItemData(iIndex));
   }

   // Message map and handlers

   BEGIN_MSG_MAP(CThemedIconListCtrl)
      MESSAGE_HANDLER(WM_CREATE, OnCreate)
      MESSAGE_HANDLER(WM_SETFONT, OnSetFont)
      MESSAGE_HANDLER(WM_APP_ADDITEM, OnAddItem)
      REFLECTED_NOTIFY_CODE_HANDLER(LVN_DELETEITEM, OnDeleteItem)
      CHAIN_MSG_MAP_ALT( CCustomDraw< CThemedIconListCtrl >, 1 )
   END_MSG_MAP()

   LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
   {
      Init();
      bHandled = FALSE;
      return 0;
   }

   LRESULT OnSetFont(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
   {
      m_fontNormal = (HFONT) wParam;
      bHandled = FALSE;
      return 0;
   }

   LRESULT OnAddItem(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
   {
      // NOTE: lParam == LISTITEM*
      int iItem = InsertItem(LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM, GetItemCount(), _T(""), 0, 0, 0, lParam);
      if( iItem == 0 ) SelectItem(0);
      return 0;
   }

   LRESULT OnDeleteItem(int /*idCtrl*/, LPNMHDR pnmh, BOOL& /*bHandled*/)
   {
      LPNMLISTVIEW pnmlv = (LPNMLISTVIEW) pnmh;
      LISTITEM* pItem = reinterpret_cast<LISTITEM*>(pnmlv->lParam);
      ::DestroyIcon(pItem->hIcon);
      delete pItem;
      return 0;
   }

   // Implementation

   DWORD OnPrePaint(int /*idCtrl*/, LPNMCUSTOMDRAW /*lpNMCustomDraw*/)
   {
      return CDRF_NOTIFYITEMDRAW;
   }

   DWORD OnItemPrePaint(int /*idCtrl*/, LPNMCUSTOMDRAW lpNMCustomDraw)
   {
      LPNMLVCUSTOMDRAW lpNMLVCD = (LPNMLVCUSTOMDRAW) lpNMCustomDraw;

      LISTITEM* pItem = reinterpret_cast<LISTITEM*>(lpNMLVCD->nmcd.lItemlParam);
      if( pItem == NULL ) return CDRF_DODEFAULT;

      CDCHandle dc = lpNMLVCD->nmcd.hdc;
      //CRect rcItem = lpNMLVCD->nmcd.rc;
      //bool bIsSelected = (lpNMLVCD->nmcd.uItemState & CDIS_SELECTED) != 0;
      CRect rcItem;
      GetItemRect(lpNMLVCD->nmcd.dwItemSpec, &rcItem, LVIR_BOUNDS);
      bool bIsSelected = (GetSelectedIndex() == (int) lpNMLVCD->nmcd.dwItemSpec);

      // Fill background      
      dc.FillSolidRect(&rcItem, lpNMLVCD->clrTextBk);

      // Paint shaded selection background?
      if( bIsSelected ) {
         CDC dcCompat;
         dcCompat.CreateCompatibleDC(dc);
         BITMAP BmpInfo = { 0 };
         m_bmpSelection.GetBitmap(&BmpInfo);
         HBITMAP hOldBitmap = dcCompat.SelectBitmap(m_bmpSelection);
         AlphaBlendStretch(dc, rcItem, dcCompat.m_hDC, CRect(0, 0, BmpInfo.bmWidth, BmpInfo.bmHeight), 255, CRect(8, 8, 8, 8));
         dcCompat.SelectBitmap(hOldBitmap);
      }

      rcItem.DeflateRect(10, 10);

      // Paint icon

      POINT ptIcon = { rcItem.left + (rcItem.Width() / 2) - (CXY_ICON / 2), rcItem.top };
      dc.DrawIconEx(ptIcon, pItem->hIcon, CSize(CXY_ICON, CXY_ICON));

      rcItem.top += CXY_ICON + 16;

      // Paint text
      COLORREF clrText = RGB(128,128,128);
      if( bIsSelected ) clrText = RGB(40,90,200);
      dc.SetTextColor(clrText);
      dc.SetBkMode(TRANSPARENT);
      dc.DrawText(pItem->szTitle, _tcslen(pItem->szTitle), &rcItem, DT_SINGLELINE | DT_TOP | DT_CENTER);

      return CDRF_SKIPDEFAULT;
   }
};
