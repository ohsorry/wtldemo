
#pragma once


//////////////////////////////////////////////////////////////////////
// Themed Button control
//

class CThemedButtonCtrl : 
   public CWindowImpl<CThemedButtonCtrl, CButton>, 
   public CCustomDraw< CThemedButtonCtrl >
{
public:
   CWindow m_wndThemeParent;
   CBrush m_brBack;
   CFontHandle m_fontNormal;
   CBitmapHandle m_bmpButtonParts;
   CRect m_rcCorners;
   int m_iNormalState;
   COLORREF m_clrText;
   COLORREF m_clrSelected;
   COLORREF m_clrHot;
   COLORREF m_clrDisabled;

   // Operations

   BOOL SubclassWindow(HWND hWnd)
   {
      ATLASSERT(m_hWnd==NULL);
      ATLASSERT(::IsWindow(hWnd));
      BOOL bRet = CWindowImpl<CThemedButtonCtrl, CButton>::SubclassWindow(hWnd);
      if( bRet ) Init();
      return bRet;
   }

   void Init()
   {
      m_wndThemeParent = GetTopLevelWindow();
      // Default colours
      m_clrText = RGB(0,0,0);
      m_clrSelected = RGB(255,255,255);
      m_clrHot = RGB(255,255,255);
      m_clrDisabled = RGB(40,40,40);
      // Round corners on button
      m_rcCorners.SetRect(8, 8, 8, 8);
   }

   void SetThemeParent(HWND hWnd)
   {
      ATLASSERT(::IsWindow(hWnd));
      m_wndThemeParent = hWnd;
      if( !m_brBack.IsNull() ) m_brBack.DeleteObject();
   }

   void SetTextColors(COLORREF clrText, COLORREF clrSelected, COLORREF clrHot, COLORREF clrDisabled, int iNormalState = 0)
   {
      m_clrText = clrText;
      m_clrSelected = clrSelected;
      m_clrHot = clrHot;
      m_clrDisabled = clrDisabled;
      m_iNormalState = iNormalState;
      if( IsWindow() ) Invalidate();
   }

   void SetBitmap(HBITMAP hBitmap, RECT rcCorners)
   {
      m_bmpButtonParts = hBitmap;
      m_rcCorners = rcCorners;
      if( IsWindow() ) Invalidate();
   }

   // Message map and handlers

   BEGIN_MSG_MAP(CThemedButtonCtrl)
      MESSAGE_HANDLER(WM_CREATE, OnCreate)
      MESSAGE_HANDLER(WM_MOVE, OnSize)
      MESSAGE_HANDLER(WM_SIZE, OnSize)
      MESSAGE_HANDLER(WM_SETFONT, OnSetFont)
      CHAIN_MSG_MAP_ALT( CCustomDraw< CThemedButtonCtrl >, 1 )
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

   LRESULT OnSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
   {
      if( !m_brBack.IsNull() ) m_brBack.DeleteObject();
      return TRUE;
   }

   // Implementation

   DWORD OnPreErase(int /*uCtrl*/, LPNMCUSTOMDRAW lpNMCD)
   {
      ATLASSERT(!m_bmpButtonParts.IsNull());

      CDCHandle dc = lpNMCD->hdc;
      CRect rcItem = lpNMCD->rc;
      bool bIsHot = ((lpNMCD->uItemState & CDIS_HOT) != 0);
      bool bIsDisabled = ((lpNMCD->uItemState & CDIS_DISABLED) != 0);
      bool bIsSelected = ((lpNMCD->uItemState & CDIS_SELECTED) != 0);

      // Preserve background
      if( m_brBack.IsNull() ) m_brBack = AtlGetBackgroundBrush(m_hWnd, m_wndThemeParent);
      HBRUSH hOldBrush = dc.SelectBrush(m_brBack);
      dc.PatBlt(rcItem.left, rcItem.top, rcItem.Width(), rcItem.Height(), PATCOPY);
      dc.SelectBrush(hOldBrush);

      // Draw button
      int xoffset = m_iNormalState;
      if( bIsHot ) xoffset = 1;
      if( bIsSelected ) xoffset = 4;
      if( bIsDisabled ) xoffset = 3;

      BITMAP BmpInfo = { 0 };
      m_bmpButtonParts.GetBitmap(&BmpInfo);
      int cxPart = BmpInfo.bmWidth / 5;
      CRect rcButtonPart(xoffset * cxPart, 0, (xoffset + 1) * cxPart, BmpInfo.bmHeight);

      CDC dcCompat;
      dcCompat.CreateCompatibleDC(dc);
      HBITMAP hOldBitmap = dcCompat.SelectBitmap(m_bmpButtonParts);
      AlphaBlendStretch(dc, rcItem, dcCompat.m_hDC, rcButtonPart, 255, m_rcCorners);
      dcCompat.SelectBitmap(hOldBitmap);

      // Draw text
      TCHAR szText[200] = { 0 };
      GetWindowText(szText, (sizeof(szText) / sizeof(TCHAR)) - 1);
      COLORREF clrText = m_clrText;
      if( bIsHot ) clrText = m_clrHot;
      if( bIsSelected ) clrText = m_clrSelected;
      HFONT hOldFont = dc.SelectFont(m_fontNormal);
      dc.SetBkMode(TRANSPARENT);
      dc.SetTextColor(clrText);
      dc.DrawText(szText, _tcslen(szText), &rcItem, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
      dc.SelectFont(hOldFont);

      return CDRF_SKIPDEFAULT;
   }

   DWORD OnPrePaint(int /*uCtrl*/, LPNMCUSTOMDRAW /*lpNMCD*/)
   {
      return CDRF_SKIPDEFAULT;
   }
};
