#include "MyWindow.h"
int APIENTRY WinMain( HINSTANCE hInstance, HINSTANCE, LPSTR, int )
{
   _Module.Init( NULL, hInstance );

   CMyWindow wnd;
   wnd.Create( NULL, CWindow::rcDefault, _T("Hello"),
      WS_OVERLAPPEDWINDOW|WS_VISIBLE );

   MSG msg;
   while( GetMessage( &msg, NULL, 0, 0 ) ){
      TranslateMessage( &msg );
      DispatchMessage( &msg );
   }

   _Module.Term();
   return msg.wParam;
}
