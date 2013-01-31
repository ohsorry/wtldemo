// ==========================================================================
// GetDataDlg.h : 
// ==========================================================================
#ifndef ___GETDATADLG_H___
#define ___GETDATADLG_H___
#pragma once


class CGetDataDlg : public CDialog
{
public:
	DECLARE_DYNAMIC(CGetDataDlg)

	// Construction / Destruction
	CGetDataDlg(CWnd* pParent = NULL);
	virtual ~CGetDataDlg();

	// Dialog Data
	enum { IDD = IDD_GET_DATA };

protected:
	CString m_strTitle;
	CString m_strData;

public: 
	// Overrides
	virtual BOOL OnInitDialog();

	// Implementation
	CString GetTitle(){return m_strTitle;}
	void SetTitle(CString s){m_strTitle=s;}
	CString GetData(){return m_strData;}

protected:
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedOk();
};


#endif // ___GETDATADLG_H___
