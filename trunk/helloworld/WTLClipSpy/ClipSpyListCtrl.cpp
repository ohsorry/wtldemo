// ClipSpyListCtrl.cpp: implementation of the CClipSpyListCtrl class.//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "ClipSpyListCtrl.h"
#include "mainfrm.h"

extern CMainFrame* g_pMainWnd;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction

CClipSpyListCtrl::CClipSpyListCtrl() : m_pDropTarget(NULL)
{
    m_mapFormatNames[CF_TEXT] = _T("CF_TEXT");
    m_mapFormatNames[CF_BITMAP] = _T("CF_BITMAP");
    m_mapFormatNames[CF_METAFILEPICT] = _T("CF_METAFILEPICT");
    m_mapFormatNames[CF_SYLK] = _T("CF_SYLK");
    m_mapFormatNames[CF_DIF] = _T("CF_DIF");
    m_mapFormatNames[CF_TIFF] = _T("CF_TIFF");
    m_mapFormatNames[CF_OEMTEXT] = _T("CF_OEMTEXT");
    m_mapFormatNames[CF_DIB] = _T("CF_DIB");
    m_mapFormatNames[CF_PALETTE] = _T("CF_PALETTE");
    m_mapFormatNames[CF_PENDATA] = _T("CF_PENDATA");
    m_mapFormatNames[CF_RIFF] = _T("CF_RIFF");
    m_mapFormatNames[CF_WAVE] = _T("CF_WAVE");
    m_mapFormatNames[CF_UNICODETEXT] = _T("CF_UNICODETEXT");
    m_mapFormatNames[CF_ENHMETAFILE] = _T("CF_ENHMETAFILE");
    m_mapFormatNames[CF_HDROP] = _T("CF_HDROP");
    m_mapFormatNames[CF_LOCALE] = _T("CF_LOCALE");
    m_mapFormatNames[CF_DIBV5] = _T("CF_DIBV5");
    m_mapFormatNames[CF_OWNERDISPLAY] = _T("CF_OWNERDISPLAY");
    m_mapFormatNames[CF_DSPTEXT] = _T("CF_DSPTEXT");
    m_mapFormatNames[CF_DSPBITMAP] = _T("CF_DSPBITMAP");
    m_mapFormatNames[CF_DSPMETAFILEPICT] = _T("CF_DSPMETAFILEPICT");
    m_mapFormatNames[CF_DSPENHMETAFILE] = _T("CF_DSPENHMETAFILE");
}

CClipSpyListCtrl::~CClipSpyListCtrl()
{
}


//////////////////////////////////////////////////////////////////////
// Message handlere

void CClipSpyListCtrl::OnChangeCBChain ( HWND hwndRemove, HWND hwndNext )
{
    // If the next window in the chain is being removed, reset our
    // "next window" handle.
    if ( m_hwndNextClipboardViewer == hwndRemove )
        m_hwndNextClipboardViewer = hwndNext;

    // If there is a next clipboard viewer, pass the message on to it.
    if ( NULL != m_hwndNextClipboardViewer )
        {
        ::SendMessage ( m_hwndNextClipboardViewer, WM_CHANGECBCHAIN, 
                        (WPARAM) hwndRemove, (LPARAM) hwndNext );
        }
}

void CClipSpyListCtrl::OnDrawClipboard()
{
    // Note the m_bCallingSetClipboardViewer flag.  This flag is set before
    // the call to SetClipboardViewer().  We get a WM_DRAWCLIPBOARD message
    // before SetClipboardViewer() returns, but we must not process it.
    // So we don't do anything if that flag is set.

    if ( !m_bCallingSetClipboardViewer )
        {
        // If there is a next clipboard viewer, pass the message on to it.
        if ( NULL != m_hwndNextClipboardViewer )
            ::SendMessage ( m_hwndNextClipboardViewer, WM_DRAWCLIPBOARD, 0, 0 );

        // Read the contents of the clipboard.
        ReadClipboard();
        }
}

