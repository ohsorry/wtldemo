#ifndef _MYLISTVIEW_H
#define _MYLISTVIEW_H
#include "atlcrack.h"
class MyListView : public CWindowImpl<MyListView, CListViewCtrl>,
                   public CCustomDraw<MyListView>                   
{
public:

#if 0

#if (_WTL_VER >= 0x0700)
	BOOL m_bHandledCD;

	BOOL IsMsgHandled() const
	{
		return m_bHandledCD;
	}

	void SetMsgHandled(BOOL bHandled)
	{
		m_bHandledCD = bHandled;
	}
#if (_WTL_VER > 0x0700)
#pragma message("WTL > 7.0 Revisit WTL Bug Kludge:  Is IsMsgHandled, " \
	"SetMsgHandled still required in derived class when "  \
	"using CCustomDraw?");
#endif //(_WTL_VER > 0x0700)
#endif //(_WTL_VER >= 0x0700)
#endif

  BEGIN_MSG_MAP_EX(MyListView)    
    CHAIN_MSG_MAP(CCustomDraw<MyListView>)
	END_MSG_MAP()          	

	DWORD OnPrePaint(int /*idCtrl*/, LPNMCUSTOMDRAW /*lpNMCustomDraw*/)
	{		
    return  CDRF_NOTIFYITEMDRAW;
	}

 	DWORD OnItemPrePaint(int /*idCtrl*/, LPNMCUSTOMDRAW lpNMCustomDraw)
	{
      NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>( lpNMCustomDraw );    

     // This is the prepaint stage for an item. Here's where we set the
     // item's text color. Our return value will tell Windows to draw the
     // item itself, but it will use the new color we set here for the background     

     COLORREF crText;

     if ( (pLVCD->nmcd.dwItemSpec % 2) == 0 )
      crText = RGB(255,0,255);
     else 
      crText = RGB(255,255,255);        

      // Store the color back in the NMLVCUSTOMDRAW struct.
      pLVCD->clrTextBk = crText;

      // Tell Windows to paint the control itself.
      return CDRF_DODEFAULT;
	}

};

#endif _MYLISTVIEW_H
