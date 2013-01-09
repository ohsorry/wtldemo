#if !defined(__mytabctrl_h__)
#define __mytabctrl_h__

class CMyTabCtrl : public CWindowImpl<CMyTabCtrl, CTabCtrl>
{
// Construction
public:
	CMyTabCtrl();
	virtual ~CMyTabCtrl();

	BEGIN_MSG_MAP_EX(CMyTabCtrl)
		MSG_WM_LBUTTONDOWN(OnLButtonDown)
	END_MSG_MAP()

	void Init();
protected:
	CWindow *m_tabPages[5];
	int m_tabCurrent;
	int m_nNumberOfPages;

	LRESULT OnLButtonDown(UINT nFlags, CPoint pt);
	void SetRectangle();
};


#endif // !defined(__mytabctrl_h__)
