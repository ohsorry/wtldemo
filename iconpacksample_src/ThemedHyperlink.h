
#pragma once


//////////////////////////////////////////////////////////////////////
// Themed Hyperlink control
//

class CThemedHyperlinkCtrl : 
   public CWindowImpl<CThemedHyperlinkCtrl, CButton>, 
   public CCustomDraw< CThemedHyperlinkCtrl >
{
public:
   CWindow m_wndThemeParent;
   CBrush m_brBack;
   CFontHandle m_fontNormal;
   CFont m_fontUnderlined;
   COLORREF m_clrText;
   COLORREF m_clrSelected;
   COLORREF m_clrHot;
   UINT m_uTextStyle;

   // Operations

   BOOL SubclassWindow(HWND hWnd)
   {
      ATLASSERT(m_hWnd==NULL);
      ATLASSERT(::IsWindow(hWnd));
      BOOL bRet = CWindowImpl<CThemedHyperlinkCtrl, CButton>::SubclassWindow(hWnd);
      if( bRet ) Init();
      return bRet;
   }

   void Init()
   {
      m_wndThemeParent = GetTopLevelParent();
      // Generate underlined font
      SetFont(GetFont());
      // Default colours and style
      m_clrText = RGB(40,90,200);
      m_clrSelected = RGB(40,90,200);
      m_clrHot = RGB(60,120,220);
      m_uTextStyle = DT_SINGLELINE | DT_VCENTER | DT_CENTER | DT_NOPREFIX;
      // Narrow down control size to encompass text
      SIZE sizeIdeal = { 0 };
      GetIdealSize(&sizeIdeal);
      ResizeClient(sizeIdeal.cx, sizeIdeal.cy);
   }

   void SetThemeParent(HWND hWnd)
   {
      ATLASSERT(::IsWindow(hWnd));
      m_wndThemeParent = hWnd;
      if( !m_brBack.IsNull() ) m_brBack.DeleteObject();
   }

   void SetColors(COLORREF clrNormal, COLORREF clrSelected, COLORREF clrHot)
   {
      m_clrText = clrNormal;
      m_clrSelected = clrSelected;
      m_clrHot = clrHot;
      if( IsWindow() ) Invalidate();
   }

   void SetTextStyle(UINT uStyle)
   {
      m_uTextStyle = uStyle;
      if( IsWindow() ) Invalidate();
   }

   // Message map and handlers

   BEGIN_MSG_MAP(CThemedHyperlink)
      MESSAGE_HANDLER(WM_CREATE, OnCreate)
      MESSAGE_HANDLER(WM_MOVE, OnSize)
      MESSAGE_HANDLER(WM_SIZE, OnSize)
      MESSAGE_HANDLER(WM_SETFONT, OnSetFont)
      CHAIN_MSG_MAP_ALT( CCustomDraw< CThemedHyperlinkCtrl >, 1 )
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
      // Create underlined font
      if( !m_fontUnderlined.IsNull() ) m_fontUnderlined.DeleteObject();
      CLogFont lf = m_fontNormal;
      lf.lfUnderline = 1;
      m_fontUnderlined.CreateFontIndirect(&lf);
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
      CDCHandle dc = lpNMCD->hdc;
      CRect rcItem = lpNMCD->rc;
      bool bIsHot = ((lpNMCD->uItemState & CDIS_HOT) != 0);

      // Preserve background
      if( m_brBack.IsNull() ) m_brBack = AtlGetBackgroundBrush(m_hWnd, m_wndThemeParent);
      HBRUSH hOldBrush = dc.SelectBrush(m_brBack);
      dc.PatBlt(rcItem.left, rcItem.top, rcItem.Width(), rcItem.Height(), PATCOPY);
      dc.SelectBrush(hOldBrush);

      // Draw text
      TCHAR szText[200] = { 0 };
      GetWindowText(szText, (sizeof(szText) / sizeof(TCHAR)) - 1);
      HFONT hOldFont = dc.SelectFont(bIsHot ? (HFONT) m_fontUnderlined : (HFONT) m_fontNormal);
      dc.SetBkMode(TRANSPARENT);
      dc.SetTextColor(bIsHot ? m_clrHot : m_clrText);
      dc.DrawText(szText, _tcslen(szText), &rcItem, m_uTextStyle);
      dc.SelectFont(hOldFont);

      return CDRF_SKIPDEFAULT;
   }

   DWORD OnPrePaint(int /*uCtrl*/, LPNMCUSTOMDRAW /*lpNMCD*/)
   {
      return CDRF_SKIPDEFAULT;
   }
};
