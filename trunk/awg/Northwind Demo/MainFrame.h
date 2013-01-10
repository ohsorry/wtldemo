// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

class CMainFrame : 
	public CFrameWindowImpl<CMainFrame>,
	public CMessageFilter,
	public CGridCtrl::CListener
{
public:
	DECLARE_FRAME_WND_CLASS(NULL, IDR_MAINFRAME)

	CGridCtrl			m_view, m_detail;
	CDlgEmployees*		m_pEmployees;

	CCommandBarCtrl		m_CmdBar;
	CHorSplitterWindow	m_hzSplit;
	CPaneContainer		m_tPane;
	CPaneContainer		m_bPane;

	ADODB::_ConnectionPtr	m_conn;


	CMainFrame() {
		m_pEmployees = NULL;
	}

	virtual ~CMainFrame() {
		if(m_pEmployees) delete m_pEmployees;
	}

	virtual BOOL PreTranslateMessage(MSG* pMsg) {
		if(CFrameWindowImpl<CMainFrame>::PreTranslateMessage(pMsg))
			return TRUE;

		if(m_view.PreTranslateMessage(pMsg))
			return TRUE;

		if(m_detail.PreTranslateMessage(pMsg))
			return TRUE;

		if(m_pEmployees && m_pEmployees->m_grid.PreTranslateMessage(pMsg))
			return TRUE;

		return FALSE;
	}

	BEGIN_MSG_MAP(CMainFrame)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_USER+7, OnPostOpen)
		COMMAND_ID_HANDLER(ID_APP_EXIT, OnFileExit)
		COMMAND_ID_HANDLER(ID_APP_ABOUT, OnAppAbout)
		COMMAND_ID_HANDLER(ID_FILE_EMPLOYEES, OnFileEmployees)
		CHAIN_MSG_MAP(CFrameWindowImpl<CMainFrame>)
	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
		// create command bar window
		HWND hWndCmdBar = m_CmdBar.Create(m_hWnd, rcDefault, NULL, ATL_SIMPLE_CMDBAR_PANE_STYLE);
		// attach menu
		m_CmdBar.AttachMenu(GetMenu());
		// load command bar images
		m_CmdBar.LoadImages(IDR_MAINFRAME);
		// remove old menu
		SetMenu(NULL);

		HWND hWndToolBar = CreateSimpleToolBarCtrl(m_hWnd, IDR_MAINFRAME, FALSE, ATL_SIMPLE_TOOLBAR_PANE_STYLE);

		CreateSimpleReBar(ATL_SIMPLE_REBAR_NOBORDER_STYLE);
		AddSimpleReBarBand(hWndCmdBar);
		AddSimpleReBarBand(hWndToolBar, NULL, TRUE);

		CreateSimpleStatusBar();
		m_statusBar.Attach(m_hWndStatusBar);

		// create the horizontal splitter. Note that vSplit is parent of hzSplit
		m_hWndClient = m_hzSplit.Create(m_hWnd, rcDefault, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
		// set the horizontal splitter parameters
		m_hzSplit.m_cxyMin = 135; // minimum size
		m_hzSplit.SetSplitterPos(100); // from top

		// create the top container.  Note use of hzSplit as parent
		m_tPane.Create(m_hzSplit);
		// add container to top pane (0) of horizontal splitter
		m_hzSplit.SetSplitterPane(0, m_tPane);
		m_tPane.SetTitle("Orders");
		m_tPane.SetPaneContainerExtendedStyle(PANECNT_NOCLOSEBUTTON);

		m_view.Create(m_tPane, rcDefault, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, WS_EX_CLIENTEDGE, 1);
		m_view.SetExtendedGridStyle(GS_EX_LINENUMBERS,GS_EX_LINENUMBERS);
		m_view.SetListener(this);
		m_tPane.SetClient(m_view);

		m_bPane.Create(m_hzSplit);
		m_hzSplit.SetSplitterPane(1, m_bPane);
		m_bPane.SetTitle("Order Details");
		m_bPane.SetPaneContainerExtendedStyle(PANECNT_NOCLOSEBUTTON);

		m_detail.Create(m_bPane, rcDefault, NULL, WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|WS_CLIPCHILDREN, WS_EX_CLIENTEDGE, 2);
		m_detail.SetExtendedGridStyle(GS_EX_LINENUMBERS,GS_EX_LINENUMBERS);
		m_detail.AddColumn("Product",240,CGridCtrl::EDIT_DROPDOWNLIST,CGridCtrl::LEFT,VT_I4,"ProductID");
		m_detail.AddColumn("Unit Price",100,CGridCtrl::EDIT_TEXT,CGridCtrl::RIGHT,VT_R8,"UnitPrice");
		m_detail.AddColumn("Quantity",100,CGridCtrl::EDIT_TEXT,CGridCtrl::RIGHT,VT_I4,"Quantity");
		m_detail.AddColumn("Discount",100,CGridCtrl::EDIT_TEXT,CGridCtrl::RIGHT,VT_R8,"Discount");
		
		m_detail.SetListener(this);
		m_bPane.SetClient(m_detail);

		// register object for message filtering and idle updates
		CMessageLoop* pLoop = _Module.GetMessageLoop();
		ATLASSERT(pLoop != NULL);
		pLoop->AddMessageFilter(this);
		PostMessage(WM_USER+7);

		return 0;
	}

	LRESULT OnPostOpen(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
		CWaitCursor wait;

		m_hzSplit.SetSplitterPos(240);
		UpdateWindow();

		_variant_t vt;
		m_conn.CreateInstance("ADODB.Connection");
		m_conn->Open("Provider=sqloledb;Data Source=(local);Initial Catalog=northwind","sa","",0);
		
		m_view.AddColumn("ID",80,CGridCtrl::EDIT_NONE,CGridCtrl::LEFT,VT_I4,"OrderID");
		m_view.AddColumn("Customer",180,CGridCtrl::EDIT_DROPDOWNLIST,CGridCtrl::LEFT,VT_BSTR,"CustomerID");
		m_view.AddColumn("Employee",180,CGridCtrl::EDIT_DROPDOWNLIST,CGridCtrl::LEFT,VT_I4,"EmployeeID");
		m_view.AddColumn("OrderDate",180,CGridCtrl::EDIT_DATE,CGridCtrl::LEFT,VT_DATE,"OrderDate");

		ADODB::_RecordsetPtr rsCustomers = m_conn->Execute("select CustomerID,CompanyName from Customers",&vt,0);
		while(!rsCustomers->adEOF) {
			m_view.AddColumnLookup("CustomerID",rsCustomers->Fields->Item["CustomerID"]->Value,rsCustomers->Fields->Item["CompanyName"]->Value);
			rsCustomers->MoveNext();
		}

		ADODB::_RecordsetPtr rsEmployees = m_conn->Execute("select EmployeeID,Name=LastName + ', ' + FirstName from Employees",&vt,0);
		while(!rsEmployees->adEOF) {
			m_view.AddColumnLookup("EmployeeID",rsEmployees->Fields->Item["EmployeeID"]->Value,rsEmployees->Fields->Item["Name"]->Value);
			rsEmployees->MoveNext();
		}

		ADODB::_RecordsetPtr rsProducts = m_conn->Execute("select ProductID,ProductName from Products",&vt,0);
		while(!rsProducts->adEOF) {
			m_detail.AddColumnLookup("ProductID",rsProducts->Fields->Item["ProductID"]->Value,rsProducts->Fields->Item["ProductName"]->Value);
			rsProducts->MoveNext();
		}

		ADODB::_RecordsetPtr rs = m_conn->Execute("select * from Orders",&vt,0);
		m_view.SetRedraw(FALSE);
		while(!rs->adEOF) {
			long nItem = m_view.AddRow();
			m_view.SetItem(nItem,0L,rs->Fields->Item["OrderID"]->Value);
			m_view.SetItem(nItem,1L,rs->Fields->Item["CustomerID"]->Value);
			m_view.SetItem(nItem,2L,rs->Fields->Item["EmployeeID"]->Value);
			m_view.SetItem(nItem,3L,rs->Fields->Item["OrderDate"]->Value);
			rs->MoveNext();
		}
		m_view.SetRedraw(TRUE);

		return 0;
	}

	LRESULT OnFileExit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		PostMessage(WM_CLOSE);
		return 0;
	}
	
	LRESULT OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		CAboutDlg().DoModal(m_hWnd);
		return 0;
	}

	LRESULT OnFileEmployees(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		if(m_pEmployees) delete m_pEmployees;
		m_pEmployees = new CDlgEmployees(m_conn);
		m_pEmployees->Create(m_hWnd);
		m_pEmployees->ShowWindow(SW_SHOW);
		return 0;
	}

	bool					m_bStatusBar;
	bool					m_bToolBar;
	CStatusBarCtrl			m_statusBar;

	virtual bool OnRowChanging(UINT uID,long nRow) {
		if(uID==1 && m_detail.GetModified()) {
			if(IDYES==AtlMessageBox(m_hWnd,"Order details has changed. Save changes?",IDR_MAINFRAME,MB_YESNO|MB_ICONQUESTION)) {
				AtlMessageBox(m_hWnd,"TODO: No saving in this demo. Row change aborted!",IDR_MAINFRAME,MB_OK|MB_ICONEXCLAMATION);
				return false;
			}
		}

		return true;
	}

	virtual void OnRowChanged(UINT uID,long nRow) {
		if(uID!=1) return;

		CWaitCursor wait;
		CString tmp;
		tmp.Format("Changed row to %d",nRow);
		m_statusBar.SetWindowText(tmp);

		m_detail.DeleteAllItems();
		if(nRow<0) return;
		
		
		CString sql;
		sql.Format("select * from \"Order Details\" where OrderID=%d",(long)m_view.GetItem(nRow,0));

		_variant_t vt;
		ADODB::_RecordsetPtr rs = m_conn->Execute((LPCTSTR)sql,&vt,0);
		m_detail.SetRedraw(FALSE);
		while(!rs->adEOF) {
			long nItem = m_detail.AddRow();
			m_detail.SetItem(nItem,0,rs->Fields->Item["ProductID"]->Value);
			m_detail.SetItem(nItem,1,rs->Fields->Item["UnitPrice"]->Value);
			m_detail.SetItem(nItem,2,rs->Fields->Item["Quantity"]->Value);
			m_detail.SetItem(nItem,3,rs->Fields->Item["Discount"]->Value);
			rs->MoveNext();
		}
		rs->Close();
		m_detail.SetRedraw(TRUE);
	}

	virtual void OnEdit(UINT uID,long nRow) {
		m_statusBar.SetWindowText("Editing");
	}

	virtual bool OnDeleteRow(UINT uID,long nRow) {
		return AtlMessageBox(m_hWnd,"Delete this row?",IDR_MAINFRAME,MB_YESNO|MB_ICONQUESTION)==IDYES;
	}

	virtual void OnNewRow(UINT uID,long nRow) {
		m_view.SetItem(nRow,3,3);
		m_view.SetItem(nRow,0,(long)1);
	}

	virtual void OnRowActivate(UINT uID,long nRow) {
		CString tmp;
		tmp.Format("Row %d was activated",nRow);
		m_statusBar.SetWindowText(tmp);
	}

	virtual void OnModified(UINT uID,LPCTSTR pszColumn,_variant_t vtValue) {
		CString tmp;
		tmp.Format("%s: %s",pszColumn,(LPCTSTR)(_bstr_t)vtValue);
		//AtlMessageBox(m_hWnd,(LPCTSTR)tmp);
		m_statusBar.SetWindowText(tmp);
	}

	virtual COLORREF GetCellColor(UINT uID,long nRow,LPCTSTR pszColumn) {
		switch(uID) {
			case 1:
				if(m_view.GetModified(nRow))
					return RGB(255,240,240);
				break;
			case 2:
				if(m_detail.GetModified(nRow))
					return RGB(255,240,240);
				break;
		}

		return (COLORREF)-1;
	}

};
