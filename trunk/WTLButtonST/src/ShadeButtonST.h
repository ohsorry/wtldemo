#ifndef _SHADEBUTTONST_H_
#define _SHADEBUTTONST_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ButtonST.h"
#include "xDib.h"

class CShadeButtonST : public CButtonST  
{
public:
	CShadeButtonST();
	virtual ~CShadeButtonST();

	enum	{	SHS_NOISE = 0,
				SHS_DIAGSHADE,
				SHS_HSHADE,
				SHS_VSHADE,
				SHS_HBUMP,
				SHS_VBUMP,
				SHS_SOFTBUMP,
				SHS_HARDBUMP,
				SHS_METAL	};

	void SetShade(UINT shadeID=0,BYTE granularity=8,BYTE highlight=10,BYTE coloring=0,COLORREF color=0);

private:
	CxDib m_dNormal,m_dDown,m_dDisabled,m_dOver,m_dh,m_dv;

protected:
	virtual DWORD OnDrawBorder(CDCHandle pDC, LPCRECT pRect);
	virtual DWORD OnDrawBackground(CDCHandle pDC, LPCRECT pRect);
};

#endif