void CClipSpyListCtrl::OnDestroy()
{
    if ( NULL != m_hwndNextClipboardViewer )
        ChangeClipboardChain ( m_hwndNextClipboardViewer );

    if ( m_pDropTarget )
        {
        m_pDropTarget->RevokeDT();
        m_pDropTarget->Release();
        }
}

DWORD CClipSpyListCtrl::OnPrePaint(int /*idCtrl*/, LPNMCUSTOMDRAW /*lpNMCustomDraw*/)
{
    return CDRF_NOTIFYITEMDRAW;
}

DWORD CClipSpyListCtrl::OnItemPrePaint(int /*idCtrl*/, LPNMCUSTOMDRAW pNMCD )
{
    if ( pNMCD->lItemlParam > 0 )
        ((NMLVCUSTOMDRAW*) pNMCD)->clrText = GetSysColor ( COLOR_GRAYTEXT );

    return CDRF_DODEFAULT;
}

HRESULT CClipSpyListCtrl::OnDrop ( IDataObject* pDataObj, DWORD* pdwEffect )
{
    ReadDataAndFillList ( pDataObj );

    *pdwEffect = DROPEFFECT_COPY | DROPEFFECT_LINK;
    return S_OK;
}


//////////////////////////////////////////////////////////////////////
// Command handlers

void CClipSpyListCtrl::OnReadClipboard ( UINT uCode, int nID, HWND hwndCtrl )
{
    ReadClipboard();
}

void CClipSpyListCtrl::OnClearClipboard ( UINT uCode, int nID, HWND hwndCtrl )
{
    if ( OpenClipboard() )
        {
        EmptyClipboard();
        CloseClipboard();
        }
}


//////////////////////////////////////////////////////////////////////
// Other methods

void CClipSpyListCtrl::Init()
{
    SetExtendedListViewStyle ( LVS_EX_FULLROWSELECT );

    InsertColumn ( 0, _T("Format"), LVCFMT_LEFT, 0, 0 );
    InsertColumn ( 1, _T("Data size"), LVCFMT_LEFT, 0, 1 );

    SetColumnWidth ( 0, LVSCW_AUTOSIZE_USEHEADER );
    SetColumnWidth ( 1, LVSCW_AUTOSIZE_USEHEADER );

    // Register this window as a clipboard viewer.  This makes us get the
    // WM_DRAWCLIPBOARD and WM_CHANGECBCHAIN messages.  The 
    // m_bCallingSetClipboardViewer is necessary because SetClipboardViewer()
    // sends us a WM_DRAWCLIPBOARD but we must not process it.  See
    // OnDrawClipboard() for more details.
    m_bCallingSetClipboardViewer = true;
    m_hwndNextClipboardViewer = SetClipboardViewer();
    m_bCallingSetClipboardViewer = false;

    // Init our drop target object
    m_pDropTarget = CDropTargetImpl<CClipSpyListCtrl>::CreateMe ( this );

    if ( m_pDropTarget )
        m_pDropTarget->InitializeDT();

    // Read the stuff on the clipboard, if there is any.
    ReadClipboard();
}

void CClipSpyListCtrl::ReadClipboard()
{
CComPtr<IDataObject> pDO;

    if ( FAILED( OleGetClipboard ( &pDO ) ))
        return;

    ReadDataAndFillList ( pDO );
}

