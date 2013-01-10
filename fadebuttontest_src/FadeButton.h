#if !defined(AFX_FADEBUTTON_H__20020625_3FF0_6A9B_6234_0080AD509054__INCLUDED_)
#define AFX_FADEBUTTON_H__20020625_3FF0_6A9B_6234_0080AD509054__INCLUDED_

/////////////////////////////////////////////////////////////////////////////
// Fade Button - A DIB button control
//
// Written by Bjarke Viksoe (bjarke@viksoe.dk)
// Copyright (c) 2002 Bjarke Viksoe.
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

#pragma once

#ifndef __cplusplus
  #error WTL requires C++ compilation (use a .cpp suffix)
#endif

#ifndef __ATLAPP_H__
  #error This control requires atlapp.h to be included first
#endif

#ifndef __ATLCTRLS_H__
  #error This control requires atlctrls.h to be included first
#endif

#ifndef __ATLGDIX_H__
  #error This control requires my atlgdix.h header to be included first
#endif

#ifndef __ATLDIB_H__
  #error This control requires my atldib.h header to be included first
#endif

#ifndef __ATLDIBFILTERS_H__
  #error This control requires my atldibfilters.h header to be included first
#endif


// From Platform SDK (Windows 5.0 only)
#ifndef ODS_HOTLIGHT
#define ODS_HOTLIGHT 0x0040
#define ODS_INACTIVE 0x0080
#endif


/////////////////////////////////////////////////////////////////////////////
// DIB Fade button

