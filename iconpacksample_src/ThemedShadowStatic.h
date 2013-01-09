
#pragma once


//////////////////////////////////////////////////////////////////////
// Themed Image control
//

class CThemedShadowStaticCtrl : public CWindowImpl<CThemedShadowStaticCtrl, CStatic>
{
public:
   enum { TIMERID_FADE = 189 };

   CWindow m_wndThemeParent;
   CBrush m_brBack;
   CBitmap m_bmpShadow;
   CBitmap m_bmpCurImage;
   CBitmap m_bmpOldImage;
   CSize m_sizeInset;
   DWORD m_dwFadeStartTick;
   DWORD m_dwFadeTimeoutTick;

   // Operations

   BOOL SubclassWindow(HWND hWnd)
   {
      ATLASSERT(m_hWnd==NULL);
      ATLASSERT(::IsWindow(hWnd));
      BOOL bRet = CWindowImpl<CThemedShadowStaticCtrl, CStatic>::SubclassWindow(hWnd);
      if( bRet ) Init();
      return bRet;
   }

   void Init()
   {
      m_wndThemeParent = GetTopLevelParent();
      m_bmpShadow = AtlLoadGdiplusImage(IDB_SHADOW, _T("PNG"));
      m_dwFadeStartTick = m_dwFadeTimeoutTick = 0;
      Invalidate();
   }

   void SetThemeParent(HWND hWnd)
   {
      ATLASSERT(::IsWindow(hWnd));
      m_wndThemeParent = hWnd;
      if( !m_brBack.IsNull() ) m_brBack.DeleteObject();
   }

   void SetFillBitmap(ATL::_U_STRINGorID nRes, ATL::_U_STRINGorID pstrType = RT_BITMAP, DWORD dwFadeTime = 0)
   {
      ATLASSERT(::IsWindow(m_hWnd));
      m_bmpOldImage = m_bmpCurImage.Detach();
      m_bmpCurImage = AtlLoadGdiplusImage(nRes, pstrType);
      m_dwFadeStartTick = ::GetTickCount();
      m_dwFadeTimeoutTick = m_dwFadeStartTick + dwFadeTime;
      if( dwFadeTime > 0 ) SetTimer(TIMERID_FADE, 25);
      Invalidate();
   }

   void SetInset(SIZE sizeInset)
   {
      m_sizeInset = sizeInset;
      if( IsWindow() ) Invalidate();
   }

   // Message map and handlers

   BEGIN_MSG_MAP(CThemedShadowStaticCtrl)
      MESSAGE_HANDLER(WM_CREATE, OnCreate)
      MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBkgnd)
      MESSAGE_HANDLER(WM_MOVE, OnSize)
      MESSAGE_HANDLER(WM_SIZE, OnSize)
      MESSAGE_HANDLER(WM_PAINT, OnPaint)
      MESSAGE_HANDLER(WM_PRINTCLIENT, OnPaint)
      MESSAGE_HANDLER(WM_TIMER, OnTimer)
   END_MSG_MAP()

   LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
   {
      Init();
      bHandled = FALSE;
      return 0;
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

   LRESULT OnTimer(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
   {
      bHandled = FALSE;
      if( wParam != TIMERID_FADE ) return 0;
      Invalidate();
      if( ::GetTickCount() > m_dwFadeTimeoutTick ) {
         KillTimer(TIMERID_FADE);
         m_dwFadeStartTick = m_dwFadeTimeoutTick = 0;
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

      // Paint shadow
      CDC dcCompat;
      dcCompat.CreateCompatibleDC(dc);
      BLENDFUNCTION bf = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };
      BITMAP ShadowInfo = { 0 };
      m_bmpShadow.GetBitmap(&ShadowInfo);
      HBITMAP hOldBitmap = dcCompat.SelectBitmap(m_bmpShadow);
      AlphaBlendStretch(dc, rcClient, dcCompat.m_hDC, CRect(0, 0, ShadowInfo.bmWidth, ShadowInfo.bmHeight), 255, CRect(10, 10, 10, 10));

      // Paint image on top; may be fading in an animation...
      if( !m_bmpCurImage.IsNull() ) 
      {
         BITMAP ImageInfo = { 0 };
         m_bmpCurImage.GetBitmap(&ImageInfo);
         CRect rcDest = rcClient;
         rcDest.DeflateRect(m_sizeInset);
         if( m_dwFadeStartTick != 0 && !m_bmpOldImage.IsNull() && (m_dwFadeTimeoutTick - m_dwFadeStartTick) != 0 )
         {
            DWORD dwTick = ::GetTickCount();
            if( dwTick > m_dwFadeTimeoutTick ) dwTick = m_dwFadeTimeoutTick;
            DWORD dwFade = ((dwTick - m_dwFadeStartTick) * 255) / (m_dwFadeTimeoutTick - m_dwFadeStartTick);
            BLENDFUNCTION bf = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };
            dcCompat.SelectBitmap(m_bmpOldImage);
            bf.SourceConstantAlpha = (BYTE) (255 - dwFade);
            dc.AlphaBlend(rcDest.left, rcDest.top, rcDest.Width(), rcDest.Height(), dcCompat, 0, 0, ImageInfo.bmWidth, ImageInfo.bmHeight, bf);
            dcCompat.SelectBitmap(m_bmpCurImage);
            bf.SourceConstantAlpha = (BYTE) (dwFade);
            dc.AlphaBlend(rcDest.left, rcDest.top, rcDest.Width(), rcDest.Height(), dcCompat, 0, 0, ImageInfo.bmWidth, ImageInfo.bmHeight, bf);
         }
         else
         {
            dcCompat.SelectBitmap(m_bmpCurImage);
            dc.AlphaBlend(rcDest.left, rcDest.top, rcDest.Width(), rcDest.Height(), dcCompat, 0, 0, ImageInfo.bmWidth, ImageInfo.bmHeight, bf);
         }
      }

      dcCompat.SelectBitmap(hOldBitmap);
  }
};

