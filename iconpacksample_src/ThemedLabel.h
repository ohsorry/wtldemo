
#pragma once


//////////////////////////////////////////////////////////////////////
// Themed Label control
//

class CThemedLabelCtrl : public CWindowImpl<CThemedLabelCtrl, CStatic>
{
public:
   CWindow m_wndThemeParent;
   CBrush m_brBack;
   CFontHandle m_Font;
   COLORREF m_clrText;
   UINT m_uTextStyle;

   BOOL SubclassWindow(HWND hWnd)
   {
      ATLASSERT(m_hWnd==NULL);
      ATLASSERT(::IsWindow(hWnd));
      BOOL bRet = CWindowImpl<CThemedLabelCtrl, CStatic>::SubclassWindow(hWnd);
      if( bRet ) Init();
      return bRet;
   }

   void Init()
   {
      m_wndThemeParent = GetTopLevelParent();
      m_uTextStyle = DT_SINGLELINE | DT_LEFT | DT_TOP | DT_NOPREFIX;
      m_Font = GetParent().GetFont();
      m_clrText = RGB(0,0,0);
   }

   void SetThemeParent(HWND hWnd)
   {
      ATLASSERT(::IsWindow(hWnd));
      m_wndThemeParent = hWnd;
      if( !m_brBack.IsNull() ) m_brBack.DeleteObject();
   }

   void SetTextColor(COLORREF clrText)
   {
      m_clrText = clrText;
      if( IsWindow() ) Invalidate();
   }

   void SetTextStyle(UINT uStyle)
   {
      m_uTextStyle = uStyle;
      if( IsWindow() ) Invalidate();
   }

   BEGIN_MSG_MAP(CThemedLabelCtrl)
      MESSAGE_HANDLER(WM_CREATE, OnCreate)
      MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBkgnd)
      MESSAGE_HANDLER(WM_MOVE, OnSize)
      MESSAGE_HANDLER(WM_SIZE, OnSize)
      MESSAGE_HANDLER(WM_SETFONT, OnSetFont)
      MESSAGE_HANDLER(WM_SETTEXT, OnSetText)
      MESSAGE_HANDLER(WM_PAINT, OnPaint)
      MESSAGE_HANDLER(WM_PRINTCLIENT, OnPaint)
   END_MSG_MAP()

   LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
   {
      Init();
      bHandled = FALSE;
      return 0;
   }

   LRESULT OnSetFont(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
   {
      m_Font = (HFONT) wParam;
      bHandled = FALSE;
      return 0;
   }

   LRESULT OnSetText(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
   {
      // Have to override WM_SETTEXT because Windows redraws the control internally so we'll
      // miss the themed background.
      LRESULT lRes = DefWindowProc();
      Invalidate();
      RedrawWindow();
      return lRes;
   }

   LRESULT OnSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
   {
      if( !m_brBack.IsNull() ) m_brBack.DeleteObject();
      return TRUE;
   }

   LRESULT OnEraseBkgnd(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
   {
      return 1;
   }

   LRESULT OnPaint(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
   {
      if( wParam != NULL )
      {
         DoPaint((HDC) wParam);
      }
      else
      {
         CPaintDC dc(m_hWnd);
         DoPaint(dc.m_hDC);
      }
      return 0;
   }

   // Implementation

   void DoPaint(CDCHandle dc)
   {
      CRect rcClient;
      GetClientRect(&rcClient);

      // Preserve background
      if( m_brBack.IsNull() ) m_brBack = AtlGetBackgroundBrush(m_hWnd, m_wndThemeParent);
      HBRUSH hOldBrush = dc.SelectBrush(m_brBack);
      dc.PatBlt(rcClient.left, rcClient.top, rcClient.Width(), rcClient.Height(), PATCOPY);
      dc.SelectBrush(hOldBrush);

      // Draw label text
      TCHAR szText[600] = { 0 };
      GetWindowText(szText, (sizeof(szText) / sizeof(TCHAR)) - 1);
      HFONT hOldFont = dc.SelectFont(m_Font);
      dc.SetBkMode(TRANSPARENT);
      dc.SetTextColor(m_clrText);
      dc.DrawText(szText, _tcslen(szText), &rcClient, m_uTextStyle);
      dc.SelectFont(hOldFont);
   }
};

