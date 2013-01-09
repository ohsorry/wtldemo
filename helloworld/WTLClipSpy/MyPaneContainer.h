// MyPaneContainer.h: interface for the CMyPaneContainer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYPANECONTAINER_H__1B6F91E9_7C08_4075_A9D1_AA29D0E882C4__INCLUDED_)
#define AFX_MYPANECONTAINER_H__1B6F91E9_7C08_4075_A9D1_AA29D0E882C4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMyPaneContainer : public CPaneContainerImpl<CMyPaneContainer>
{
public:
    DECLARE_WND_CLASS_EX(_T("My_PaneContainer"), 0, -1)

    CMyPaneContainer() : m_bColoredHeader(false)
    { }

    // Operations
    void EnableColoredHeader ( bool bEnable = true );

    // Overrides
    BOOL GetToolTipText(LPNMHDR lpnmh);
    void DrawPaneTitle(CDCHandle dc);

    // NOTE: There is a bug in CPaneContainer that prevents this from being
    // called after you switch the container to vertical mode. Change
    // CalcSize() to pT->CalcSize() in CPaneContainer::SetPaneContainerExtendedStyle()
    void CalcSize() { m_cxyHeader = 40; }

protected:
    bool m_bColoredHeader;
};

#endif // !defined(AFX_MYPANECONTAINER_H__1B6F91E9_7C08_4075_A9D1_AA29D0E882C4__INCLUDED_)
