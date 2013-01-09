/* 
 *  Filename:    GradientBarCtrl.h
 *  Description: Gradient bar control implementation
 *  Copyright:   Julijan Šribar, 2012
 *
 * This software is provided 'as-is', without any express or implied
 * warranty.  In no event will the author(s) be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 */

#pragma once

#include <atlctrls.h>
#include <atlmisc.h>
#include <atltheme.h>
#include "HSL.h"


#define HUE_RED 0
#define HUE_GREEN 120
#define HUE_RANGE_DEFAULT 35


// Default class used by CGradientBarCtrlImpl to evaluate the color

class CEvaluateColor
{
public:
	CEvaluateColor()
		: m_hueMin(HUE_RED)
		, m_hueMax(HUE_GREEN)
		, m_displayedHueRange(HUE_RANGE_DEFAULT)
	{
		ATLASSERT(m_hueMax - m_hueMin > m_displayedHueRange);
	}

	COLORREF GetRgbCorrected(int value, int currentColumn, int totalColumns, COLORREF originalColor)
	{
		// evaluate hue for the value provided
		double hue = GetHue(value, currentColumn, totalColumns);
		// create HSL from original color
		HSL hsl(originalColor);
		// and apply calculated hue
		hsl.SetHue(hue);
		// convert to RGB and return
		return hsl.GetRGB();
	}

	COLORREF GetRgb(int value, int currentColumn, int totalColumns)
	{
		// evaluate hue for the value provided
		double hue = GetHue(value, currentColumn, totalColumns);
		// create HSL for evaluated hue, full saturation and 50% lightness
		HSL hsl(hue, 1., 0.5);
		// convert to RGB data and return
		return hsl.GetRGB();
	}

private:
	double m_hueMin;
	double m_hueMax;
	double m_displayedHueRange;

	double GetHue(int value, int currentColumn, int totalColumns)
	{
		return GetHueStart(value) + m_displayedHueRange * currentColumn / totalColumns;
	}

	double GetHueStart(int value)
	{
		return m_hueMin + (m_hueMax - m_hueMin - m_displayedHueRange) * value / 100.;
	}
};

#define INITIAL_DELAY_TIMER_ID 1
#define INITIAL_DELAY 1025

#define MARQUEE_TIMER_ID 2
#define MARQUEE_TICK 36
#define MARQUEE_DELTAX 12
#define MARQUEE_WIDTH 120

// CGradientBarCtrlImpl implementation

template<typename T, typename TEvaluateColor = CEvaluateColor, typename TBase = CStatic, typename TWinTraits = CControlWinTraits>
class ATL_NO_VTABLE CGradientBarCtrlImpl : public CWindowImpl<T, TBase, TWinTraits>, public CThemeImpl<T>
{
public:
	CGradientBarCtrlImpl()
		: m_value(0)
		, m_displayValue(true)
		, m_marqueeEnabled(false)
		, m_marqueeRunning(false)
	{
	}

	BEGIN_MSG_MAP(CGradientBarCtrlImpl)
		CHAIN_MSG_MAP(CThemeImpl<CGradientBarCtrl>)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
		MESSAGE_HANDLER(WM_TIMER, OnTimer)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		DEFAULT_REFLECTION_HANDLER()
    END_MSG_MAP()

	// message handlers

	LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		RECT rc;
		GetClientRect(&rc);
		
		PAINTSTRUCT ps;
		CDCHandle dc = BeginPaint(&ps);
		CDCHandle dcMem;
		dcMem.CreateCompatibleDC(dc);

		CBitmap bmp;
		bmp.CreateCompatibleBitmap(dc, rc.right, rc.bottom);
		HBITMAP oldBmp = dcMem.SelectBitmap(bmp);

		DrawControl(dcMem);
		if (m_displayValue)
			DrawValue(dcMem);
	
		dc.BitBlt(0, 0, rc.right, rc.bottom, dcMem, 0, 0, SRCCOPY);

		dcMem.SelectBitmap(oldBmp);
		dcMem.DeleteDC();

		EndPaint(&ps);
		
		bHandled = TRUE;
		return 1;
	}

	LRESULT OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		RECT rect;
		GetClientRect(&rect);
		switch (wParam)
		{
		case INITIAL_DELAY_TIMER_ID:
			// initial delay has expired
			KillTimer(INITIAL_DELAY_TIMER_ID);
			// run the marquee
			m_marqueeX = -MARQUEE_WIDTH;
			m_marqueeRunning = true;
			SetTimer(MARQUEE_TIMER_ID, MARQUEE_TICK);
			break;
		case MARQUEE_TIMER_ID:
			// draw marquee at new position
			m_marqueeX += MARQUEE_DELTAX;
			Invalidate();
			if (m_marqueeX >= rect.right)
			{
				// marquee passed the end of bar so reset timers
				KillTimer(MARQUEE_TIMER_ID);
				m_marqueeRunning = false;
				ResetInitialDelayTimer();
			}
			break;
		default:
			bHandled = false;
			break;
		}
		return 0;
	}

	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		KillTimer(INITIAL_DELAY_TIMER_ID);
		KillTimer(MARQUEE_TIMER_ID);
		return 0;
	}

	// public methods

	void SetValue(int value)
	{
		ATLASSERT(value >= 0 && value <= 100);
		m_value = value;
		if (!m_marqueeRunning)
			Invalidate();
		// on value change
		ResetInitialDelayTimer();
	}

	// should display numeric value
	void DisplayValue(bool show = true)
	{
		m_displayValue = show;
		Invalidate();
	}

	// enable marquee
	void EnableMarquee(bool enable = true)
	{
		// marquee can be displayed only when application is themed
		if (IsAppThemed() == FALSE)
			return;
		m_marqueeEnabled = enable;
		ResetInitialDelayTimer();
	}

