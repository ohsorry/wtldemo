// MainFrm.cpp : implmentation of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "aboutdlg.h"
#include "MainFrm.h"

static const TCHAR HEXCHAR[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8',
                                 '9', 'A', 'B', 'C', 'D', 'E', 'F' };

/////////////////////////////////////////////////////////////////////////////
// Message handlers

BOOL CMainFrame::PreTranslateMessage(MSG* pMsg)
{
    return CFrameWindowImpl<CMainFrame>::PreTranslateMessage(pMsg);
}

LRESULT CMainFrame::OnCreate ( LPCREATESTRUCT lpcs )
{
    CreateSimpleStatusBar();
    UISetCheck(ID_VIEW_STATUS_BAR, 1);
    UIEnable(ID_FILE_SAVE, false);

    // register object for message filtering and idle updates
    CMessageLoop* pLoop = _Module.GetMessageLoop();
    ATLASSERT(pLoop != NULL);
    pLoop->AddMessageFilter(this);

    // Create the splitter windows.
const DWORD dwSplitStyle = WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;

    m_wndHorzSplit.Create ( *this, rcDefault, NULL, dwSplitStyle );
    m_wndVertSplit.Create ( m_wndHorzSplit, rcDefault, NULL, dwSplitStyle );

    // Create the pane containers.
    m_wndTreeContainer.Create ( m_wndVertSplit, IDS_LIST_HEADER );
    m_wndIEContainer.Create ( m_wndHorzSplit, _T("") );

    // Create the left pane (list of clip formats)
    m_wndFormatList.Create ( m_wndTreeContainer, rcDefault );

    // Create the right pane (rich edit ctrl)
const DWORD dwRichEditStyle = WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS |
                                WS_HSCROLL | WS_VSCROLL | WS_BORDER |
                                ES_READONLY | ES_AUTOHSCROLL | ES_AUTOVSCROLL | ES_MULTILINE;

    m_wndDataViewer.Create ( m_wndVertSplit, rcDefault, NULL, dwRichEditStyle );

    m_wndDataViewer.SetFont ( AtlGetStockFont ( ANSI_FIXED_FONT ) );

    // Create the bottom pane (browser)
CAxWindow wndIE;
const DWORD dwIEStyle = WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN |
                        WS_HSCROLL | WS_VSCROLL,
            dwIEExStyle = WS_EX_CLIENTEDGE;

    wndIE.Create ( m_wndIEContainer, rcDefault, _T("http://www.google.com"),
                   dwIEStyle, dwIEExStyle );

    // Set the horizontal splitter as the client area window.
    m_hWndClient = m_wndHorzSplit;

    // Set up the pane containers
    m_wndTreeContainer.SetClient ( m_wndFormatList );
    m_wndIEContainer.SetClient ( wndIE );
    m_wndTreeContainer.SetPaneContainerExtendedStyle ( PANECNT_NOCLOSEBUTTON );
    m_wndIEContainer.SetPaneContainerExtendedStyle ( PANECNT_VERTICAL );

    // Set up the splitter panes
    m_wndHorzSplit.SetSplitterPanes ( m_wndVertSplit, m_wndIEContainer );
    m_wndVertSplit.SetSplitterPanes ( m_wndTreeContainer, m_wndDataViewer );
    
    // Set the splitter as the client area window, and resize the splitter 
    // to match the frame size.
    UpdateLayout();

    m_wndVertSplit.m_cxyMin = 150;
    m_wndHorzSplit.m_cxyMin = 100;
    
    m_wndVertSplit.SetSplitterPos ( 200 );
    m_wndHorzSplit.SetSplitterExtendedStyle ( SPLIT_BOTTOMALIGNED );
    m_wndHorzSplit.SetSplitterPos();

    // Init the data format list.
    m_wndFormatList.Init();

    return 0;
}

void CMainFrame::OnSettingChange ( UINT uWhatChanged, LPCTSTR szWhatChanged )
{
    m_wndVertSplit.SendMessage ( WM_SETTINGCHANGE );
}


/////////////////////////////////////////////////////////////////////////////
// Command handlers