// ReadDataAndFillList(): Reads the data from the passed-in COleDataObject
// and fills in the list control with the available formats.
void CClipSpyListCtrl::ReadDataAndFillList ( IDataObject* pDO )
{
FORMATETC etc;
TCHAR     szFormat[256];
HGLOBAL   hgData;
UINT      uDataSize;
CString   sSize;
int       nItem = 0;

    // Empty the list control and the doc.
    DeleteAllItems();
    g_pMainWnd->ClearDataList();

    // Get all the data and pass it to the main frame for storage.
CComPtr<IEnumFORMATETC> pEnumFormats;

    pDO->EnumFormatEtc ( DATADIR_GET, &pEnumFormats );

    while ( S_OK == pEnumFormats->Next ( 1, &etc, NULL ) )
        {
        bool bReadData = false;
        STGMEDIUM stg = {0};
        HRESULT hr;

        if ( FAILED( pDO->QueryGetData ( &etc ) ))
            continue;

        // See if this is a built-in clipboard format.  If so, we already have
        // a description string for it - we just have to find it in the 
        // clip format name map.  If we don't find the format in the map,
        // then it's a custom format, and we need to get the name from Windows.
        CClipFormatNameMap::const_iterator it;

        it = m_mapFormatNames.find ( etc.cfFormat );

        if ( it != m_mapFormatNames.end() )
            lstrcpyn ( szFormat, it->second, countof(szFormat) );
        else
            GetClipboardFormatName ( etc.cfFormat, szFormat, countof(szFormat) );

        // Get an HGLOBAL or IStream on the next data format.
        etc.tymed |= TYMED_HGLOBAL | TYMED_ISTREAM;

        hr = pDO->GetData ( &etc, &stg );

        if ( SUCCEEDED(hr) )
            {
            // If the clipboard returned an HGLOBAL, read the data from the
            // global memory object.
            if ( stg.tymed & TYMED_HGLOBAL )
                {
                hgData = stg.hGlobal;

                if ( NULL != hgData )
                    {
                    uDataSize = GlobalSize ( hgData );

                    sSize.Format ( _T("%u (0x%X)"), uDataSize, uDataSize );
                    g_pMainWnd->AddDataBlock ( hgData, uDataSize );

                    bReadData = true;
                    }
                }

            // If the clipboard returned an IStream, read all the data from 
            // the stream.
            if ( !bReadData && (stg.tymed & TYMED_ISTREAM) )
                {
                LARGE_INTEGER li;
                ULARGE_INTEGER uli;

                li.QuadPart = 0;

                if ( SUCCEEDED( stg.pstm->Seek ( li, STREAM_SEEK_END, &uli )))
                    {
                    HGLOBAL hg = GlobalAlloc ( GMEM_MOVEABLE | GMEM_SHARE,
                                               uli.LowPart );
                    void* pv = GlobalLock ( hg );

                    stg.pstm->Seek ( li, STREAM_SEEK_SET, NULL );
                    
                    if ( SUCCEEDED( stg.pstm->Read ( pv, uli.LowPart, (PULONG) &uDataSize )))
                        {
                        GlobalUnlock ( hg );

                        sSize.Format ( _T("%u (0x%X)"), uDataSize, uDataSize );
                        g_pMainWnd->AddDataBlock ( hg, uDataSize );
                    
                        // Free the memory we just allocated.
                        GlobalFree ( hg );
    
                        bReadData = true;
                        }
                    else
                        GlobalUnlock ( hg );
                    }
                }

            ReleaseStgMedium ( &stg );
            }   // end if (GetData() succeeded)

        InsertItem ( nItem, szFormat );
        
        if ( bReadData )
            {
            SetItemText ( nItem, 1, sSize );
            }
        else
            {
            // If we couldn't get the data for this format, set the item data 
            // to 1 so that the NM_CUSTOMDRAW handler will draw the text in grey.
            g_pMainWnd->AddEmptyBlock();
            SetItemData ( nItem, 1 );

            if ( FAILED(hr) )
                SetItemText ( nItem, 1, _T("<Data unavailable>") );
            else
                SetItemText ( nItem, 1, _T("<Not displayable format>") );
            }

        nItem++;
        }   // end while

    SetColumnWidth ( 0, LVSCW_AUTOSIZE_USEHEADER );
    SetColumnWidth ( 1, LVSCW_AUTOSIZE_USEHEADER );
}
