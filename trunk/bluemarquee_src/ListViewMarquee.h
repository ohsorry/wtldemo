#if !defined(AFX_LISTVIEWMARQUEE_H__20051016_6048_AD90_AD55_0080AD509054__INCLUDED_)
#define AFX_LISTVIEWMARQUEE_H__20051016_6048_AD90_AD55_0080AD509054__INCLUDED_

#pragma once

/////////////////////////////////////////////////////////////////////////////
// CListViewMarquee - The Windows Explorer selection rectangle
//
// Written by Bjarke Viksoe (bjarke@viksoe.dk)
// Copyright (c) 2005 Bjarke Viksoe.
//
// Add a member...
//   CListViewMarquee m_ListMarquee;
// then initialize...
//   m_ListMarquee.Install(hwndList);
// and add the following macro to the parent's message map:
//   CHAIN_MSG_MAP_ALT_MEMBER( m_ListMarquee, 1 )
//   REFLECT_NOTIFICATIONS()
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
//
#ifndef __cplusplus
   #error ATL requires C++ compilation (use a .cpp suffix)
#endif

#ifndef __ATLAPP_H__
   #error ListViewMarquee.h requires atlapp.h to be included first
#endif

#if _WIN32_WINNT < 0x0501
   #error ListViewMarquee.h requires Windows XP or 2000 setup
#endif


#ifndef BlendRGB
   #define BlendRGB(c1, c2, factor) \
      RGB( GetRValue(c1) + ((GetRValue(c2) - GetRValue(c1)) * factor / 100L), \
           GetGValue(c1) + ((GetGValue(c2) - GetGValue(c1)) * factor / 100L), \
           GetBValue(c1) + ((GetBValue(c2) - GetBValue(c1)) * factor / 100L) )
#endif


typedef CWinTraits<WS_POPUPWINDOW, WS_EX_TRANSPARENT | WS_EX_LAYERED | WS_EX_TOPMOST | WS_EX_TOOLWINDOW> CListViewMarqueeTraits;

class CListViewMarquee : public CWindowImpl< CListViewMarquee, CWindow, CListViewMarqueeTraits >
{
public:
   DECLARE_WND_CLASS_EX(_T("ListViewMarquee"), CS_OWNDC, -1);

   UINT m_uCtrlID;
   CListViewCtrl m_ctrlList;
   POINT m_ptOrigin;
   POINT m_ptOffset;
   RECT m_rcSelection;

   CListViewMarquee() : m_uCtrlID(0)
   {
   }

