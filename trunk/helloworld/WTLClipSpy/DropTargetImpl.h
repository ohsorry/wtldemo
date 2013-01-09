// DropTargetImpl.h: interface for the CDropTargetImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DROPTARGETIMPL_H__19B8F137_070D_4375_BA0D_9B8B9263CC33__INCLUDED_)
#define AFX_DROPTARGETIMPL_H__19B8F137_070D_4375_BA0D_9B8B9263CC33__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

struct __declspec(uuid("4657278B-411B-11d2-839A-00C04FD918D0")) IDropTargetHelper;

template <class T>
class CDropTargetImpl : public CComObjectRootEx<CComSingleThreadModel>,
                        public CComCoClass<T>,
                        public IDropTarget
{
public:
    BEGIN_COM_MAP(CDropTargetImpl<T>)
        COM_INTERFACE_ENTRY(IDropTarget)
    END_COM_MAP()

    static CDropTargetImpl<T>* CreateMe ( T* pT )
    {
    CComObject< CDropTargetImpl<T> >* pObj;

        if ( FAILED( CComObject< CDropTargetImpl<T> >::CreateInstance ( &pObj ) ))
            return NULL;
        
        pObj->AddRef();
        pObj->m_pDropTargetWindow = pT;

        return pObj;
    }

    void InitializeDT()
    {
    CComQIPtr<IDropTarget> pDT = this;

        RegisterDragDrop ( m_pDropTargetWindow->m_hWnd, pDT );

        m_pDropHelper.CoCreateInstance ( CLSID_DragDropHelper, NULL, CLSCTX_INPROC_SERVER );
    }
    
    void RevokeDT()
    {
        RevokeDragDrop ( m_pDropTargetWindow->m_hWnd );
    }

    // IDropTarget
    STDMETHOD(DragEnter)( IDataObject* pDataObj, DWORD grfKeyState,
                          POINTL pt, DWORD* pdwEffect )
    {
    DWORD dwEffect = DROPEFFECT_COPY | DROPEFFECT_LINK;

        if ( m_pDropHelper )
            m_pDropHelper->DragEnter ( m_pDropTargetWindow->m_hWnd, 
                                       pDataObj, &(POINT&) pt, dwEffect );

        *pdwEffect = dwEffect;
        return S_OK;

    }
        
    STDMETHOD(DragOver)( DWORD grfKeyState, POINTL pt, DWORD* pdwEffect )
    {
    DWORD dwEffect = DROPEFFECT_COPY | DROPEFFECT_LINK;

        if ( m_pDropHelper )
            m_pDropHelper->DragOver ( &(POINT&) pt, dwEffect );

        *pdwEffect = dwEffect;
        return S_OK;

    }
        
    STDMETHOD(DragLeave)()
    {
        if ( m_pDropHelper )
            m_pDropHelper->DragLeave();

        return S_OK;
    }

    STDMETHOD(Drop)( IDataObject* pDataObj, DWORD grfKeyState,
                     POINTL pt, DWORD* pdwEffect )
    {
    DWORD dwEffect = DROPEFFECT_COPY;

        if ( m_pDropHelper )
            m_pDropHelper->Drop ( pDataObj, &(POINT&) pt, dwEffect );

        return m_pDropTargetWindow->OnDrop ( pDataObj, pdwEffect );
    }

protected:
    CDropTargetImpl()
    {
        // only creatable thru CreateMe()
    }

    CComPtr<IDropTargetHelper> m_pDropHelper;
    T* m_pDropTargetWindow;
};

#endif // !defined(AFX_DROPTARGETIMPL_H__19B8F137_070D_4375_BA0D_9B8B9263CC33__INCLUDED_)