template< class T, class TBase = CButton, class TWinTraits = CControlWinTraits >
class ATL_NO_VTABLE CFadeButtonImpl : 
   public CWindowImpl< T, TBase, TWinTraits >,
   public COwnerDraw< T >,
   public CMouseHover< T >
{
public:
   DECLARE_WND_SUPERCLASS(NULL, TBase::GetWndClassName())

   enum { FADE_TIMERID = 33 };

   CToolTipCtrl m_tip;
   DWORD m_dwTickStart;
   DWORD m_dwDelay;
   DWORD m_dwSteps;
   COLORREF m_clrText;
   UINT m_uState;
   bool m_bAnimating;

   // Images
   CDib24 m_dibCurrent;
   CDib24 m_dibNormal;
   CDib24 m_dibOver;

   CFadeButtonImpl() : 
      m_dwDelay(300UL),
      m_dwSteps(4UL),
      m_uState(0U),
      m_bAnimating(false)
   {
   }

   // Operations
   
   BOOL SubclassWindow(HWND hWnd)
   {
      BOOL bRet = CWindowImpl< T, TBase, TWinTraits >::SubclassWindow(hWnd);
      if( bRet ) _Init();
      return bRet;
   }
   BOOL SetToolTipText(LPCTSTR pstrText)
   {
      ATLASSERT(::IsWindow(m_hWnd));
      if( pstrText==NULL ) {
         if( m_tip.IsWindow() ) m_tip.Activate(FALSE);
         return TRUE;
      }
      m_tip.Activate(TRUE);
      m_tip.AddTool(m_hWnd, pstrText);
      return TRUE;
   }
   BOOL SetImages(UINT iNormal, UINT iOver)
   {
      if( !m_dibNormal.LoadBitmap(iNormal) ) return FALSE;
      if( !m_dibOver.LoadBitmap(iOver) ) return FALSE;
      return TRUE;
   }
   void SetTextColor(COLORREF clrText)
   {
      ATLASSERT(::IsWindow(m_hWnd));
      m_clrText = clrText;
      Invalidate();
   }
   void SetDelay(DWORD dwDelay, DWORD dwSteps)
   {
      ATLASSERT(dwDelay!=0 && dwSteps!=0);
      ATLASSERT(dwSteps<dwDelay);
      m_dwDelay = dwDelay;
      m_dwSteps = dwSteps;
   }

   // COwnerDraw

   void DrawItem(LPDRAWITEMSTRUCT lpDIS)
   {
      // Offscreen paint
      RECT rc = lpDIS->rcItem;
      CMemDC dc(lpDIS->hDC, &rc);

      // Paint black frame around buttons with focus
      if( lpDIS->itemState & ODS_FOCUS ) {
         HPEN hOldPen = dc.SelectStockPen(BLACK_PEN);
         HBRUSH hOldBrush = dc.SelectStockBrush(HOLLOW_BRUSH);
         dc.Rectangle(&rc);
         ::InflateRect(&rc, -1, -1);
         dc.SelectPen(hOldPen);
         dc.SelectBrush(hOldBrush);
      }

      // Paint the button canvas
      UINT uFrame = DFCS_BUTTONPUSH;
      if( lpDIS->itemState & ODS_SELECTED ) uFrame |= DFCS_PUSHED;
      if( lpDIS->itemState & ODS_DISABLED ) uFrame |= DFCS_INACTIVE;
      dc.DrawFrameControl(&rc, DFC_BUTTON, uFrame);

      RECT rcFocus = rc;
      ::InflateRect(&rcFocus, -3, -3);

      // Calculate a new image if needed.
      // Only change image if the state has changed from last time
      // we painted it...
      UINT uNewState = lpDIS->itemState;
      if( uNewState & ODS_DISABLED ) {
         if( (m_uState & ODS_DISABLED)==0 ) {
            m_dibCurrent = m_dibNormal;
            CGreyScaleImageFilter::Process(&m_dibCurrent);
         }
      }
      else if( uNewState & ODS_SELECTED ) {
         if( (m_uState & ODS_SELECTED)==0 ) m_dibCurrent = m_dibOver;
      }
      else if( m_fMouseOver ) {
         if( m_bAnimating  ) {
            // Create a new image (faded between the two bitmaps)
            DWORD dwTick = ::GetTickCount();
            short iPercent = (short)( (100UL * (dwTick - m_dwTickStart)) / m_dwDelay );
            if( iPercent > 100 ) iPercent = 100;
            m_dibCurrent = m_dibNormal;
            CFadeImageFilter::Process(&m_dibCurrent, &m_dibOver, iPercent);
         }
         else {
            m_dibCurrent = m_dibOver;
         }
         // Ok, it's actually a new button style...
         uNewState |= ODS_HOTLIGHT;
      }
      else {
         m_dibCurrent = m_dibNormal;
      }
      m_uState = uNewState;

      // Calculate where we should paint text and image
      DWORD dwStyle = GetStyle();  
      RECT rcImage = rc;
      int xOffset = 0;
      int yOffset = ( (rcImage.bottom-rcImage.top) - m_dibCurrent.GetHeight() )/2;
      if( dwStyle & BS_LEFT ) {
         xOffset = 4;
         rc.left += m_dibCurrent.GetWidth() + 8;
      }
      else {
         xOffset = rc.right-m_dibCurrent.GetWidth() - 4;
         rc.right -= m_dibCurrent.GetWidth() + 8;
      }

      // Draw image
      RECT rcText = rc;
      if( lpDIS->itemState & ODS_SELECTED ) {
         xOffset++;
         yOffset++;
         ::OffsetRect(&rcText, 1, 1);
      }
      m_dibCurrent.Draw(dc, rcImage.left + xOffset, rcImage.top + yOffset);
      
      // Draw text
      TCHAR szText[64] = { 0 };
      GetWindowText(szText, (sizeof(szText)/sizeof(TCHAR))-1);
      HFONT hOldFont = dc.SelectFont(GetFont());
      dc.SetBkMode(TRANSPARENT);
      dc.SetTextColor(m_clrText);
      dc.DrawText(szText, -1, &rcText, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
      dc.SelectFont(hOldFont);

      if( lpDIS->itemState & ODS_FOCUS ) dc.DrawFocusRect(&rcFocus);
   }

   // Message map and handlers

   typedef CFadeButtonImpl< T, TBase, TWinTraits > thisClass;

   BEGIN_MSG_MAP(thisClass)
      CHAIN_MSG_MAP( CMouseHover< T > )
      CHAIN_MSG_MAP_ALT( COwnerDraw<T>, 1 )
      MESSAGE_HANDLER(WM_CREATE, OnCreate)
      MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBkgnd)
      MESSAGE_HANDLER(WM_LBUTTONDBLCLK, OnLButtonDblClick)
      MESSAGE_HANDLER(WM_MOUSEENTER, OnMouseEnter)
      MESSAGE_RANGE_HANDLER(WM_MOUSEFIRST, WM_MOUSELAST, OnMouseMessage)
      MESSAGE_HANDLER(WM_TIMER, OnTimer)
   END_MSG_MAP()

   LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
   {      
      LRESULT lRes = DefWindowProc();
      _Init();
      return lRes;
   }
   LRESULT OnEraseBkgnd(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
   {
      return 1;
   }
   LRESULT OnLButtonDblClick(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
   {
      // HACK: Ideally we would remove the CS_DBLCLCKS class style, but since we're
      //       subclassing the control we're forced to make it act like a regular click.
      //       Otherwise double-clicks will not show the button as pushed.
      return DefWindowProc(WM_LBUTTONDOWN, wParam, lParam);
   }   
   LRESULT OnMouseMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
   {
      if( m_tip.IsWindow() ) {
         MSG msg = { m_hWnd, uMsg, wParam, lParam };
         m_tip.RelayEvent(&msg);
      }
      bHandled = FALSE;
      return 0;
   }
   LRESULT OnMouseEnter(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
   {
      // Don't fade on displays with 256 colours or less...
      CWindowDC dc(NULL);
      if( dc.GetDeviceCaps(BITSPIXEL) <= 8 ) return 0;
      // Start fade
      m_dwTickStart = ::GetTickCount();
      SetTimer(FADE_TIMERID, m_dwDelay / m_dwSteps);
      m_bAnimating = true;
      return 0;
   }
   LRESULT OnTimer(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
   {
      if( wParam != FADE_TIMERID ) {
         bHandled = FALSE;
         return 0;
      }
      DWORD dwTick = ::GetTickCount();
      if( dwTick - m_dwTickStart > m_dwDelay ) {
         KillTimer(FADE_TIMERID);
         m_bAnimating = false;
      }
      Invalidate();
      UpdateWindow();
      return 0;
   }

   // Implementation
   
   void _Init()
   {
      // We need this style to prevent Windows from painting the button
      ModifyStyle(0x0F | BS_NOTIFY, BS_OWNERDRAW);
      
      // Create a tool tip
      m_tip.Create(m_hWnd);
      ATLASSERT(m_tip.IsWindow());

      // Set other data members
      m_clrText = ::GetSysColor(COLOR_BTNTEXT);
      m_dibCurrent = m_dibNormal;
   }
};

class CFadeButtonCtrl : public CFadeButtonImpl<CFadeButtonCtrl>
{
public:
   DECLARE_WND_SUPERCLASS(_T("WTL_FadeButton"), GetWndClassName())
};


#endif // !defined(AFX_FADEBUTTON_H__20020625_3FF0_6A9B_6234_0080AD509054__INCLUDED_)