   BEGIN_MSG_MAP(CListViewMarquee)
      MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
      MESSAGE_HANDLER(WM_LBUTTONUP, OnLButtonUp)
      MESSAGE_HANDLER(WM_PAINT, OnPaint)
      MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBkGnd)
      MESSAGE_HANDLER(WM_CAPTURECHANGED, OnCaptureChanged)
   ALT_MSG_MAP( 1 )
      NOTIFY_HANDLER(m_uCtrlID, LVN_MARQUEEBEGIN, OnMarqueeBegin);
   END_MSG_MAP()

   void Install(HWND hWndList)
   {
      m_ctrlList = hWndList;
      m_uCtrlID = m_ctrlList.GetDlgCtrlID();
      ATLASSERT((m_ctrlList.GetStyle()&LVS_SINGLESEL)==0);
      // Create floating window
      Create(m_ctrlList, rcDefault);
      ATLASSERT(IsWindow());
      // Really don't want these attributes
      ModifyStyle(WS_VISIBLE | WS_BORDER /*| WS_CAPTION*/ | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 0);
      // Set alpha layer
      const BYTE AlphaPercent = 60;
      ::SetLayeredWindowAttributes(m_hWnd, RGB(255,0,0), AlphaPercent, LWA_COLORKEY | LWA_ALPHA);
	  ShowWindow(TRUE);
   }

   LRESULT OnCaptureChanged(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
   {
      // Reset canvas now to reduce flicker
      ::ZeroMemory(&m_rcSelection, sizeof(RECT));
      Invalidate();
      UpdateWindow();
      // Hide it again...
      ShowWindow(FALSE);
      return 0;
   }
   LRESULT OnMouseMove(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
   {
      POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
      ::SetRect(&m_rcSelection, pt.x, pt.y, m_ptOrigin.x, m_ptOrigin.y);
      if( m_rcSelection.left > m_rcSelection.right ) {
         INT temp = m_rcSelection.left;
         m_rcSelection.left = m_rcSelection.right;
         m_rcSelection.right = temp;
      }
      if( m_rcSelection.top > m_rcSelection.bottom ) {
         INT temp = m_rcSelection.top;
         m_rcSelection.top = m_rcSelection.bottom;
         m_rcSelection.bottom = temp;
      }
      RECT rcTemp, rcItem;
      int nCount = m_ctrlList.GetItemCount();
      for( int i = 0; i < nCount; i++ ) {
         // Get the item's rectangle. Here we ask for the icon area, but
         // hit-testing against the entire item rectangle could be an alternative
         m_ctrlList.GetItemRect(i, &rcItem, LVIR_ICON);
         ::OffsetRect(&rcItem, m_ptOffset.x, m_ptOffset.y);
         UINT uState = m_ctrlList.GetItemState(i, LVIS_SELECTED);
         if( ::IntersectRect(&rcTemp, &rcItem, &m_rcSelection) ) {
            if( uState == 0 ) m_ctrlList.SetItemState(i, LVIS_SELECTED, LVIS_SELECTED);
         }
         else {
            if( uState != 0 ) m_ctrlList.SetItemState(i, 0, LVIS_SELECTED);
         }
      }
      Invalidate();
      m_ctrlList.UpdateWindow();
      return 0;
   }
   LRESULT OnLButtonUp(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
   {
      ::ReleaseCapture();
      return 0;
   }
   LRESULT OnEraseBkGnd(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
   {
      return 1;
   }
   LRESULT OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
   {
      CPaintDC dc = m_hWnd;
      RECT rcClient;
      GetClientRect(&rcClient);
      CBrush brBack;
      brBack.CreateSolidBrush(RGB(0,0,0));
      dc.FillRect(&rcClient, brBack);
      COLORREF clrFill = ::GetSysColor(COLOR_HIGHLIGHT);
      COLORREF clrBorder = BlendRGB(clrFill, RGB(0,255,0), 80);
      CPen penBlue;
      CBrush brBlue;
      brBlue.CreateSolidBrush(clrFill);
      penBlue.CreatePen(PS_SOLID, 1, clrBorder);
      HPEN hOldPen = dc.SelectPen(penBlue);
      HBRUSH hOldBrush = dc.SelectBrush(brBlue);
      dc.Rectangle(&m_rcSelection);
      dc.SelectPen(hOldPen);
      dc.SelectBrush(hOldBrush);
      return 0;
   }

   LRESULT OnMarqueeBegin(int /*idCtrl*/, LPNMHDR pnmh, BOOL& /*bHandled*/)
   {
      LPNMLISTVIEW pnmLV = (LPNMLISTVIEW) pnmh;
      POINT pt = pnmLV->ptAction;
      m_ptOrigin = pt;
      m_ptOffset.x = m_ptOffset.y = 0;
      ::SetRect(&m_rcSelection, pt.x, pt.y, pt.x, pt.y);
      RECT rcWindow;
      m_ctrlList.GetWindowRect(&rcWindow);
      // We'll need to adjust the client rectangle to avoid painting on the
      // scrollbars and headers. I wish there was a ListView method for this.
      if( m_ctrlList.GetStyle() & WS_HSCROLL ) rcWindow.bottom -= ::GetSystemMetrics(SM_CYHSCROLL);
      if( m_ctrlList.GetStyle() & WS_VSCROLL ) rcWindow.right -= ::GetSystemMetrics(SM_CXVSCROLL);
      if( m_ctrlList.GetView() & LVS_REPORT ) {
         CHeaderCtrl ctrlHeader = m_ctrlList.GetHeader();
         if( ctrlHeader.IsWindow() ) {
            RECT rcHeader;
            ctrlHeader.GetWindowRect(&rcHeader);
            rcWindow.top += rcHeader.bottom - rcHeader.top;
            m_ptOffset.y = -(rcHeader.bottom - rcHeader.top);
         }
      }
      // Place the window and ask it to repaint at once...
      // The SWP_NOACTIVATE flag is important to avoid flicker.
      SetWindowPos(HWND_TOPMOST, &rcWindow, SWP_SHOWWINDOW | SWP_NOACTIVATE | SWP_DEFERERASE);
      SendMessage(WM_MOUSEMOVE, 0, MAKELPARAM(pt.x, pt.y));
      // We need all the mouse-events we can get...
      SetCapture();
      ATLASSERT(GetCapture()==m_hWnd);      
      return 1L;  // We shouldn't allow the default marquee to display
   } 
};


#endif // !defined(AFX_LISTVIEWMARQUEE_H__20051016_6048_AD90_AD55_0080AD509054__INCLUDED_)
