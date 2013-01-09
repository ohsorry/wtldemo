
#pragma once


//////////////////////////////////////////////////////////////////////
// Themed Checkbox control
//

class CThemedCheckboxCtrl : 
   public CWindowImpl<CThemedCheckboxCtrl, CButton>, 
   public CCustomDraw< CThemedCheckboxCtrl >
{
public:
   CWindow m_wndThemeParent;
   CBrush m_brBack;

   // Operations

   BOOL SubclassWindow(HWND hWnd)
   {
      ATLASSERT(m_hWnd==NULL);
      ATLASSERT(::IsWindow(hWnd));
      BOOL bRet = CWindowImpl<CThemedCheckboxCtrl, CButton>::SubclassWindow(hWnd);
      if( bRet ) Init();
      return bRet;
   }

   void Init()
   {
      m_wndThemeParent = GetTopLevelWindow();
   }

   void SetThemeParent(HWND hWnd)
   {
      ATLASSERT(::IsWindow(hWnd));
      m_wndThemeParent = hWnd;
      if( !m_brBack.IsNull() ) m_brBack.DeleteObject();
   }

   // Message map and handlers

   BEGIN_MSG_MAP(CThemedCheckboxCtrl)
      MESSAGE_HANDLER(WM_CREATE, OnCreate)
      MESSAGE_HANDLER(WM_MOVE, OnSize)
      MESSAGE_HANDLER(WM_SIZE, OnSize)
      MESSAGE_HANDLER(OCM_CTLCOLORSTATIC, OnCtlColor)
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

   LRESULT OnCtlColor(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
   {
      if( m_brBack.IsNull() ) m_brBack = AtlGetBackgroundBrush(m_hWnd, m_wndThemeParent);
      CDCHandle dc = (HDC) wParam;
      dc.SetBkMode(TRANSPARENT);
      dc.SetTextColor(RGB(0,0,0));
      if( !IsWindowEnabled() ) dc.SetTextColor(RGB(40,40,40));
      return (LRESULT) (HBRUSH) m_brBack;
   }
};
