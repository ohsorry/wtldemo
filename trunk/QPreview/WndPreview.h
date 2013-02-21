/************************************************************************
模快名:		仿360安全卫士界面
功能:		尚未对模块功能进行描述
完成日期:	2010-10-20
作者:		杨伟峰 Yang Weifeng(Email: 11259500@qq.com)

本代码可以自由使用，但因使用本代码造成的后果，本人不承担任何责任
************************************************************************/

#pragma once

#include <vector>
#include "XButton.h"

using std::vector;

class CWndPreview : public CWnd
{
public:
	// 标准构造析构函数
	CWndPreview();
	virtual ~CWndPreview();

protected:

	// 图片切换所需资源信息
	struct ButtonInfo
	{
		CString sName;
		UINT nIcon; 
		UINT nBack;
	};

	DECLARE_MESSAGE_MAP()

	// WM_CREATE消息处理函数
	afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
	// WM_PAINT消息处理函数
	afx_msg void OnPaint();
	// WM_ERASEBKGND消息处理函数
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	// 窗口关闭按钮事件
	afx_msg void OnBtnClose();
	// WM_TIMER消息处理函数
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	// 在窗口销毁后调用
	virtual void PostNcDestroy();
	// WM_MOUSEMOVE消息处理函数
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	// WM_LBUTTONDOWN消息处理函数
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	// WM_LBUTTONUP消息处理函数
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	// WM_LBUTTONDBLCLK消息处理函数
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	// 鼠标离开窗口消息通知
	afx_msg LRESULT OnMouseLeave(WPARAM wparam, LPARAM lparam);
	// 鼠标进入窗口消息通知
	afx_msg LRESULT OnMouseHover(WPARAM wparam, LPARAM lparam) ;
	// 添加图片页面函数
	int		AddButton(LPCTSTR szName, UINT nIcon, UINT nBack );
	// 获取指定切换按钮所在位置
	BOOL	GetButtonRect(int iButton, CRect& rtButton);
	// 测试鼠标所在切换按钮位置
	int		HitTest(CPoint point, CRect& rtButton);
	// 测试鼠标所在其它按钮位置
	int		XButtonHitTest(CPoint point, CRect& rtButton);

private:
	// 指示鼠标已经进行窗口
	BOOL	m_bHovering;
	// 指示已经开始进行鼠标进入离开窗口事件跟踪
	BOOL	m_bTracking;
	// 当前鼠标所在按钮
	int		m_iHovering;
	// 当前选定的按钮
	int		m_iSelected;
	// 当前窗口图片是否自动移动
	BOOL	m_bAutoMove;
	// 当前鼠标是否已经按下
	BOOL	m_bMouseDown;
	// 当前鼠标按下位置
	int		m_iMouseDownX;
	// 当前鼠标按下后移动宽度
	int		m_iMouseMoveWidth;
	// 当前窗口图片每秒自动移动的宽度
	int		m_iMoveWidthPerSecond;
	// 主窗口图片切换按钮列表
	vector<ButtonInfo>	m_ButtonList;
	// 当前鼠标所在其它按钮编号
	int					m_iXButtonHovering;
	// 其它按钮所在位置
	vector<CRect>		m_XButtonRectList;
	// 其它按钮列表
	vector<CXButton>	m_XButtonList;
};

