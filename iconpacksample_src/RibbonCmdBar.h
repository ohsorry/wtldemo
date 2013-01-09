/////////////////////////////////////////////////////////////////////////////
// Ribbon-like Command Bar
//
// Written by Bjarke Viksoe (bjarke@viksoe.dk)
// Copyright (c) 2008 Bjarke Viksoe.
//
// This code may be used in compiled form in any way you desire. This
// source file may be redistributed by any means PROVIDING it is 
// not sold for profit without the authors written consent, and 
// providing that this notice and the authors name is included. 
//
// This file is provided "as is" with no expressed or implied warranty.
// The author accepts no liability if it causes any damage to you or your
// computer whatsoever. It's free, so don't hassle me about it.
//
// Beware of bugs.
//

#ifndef __RIBBONCMDBAR_H__
#define __RIBBONCMDBAR_H__


class CRibbonCmdBar : public CWindowImpl<CRibbonCmdBar>
{
public:
   DECLARE_WND_CLASS(_T("WTL_RibbonCmdBar"))

   enum { FADE_TIMEOUT = 300 };
   enum { NO_FADE = 0 };
   enum { TIMERID_FADE = 42 };

   // Data members

   CMenu m_Menu;               // Menu layout template
   CWindow m_wndNotify;        // Window that receives WM_COMMAND notifications
   int m_iBarIndex;            // Currently selected item in top bar
   int m_iSubIndex;            // Currently selected item in 2nd bar
   int m_cyTabHeight;          // Height of top bar
   bool m_bTracking;           // Is currently tracking mouse?
   CFont m_Font;               // Text font
   TEXTMETRIC m_tmFont;        // Metrics of text font
   CBrush m_brBack;            // Current background
   CBitmap m_bmpBar;           // Bitmap of bar background
   CBitmap m_bmpTabs;          // Bitmap of top bar buttons
   CBitmap m_bmpTabRow;        // Bitmap of 2nd bar buttons

   typedef struct tagRIBBONBUTTON
   {
      UINT uCmd;               // WM_COMMAND identifier
      TCHAR szText[100];       // Button Text
      UINT uCurState;          // Current ODS_xxx state
      UINT uOldState;          // Previous ODS_xxx state
      RECT rcItem;             // Current rect coordinates
      DWORD dwStartTick;       // Time when fade started
      DWORD dwTimeoutTick;     // Time when fade should end
   } RIBBONBUTTON;

   typedef CSimpleValArray<RIBBONBUTTON> CButtonArray;

   CButtonArray m_aBarButtons;
   CButtonArray m_aSubButtons;

   // Constructor

   CRibbonCmdBar() : m_iBarIndex(-1), m_iSubIndex(-1), m_bTracking(false)
   {
   }

   // Operations

   BOOL SetMenu(UINT nMenuRes)
   {
      ATLASSERT(::IsWindow(m_hWnd));
      m_Menu.LoadMenu(nMenuRes);
      ATLASSERT(!m_Menu.IsNull());
      if( m_Menu.IsNull() ) return FALSE;
      // Create upper bar row
      _CreateButtonRow(m_Menu.m_hMenu, m_aBarButtons, CPoint(16, 4), CRect(14, 6, 14, 6), 4);
      // Renew selection
      m_iBarIndex = m_iSubIndex = -1;
      m_aSubButtons.RemoveAll();
      return SetSelectedIndex(0, 0);
   }

   void SetNotifyHwnd(HWND hWnd)
   {
      ATLASSERT(::IsWindow(hWnd));
      m_wndNotify = hWnd;
   }

   BOOL SetSelectedIndex(int iBarPos, int iSubPos, BOOL bUpdateVisuals = TRUE)
   {
      ATLASSERT(::IsWindow(m_hWnd));
      if( iBarPos >= m_aBarButtons.GetSize() ) return FALSE;
      // Create bottom row if needed...
      if( iBarPos != m_iBarIndex ) {
         _CreateButtonRow(m_Menu.GetSubMenu(iBarPos), m_aSubButtons, CPoint(8, m_cyTabHeight + 4), CRect(14, 4, 14, 4), 0);
      }
      if( iSubPos >= m_aSubButtons.GetSize() ) iSubPos = -1;
      // Visually select items on both rows?
      if( bUpdateVisuals ) {
         _SetItemState(m_aBarButtons, iBarPos, ODS_CHECKED, ODS_CHECKED, FADE_TIMEOUT);
         _SetItemState(m_aSubButtons, iSubPos, ODS_CHECKED, ODS_CHECKED, FADE_TIMEOUT);
      }
      // Set current selection
      m_iBarIndex = iBarPos;
      m_iSubIndex = iSubPos;
      return Invalidate();
   }

