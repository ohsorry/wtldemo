// ==========================================================================
// MainDlg.h : 
// ==========================================================================
#ifndef ___MAINDLG_H___
#define ___MAINDLG_H___
#pragma once


class CMainDlg : public CDialog
{
public:
	// Construction
	CMainDlg(CWnd* pParent = NULL);	

protected:	
	// Dialog Data
	enum { IDD = IDD_MAIN_DIALOG };

	HICON m_hIcon; // Main Icon

	// Rubber Band/Window Selection Properties
	HCURSOR		m_hSelectCursor;
	BOOL		m_bMouseHook;
	BOOL		m_bKeyboardHook;
	BOOL		m_bDrawing;
	BOOL		m_bSelecting;
	CDC*		m_pDesktopDC;	
	BOOL		m_bDitherLast;
	CPoint		m_ptFirst;
	CPoint		m_ptLast;
	HWND		m_hHilightWnd;
	BOOL		m_bStayInTray; 
	int			m_iBandPx;
	HBITMAP		m_hTmpDraw;
	CLSID		m_clsidImgType;

	// Properties
	CString		m_strImagePath;
	CString		m_strImageName;
	BOOL		m_bMinToTray;
	int			m_iNamingOpt;

	// Tray Icon 
	NOTIFYICONDATA	m_NID;

	// Hooks
	static HHOOK m_hMouseHook;
	static HHOOK m_hKeyboardHook;
	static CMainDlg* m_pThis;

public:
	// Overrides
	virtual BOOL OnInitDialog();

	// Persistence
	void ReadRegistrySettings();
	void WriteRegistrySettings();

protected:
	// Mouse Hook Notifications
	void OnMouseHookLButtonDown(UINT nFlags, CPoint point);
	void OnMouseHookLButtonUp(UINT nFlags, CPoint point);
	void OnMouseHookMouseMove(UINT nFlags, CPoint point);
	void OnMouseHookRButtonDown(UINT nFlags, CPoint point);

	// Helper Functions
	CString GetOutputFileName();
	BOOL ActivateSelectionHook(BOOL bActivate);
	BOOL ActivateKeyboardHook(BOOL bActivate);
	void DrawFocusRect(CPoint pt, BOOL bRemoveRect = FALSE);
	HBITMAP GetScreenshot(HWND hWnd, CRect* pRect = NULL);
	HBITMAP CropDDB(HBITMAP hSrcBmp, CRect rCrop);
	int GetEncoderClsid(const WCHAR* format, CLSID* pClsid); // MSDN Example
	BOOL SaveImageAs(HBITMAP hBmp, CString strFile); 
	//BOOL SaveImageAsJpeg(HBITMAP hBmp, CString strFile); 
	void UpdateToolTip(CString strToolTip);
	void HighlightWindow(HWND hWnd);
	void RefreshWindow(HWND hWnd);
	void GetControlValues();
	void SetControlValues();

	// Windows Messages
	DECLARE_MESSAGE_MAP()
	afx_msg void OnDestroy();
	afx_msg void OnClose();
	afx_msg void OnSize(UINT nType, int cx, int cy);

	// Control Messages
	afx_msg void OnCbnSelchangeImageExt();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnBnClickedScreenDraw();
	afx_msg void OnBnClickedWindowSelect();
	afx_msg void OnBnClickedBrowseImgPath();
	afx_msg void OnEnKillfocusLineWidth();

	// Our Messages
	LRESULT OnDrawComplete(WPARAM wParam, LPARAM lParam);
	LRESULT OnSelectionComplete(WPARAM wParam, LPARAM lParam);
	LRESULT OnMouseHookCancel(WPARAM wParam, LPARAM lParam);
	LRESULT OnPrintScreen(WPARAM wParam, LPARAM lParam);
	LRESULT OnTrayNotification(WPARAM wParam, LPARAM lParam);

	// Tray Menu Items
	afx_msg void OnTrayShow();
	afx_msg void OnShutdown();
	afx_msg void OnShowAbout();
	afx_msg void OnDrawSelection();
	afx_msg void OnWindowSelect();

	// Hook Callback
	static LRESULT CALLBACK LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);
};


#endif // ___MAINDLG_H___
