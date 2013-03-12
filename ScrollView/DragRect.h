#ifndef _drag_rect_h_
#define _drag_rect_h_
#include "atlimage.h"

enum eEdge_Direc
{
	eEdge_Bottom = 0,
	eEdge_Right,
	eEdge_BandR,
};

class DragRect
{
public:
	void OnMovseMove(int x,int y)
	{
		
	}
	
	bool PtInRect(int x,int y)
	{
		POINT pt;
		pt.x = x;
		pt.y = y;
		return mRc.PtInRect(pt);
	}
	



	
private:
	CRect mRc;
};

#endif