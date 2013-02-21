/************************************************************************
模快名:		仿360安全卫士界面
功能:		尚未对模块功能进行描述
完成日期:	2010-10-20
作者:		杨伟峰 Yang Weifeng(Email: 11259500@qq.com)

本代码可以自由使用，但因使用本代码造成的后果，本人不承担任何责任
************************************************************************/

#pragma once
#include <map>
#include <memory>
using namespace std;

class CImageInfo
{
public:
	static CImageInfo * Instance();

	~CImageInfo(void);

	// 释放所有界面资源
	void	Release();

	// 释放界面资源Id加载界面资源
	Image*	ImageFromResource(UINT uId);

private:
	CImageInfo(void);
	static std::auto_ptr<CImageInfo> sm_inst;

	Image*	FromResource(UINT uId);

	map<UINT, Image*> m_mpImage;
};
