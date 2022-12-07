
// BBanggDB-PosDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "BBanggDB-Pos.h"
#include "BBanggDB-PosDlg.h"
#include "afxdialogex.h"
#include <string>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CBBanggDBPosDlg dialog



CBBanggDBPosDlg::CBBanggDBPosDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_BBANGGDBPOS_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CBBanggDBPosDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_PRODUCTINFO, ListCurrentProductInfo);
}

BEGIN_MESSAGE_MAP(CBBanggDBPosDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_ADD_PRODUCT, &CBBanggDBPosDlg::OnBnClickedButtonAddProduct)
	ON_BN_CLICKED(IDC_BUTTON_ORDER, &CBBanggDBPosDlg::OnBnClickedButtonOrder)
	ON_BN_CLICKED(IDC_BUTTON_STASTICS, &CBBanggDBPosDlg::OnBnClickedButtonStastics)
END_MESSAGE_MAP()


// CBBanggDBPosDlg message handlers

BOOL CBBanggDBPosDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	MakeMap();
	DBLink = new CDatabase;
	DBLink->OpenEx(_T("DSN=Localhost"));  //나중에 이 부분을 본인이 설정한 ODBC로 바꾸세요
	DBSet = new CRecordset(DBLink);
	SetTimer(0, 5000,NULL); // 5초마다 재고현황 자동갱신
	CString temp = _T("SELECT * FROM branch_stock ");
	if (DBSet->Open(CRecordset::dynamic, temp))
	{
		while (!DBSet->IsEOF())
		{
			CString stemp;
			for (int i = 0; i < DBSet->GetODBCFieldCount(); ++i)
			{

				CString sstemp;
				DBSet->GetFieldValue(short(i), sstemp);
				stemp += MappingField[i] + sstemp;
			}
			ListCurrentProductInfo.AddString(stemp);
			DBSet->MoveNext();
		}
	}
	DBSet->Close();
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CBBanggDBPosDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CBBanggDBPosDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CBBanggDBPosDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	ListCurrentProductInfo.ResetContent();
	CString temp = _T("SELECT * FROM branch_stock ");
	if (DBSet->Open(CRecordset::dynamic, temp))
	{
		while (!DBSet->IsEOF())
		{
			CString stemp;
			for (int i = 0; i < DBSet->GetODBCFieldCount(); ++i)
			{

				CString sstemp;
				DBSet->GetFieldValue(short(i), sstemp);
				stemp += MappingField[i] + sstemp;
			}
			ListCurrentProductInfo.AddString(stemp);
			DBSet->MoveNext();
		}
	}
	DBSet->Close();
	CDialogEx::OnTimer(nIDEvent);
}

void CBBanggDBPosDlg::MakeMap()
{
	MappingField.insert(std::make_pair(0, _T("브랜드명: ")));
	MappingField.insert(std::make_pair(1, _T("     제품명: ")));
	MappingField.insert(std::make_pair(2, _T("     재고 수량: ")));
}


void CBBanggDBPosDlg::OnBnClickedButtonAddProduct()
{
	// TODO: Add your control notification handler code here
	AddProduct* AddProductDlg = new AddProduct;
	int retval = AddProductDlg->DoModal();
	if (retval == IDOK) // 주문 버튼을 누르면 재고 수량 수정
	{
		CString temp = _T("UPDATE branch_stock SET stock_volume = stock_volume + ");
		CString ttemp(std::to_string(AddProductDlg->GetNumber()).c_str());
		temp += ttemp;
		CString tttemp = _T(" where branch_name = '");
		temp += tttemp;
		temp += AddProductDlg->GetBrand();
		temp += _T("' and product_name = '");
		temp += AddProductDlg->GetProduct();
		temp += _T("'");
		// 쿼리 시작
		DBLink->BeginTrans(); // 트랜잭션 시작
		DBLink->ExecuteSQL(temp); // 명령 실행
		if (!DBLink->CommitTrans()) // commit 에러가 날경우 자동으로 에러메세지와 함께 롤백됨
		{
			AfxMessageBox(_T("에러 발생"));
			DBLink->Rollback();
		}
	}
}


void CBBanggDBPosDlg::OnBnClickedButtonOrder()
{
	// TODO: Add your control notification handler code here
	OrderDLG* dlg = new OrderDLG;
	if (dlg->DoModal() == IDOK)
	{
		for (int i = 0; i < dlg->GetSQL().size(); ++i)
		{
			//이게 SQL에서 C++로 넘어오면서 문자열 해석이 그지같아져서 SQL문이 복잡해졌네요
			CString stemp = _T("SELECT FORMAT(CONVERT(NUMERIC,(product_price * ");
			stemp+= CString(std::to_string(dlg->GetSQL()[i].second).c_str()) +_T(" )), N'#,0') FROM bakery_catalog WHERE product_name = '");
			stemp += dlg->GetSQL()[i].first + _T("'");
			CString price;
			DBSet->Open(CRecordset::dynaset, stemp);
			DBSet->GetFieldValue(short(0), price);
			DBSet->Close();
			price.Remove(',');

			CString temp(_T("INSERT INTO order_info (branch_name, sale_price, customer_id, settlement_flag) VALUES("));
			temp += _T("'") + dlg->GetBranchCus()[i].first + _T("'") + _T(",") + price;
			temp += _T(",") + dlg->GetBranchCus()[i].second + _T(", 'Y')");
			temp += _T(", 'Y')");
			DBLink->BeginTrans();
			DBLink->ExecuteSQL(temp);
			DBLink->CommitTrans();
			DBSet->Open(CRecordset::dynaset, _T("SELECT @@IDENTITY"));
			CString ordernum;
			DBSet->GetFieldValue(short(0), ordernum);
			DBSet->Close();

			CString ttemp = _T("INSERT INTO order_product (order_no, product_name, sale_volume) VALUES(");
			ttemp += ordernum;
			ttemp += _T(",");
			ttemp += _T("'") + dlg->GetSQL()[i].first;
			ttemp += _T("'");
			ttemp += _T(",") + CString(std::to_string(dlg->GetSQL()[i].second).c_str());
			ttemp += _T(")");
			DBLink->BeginTrans();
			DBLink->ExecuteSQL(_T("SET IDENTITY_INSERT order_product ON"));
			DBLink->ExecuteSQL(ttemp);
			DBLink->ExecuteSQL(_T("SET IDENTITY_INSERT order_product OFF"));
			DBLink->CommitTrans();

			CString tttemp = _T("UPDATE order_info SET settlement_datetime = getdate() WHERE order_no = ");
			tttemp += ordernum;
			DBLink->BeginTrans();
			DBLink->ExecuteSQL(tttemp);
			DBLink->CommitTrans();

			CString updateTemp = _T("UPDATE branch_stock SET stock_volume = stock_volume - ");
			updateTemp += CString(std::to_string(dlg->GetSQL()[i].second).c_str()) += _T(" WHERE branch_name = '") + dlg->GetBranchCus()[i].first + _T("' AND product_name = '") + dlg->GetSQL()[i].first + _T("'");
			DBLink->BeginTrans();
			DBLink->ExecuteSQL(updateTemp);
			DBLink->CommitTrans();
		}
	}
}


void CBBanggDBPosDlg::OnBnClickedButtonStastics()
{
	// TODO: Add your control notification handler code here
	TotalAnalsys* end = new TotalAnalsys;
	end->DoModal();
}