void CMainFrame::OnViewStatusBar ( UINT uCode, int nID, HWND hwndCtrl )
{
BOOL bVisible = !::IsWindowVisible(m_hWndStatusBar);

    ::ShowWindow(m_hWndStatusBar, bVisible ? SW_SHOWNOACTIVATE : SW_HIDE);
    UISetCheck(ID_VIEW_STATUS_BAR, bVisible);

    UpdateLayout();
}

void CMainFrame::OnFileExit ( UINT uCode, int nID, HWND hwndCtrl )
{
    PostMessage(WM_CLOSE);
}

void CMainFrame::OnFileSave ( UINT uCode, int nID, HWND hwndCtrl )
{
int nSelItem = m_wndFormatList.GetSelectedIndex();
CFileDialog dlg ( false, _T("bin"), NULL, 
                  OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST,
                  _T("All files (*.*)\0*.*\0") );

    if ( IDOK == dlg.DoModal() )
        {
        DWORD dwWritten = 0;
        HANDLE hFile = CreateFile ( dlg.m_szFileName, GENERIC_WRITE, 0, NULL,
                                    CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );

        if ( INVALID_HANDLE_VALUE != hFile )
            {
            WriteFile ( hFile, m_vpbyDataBlocks[nSelItem],
                        m_vuDataBlockSizes[nSelItem], &dwWritten, NULL );

            CloseHandle ( hFile );
            }
        }
}

void CMainFrame::OnAppAbout ( UINT uCode, int nID, HWND hwndCtrl )
{
CAboutDlg dlg;

    dlg.DoModal();
}

void CMainFrame::OnShowBottomPane ( UINT uCode, int nID, HWND hwndCtrl )
{
    m_wndHorzSplit.SetSinglePaneMode ( SPLIT_PANE_NONE );
}

void CMainFrame::OnHideBottomPane ( UINT uCode, int nID, HWND hwndCtrl )
{
    m_wndHorzSplit.SetSinglePaneMode ( SPLIT_PANE_TOP );
}

void CMainFrame::OnLockSplitters ( UINT uCode, int nID, HWND hwndCtrl )
{
    m_wndHorzSplit.SetSplitterExtendedStyle ( SPLIT_NONINTERACTIVE,
                                              SPLIT_NONINTERACTIVE );

    m_wndVertSplit.SetSplitterExtendedStyle ( SPLIT_NONINTERACTIVE,
                                              SPLIT_NONINTERACTIVE );
}

void CMainFrame::OnUnlockSplitters ( UINT uCode, int nID, HWND hwndCtrl )
{
    m_wndHorzSplit.SetSplitterExtendedStyle ( 0, SPLIT_NONINTERACTIVE );
    m_wndVertSplit.SetSplitterExtendedStyle ( 0, SPLIT_NONINTERACTIVE );
}

void CMainFrame::OnPatternBars ( UINT uCode, int nID, HWND hwndCtrl )
{
    m_wndVertSplit.m_cxySplitBar = 10;
    m_wndHorzSplit.m_cxySplitBar = 12;

    m_wndVertSplit.EnablePatternBar();
    m_wndHorzSplit.EnablePatternBar();
}

void CMainFrame::OnNormalBars ( UINT uCode, int nID, HWND hwndCtrl )
{
    m_wndVertSplit.m_cxySplitBar = 4;
    m_wndHorzSplit.m_cxySplitBar = 4;

    m_wndVertSplit.EnablePatternBar(false);
    m_wndHorzSplit.EnablePatternBar(false);
}

void CMainFrame::OnColoredContainers ( UINT uCode, int nID, HWND hwndCtrl )
{
    m_wndTreeContainer.EnableColoredHeader();
    m_wndIEContainer.EnableColoredHeader();
}

void CMainFrame::OnNormalContainers ( UINT uCode, int nID, HWND hwndCtrl )
{
    m_wndTreeContainer.EnableColoredHeader(false);
    m_wndIEContainer.EnableColoredHeader(false);
}


/////////////////////////////////////////////////////////////////////////////
// Notification handlers

