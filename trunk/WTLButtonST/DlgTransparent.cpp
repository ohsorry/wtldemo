#include "stdafx.h"
#include "resource.h"
#include "DlgTransparent.h"

//=============================================================================
// 
//=============================================================================
CBackgroundUtil::CBackgroundUtil()
{
}


CBackgroundUtil::~CBackgroundUtil()
{
	m_BmpPattern.Detach();
	m_BmpPalette.Detach();
}


bool CBackgroundUtil::SetBitmap(UINT uResourceID)
{
	BITMAP bm;
	bool bRet;

	// Detach previous resources
	m_BmpPattern.Detach();  
	m_BmpPalette.Detach();  

	// Default return value
	bRet = TRUE;

	// Load new bitmap
	if (uResourceID != 0)
	{
		bRet = GetBitmapAndPalette(uResourceID, m_BmpPattern, m_BmpPalette);
		// If all ok
		if (bRet == TRUE)
		{
			// Get dimension
			m_BmpPattern.GetBitmap(&bm);
			// Width of the bitmap
			m_nBmpWidth = bm.bmWidth;
			// Height of the bitmap
			m_nBmpHeight = bm.bmHeight;
		}
	}

	return bRet;
} // End of SetBitmap


bool CBackgroundUtil::GetBitmapAndPalette(UINT nIDResource, CBitmap & bitmap, CPalette & pal)
{
	LPCTSTR lpszResourceName = (LPCTSTR)nIDResource;

	HBITMAP hBmp = (HBITMAP)::LoadImage( _Module.GetResourceInstance(), 
		lpszResourceName, IMAGE_BITMAP, 0,0, LR_CREATEDIBSECTION);

	if (hBmp == NULL) return FALSE;

	bitmap.Attach(hBmp);

	// Create a logical palette for the bitmap
	DIBSECTION ds;
	BITMAPINFOHEADER &bmInfo = ds.dsBmih;
	::GetObject(bitmap, sizeof(ds), &ds);

	int nColors = bmInfo.biClrUsed ? bmInfo.biClrUsed : 1 << bmInfo.biBitCount;

	// Create a halftone palette if colors > 256. 
	CClientDC dc(NULL); // Desktop DC

	if(nColors > 256)
		pal.CreateHalftonePalette(dc);
	else
	{
		// Create the palette
		RGBQUAD *pRGB = new RGBQUAD[nColors];
		CDC memDC;
		memDC.CreateCompatibleDC(dc);
		memDC.SelectBitmap(bitmap);
		::GetDIBColorTable( memDC, 0, nColors, pRGB );
		UINT nSize = sizeof(LOGPALETTE) + (sizeof(PALETTEENTRY) * nColors);
		LOGPALETTE *pLP = (LOGPALETTE *) new BYTE[nSize];
		pLP->palVersion = 0x300;
		pLP->palNumEntries = nColors;
		for (int i=0; i < nColors; i++)
		{
			pLP->palPalEntry[i].peRed = pRGB[i].rgbRed;
			pLP->palPalEntry[i].peGreen = pRGB[i].rgbGreen;
			pLP->palPalEntry[i].peBlue = pRGB[i].rgbBlue;
			pLP->palPalEntry[i].peFlags = 0;
		}
		pal.CreatePalette( pLP );
		delete[] pLP;
		delete[] pRGB;
	}
	return TRUE;
} // End of GetBitmapAndPalette


bool CBackgroundUtil::TileBitmap(CDCHandle pDC, CRect rc)
{
	CDC MemDC;

	int x = 0, y = 0;

	// If there is a bitmap loaded
	if (m_BmpPattern != 0)
	{
		MemDC.CreateCompatibleDC(pDC);
		CBitmapHandle pOldBitmap = MemDC.SelectBitmap(m_BmpPattern);

		// Tile the bitmap
		while (y < rc.Height()) 
		{
			while(x < rc.Width()) 
			{
				pDC.BitBlt(x, y, m_nBmpWidth, m_nBmpHeight, MemDC, 0, 0, SRCCOPY);
				x += m_nBmpWidth;
			}
			x = 0;
			y += m_nBmpHeight;
		}

		MemDC.SelectBitmap(pOldBitmap);

		return true;
	}

	return false; // Normal behaviour
}

//=============================================================================
// 
//=============================================================================
CDlgTransparent::CDlgTransparent()
{
	m_bFirstTime = true;
}


LRESULT CDlgTransparent::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) 
{
	m_btnLogOff.SubclassWindow(GetDlgItem(IDC_BTNLOGOFF));
	m_btnWorkgroup.SubclassWindow(GetDlgItem(IDC_BTNWORKGROUP));
	m_btnOpen.SubclassWindow(GetDlgItem(IDC_BTNOPEN));
	m_btnSearch.SubclassWindow(GetDlgItem(IDC_BTNSEARCH));
	m_btnExplorer.SubclassWindow(GetDlgItem(IDC_BTNEXPLORER));
	m_btnHelp.SubclassWindow(GetDlgItem(IDC_BTNHELP));
	m_btnAbout.SubclassWindow(GetDlgItem(IDC_BTNABOUT));
	m_btnCannibal.SubclassWindow(GetDlgItem(IDC_BTNCANNIBAL));
	m_chkCheckbox.SubclassWindow(GetDlgItem(IDC_CHECK));

	SetBitmap(IDB_SKY);

	// Log off button
	m_btnLogOff.SetIcon(IDI_LOGOFF);
	// Workgroup button
	m_btnWorkgroup.SetIcon(IDI_WORKGROUP);
	// Toolbar buttons
	m_btnOpen.SetIcon(IDI_OPEN);
	m_btnOpen.SetTooltipText(_T("Open"));
	m_btnSearch.SetIcon(IDI_SEARCH1);
	m_btnSearch.SetTooltipText(_T("Search"));
	m_btnExplorer.SetIcon(IDI_EXPLORER);
	m_btnExplorer.SetTooltipText(_T("Internet explorer"));
	m_btnHelp.SetIcon(IDI_HELP);
	m_btnHelp.SetTooltipText(_T("Help"));
	m_btnAbout.SetIcon(IDI_ABOUT);
	m_btnAbout.SetTooltipText(_T("About"));
	// Cannibal button
	m_btnCannibal.SetBitmaps(IDB_CANNIBAL, RGB(0, 255, 0));
	// Checkbox
	m_chkCheckbox.SetIcon(IDI_LEDON, IDI_LEDOFF);

	return true;
}

LRESULT CDlgTransparent::OnSetFocus(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) 
{
	DefWindowProc();

	// Do only one time
	if (m_bFirstTime)
	{
		m_bFirstTime = FALSE;
		m_btnLogOff.DrawTransparent(true);
		m_btnWorkgroup.DrawTransparent(true);
		m_btnOpen.DrawTransparent(true);
		m_btnSearch.DrawTransparent(true);
		m_btnExplorer.DrawTransparent(true);
		m_btnHelp.DrawTransparent(true);
		m_btnAbout.DrawTransparent(true);
		m_btnCannibal.DrawTransparent(true);
		m_chkCheckbox.DrawTransparent(true);
	} // if
	return 0;
}

LRESULT CDlgTransparent::OnEraseBkgnd(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled) 
{
	CRect rc;

	GetClientRect(rc);

	if (TileBitmap((HDC) wParam, rc))
		return TRUE;

	bHandled = false;
	return FALSE;
}	