private:
	int m_value;
	bool m_displayValue;
	TEvaluateColor m_evaluateColor;

	bool m_marqueeEnabled;
	bool m_marqueeRunning;
	int m_marqueeX;
	

	// private methods

	void DrawControl(CDCHandle& dc)
	{
		if (IsAppThemed())
			DrawThemedBar(dc);
		else
			DrawClassicBar(dc);
	}

	void DrawThemedBar(CDCHandle& dc)
	{
		CRect rect;
		GetClientRect(&rect);
		// first draw progress bar using theme
		OpenThemeData(L"PROGRESS");
		DrawThemeBackground(dc, PP_BAR, 0, &rect, NULL);
		// draw the (green) chunk
		DrawThemeBackground(dc, PP_FILL, PBFS_NORMAL, &rect, NULL);
		// draw running overlay
		if (m_marqueeRunning)
		{
			CRect rectOverlay(rect);
			rectOverlay.right = MARQUEE_WIDTH;
			rectOverlay.MoveToX(m_marqueeX);
			DrawThemeBackground(dc, PP_MOVEOVERLAY, PBFS_NORMAL, &rectOverlay, &rect);
		}
		CloseThemeData();

		// correct individual pixels
		int columns = rect.Width() + 1;
		int rows = rect.Height() + 1;
		for (int column = 0; column < columns; ++column)
		{
			for (int row = 0; row < rows; ++row)
			{
				// pick color from theme bar
				COLORREF color = dc.GetPixel(column, row);
				// and correct it using theme lightness and saturation components
				color = m_evaluateColor.GetRgbCorrected(m_value, column, columns, color);
				dc.SetPixel(column, row, color);
			}
		}
	}

	void DrawClassicBar(CDCHandle& dc)
	{
		CRect rect;
		GetClientRect(&rect);
		// draw the edge (similarly to progress bar, border is allways displayed)
		dc.DrawEdge(&rect, EDGE_ETCHED, BF_RECT);
		rect.DeflateRect(1, 1, 1, 1);
		// since bar doesn't fill the entire client area, fill 
		// the client with button face color first
		dc.FillSolidRect(&rect, ::GetSysColor(COLOR_BTNFACE));

		// draw individual pixels
		int columns = rect.Width();
		int rows = rect.Height();
		for (int column = 2; column < columns; ++column)
		{
			COLORREF color = m_evaluateColor.GetRgb(m_value, column, columns);
			for (int row = 2; row < rows; ++row)
				dc.SetPixel(column, row, color);
		}
	}

	// draw percentage on the bar
	void DrawValue(CDCHandle& dc)
	{
		CString text;
		text.Format(_T("%d %%"), m_value);
		RECT rect;
		GetClientRect(&rect);
		int oldBkMode = dc.SetBkMode(TRANSPARENT);

		LOGFONT lf = { 0 };
		lf.lfHeight = -MulDiv(8, ::GetDeviceCaps(dc, LOGPIXELSY), 72);
		lf.lfWidth = 0;
		lf.lfWeight = FW_NORMAL;
		lf.lfCharSet = DEFAULT_CHARSET;
		lf.lfOutPrecision = OUT_DEFAULT_PRECIS;
		lf.lfClipPrecision = CLIP_DEFAULT_PRECIS;
		lf.lfQuality = DEFAULT_QUALITY;
		lf.lfPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
		_tcscpy_s(lf.lfFaceName, _T("MS Shell Dlg 2"));
		HFONT shellDlgFont = CreateFontIndirect(&lf);
		HFONT oldFont = dc.SelectFont(shellDlgFont);
		dc.DrawText((LPTSTR)(LPCTSTR)text, -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		dc.SelectFont(oldFont);
		DeleteObject(shellDlgFont);

		dc.SetBkMode(oldBkMode);
	}

	// restart initial delay timer
	void ResetInitialDelayTimer()
	{
		KillTimer(INITIAL_DELAY_TIMER_ID);
		if (m_marqueeEnabled)
			SetTimer(INITIAL_DELAY_TIMER_ID, INITIAL_DELAY);
	}
};

class CGradientBarCtrl : public CGradientBarCtrlImpl<CGradientBarCtrl>
{
public:
	DECLARE_WND_SUPERCLASS(_T("WTL Gradient Bar Control"), GetWndClassName())  
};
