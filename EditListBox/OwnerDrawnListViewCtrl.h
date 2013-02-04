// (C) 2004 - Mark Petrik Sosa

#pragma once
#include "stdafx.h"

template <class TBase>
class COwnerDrawnListViewCtrl : public CWindowImpl<TBase, CListViewCtrl>,
								public COwnerDraw<TBase>
{
public:
	BEGIN_MSG_MAP(COwnerDrawnListViewCtrl)
		CHAIN_MSG_MAP_ALT(COwnerDraw<TBase>, 1)
		DEFAULT_REFLECTION_HANDLER()
	END_MSG_MAP()

	// If this control was .SubclassWindowed instead of .Create(Xe)d, then it 
	// will not receive a WM_MEASUREITEM since this message occurs before 
	// WM_INITDIALOG. By moving the window we can get windows to generate
	// another one for us.
	// ---------------------------------------------------------------------
	void ForceMeasureItemMessage()
	{
		// I just want to nudge it up a little.
		CRect window_rect;
		GetWindowRect(&window_rect);
		CPoint pt = window_rect.TopLeft();
		::ScreenToClient(GetParent(), &pt);
		window_rect.right = pt.x + window_rect.Width();
		window_rect.left = pt.x;
		window_rect.bottom = pt.y + window_rect.Height();
		window_rect.top = pt.y + 1;
		MoveWindow(window_rect);

		// Alright now move it back.
		window_rect.top = pt.y;
		MoveWindow(window_rect);
	}

	// We are required to have this empty DeleteItem to avoid ambiguity issues
	// with COwnderDraw::DeleteItem and CListViewCtrl::DeleteItem.
	// ---------------------------------------------------------------------
	void DeleteItem(LPDELETEITEMSTRUCT /*lpDeleteItemStruct*/) {};

	void GetCellRect(int header_column, const CRect& item_rect, CRect& cell_rect)
	{
		CHeaderCtrl header = GetHeader();
		CRect header_rect;
		header.GetItemRect(header_column, &header_rect);

		// If we don't do this, when we scroll to the right, we will be 
		// drawing as if we weren't and your cells won't line up with the
		// columns.
		int x_offset = -GetScrollPos(SB_HORZ);

		cell_rect.left = x_offset + header_rect.left;
		cell_rect.right = x_offset + header_rect.right;
		cell_rect.top = item_rect.top;
		cell_rect.bottom = item_rect.bottom;
	}

	// These are the functions you can overload in your class for Owner Drawing.
	// You will need to at least handle DrawItem().
	// If your going to use MeasureItem, make sure you call 
	// ForceMeasureItemMessage if you didn't create it yourself
	// ---------------------------------------------------------------------
	//void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/)
	//void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
	//int CompareItem(LPCOMPAREITEMSTRUCT /*lpCompareItemStruct*/)
	//void DeleteItem(LPDELETEITEMSTRUCT /*lpDeleteItemStruct*/)
};