
#pragma once


//////////////////////////////////////////////////////////////////////
// Themed Multi-row ListBox control
//

class CThemedRowListBoxCtrl : 
   public CWindowImpl<CThemedRowListBoxCtrl, CListBox>, 
   public COwnerDraw< CThemedRowListBoxCtrl >
{
public:
   CWindow m_wndThemeParent;
   CBrush m_brBack;
   CFontHandle m_fontNormal;
   CFont m_fontSmall;
   CBitmap m_bmpSelection;
   CIcon m_Icons[3];
   int m_cxItem;

   enum { CXY_ICON = 40 };

   typedef struct tagLISTITEM
   {
      TCHAR szTitle[200];
      TCHAR szAuthor[200];
   } LISTITEM;

   CSimpleArray<LISTITEM> m_aItems;

   // Operations

   BOOL SubclassWindow(HWND hWnd)
   {
      ATLASSERT(m_hWnd==NULL);
      ATLASSERT(::IsWindow(hWnd));
      BOOL bRet = CWindowImpl<CThemedRowListBoxCtrl, CListBox>::SubclassWindow(hWnd);
      if( bRet ) Init();
      return bRet;
   }

   void Init()
   {
      m_wndThemeParent = GetTopLevelWindow();
      m_bmpSelection = AtlLoadGdiplusImage(IDB_SELECTION, _T("PNG"));
      // Generate the smaller font
      SetFont(GetFont());
      // Get the icons...
      int iIndex = 0;
      TCHAR szPath[MAX_PATH] = { 0 };
      m_Icons[0] = ExtractStockIcon(m_hWnd, CSIDL_MYDOCUMENTS, CXY_ICON, szPath, iIndex);
      m_Icons[1] = ExtractStockIcon(m_hWnd, CSIDL_DRIVES, CXY_ICON, szPath, iIndex);
      m_Icons[2] = ExtractStockIcon(m_hWnd, CSIDL_NETWORK, CXY_ICON, szPath, iIndex);
      // Prepare multi-column listbox
      m_cxItem = LOWORD( ::GetDialogBaseUnits() ) * 20;
      CListBox::SetColumnWidth(m_cxItem);
      // Set default height
      CRect rcClient;
      GetClientRect(&rcClient);
      CListBox::SetItemHeight(0, rcClient.Height());
   }

   void SetThemeParent(HWND hWnd)
   {
      ATLASSERT(::IsWindow(hWnd));
      m_wndThemeParent = hWnd;
      if( !m_brBack.IsNull() ) m_brBack.DeleteObject();
   }

   int AddItem(LISTITEM Item)
   {
      m_aItems.Add(Item);
      int iItem = CListBox::AddString(_T(""));
      if( iItem == LB_ERR ) return LB_ERR;
      CListBox::SetItemData(iItem, m_aItems.GetSize() - 1);
      CListBox::SetHorizontalExtent(m_cxItem * m_aItems.GetSize());
      return iItem;
   }

   void ResetContent()
   {
      m_aItems.RemoveAll();
      CListBox::ResetContent();
   }

   int GetCount() const
   {
      return m_aItems.GetSize();
   }

   // Deprecated methods

   int InsertString(int, LPCTSTR)
   {
      ATLASSERT(false);
      return -1;
   }

   int AddString(LPCTSTR)
   {
      ATLASSERT(false);
      return -1;
   }

   // Message map and handlers

   BEGIN_MSG_MAP(CThemedRowListBoxCtrl)
      MESSAGE_HANDLER(WM_CREATE, OnCreate)
      MESSAGE_HANDLER(WM_MOVE, OnSize)
      MESSAGE_HANDLER(WM_SIZE, OnSize)
      MESSAGE_HANDLER(WM_SETFONT, OnSetFont)
      CHAIN_MSG_MAP_ALT( COwnerDraw< CThemedRowListBoxCtrl >, 1 )
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
      // Create smaller font
      if( !m_fontSmall.IsNull() ) m_fontSmall.DeleteObject();
      CLogFont lf = m_fontNormal;
      lf.MakeLarger(-1);
      m_fontSmall.CreateFontIndirect(&lf);
      bHandled = FALSE;
      return 0;
   }

   LRESULT OnSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
   {
      if( !m_brBack.IsNull() ) m_brBack.DeleteObject();
      // Adjust new height
      int cy = HIWORD(lParam);
      CListBox::SetItemHeight(0, cy);
      return TRUE;
   }

   // Implementation

   void DrawItem(LPDRAWITEMSTRUCT lpDIS)
   {
      CDCHandle dc = lpDIS->hDC;
      CRect rcItem = lpDIS->rcItem;
      bool bIsSelected = (lpDIS->itemState & ODS_SELECTED) != 0;

      int iIndex = lpDIS->itemData;
      const LISTITEM& Item = m_aItems[iIndex >= 0 ? iIndex : 0];

      // Preserve background
      if( m_brBack.IsNull() ) m_brBack = AtlGetBackgroundBrush(m_hWnd, m_wndThemeParent);
      HBRUSH hOldBrush = dc.SelectBrush(m_brBack);
      dc.PatBlt(rcItem.left, rcItem.top, rcItem.Width(), rcItem.Height(), PATCOPY);
      dc.SelectBrush(hOldBrush);

      // A little less close to the scrollbar looks nicer
      rcItem.bottom -= 1;

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

      // Draw icons
      CPoint ptIcon(rcItem.TopLeft());
      dc.DrawIconEx(ptIcon.x, ptIcon.y, m_Icons[0], CXY_ICON, CXY_ICON);
      ptIcon.x += CXY_ICON + 5;
      dc.DrawIconEx(ptIcon.x, ptIcon.y, m_Icons[1], CXY_ICON, CXY_ICON);
      ptIcon.x += CXY_ICON + 5;
      dc.DrawIconEx(ptIcon.x, ptIcon.y, m_Icons[2], CXY_ICON, CXY_ICON);

      rcItem.top += 60;
      rcItem.bottom -= 4;

      // Draw author text
      TCHAR szText[240] = { 0 };
      ::wsprintf(szText, _T("By: %s"), Item.szAuthor);
      dc.SelectFont(m_fontSmall);
      dc.SetBkMode(TRANSPARENT);
      dc.SetTextColor(RGB(128,128,128));
      dc.DrawText(szText, _tcslen(szText), &rcItem, DT_SINGLELINE | DT_BOTTOM | DT_LEFT);

      // Draw title text
      COLORREF clrText = RGB(128,128,128);
      if( bIsSelected ) clrText = RGB(40,90,200);
      HFONT hOldFont = dc.SelectFont(m_fontNormal);
      dc.SetTextColor(clrText);
      dc.DrawText(Item.szTitle, _tcslen(Item.szTitle), &rcItem, DT_SINGLELINE | DT_TOP | DT_LEFT);

      dc.SelectFont(hOldFont);
   }
};
