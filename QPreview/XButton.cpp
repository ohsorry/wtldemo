/************************************************************************
模快名:		仿360安全卫士界面
功能:		尚未对模块功能进行描述
完成日期:	2010-10-20
作者:		杨伟峰 Yang Weifeng(Email: 11259500@qq.com)

本代码可以自由使用，但因使用本代码造成的后果，本人不承担任何责任
************************************************************************/

#include "StdAfx.h"
#include "XButton.h"

// 绘制按钮
void CXButton::DrawButton( Gdiplus::Graphics&  graphics)
{
	// 获取按钮图片信息
	Image* pImage = CImageInfo::Instance()->ImageFromResource(m_nImageContorl);
	UINT iCount = m_nImageCount;
	if(m_bCheck && m_nAltImageContorl != 0)
	{
		pImage = CImageInfo::Instance()->ImageFromResource(m_nAltImageContorl);;
		iCount = m_nAltImageCount;
	}

	// 获取按钮状态信息
	int	iButtonIndex = 0;
	if(m_bDisabled && iCount >= 4) iButtonIndex = 3;
	else if(m_bPressed && iCount >= 3)iButtonIndex = 2;
	else if(m_bHovering && iCount >= 2)iButtonIndex = 1;
	else iButtonIndex = 0;

	// 在指定位置绘制按钮
	int iWidth = pImage->GetWidth()/iCount;
	int iHeight = pImage->GetHeight();
	RectF grect;
	grect.X=m_rcRect.left, grect.Y=m_rcRect.top; grect.Width = m_rcRect.Width(); grect.Height = m_rcRect.Height();
	graphics.DrawImage(pImage, grect, iWidth*iButtonIndex,0,iWidth,iHeight, UnitPixel);
}

// 创建按钮
void CXButton::CreateButton( CWnd* pParent, LPRECT lpRect, UINT nImage, UINT nCount /*= 4*/, UINT nAltImage /*= _T("")*/, UINT nAltCount /*= 0*/ , UINT nID/* = 0*/ )
{
	m_pParent = pParent;
	m_rcRect = lpRect;
	m_nImageContorl = nImage;
	m_nImageCount = nCount;
	m_nAltImageContorl = nAltImage;
	m_nAltImageCount = nAltCount;
	m_nID = nID;
}