LRESULT CMainFrame::OnListItemchanged ( NMHDR* phdr )
{
NMLISTVIEW* pNMLV = (NMLISTVIEW*) phdr;

    // If the change wasn't in the state of an item, then we don't have to
    // do anything.
    if ( 0 == ( pNMLV->uChanged & LVIF_STATE ))
        return 0;

    if ( pNMLV->uNewState & LVIS_SELECTED )
        {
        // The item is now selected, so tell the rich edit view to show the
        // associated data.
        ShowDataBlock ( pNMLV->iItem );
        UIEnable ( ID_FILE_SAVE, m_vuDataBlockSizes[pNMLV->iItem] > 0 );
        }
    else
        {
        // The item is unselected, so empty out the rich edit view.  We can't
        // just call Clear() since that makes the system beep (because the edit
        // ctrl is read-only).
        m_wndDataViewer.SetRedraw ( FALSE );
        m_wndDataViewer.SetSel ( 0, -1 );
        m_wndDataViewer.ReplaceSel ( _T("") );
        m_wndDataViewer.SetRedraw();
        m_wndDataViewer.Invalidate();

        UIEnable ( ID_FILE_SAVE, false );
        }

    return 0;                           // retval ignored.
}

LRESULT CMainFrame::OnListDeleteallitems ( NMHDR* phdr )
{
    UIEnable ( ID_FILE_SAVE, false );
    return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// Operations

void CMainFrame::ClearDataList()
{
    while ( m_vpbyDataBlocks.size() > 0 )
        {
        LPBYTE pby = m_vpbyDataBlocks[0];

        if ( NULL != pby )
            delete [] pby;

        m_vpbyDataBlocks.erase ( m_vpbyDataBlocks.begin() );
        }

    m_vuDataBlockSizes.clear();

    // Empty the rich edit view.  We can't just call Clear() since that 
    // makes the system beep (because the edit ctrl is read-only).
    m_wndDataViewer.SetRedraw ( FALSE );
    m_wndDataViewer.SetSel ( 0, -1 );
    m_wndDataViewer.ReplaceSel ( _T("") );
    m_wndDataViewer.SetRedraw();
    m_wndDataViewer.Invalidate();
}

void CMainFrame::AddDataBlock ( HGLOBAL hg, UINT cbyDataSize )
{
LPBYTE pbyData = new BYTE[cbyDataSize];
void* pvData;

    if ( NULL == pbyData )
        {
        // out of memory, add an empty block instead
        AddEmptyBlock();
        return;
        }

    pvData = GlobalLock ( hg );
    
    if ( NULL == pvData )
        {
        // out of memory, add an empty block instead
        delete [] pbyData;
        AddEmptyBlock();
        return;
        }

    CopyMemory ( pbyData, pvData, cbyDataSize );

    GlobalUnlock ( hg );
    m_vpbyDataBlocks.push_back ( pbyData );
    m_vuDataBlockSizes.push_back ( cbyDataSize );
}

void CMainFrame::AddEmptyBlock()
{
    m_vpbyDataBlocks.push_back ( NULL );
    m_vuDataBlockSizes.push_back(0);
}


/////////////////////////////////////////////////////////////////////////////
// Other methods

void CMainFrame::ShowDataBlock ( int nBlockIndex )
{
CWaitCursor w;                          // this might take a while...
CProgressBarCtrl wndProgress;
const BYTE* pbyData = m_vpbyDataBlocks[nBlockIndex];
const UINT  uBytesPerLine = 16;
UINT        uDataSize = m_vuDataBlockSizes[nBlockIndex];
UINT        uOffset;
UINT        uLineOffset;
UINT        uBytesInThisLine;
TCHAR       szLine[uBytesPerLine*4+(uBytesPerLine/4)+13];   // enough space for a line
size_t      idx;
CString     sGroupOfLines;
UINT        uLinesInGroup = 0;
const UINT  uLinesPerGroup = 100;       // # of lines to add to the edit ctrl at once
double      dProgressStep, dNextUpdateOffset;

    // Get the block size, and calculate stuff for the progress bar.
    dProgressStep = uDataSize / 10.0;
    dNextUpdateOffset = dProgressStep;

    CreateProgressCtrlInStatusBar ( wndProgress );

    m_wndDataViewer.SetRedraw ( FALSE );

    for ( uOffset = 0; uOffset < uDataSize; uOffset += uBytesPerLine )
        {
        // Update progress bar if necessary.
        if ( uOffset >= dNextUpdateOffset )
            {
            dNextUpdateOffset += dProgressStep;
            wndProgress.StepIt();
            }

        // How many bytes will be in the next line?  Max of 16.
        uBytesInThisLine = uDataSize - uOffset;

        if ( uBytesInThisLine > uBytesPerLine )
            uBytesInThisLine = uBytesPerLine;

        // Init our line buffer to all spaces.
        for ( UINT c = 0; c < countof(szLine); c++ )
            szLine[c] = _T(' ');

        // First part of the line - the starting offset.
        UINT u = uOffset;
        idx = 0;

        for ( int z = 0; z < 8; z++, u <<= 4 )
            szLine[idx++] = HEXCHAR[(u & 0xF0000000) >> 28];

        idx += 2;

        // Now loop through the data and add on the hex value of each byte.
        for ( uLineOffset = 0; uLineOffset < uBytesInThisLine; uLineOffset++ )
            {
            BYTE by = pbyData[uOffset + uLineOffset];

            szLine[idx++] = HEXCHAR[(by & 0xF0) >> 4];
            szLine[idx++] = HEXCHAR[by & 0xF];
            idx++;

            if ( uLineOffset % 4 == 3 )
                idx++;
            }

        // If there were less than 16 bytes in this line, pad the line with
        // spaces so the ASCII representation will be in the right column.
        if ( uBytesInThisLine < uBytesPerLine )
            {
            idx += 1 + 3 * (uBytesPerLine - uBytesInThisLine);

            if ( uBytesInThisLine < 4 )
                idx++;
            }

        // Add on the ASCII representation
        for ( uLineOffset = 0; uLineOffset < uBytesInThisLine; uLineOffset++ )
            {
            // If the next byte isn't printable, show a period instead.
            if ( _istprint ( pbyData[uOffset + uLineOffset] ))
                szLine[idx++] = (TCHAR) pbyData[uOffset + uLineOffset];
            else
                szLine[idx++] = '.';
            }

        szLine[idx++] = '\n';
        szLine[idx++] = '\0';

        if ( 0 == ( ++uLinesInGroup % uLinesPerGroup ) )
            {
            uLinesInGroup = 0;
            m_wndDataViewer.ReplaceSel ( sGroupOfLines );
            sGroupOfLines = _T("");
            }
        else
            sGroupOfLines += szLine;
        }

    if ( !sGroupOfLines.IsEmpty() )
        m_wndDataViewer.ReplaceSel ( sGroupOfLines );

    m_wndDataViewer.SetSel(0,0);
    m_wndDataViewer.ScrollCaret();
    m_wndDataViewer.SetRedraw ( TRUE );
    m_wndDataViewer.Invalidate();
    
    wndProgress.DestroyWindow();
    CStatusBarCtrl(m_hWndStatusBar).SetText ( 0, CString(LPCTSTR(ATL_IDS_IDLEMESSAGE)) );
}

// Create a progress bar in the main frame's status bar.  Code taken from
// "The MFC Answer Book" and tweaked a bit.
void CMainFrame::CreateProgressCtrlInStatusBar ( CProgressBarCtrl& wndProgressBar )
{
CStatusBarCtrl wndStatusBar = m_hWndStatusBar;
LPCTSTR szMessage = _T("Reading data");
CRect   rc;

    wndStatusBar.GetRect ( 0, &rc );

CClientDC   dc ( wndStatusBar );
CFontHandle hFont = wndStatusBar.GetFont();
CFontHandle hOldFont = dc.SelectFont ( hFont );
CSize       sizeText;

    dc.GetTextExtent ( szMessage, lstrlen(szMessage), &sizeText );
    dc.SelectFont ( hOldFont );

    rc.left += sizeText.cx + 10;

    wndStatusBar.SetText ( 0, szMessage );
    wndStatusBar.RedrawWindow();

    wndProgressBar.Create ( wndStatusBar, rc, NULL, WS_CHILD|WS_VISIBLE );
    wndProgressBar.SetRange ( 0, 90 );
    wndProgressBar.SetPos(0);
    wndProgressBar.SetStep(10);
}
