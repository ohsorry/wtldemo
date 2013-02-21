/************************************************************************
模快名:		仿360安全卫士界面
功能:		尚未对模块功能进行描述
完成日期:	2010-10-20
作者:		杨伟峰 Yang Weifeng(Email: 11259500@qq.com)

本代码可以自由使用，但因使用本代码造成的后果，本人不承担任何责任
************************************************************************/

#pragma once

class CMemoryDC : public CDC {
public:
	CMemoryDC(CDC* pDC, CRect rect = CRect(0,0,0,0), BOOL bCopyFirst = FALSE)
		: CDC(), m_oldBitmap(NULL), m_pDC(pDC)
	{
		ASSERT(m_pDC != NULL);	// If you asserted here, you passed in a NULL CDC.
		
		m_bMemDC = !pDC->IsPrinting();
		
		if (m_bMemDC){
			// Create a Memory DC
			CreateCompatibleDC(pDC);
			if ( rect == CRect(0,0,0,0) )
				pDC->GetClipBox(&m_rect);
			else
				m_rect = rect;

			m_bitmap.CreateCompatibleBitmap(pDC, m_rect.Width(), m_rect.Height());
			m_oldBitmap = SelectObject(&m_bitmap);
			SetWindowOrg(m_rect.left, m_rect.top);
			if(bCopyFirst)
			{
				this->BitBlt(m_rect.left, m_rect.top, m_rect.Width(), m_rect.Height(),
						m_pDC, m_rect.left, m_rect.top, SRCCOPY);
			}
		} else {
			// Make a copy of the relevent parts of the current DC for printing
			m_bPrinting = pDC->m_bPrinting;
			m_hDC = pDC->m_hDC;
			m_hAttribDC = pDC->m_hAttribDC;
		}
	}
	
	~CMemoryDC()
	{
		if (m_bMemDC) {
			// Copy the offscreen bitmap onto the screen.
			m_pDC->BitBlt(m_rect.left, m_rect.top, m_rect.Width(), m_rect.Height(),
				this, m_rect.left, m_rect.top, SRCCOPY);
			// Swap back the original bitmap.
			SelectObject(m_oldBitmap);
		} else {
			// All we need to do is replace the DC with an illegal value,
			// this keeps us from accidently deleting the handles associated with
			// the CDC that was passed to the constructor.
			m_hDC = m_hAttribDC = NULL;
		}
	}
	
	// Allow usage as a pointer
	CMemoryDC* operator->() {return this;}
	
	// Allow usage as a pointer
	operator CMemoryDC*() {return this;}
private:
	CBitmap   m_bitmap;			// Offscreen bitmap
	CBitmap*  m_oldBitmap;		// bitmap originally found in CMemoryDC
	CDC*      m_pDC;			// Saves CDC passed in constructor
	CRect     m_rect;			// Rectangle of drawing area.
	BOOL      m_bMemDC;			// TRUE if CDC really is a Memory DC.
};
