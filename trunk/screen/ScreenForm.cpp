#include "ScreenForm.h"
#include "MainForm.h"

CBitmap bmp_srcreen;	//用来保存全屏图片
MainForm *mf_ptr = NULL;

void ScreenForm::OnRButtonDown(UINT nFlags, CPoint point)
{
	DestroyWindow();
	mf_ptr->ShowWindow(SW_SHOW);
}