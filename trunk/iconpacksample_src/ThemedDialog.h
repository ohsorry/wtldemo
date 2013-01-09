
#pragma once


//////////////////////////////////////////////////////////////////////
// Themed Dialog
//

template< typename T >
class CThemedDialogImpl : public CDialogImpl<T>
{
public:
   CFont m_fontNormal;
   CBrush m_brBack;
   UINT m_wCmd;

   // Operations

   void ThemedDialog_Init()
   {
      ATLASSERT(IsWindow());
      // The WS_EX_COMPOSITED enabled double-buffering and
      // prevents flicker.
      ModifyStyleEx(0, WS_EX_COMPOSITED);
      // Create and set a slightly larger font on all controls
      CLogFont lf;
      lf.SetMessageBoxFont();
      lf.lfHeight = -14;
      m_fontNormal.CreateFontIndirect(&lf);
      SendMessage(WM_SETFONT, (WPARAM) (HFONT) m_fontNormal, 0L);
      SendMessageToDescendants(WM_SETFONT, (WPARAM) (HFONT) m_fontNormal, 0L);
   }

   void SetInitCommand(UINT wCmd)
   {
      // The dialog doesn't have access to the Command Bar control, but we
      // note the ID_VIEW_xxx command that spawned it.
      m_wCmd = wCmd;
   }

   // Implementation

   void OnFinalMessage(HWND /*hWnd*/)
   {
      delete static_cast<T*>(this);
   }

   // Message map and handlers

   BEGIN_MSG_MAP(CThemedDialogImpl)
      MESSAGE_HANDLER(WM_FORWARDMSG, OnForwardMsg)
      MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBkgnd)
      MESSAGE_HANDLER(WM_SIZE, OnSize)
   END_MSG_MAP()

   LRESULT OnForwardMsg(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
   {
      return IsDialogMessage((LPMSG)lParam);
   }

   LRESULT OnSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
   {
      if( !m_brBack.IsNull() ) m_brBack.DeleteObject();
      bHandled = FALSE;
      return TRUE;
   }

   LRESULT OnEraseBkgnd(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
   {
      CDCHandle dc = (HDC) wParam;
      CRect rcClient;
      GetClientRect(&rcClient);
      if( m_brBack.IsNull() ) m_brBack = AtlGetBackgroundBrush(m_hWnd, GetParent());
      HBRUSH hOldBrush = dc.SelectBrush(m_brBack);
      dc.PatBlt(rcClient.left, rcClient.top, rcClient.Width(), rcClient.Height(), PATCOPY);
      dc.SelectBrush(hOldBrush);
      return 1;
   }
};

