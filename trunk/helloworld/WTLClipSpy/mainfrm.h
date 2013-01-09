// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__47E80B25_1566_4A13_B4A9_7F8C49BAB4FC__INCLUDED_)
#define AFX_MAINFRM_H__47E80B25_1566_4A13_B4A9_7F8C49BAB4FC__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "ClipSpyListCtrl.h"
#include "MySplitterWindowT.h"
#include "MyPaneContainer.h"

class CMainFrame : public CFrameWindowImpl<CMainFrame>,
                   public CUpdateUI<CMainFrame>,
                   public CMessageFilter
{
public:
    DECLARE_FRAME_WND_CLASS(NULL, IDR_MAINFRAME)

    BEGIN_MSG_MAP(CMainFrame)
        MSG_WM_CREATE(OnCreate)
        MSG_WM_SETTINGCHANGE(OnSettingChange)
        COMMAND_ID_HANDLER_EX(ID_APP_EXIT, OnFileExit)
        COMMAND_ID_HANDLER_EX(ID_FILE_SAVE, OnFileSave)
        COMMAND_ID_HANDLER_EX(ID_VIEW_STATUS_BAR, OnViewStatusBar)
        COMMAND_ID_HANDLER_EX(ID_APP_ABOUT, OnAppAbout)
        COMMAND_ID_HANDLER_EX(IDC_SHOW_BOTTOM_PANE, OnShowBottomPane)
        COMMAND_ID_HANDLER_EX(IDC_HIDE_BOTTOM_PANE, OnHideBottomPane)
        COMMAND_ID_HANDLER_EX(ID_PANE_CLOSE, OnHideBottomPane)
        COMMAND_ID_HANDLER_EX(IDC_LOCK_SPLITTERS, OnLockSplitters)
        COMMAND_ID_HANDLER_EX(IDC_UNLOCK_PANES, OnUnlockSplitters)
        COMMAND_ID_HANDLER_EX(IDC_PATTERN_BARS, OnPatternBars)
        COMMAND_ID_HANDLER_EX(IDC_NORMAL_BARS, OnNormalBars)
        COMMAND_ID_HANDLER_EX(IDC_COLOR_CONTAINERS, OnColoredContainers)
        COMMAND_ID_HANDLER_EX(IDC_NORMAL_CONTAINERS, OnNormalContainers)
        NOTIFY_CODE_HANDLER_EX(LVN_ITEMCHANGED, OnListItemchanged)
        NOTIFY_CODE_HANDLER_EX(LVN_DELETEALLITEMS, OnListDeleteallitems)
        CHAIN_COMMANDS_MEMBER(m_wndFormatList)
        CHAIN_MSG_MAP(CUpdateUI<CMainFrame>)
        CHAIN_MSG_MAP(CFrameWindowImpl<CMainFrame>)
        REFLECT_NOTIFICATIONS()
    END_MSG_MAP()

    BEGIN_UPDATE_UI_MAP(CMainFrame)
        UPDATE_ELEMENT(ID_VIEW_STATUS_BAR, UPDUI_MENUPOPUP)
        UPDATE_ELEMENT(ID_FILE_SAVE, UPDUI_MENUPOPUP)
    END_UPDATE_UI_MAP()

    // Message handlers
    BOOL    PreTranslateMessage(MSG* pMsg);

    LRESULT OnCreate ( LPCREATESTRUCT lpcs );
    void    OnSettingChange ( UINT uWhatChanged, LPCTSTR szWhatChanged );

    // Command handlers
    void    OnFileExit ( UINT uCode, int nID, HWND hwndCtrl );
    void    OnFileSave ( UINT uCode, int nID, HWND hwndCtrl );
    void    OnViewStatusBar ( UINT uCode, int nID, HWND hwndCtrl );
    void    OnAppAbout ( UINT uCode, int nID, HWND hwndCtrl );

    void    OnShowBottomPane ( UINT uCode, int nID, HWND hwndCtrl );
    void    OnHideBottomPane ( UINT uCode, int nID, HWND hwndCtrl );
    void    OnLockSplitters ( UINT uCode, int nID, HWND hwndCtrl );
    void    OnUnlockSplitters ( UINT uCode, int nID, HWND hwndCtrl );
    void    OnPatternBars ( UINT uCode, int nID, HWND hwndCtrl );
    void    OnNormalBars ( UINT uCode, int nID, HWND hwndCtrl );
    void    OnColoredContainers ( UINT uCode, int nID, HWND hwndCtrl );
    void    OnNormalContainers ( UINT uCode, int nID, HWND hwndCtrl );

    // Notification handlers
    LRESULT OnListItemchanged ( NMHDR* phdr );
    LRESULT OnListDeleteallitems ( NMHDR* phdr );

    // Operations
    void ClearDataList();
    void AddDataBlock ( HGLOBAL hg, UINT cbyDataSize );
    void AddEmptyBlock();

protected:
    CMySplitterWindow     m_wndVertSplit;
    CMyHorSplitterWindow  m_wndHorzSplit;
    CMyPaneContainer      m_wndTreeContainer;
    CMyPaneContainer      m_wndIEContainer;

    CClipSpyListCtrl    m_wndFormatList;
    CRichEditCtrl       m_wndDataViewer;

    void CreateProgressCtrlInStatusBar ( CProgressBarCtrl& );
    
    std::vector<LPBYTE> m_vpbyDataBlocks;
    std::vector<UINT> m_vuDataBlockSizes;

    void ShowDataBlock ( int nBlockIndex );
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__47E80B25_1566_4A13_B4A9_7F8C49BAB4FC__INCLUDED_)
