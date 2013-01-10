#pragma once

class CDlgEmployees : 
	public CDialogImpl<CDlgEmployees>, 
	public CDialogResize<CDlgEmployees>,
	public CGridCtrl::CListener
{
public:
	enum { IDD = IDD_EMPLOYEES };

	BEGIN_MSG_MAP(CDlgEmployees)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDCANCEL, OnCloseCmd)
		COMMAND_ID_HANDLER(IDC_SAVE, OnSave)
		CHAIN_MSG_MAP(CDialogResize<CDlgEmployees>)
	END_MSG_MAP()

	BEGIN_DLGRESIZE_MAP(CDlgEmployees)
		DLGRESIZE_CONTROL(777,DLSZ_SIZE_X|DLSZ_SIZE_Y)
		DLGRESIZE_CONTROL(IDCANCEL,DLSZ_MOVE_X)
		DLGRESIZE_CONTROL(IDC_SAVE,DLSZ_MOVE_X)
	END_DLGRESIZE_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		CWindow(GetParent()).EnableWindow(FALSE);

		CRect rc;
		GetDlgItem(IDC_PLACEHOLDER).GetWindowRect(rc);
		::MapWindowPoints(NULL,m_hWnd,(LPPOINT)&rc,2);
		
		m_grid.Create(m_hWnd,rc,NULL,WS_CHILD|WS_VISIBLE|WS_TABSTOP,WS_EX_CLIENTEDGE,777);
		m_grid.AddColumn("EmployeeID",100,CGridCtrl::EDIT_NONE,CGridCtrl::LEFT,VT_I4,"EmployeeID");
		m_grid.AddColumn("LastName",100,CGridCtrl::EDIT_TEXT,CGridCtrl::LEFT,VT_BSTR,"LastName");
		m_grid.AddColumn("FirstName",100,CGridCtrl::EDIT_TEXT,CGridCtrl::LEFT,VT_BSTR,"FirstName");
		m_grid.AddColumn("Title",100,CGridCtrl::EDIT_TEXT,CGridCtrl::LEFT,VT_BSTR,"Title");
		m_grid.AddColumn("TitleOfCourtesy",100,CGridCtrl::EDIT_TEXT,CGridCtrl::LEFT,VT_BSTR,"TitleOfCourtesy");
		m_grid.AddColumn("BirthDate",100,CGridCtrl::EDIT_DATE,CGridCtrl::LEFT,VT_DATE,"BirthDate");
		m_grid.AddColumn("HireDate",100,CGridCtrl::EDIT_DATE,CGridCtrl::LEFT,VT_DATE,"HireDate");

		DlgResize_Init();
		SetWindowPos(NULL,0,0,640,400,SWP_NOMOVE|SWP_NOZORDER|SWP_NOREDRAW|SWP_NOACTIVATE);
		CenterWindow();

		m_grid.SetRedraw(FALSE);
		m_rs.CreateInstance("ADODB.Recordset");
		m_rs->Open("select * from Employees",m_conn.GetInterfacePtr(),ADODB::adOpenStatic,ADODB::adLockOptimistic,ADODB::adCmdText);
		while(!m_rs->adEOF) {
			long nItem = m_grid.AddRow();
			m_grid.SetItemData(nItem,m_rs->Bookmark);
			m_grid.SetItem(nItem,0L,m_rs->Fields->Item["EmployeeID"]->Value);
			m_grid.SetItem(nItem,1L,m_rs->Fields->Item["LastName"]->Value);
			m_grid.SetItem(nItem,2L,m_rs->Fields->Item["FirstName"]->Value);
			m_grid.SetItem(nItem,3L,m_rs->Fields->Item["Title"]->Value);
			m_grid.SetItem(nItem,4L,m_rs->Fields->Item["TitleOfCourtesy"]->Value);
			m_grid.SetItem(nItem,5L,m_rs->Fields->Item["BirthDate"]->Value);
			m_grid.SetItem(nItem,6L,m_rs->Fields->Item["HireDate"]->Value);
			m_rs->MoveNext();
		}
		m_grid.SetRedraw(TRUE);

		return TRUE;
	}

	LRESULT OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		CWindow(GetParent()).EnableWindow(TRUE);
		DestroyWindow();
		return 0;
	}

	CDlgEmployees(ADODB::_ConnectionPtr& conn) : m_conn(conn) {
	}

	CGridCtrl	m_grid;
	ADODB::_ConnectionPtr&	m_conn;
	ADODB::_RecordsetPtr	m_rs;

	virtual COLORREF GetCellColor(UINT uID,long nRow,LPCTSTR pszColumn) {
		if(m_grid.GetModified(nRow))
			return RGB(255,240,240);

		return (COLORREF)-1;
	}

	LRESULT OnSave(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		CWaitCursor wait;
		if(!m_grid.GetModified())
			return 0;

		long count = m_grid.GetRowCount();
		while(count--) {
			if(m_grid.GetModified(count)) {
				m_rs->Bookmark = m_grid.GetItemData(count);
				m_rs->Fields->Item["LastName"]->Value = m_grid.GetItem(count,1);
				m_rs->Fields->Item["FirstName"]->Value = m_grid.GetItem(count,2);
			}
		}
		m_rs->Update();
		m_grid.ClearModified();

		return 0;
	}
};
