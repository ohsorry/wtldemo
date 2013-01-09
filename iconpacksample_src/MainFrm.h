// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__797DA9CF_34A2_444A_8E8B_71EFBFE018DC__INCLUDED_)
#define AFX_MAINFRM_H__797DA9CF_34A2_444A_8E8B_71EFBFE018DC__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "RibbonCmdBar.h"

#include "ThemedDialog.h"
#include "ThemedLabel.h"
#include "ThemedImage.h"
#include "ThemedButton.h"
#include "ThemedCheckbox.h"
#include "ThemedHyperlink.h"
#include "ThemedRowListBox.h"
#include "ThemedIconListView.h"
#include "ThemedShadowStatic.h"

#include "IconPackagesDlg.h"
#include "IconsPreviewDlg.h"
#include "IconsCursorsDlg.h"
#include "SystemSettingsDlg.h"
#include "AboutDlg.h"


class CMainFrame : 
   public CFrameWindowImpl<CMainFrame>, 
   public CMessageFilter
{
public:
   DECLARE_FRAME_WND_CLASS_EX(_T("WTL_IconPackagerSample"), IDR_MAINFRAME, CS_HREDRAW | CS_VREDRAW, -1)

   CRibbonCmdBar m_CmdBar;
   CBitmap m_bmpBack;
   CBitmap m_bmpWatermark;
   CPoint m_ptMinTrackSize;

   virtual BOOL PreTranslateMessage(MSG* pMsg)
   {
      if( CFrameWindowImpl<CMainFrame>::PreTranslateMessage(pMsg) ) return TRUE;
      if( ::IsWindow(m_hWndClient) && ::SendMessage(m_hWndClient, WM_FORWARDMSG, 0, (LPARAM) pMsg) ) return TRUE;
      return FALSE;
   }

   BEGIN_MSG_MAP(CMainFrame)
      MESSAGE_HANDLER(WM_CREATE, OnCreate)
      MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
      MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBkgnd)
      MESSAGE_HANDLER(WM_GETMINMAXINFO, OnGetMinMaxInfo)
      COMMAND_RANGE_HANDLER(ID_VIEW__START, ID_VIEW__END, OnChangeView)
      CHAIN_MSG_MAP( CFrameWindowImpl<CMainFrame> )
   END_MSG_MAP()

   LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
   {
      //ModifyStyleEx(0, WS_EX_COMPOSITED);

      m_bmpBack = AtlLoadGdiplusImage(IDB_BACKGROUND, _T("PNG"));
      m_bmpWatermark = AtlLoadGdiplusImage(IDB_WATERMARK, _T("PNG"));

      // Create the Ribbon Command Bar control
      m_CmdBar.Create(m_hWnd, NULL, NULL, WS_CHILD | WS_VISIBLE);
      m_CmdBar.SetMenu(IDM_RIBBONMENU);

      // Create child view dialog
      AssignView< CIconPackagesDlg >(ID_VIEW_ICONPACKAGES);

      // Register object for message filtering
      CMessageLoop* pLoop = _Module.GetMessageLoop();
      ATLASSERT(pLoop!=NULL);
      pLoop->AddMessageFilter(this);

      // Resize window to a suitable initial size
      m_ptMinTrackSize.x = LOWORD( ::GetDialogBaseUnits() ) * 102;
      m_ptMinTrackSize.y = HIWORD( ::GetDialogBaseUnits() ) * 43;
      ResizeClient( m_ptMinTrackSize.x , m_ptMinTrackSize.y );

      ::ShowWindow(m_hWndClient, SW_SHOWNORMAL);
      return 0;
   }

   LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
   {
      // Unregister object for message filtering
      CMessageLoop* pLoop = _Module.GetMessageLoop();
      ATLASSERT(pLoop!=NULL);
      pLoop->RemoveMessageFilter(this);      
      bHandled = FALSE;
      return 0;
   }

   LRESULT OnEraseBkgnd(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
   {
      CDCHandle dc = (HDC) wParam;
      CRect rcClient;
      GetClientRect(&rcClient);
      CDC dcCompat;
      dcCompat.CreateCompatibleDC();
      // Stretch background over window client area
      HBITMAP hOldBmp = dcCompat.SelectBitmap(m_bmpBack);
      BITMAP BackInfo = { 0 };
      m_bmpBack.GetBitmap(&BackInfo);
      dc.StretchBlt(0, 0, rcClient.Width(), rcClient.Height(), dcCompat, 0, 0, BackInfo.bmWidth, BackInfo.bmHeight, SRCCOPY);
      // Also paint watermark at top-right corner
      dcCompat.SelectBitmap(m_bmpWatermark);
      BITMAP WaterInfo = { 0 };
      m_bmpWatermark.GetBitmap(&WaterInfo);
      const int WATERMARK_INDENT = 8;
      BLENDFUNCTION bf = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };
      CRect rcWater( rcClient.Width() - WaterInfo.bmWidth - WATERMARK_INDENT, WATERMARK_INDENT, rcClient.Width() - WATERMARK_INDENT, WATERMARK_INDENT + WaterInfo.bmHeight );
      dc.AlphaBlend(rcWater.left, rcWater.top, rcWater.Width(), rcWater.Height(), dcCompat, 0, 0, WaterInfo.bmWidth, WaterInfo.bmHeight, bf);       
      dcCompat.SelectBitmap(hOldBmp);
      return 1;
   }

   LRESULT OnGetMinMaxInfo(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
   {
      LPMINMAXINFO lpMMI = (LPMINMAXINFO) lParam;
      lpMMI->ptMinTrackSize =  m_ptMinTrackSize;
      return 0;
   }

   LRESULT OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
   {
      CPaintDC dc(m_hWnd);
      return 0;
   }

   LRESULT OnChangeView(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
   {
      CWindow wndOldView = m_hWndClient;
      switch( wID ) {
      case ID_VIEW_ICONPACKAGES:       AssignView< CIconPackagesDlg >(wID); break;
      case ID_VIEW_PREVIEW:            AssignView< CIconsPreviewDlg >(wID); break;
      case ID_VIEW_COLOURS:            AssignView< CIconPackagesDlg >(wID); break;
      case ID_VIEW_LIVEFOLDERS:        AssignView< CIconsPreviewDlg >(wID); break;
      case ID_VIEW_ICONS_DESKTOP:      AssignView< CIconsCursorsDlg >(wID); break;
      case ID_VIEW_ICONS_START:        AssignView< CIconsCursorsDlg >(wID); break;
      case ID_VIEW_ICONS_FOLDERS:      AssignView< CIconsCursorsDlg >(wID); break;
      case ID_VIEW_ICONS_DRIVES:       AssignView< CIconsCursorsDlg >(wID); break;
      case ID_VIEW_ICONS_OTHERS:       AssignView< CIconsCursorsDlg >(wID); break;
      case ID_VIEW_SETTINGS_SYSTEM:    AssignView< CSystemSettingsDlg >(wID); break;
      case ID_VIEW_SETTINGS_ADVANCED:  AssignView< CSystemSettingsDlg >(wID); break;
      case ID_VIEW_ABOUT:              AssignView< CAboutDlg >(wID); break;
      }
      UpdateLayout(FALSE);
      ::ShowWindow(m_hWndClient, SW_SHOWNORMAL);
      wndOldView.DestroyWindow();
      return 0;
   }

   // Implementation

   template< typename T >
   void AssignView(UINT wID) 
   { 
      T* p = new T;
      ATLASSERT(p);
      if( p == NULL ) return;
      p->SetInitCommand(wID);
      m_hWndClient = p->Create(m_hWnd); 
      ATLASSERT(::IsWindow(m_hWndClient));
      ATLASSERT(!::IsWindowVisible(m_hWndClient));  // Disable WS_VISIBLE
   }

   void UpdateLayout(BOOL bResizeBars = TRUE)
   {
      CRect rcClient;
      GetClientRect(&rcClient);
      CRect rcBarOrig;
      m_CmdBar.GetWindowRect(&rcBarOrig);
      CRect rcNavBar(0, 0, rcClient.Width(), rcBarOrig.Height());
      if( bResizeBars ) m_CmdBar.SetWindowPos(NULL, &rcNavBar, SWP_NOACTIVATE | SWP_NOZORDER);
      rcClient.top += rcBarOrig.Height();
      ::SetWindowPos(m_hWndClient, NULL, rcClient.left, rcClient.top, rcClient.Width(), rcClient.Height(), SWP_NOACTIVATE | SWP_NOZORDER);
   }
};


#endif // AFX_MAINFRM_H__797DA9CF_34A2_444A_8E8B_71EFBFE018DC__INCLUDED_
