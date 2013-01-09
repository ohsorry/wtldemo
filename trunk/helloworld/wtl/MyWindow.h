
#include "atl_inc.h"
CComModule _Module;
#include "resource.h"
#include "AboutDialog.h"

template<class T, COLORREF t_crBrushColor>
class CPaintBkgnd : public CMessageMap
{
public:
	CPaintBkgnd(){m_hbrBkgnd = CreateSolidBrush(t_crBrushColor);}
	~CPaintBkgnd(){DeleteObject(m_hbrBkgnd);}
	BEGIN_MSG_MAP(CPaintBkgnd)
		MESSAGE_HANDLER(WM_ERASEBKGND,OnEraseBkgnd)
	END_MSG_MAP()

	LRESULT OnEraseBkgnd(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL&)
	{
		T* pt = static_cast<T*>(this);
		HDC hdc = (HDC)wParam;
		RECT rcClient;

		pt->GetClientRect(&rcClient);
		FillRect(hdc,&rcClient,m_hbrBkgnd);
		return 1;
	}

protected:
	HBRUSH m_hbrBkgnd;
};

class CMyWindow :	public CWindowImpl<CMyWindow> ,
					public CPaintBkgnd<CMyWindow,RGB(1,1,255)>
{
public:
	typedef CPaintBkgnd<CMyWindow, RGB(1,1,255)> CPaintBkgndBase;
	DECLARE_WND_CLASS(_T("XXX"))
   BEGIN_MSG_MAP( CMyWindow )
      MESSAGE_HANDLER( WM_PAINT, OnPaint )
      MESSAGE_HANDLER( WM_DESTROY, OnDestroy )
	  MESSAGE_HANDLER(WM_NCPAINT, OnNcPaint)
	  MESSAGE_HANDLER(WM_CREATE, OnCreate)
	  COMMAND_ID_HANDLER(ID_ABOUT, OnAbout)
	  CHAIN_MSG_MAP(CPaintBkgndBase)
   END_MSG_MAP()
 
   LRESULT OnCreate(UINT,WPARAM,LPARAM, BOOL&)
   {
	   HMENU hMenu = LoadMenu(_Module.GetResourceInstance(),MAKEINTRESOURCE(IDR_MENU1));
	   SetMenu(hMenu);
	   return 0;
   }
   LRESULT OnPaint( UINT, WPARAM, LPARAM, BOOL& ){
      PAINTSTRUCT ps;
      HDC hDC = GetDC();
      BeginPaint( &ps );
      TextOut( hDC, 0, 0, _T("Hello world"), 11 );
      EndPaint( &ps );
      return 0;
   }
 
   LRESULT OnDestroy( UINT, WPARAM, LPARAM, BOOL& ){
      PostQuitMessage( 0 );
      return 0;
   }
   LRESULT OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam,BOOL& bHandled)
   {

		HBRUSH hBrush;
		hBrush=(HBRUSH)GetStockObject(BLACK_BRUSH);
		HDC hdc;  
		hdc = GetWindowDC(); 
		// Paint into this DC 
		RECT rcWin;  
		GetWindowRect(&rcWin);  
		OffsetRect( &rcWin, -rcWin.left, -rcWin.top );
		for(int i=0; i<3; i++)  
		{ 
			FrameRect(hdc, &rcWin, hBrush);
			InflateRect(&rcWin, -1, -1);
		} 
		ReleaseDC(hdc);
		return 0;
   }
   LRESULT OnAbout(UINT uNotify,int id, HWND hwnd,BOOL &bHandled)
   {
	   //MessageBoxA(NULL, "","",MB_OK);
	   AboutDialog dig;
	   dig.DoModal();
	   return 0;
   }

};