#define STRICT
#include <windows.h>
#include <tchar.h>
#include "CustCtrl.h"

static TCHAR szClassName[] = _T("CustCtrl345");

LRESULT CALLBACK CustWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

//
//	Custom Control state structure:
//
typedef struct
{
    COLORREF crForeGnd;    // Foreground text colour
    COLORREF crBackGnd;    // Background text colour
    HFONT    hFont;        // The font
    HWND     hwnd;         // The control's window handle
} CustCtrl;

//
// Get a state structure from the specified window
//
static CustCtrl * GetCustCtrl(HWND hwnd)
{
    return (CustCtrl *)GetWindowLong(hwnd, 0);
}

//
// Set a state structure for the specified window
//
void SetCustCtrl(HWND hwnd, CustCtrl *ccp)
{
    SetWindowLong(hwnd, 0, (LONG)ccp);
}

//
//	Register the custom control window class
//
void InitCustomControl()
{
    WNDCLASSEX wc;
    
    wc.cbSize         = sizeof(wc);
    wc.lpszClassName  = szClassName;
    wc.hInstance      = GetModuleHandle(0);
    wc.lpfnWndProc    = CustWndProc;
    wc.hCursor        = LoadCursor (NULL, IDC_ARROW);
    wc.hIcon          = 0;
    wc.lpszMenuName   = 0;
    wc.hbrBackground  = (HBRUSH)GetSysColorBrush(COLOR_BTNFACE);
    wc.style          = 0;
    wc.cbClsExtra     = 0;
    wc.cbWndExtra     = sizeof( CustCtrl * );
    wc.hIconSm        = 0;

    RegisterClassEx(&wc);
}

//
//	Create a custom control. Not necessary if
//  you are using a dialog box.
//
HWND CreateCustomControl(HWND hwndParent)
{
    HWND hwndCtrl;

    hwndCtrl = CreateWindowEx(
                 WS_EX_CLIENTEDGE, // give it a standard border
                 szClassName,
                 _T("A custom control"),
                 WS_VISIBLE | WS_CHILD,
                 0, 0, 100, 100,
                 hwndParent,
                 NULL, GetModuleHandle(0), NULL
               );

    return hwndCtrl;
}

//
//	WM_PAINT handler
//
static LRESULT CustCtrl_OnPaint(CustCtrl *ccp, WPARAM wParam, LPARAM lParam)
{
    HDC          hdc;
    PAINTSTRUCT  ps;
    HANDLE       hOldFont;
    TCHAR        szText[200];
    RECT         rect;
	SIZE		 sz;
	int			 x, y;

    // Get a device context for this window
    hdc = BeginPaint(ccp->hwnd, &ps);

    // Set the font we are going to use
    hOldFont = SelectObject(hdc, ccp->hFont);

    // Set the text colours
    SetTextColor(hdc, ccp->crForeGnd);
    SetBkColor  (hdc, ccp->crBackGnd);

    // Find the text to draw
    GetWindowText(ccp->hwnd, szText, sizeof(szText));
   
    // Work out where to draw
    GetClientRect(ccp->hwnd, &rect);

	// Find out how big the text will be
	GetTextExtentPoint32(hdc, szText, lstrlen(szText), &sz);

	// Center the text
	x = (rect.right  - sz.cx) / 2;
	y = (rect.bottom - sz.cy) / 2;
    
    // Draw the text
	ExtTextOut(hdc, x, y, ETO_OPAQUE, &rect, szText, lstrlen(szText), 0);

    // Restore the old font when we have finished
    SelectObject(hdc, hOldFont);

    // Release the device context
    EndPaint(ccp->hwnd, &ps);

    return 0;
}

//
//	WM_LBUTTONDOWN handler
//
static LRESULT CustCtrl_OnLButtonDown(CustCtrl *ccp, WPARAM wParam, LPARAM lParam)
{
    COLORREF col = RGB( rand()%256, rand()%256, rand()%256 );

	// Change the foreground colour
    ccp->crForeGnd = col;

    // Use the inverse of the foreground colour
	ccp->crBackGnd = ((~col) & 0x00ffffff);

    // Now redraw the control
    InvalidateRect(ccp->hwnd, NULL, FALSE);
    UpdateWindow(ccp->hwnd); 

    return 0;
}

//
//	WM_SETFONT handler
//
static LRESULT CustCtrl_OnSetFont(CustCtrl *ccp, WPARAM wParam, LPARAM lParam)
{
    // Change the font
    ccp->hFont = (HFONT)wParam; 
    return 0;
}

//
//	Custom Control window procedure
//
static LRESULT CALLBACK CustWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    // retrieve the custom structure POINTER for THIS window
    CustCtrl *ccp = GetCustCtrl(hwnd);

	switch(msg)
    {
    case WM_NCCREATE:
        
        // Allocate a new CustCtrl structure for this window.
        ccp = malloc( sizeof(CustCtrl) );
        
        // Failed to allocate, stop window creation.
        if(ccp == NULL) return FALSE;

        // Initialize the CustCtrl structure. 
        ccp->hwnd      = hwnd;
        ccp->crForeGnd = GetSysColor(COLOR_WINDOWTEXT);
        ccp->crBackGnd = GetSysColor(COLOR_WINDOW);
        ccp->hFont     = GetStockObject(DEFAULT_GUI_FONT);
           
		// Assign the window text as specified in the call to CreateWindow
		SetWindowText(ccp->hwnd, ((LPCREATESTRUCT)lParam)->lpszName);

		// Attach custom structure to this window.
        SetCustCtrl(hwnd, ccp);

        // Continue with window creation.
        return TRUE;

    // Clean up when the window is destroyed.
    case WM_NCDESTROY:
        free(ccp);
        break;

	// Draw the control
	case WM_PAINT:
		return CustCtrl_OnPaint(ccp, wParam, lParam);
	
	// Prevent flicker
	case WM_ERASEBKGND:
		return 1;

	// React to input
	case WM_LBUTTONDOWN:
		return CustCtrl_OnLButtonDown(ccp, wParam, lParam);

	// Support font changes
	case WM_SETFONT:
		return CustCtrl_OnSetFont(ccp, wParam, lParam);

	default:
        break;
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}