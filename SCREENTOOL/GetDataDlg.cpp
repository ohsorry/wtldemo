// ==========================================================================
// GetDataDlg.cpp : 
// ==========================================================================
#include "stdafx.h"
#include "SCREENTOOL.h"
#include "GetDataDlg.h"
#include ".\getdatadlg.h"


BEGIN_MESSAGE_MAP(CGetDataDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()

IMPLEMENT_DYNAMIC(CGetDataDlg, CDialog)

//=================================================
// Construction / Destruction
//=================================================
CGetDataDlg::CGetDataDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGetDataDlg::IDD, pParent)
{
	m_strTitle = _T("");
	m_strData = _T("");
}

CGetDataDlg::~CGetDataDlg()
{
}

//=================================================
// Overrides
//=================================================
BOOL CGetDataDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	SetWindowText(m_strTitle);
	CEdit* pEdit = NULL;
	VERIFY(pEdit = (CEdit*)GetDlgItem(IDC_TEXT));
	pEdit->SetLimitText(50);
	//pEdit->SetFocus();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

//=================================================
// Message Handlers
//=================================================
void CGetDataDlg::OnBnClickedOk()
{
	CWnd* pWnd = GetDlgItem(IDC_TEXT);
	ASSERT(pWnd);
	pWnd->GetWindowText(m_strData);
	OnOK();
}
