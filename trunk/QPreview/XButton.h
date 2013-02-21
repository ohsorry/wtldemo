/************************************************************************
模快名:		仿360安全卫士界面
功能:		尚未对模块功能进行描述
完成日期:	2010-10-20
作者:		杨伟峰 Yang Weifeng(Email: 11259500@qq.com)

本代码可以自由使用，但因使用本代码造成的后果，本人不承担任何责任
************************************************************************/

#pragma once

class CXButton
{
public:
	// 标准构造函数
	CXButton()
	{
		m_bCheck = FALSE;
		m_bHovering = FALSE;
		m_bPressed = FALSE;
		m_bDisabled = FALSE;
		m_nImageContorl = 0;
		m_nAltImageContorl = 0;
	}

	// 设置按钮矩形位置
	void SetRect(LPCRECT lpRect){m_rcRect = lpRect;};
	// 获取按钮矩形位置
	void GetRect(LPRECT lpRect){*lpRect = *m_rcRect;};
	// 检测指定点是否处于按钮内
	BOOL PtInButton(CPoint& point){return m_rcRect.PtInRect(point);};
	// 绘制按钮
	void DrawButton( Gdiplus::Graphics&  graphics);
	// 创建按钮
	void CreateButton(CWnd* pParent, LPRECT lpRect,UINT nImage, UINT nCount = 4, UINT sAltImage = 0, UINT nAltCount = 0, UINT nID = 0);
	// 设置按钮状态
	void EnableButton(BOOL bEnable)
	{
		m_bDisabled = !bEnable;
		m_pParent->InvalidateRect(m_rcRect);
	}

	// 设置按钮选定状态
	void SetCheck(BOOL bCheck)
	{
		m_bCheck = bCheck;
		m_pParent->InvalidateRect(m_rcRect);
	}
	// 获取按钮Id
	UINT GetControlId(){return m_nID;};
	// 触发按钮事件
	void DoCommand()
	{
		if(!m_bDisabled)
		{
			m_bCheck = !m_bCheck;
			m_pParent->InvalidateRect(m_rcRect);
			m_pParent->PostMessage(WM_COMMAND, m_nID);
		}
	}

	// 鼠标按下按钮事件
	void LButtonDown()
	{
		if(!m_bDisabled)
		{
			m_bPressed = TRUE;
			m_pParent->InvalidateRect(m_rcRect);
		}
	};
	// 鼠标放下按钮事件
	void LButtonUp()
	{
		if(!m_bDisabled)
		{
			if(m_bPressed == TRUE)
			{
				m_bCheck = !m_bCheck;
				m_bPressed = FALSE;
				m_pParent->InvalidateRect(m_rcRect);
				m_pParent->PostMessage(WM_COMMAND, m_nID);
			}
		}
	}
	// 鼠标进入按钮区域事件
	void MouseHover()
	{
		if(!m_bDisabled)
		{
			m_bHovering = TRUE;
			m_pParent->InvalidateRect(m_rcRect);
		}
	}
	// 鼠标离开按钮区域事件
	void MouseLeave()
	{
		if(!m_bDisabled)
		{
			m_bPressed = FALSE;
			m_bHovering = FALSE;
			m_pParent->InvalidateRect(m_rcRect);
		}
	}
private:
	// 按钮矩形位置
	CRect	m_rcRect;
	// 标识鼠标进入按钮区域
	BOOL	m_bHovering;
	// 标识已经点击按钮
	BOOL	m_bPressed;
	// 标识按钮已经被禁用
	BOOL	m_bDisabled;
	// 标识按钮已经被选中
	BOOL	m_bCheck;
	// 按钮图片编号
	UINT	m_nImageContorl;
	// 按钮按下状态图片编号
	UINT	m_nAltImageContorl;
	// 按钮图片中包含的图片个数
	UINT	m_nImageCount;
	// 按钮选中图片中包含的图片个数
	UINT	m_nAltImageCount;
	// 按钮Id号
	int		m_nID;
	// 按钮父窗口
	CWnd*	m_pParent;
};
