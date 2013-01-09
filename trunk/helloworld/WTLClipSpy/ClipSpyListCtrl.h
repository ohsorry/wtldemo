// ClipSpyListCtrl.h: interface for the CClipSpyListCtrl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CLIPSPYLISTCTRL_H__71076F4E_4901_4E6F_B6E8_708741A683DC__INCLUDED_)
#define AFX_CLIPSPYLISTCTRL_H__71076F4E_4901_4E6F_B6E8_708741A683DC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DropTargetImpl.h"

typedef CWinTraitsOR<WS_BORDER | LVS_REPORT | LVS_SINGLESEL | 
                       LVS_NOSORTHEADER | LVS_SHOWSELALWAYS> CListTraits;

class CClipSpyListCtrl : public CWindowImpl<CClipSpyListCtrl, CListViewCtrl, CListTraits>,
                         public CCustomDraw<CClipSpyListCtrl>
{
public:
    DECLARE_WND_SUPERCLASS(NULL, WC_LISTVIEW)

    CClipSpyListCtrl();
    ~CClipSpyListCtrl();

    BEGIN_MSG_MAP(CClipSpyListCtrl)
        MSG_WM_CHANGECBCHAIN(OnChangeCBChain)
        MSG_WM_DRAWCLIPBOARD(OnDrawClipboard)
        MSG_WM_DESTROY(OnDestroy)
        COMMAND_ID_HANDLER_EX(IDC_READ_CLIPBOARD, OnReadClipboard)
        COMMAND_ID_HANDLER_EX(IDC_CLEAR_CLIPBOARD, OnClearClipboard)
        CHAIN_MSG_MAP_ALT(CCustomDraw<CClipSpyListCtrl>, 1)
        DEFAULT_REFLECTION_HANDLER()
    END_MSG_MAP()

    // Message handlers
    void OnChangeCBChain ( HWND hwndRemove, HWND hwndNext );
    void OnDrawClipboard();
    void OnDestroy();

    // Command handlers
    void OnReadClipboard ( UINT uCode, int nID, HWND hwndCtrl );
    void OnClearClipboard ( UINT uCode, int nID, HWND hwndCtrl );

    // Notification handlers
    DWORD OnPrePaint(int /*idCtrl*/, LPNMCUSTOMDRAW /*lpNMCustomDraw*/);
    DWORD OnItemPrePaint(int /*idCtrl*/, LPNMCUSTOMDRAW pNMCD );

    // IDropTarget
    HRESULT OnDrop ( IDataObject* pDataObj, DWORD* pdwEffect );

    // Other methods
    void Init();
    void ReadClipboard();
    void ReadDataAndFillList ( IDataObject* pDO );

    HWND m_hwndNextClipboardViewer;
    bool m_bCallingSetClipboardViewer;

    CClipFormatNameMap m_mapFormatNames;
    CDropTargetImpl<CClipSpyListCtrl>* m_pDropTarget;
};

#endif // !defined(AFX_CLIPSPYLISTCTRL_H__71076F4E_4901_4E6F_B6E8_708741A683DC__INCLUDED_)