   BOOL SelectItem(UINT nID)
   {
      int iBarPos, iSubPos;
      if( !_FindItemFromID(nID, iBarPos, iSubPos) ) return FALSE;
      return SetSelectedIndex(iBarPos, iSubPos);
   }

   UINT GetSelectedItem() const
   {
      if( m_iSubIndex < 0 ) return 0;
      return m_aSubButtons[m_iSubIndex].uCmd;
   }

   void UpdateLayout()
   {
      if( !m_brBack.IsNull() ) m_brBack.DeleteObject();
      Invalidate();
   }

   // Message map and handlers

   BEGIN_MSG_MAP(CRibbonCmdBar)
      MESSAGE_HANDLER(WM_CREATE, OnCreate)
      MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBkgnd)
      MESSAGE_HANDLER(WM_PAINT, OnPaint)
      MESSAGE_HANDLER(WM_PRINTCLIENT, OnPaint)
      MESSAGE_HANDLER(WM_TIMER, OnTimer)
      MESSAGE_HANDLER(WM_MOVE, OnSize)
      MESSAGE_HANDLER(WM_SIZE, OnSize)
      MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
      MESSAGE_HANDLER(WM_MOUSELEAVE, OnMouseLeave)
      MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
      MESSAGE_HANDLER(WM_LBUTTONUP, OnLButtonUp)
   END_MSG_MAP()

   LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
   {
      ModifyStyleEx(0, WS_EX_COMPOSITED);

      m_bmpBar = AtlLoadGdiplusImage(IDB_BAR, _T("PNG"));
      m_bmpTabs = AtlLoadGdiplusImage(IDB_TABS, _T("PNG"));
      m_bmpTabRow = AtlLoadGdiplusImage(IDB_TABROW, _T("PNG"));

      if( !m_wndNotify.IsWindow() ) m_wndNotify = GetParent();

      _CreateFont();
      m_cyTabHeight = m_tmFont.tmHeight + 14;
      ResizeClient(-1, 3 + (m_cyTabHeight * 2));

      bHandled = FALSE;
      return 0;
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
      // Check if we're still repainting buttons in animation; if so, schedule
      // repaint, otherwise kill timer.
      DWORD dwTick = ::GetTickCount();
      bool bStillAnimating = false;
      for( int i = 0; i < m_aBarButtons.GetSize(); i++ ) {
         RIBBONBUTTON& Btn = m_aBarButtons[i];
         if( Btn.dwTimeoutTick == 0 ) continue;
         InvalidateRect(&Btn.rcItem, FALSE);
         if( Btn.dwTimeoutTick >= dwTick ) bStillAnimating = true;
         else Btn.dwStartTick = Btn.dwTimeoutTick = 0;
      }
      for( int i = 0; i < m_aSubButtons.GetSize(); i++ ) {
         RIBBONBUTTON& Btn = m_aSubButtons[i];
         if( Btn.dwTimeoutTick == 0 ) continue;
         InvalidateRect(&Btn.rcItem, FALSE);
         if( Btn.dwTimeoutTick >= dwTick ) bStillAnimating = true;
         else Btn.dwStartTick = Btn.dwTimeoutTick = 0;
      }
      if( !bStillAnimating ) KillTimer(TIMERID_FADE);
      bHandled = TRUE;
      return 0;
   }

   LRESULT OnSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
   {
      UpdateLayout();
      return 0;
   }

   LRESULT OnMouseMove(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
   {
      CPoint pt(lParam);
      int iBarPos, iSubPos;
      _FindItemFromPos(pt, iBarPos, iSubPos);
      _SetItemState(m_aBarButtons, iBarPos, ODS_HOTLIGHT, ODS_HOTLIGHT, NO_FADE);
      _SetItemState(m_aSubButtons, iSubPos, ODS_HOTLIGHT, ODS_HOTLIGHT, NO_FADE);
      if( !m_bTracking ) m_bTracking = _StartTrackMouseLeave();
      return 0;
   }

   LRESULT OnMouseLeave(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
   {
      m_bTracking = false;
      _SetItemState(m_aBarButtons, -1, 0, ODS_HOTLIGHT, NO_FADE);
      _SetItemState(m_aSubButtons, -1, 0, ODS_HOTLIGHT, NO_FADE);
      return 0;
   }

   LRESULT OnLButtonDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
   {
      CPoint pt(lParam);
      int iBarPos, iSubPos;
      if( _FindItemFromPos(pt, iBarPos, iSubPos) ) SetCapture();
      return 0;
   }

   LRESULT OnLButtonUp(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
   {
      if( GetCapture() == m_hWnd ) ReleaseCapture();
      CPoint pt(lParam);
      int iBarPos, iSubPos;
      if( !_FindItemFromPos(pt, iBarPos, iSubPos) ) return 0;
      if( iSubPos < 0 ) iSubPos = 0;
      // Create the menu, but don't update the visual selection markers
      // since the animation should run after the dialog is created.
      if( !SetSelectedIndex(iBarPos, iSubPos, FALSE) ) return 0;
      // Send notification message (WM_COMMAND style) to parent window
      ATLASSERT(m_aSubButtons.GetSize()>0);
      UINT uCmd = m_aSubButtons[iSubPos].uCmd;
      if( uCmd > 0 ) m_wndNotify.SendMessage(WM_COMMAND, MAKEWPARAM(uCmd, 0), (LPARAM) m_hWnd);
      // Select items now.
      // This will start the fade-animation, so it is placed after 
      // the notification (dialog-creation, which could be quite slow).
      if( !SetSelectedIndex(iBarPos, iSubPos, TRUE) ) return 0;
      return 0;
   }

   // Implementation

   void DoPaint(CDCHandle dc)
   {
      CRect rcClient;
      GetClientRect(&rcClient);

      HFONT hOldFont = dc.SelectFont(m_Font);

      // Preserve background...
      if( m_brBack.IsNull() ) m_brBack = AtlGetBackgroundBrush(m_hWnd, GetParent());
      HBRUSH hOldBrush = dc.SelectBrush(m_brBack);
      dc.PatBlt(rcClient.left, rcClient.top, rcClient.Width(), rcClient.Height(), PATCOPY);
      dc.SelectBrush(hOldBrush);

      CDC dcCompat;
      dcCompat.CreateCompatibleDC(dc);

      // Draw bottom bar fill
      BITMAP BarInfo = { 0 };
      m_bmpBar.GetBitmap(&BarInfo);
      CRect rcBar(rcClient.left + 4, rcClient.top + m_cyTabHeight, rcClient.right - 4, rcClient.bottom);
      HBITMAP hOldBitmap = dcCompat.SelectBitmap(m_bmpBar);
      AlphaBlendStretch(dc.m_hDC, rcBar, dcCompat.m_hDC, CRect(0, 0, BarInfo.bmWidth, BarInfo.bmHeight), 255, CRect(10, 10, 10, 10));

      // Paint top row...
      BITMAP TabsInfo = { 0 };
      m_bmpTabs.GetBitmap(&TabsInfo);
      dcCompat.SelectBitmap(m_bmpTabs);
      for( int i = 0; i < m_aBarButtons.GetSize(); i++ ) {
         const RIBBONBUTTON& Btn = m_aBarButtons[i];
         _PaintButton(dc, Btn, dcCompat.m_hDC, TabsInfo, CRect(14, 6, 14, 6));
      }

      // Paint 2nd row...
      BITMAP RowInfo = { 0 };
      m_bmpTabRow.GetBitmap(&RowInfo);
      dcCompat.SelectBitmap(m_bmpTabRow);
      for( int i = 0; i < m_aSubButtons.GetSize(); i++ ) {
         const RIBBONBUTTON& Btn = m_aSubButtons[i];
         _PaintButton(dc, Btn, dcCompat.m_hDC, RowInfo, CRect(14, 4, 14, 4));
      }

      dcCompat.SelectBitmap(hOldBitmap);

      dc.SelectFont(hOldFont);
   }

   void _PaintButton(CDCHandle dc, const RIBBONBUTTON& Btn, CDCHandle dcSrc, BITMAP SrcBmpInfo, CRect rcMargins)
   {
      // First, paint the button (or 2 buttons when fading between states)...
      if( Btn.dwStartTick == 0 || (Btn.dwTimeoutTick - Btn.dwStartTick == 0) ) {
         CRect rcSrc = _CalcButtonBitmapRect(SrcBmpInfo, Btn.uCurState);
         AlphaBlendStretch(dc, Btn.rcItem, dcSrc, rcSrc, 255, rcMargins);
      }
      else {
         CRect rcSrc1 = _CalcButtonBitmapRect(SrcBmpInfo, Btn.uCurState);
         CRect rcSrc2 = _CalcButtonBitmapRect(SrcBmpInfo, Btn.uOldState);
         DWORD dwTick = ::GetTickCount();
         if( dwTick > Btn.dwTimeoutTick ) dwTick = Btn.dwTimeoutTick;
         DWORD dwFade = ((dwTick - Btn.dwStartTick) * 255) / (Btn.dwTimeoutTick - Btn.dwStartTick);
         AlphaBlendStretch(dc, Btn.rcItem, dcSrc, rcSrc2, (BYTE) (255 - dwFade), rcMargins);
         AlphaBlendStretch(dc, Btn.rcItem, dcSrc, rcSrc1, (BYTE) (dwFade), rcMargins);
      }
      // Paint text
      dc.SetBkMode(TRANSPARENT);
      dc.SetTextColor(RGB(0,0,0));
      RECT rcText = Btn.rcItem;
      dc.DrawText(Btn.szText, _tcslen(Btn.szText), &rcText, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
   }

   CRect _CalcButtonBitmapRect(BITMAP BmpInfo, UINT uState) const
   {
      int xoffset = 0;
      if( (uState & ODS_HOTLIGHT) != 0 ) xoffset = 3;
      if( (uState & ODS_DISABLED) != 0 ) xoffset = 2;
      if( (uState & ODS_CHECKED) != 0 ) xoffset = 1;
      int cxPart = (BmpInfo.bmWidth / 5);
      return CRect(xoffset * cxPart, 0, (1 + xoffset) * cxPart, BmpInfo.bmHeight);
   }

   UINT _ResolveButtonState(UINT uState) const
   {
      if( (uState & ODS_CHECKED) != 0 ) return ODS_CHECKED;
      if( (uState & ODS_DISABLED) != 0 ) return ODS_DISABLED;
      if( (uState & ODS_HOTLIGHT) != 0 ) return ODS_HOTLIGHT;
      return 0;
   }

   void _CreateButtonRow(CMenuHandle menu, CButtonArray& aButtons, POINT ptStart, RECT rcMargins, int cxGap)
   {
      // Start with squat
      aButtons.RemoveAll();
      if( menu.IsNull() ) return;
      // We need the text font for measuring string width
      _CreateFont();
      CClientDC dc = m_hWnd;
      HFONT hOldFont = dc.SelectFont(m_Font);
      // Now start creating the buttons from the menu template...
      for( int i = 0; i < menu.GetMenuItemCount(); i++ ) {
         RIBBONBUTTON Btn = { 0 };
         MENUITEMINFO Info = { 0 };
         Info.cbSize = sizeof(Info);
         Info.fMask = MIIM_ID | MIIM_STRING | MIIM_FTYPE | MIIM_STATE;
         Info.cch = (sizeof(Btn.szText) / sizeof(TCHAR)) - 1;
         Info.dwTypeData = Btn.szText;
         menu.GetMenuItemInfo(i, TRUE, &Info);
         if( (Info.fType & MFT_SEPARATOR) != 0 ) { ptStart.x += 8; continue; };
         if( (Info.fType & MFT_MENUBREAK) != 0 ) { ptStart.x += 8; continue; };
         if( HIWORD(Info.wID) == 0 ) Btn.uCmd = Info.wID;
         if( (Info.fState & MFS_DISABLED) != 0 ) Btn.uCurState |= ODS_DISABLED;
         CSize szText;
         dc.GetTextExtent(Btn.szText, _tcslen(Btn.szText), &szText);
         ::SetRect(&Btn.rcItem, ptStart.x, ptStart.y, ptStart.x + szText.cx + rcMargins.left + rcMargins.right, ptStart.y + m_tmFont.tmHeight + rcMargins.top + rcMargins.bottom);
         aButtons.Add(Btn);
         ptStart.x = Btn.rcItem.right + cxGap;
      }
      dc.SelectFont(hOldFont);
   }

   void _CreateFont()
   {
      if( !m_Font.IsNull() ) return;
      CLogFont lf;
      lf.SetMenuFont();
      lf.MakeLarger(2);
      m_Font.CreateFontIndirect(&lf);
      // Need to know stuff about font metrics...
      CClientDC dc = m_hWnd;
      HFONT hOldFont = dc.SelectFont(m_Font);
      dc.GetTextMetrics(&m_tmFont);
      dc.SelectFont(hOldFont);
   }

   void _SetItemState(CButtonArray& aButtons, int iPos, UINT uAddState, UINT uRemoveState, DWORD dwTimeout)
   {
      // Set the state on the chosen index; clear the state from all other buttons...
      for( int i = 0; i < aButtons.GetSize(); i++ ) {
         RIBBONBUTTON& Btn = aButtons[i];
         if( i == iPos && (Btn.uCurState & uAddState) == 0 ) {
            // Switch to new state immediately...
            if( (_ResolveButtonState(Btn.uCurState) & uAddState) != 0 ) {
               Btn.dwStartTick = ::GetTickCount();
               Btn.dwTimeoutTick = Btn.dwStartTick + dwTimeout;
            }
            Btn.uOldState = Btn.uCurState;
            Btn.uCurState |= uAddState;
            InvalidateRect(&Btn.rcItem, FALSE);
         }
         if( i != iPos && (Btn.uCurState & uRemoveState) != 0 ) {
            // If currently shown button state is affected, we start a new animation
            if( (_ResolveButtonState(Btn.uCurState) & uRemoveState) != 0 ) {
               Btn.dwStartTick = ::GetTickCount();
               Btn.dwTimeoutTick = Btn.dwStartTick + dwTimeout;
               SetTimer(TIMERID_FADE, 25);
            }
            Btn.uOldState = Btn.uCurState;
            Btn.uCurState &= ~uRemoveState;
            InvalidateRect(&Btn.rcItem, FALSE);
         }
      }
   }

   bool _FindItemFromID(UINT nID, int& iBarPos, int& iSubPos) const
   {
      iBarPos = -1, iSubPos = -1;
      for( iBarPos = 0; iBarPos < m_Menu.GetMenuItemCount(); iBarPos++ ) {
         CMenuHandle menuBar = m_Menu.GetSubMenu(iBarPos);
         for( iSubPos = 0; iSubPos < menuBar.GetMenuItemCount(); iSubPos++ ) {
            if( menuBar.GetMenuItemID(iSubPos) == nID ) return true;
         }
      }
      return false;
   }

   bool _FindItemFromPos(POINT pt, int& iBarPos, int& iSubPos) const
   {
      iBarPos = -1, iSubPos = -1;
      for( iBarPos = 0; iBarPos < m_aBarButtons.GetSize(); iBarPos++ ) {
         if( ::PtInRect(&m_aBarButtons[iBarPos].rcItem, pt) ) return true;
      }
      iBarPos = m_iBarIndex;
      for( iSubPos = 0; iSubPos < m_aSubButtons.GetSize(); iSubPos++ ) {
         if( ::PtInRect(&m_aSubButtons[iSubPos].rcItem, pt) ) return true;
      }
      iBarPos = -1, iSubPos = -1;
      return false;
   }

   bool _StartTrackMouseLeave()
   {
      TRACKMOUSEEVENT tme = { sizeof(tme), TME_LEAVE, m_hWnd };
      return _TrackMouseEvent(&tme) == TRUE;
   }
};


#endif // __RIBBONCMDBAR_H__
