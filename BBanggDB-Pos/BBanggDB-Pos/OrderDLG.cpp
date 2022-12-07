// OrderDLG.cpp : implementation file
//

#include "pch.h"
#include "BBanggDB-Pos.h"
#include "OrderDLG.h"
#include "afxdialogex.h"
#include <string>


// OrderDLG dialog

IMPLEMENT_DYNAMIC(OrderDLG, CDialogEx)

OrderDLG::OrderDLG(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_ORDER, pParent)
	//, CartSum(_T(""))
	, BrandName(_T(""))
	, ProductName(_T(""))
{

}

OrderDLG::~OrderDLG()
{
}

void OrderDLG::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_CUSTOM, ComboCustomer);
	DDX_Control(pDX, IDC_COMBO_BRAND, ComboBrand);
	DDX_Control(pDX, IDC_COMBO_PRODUCT, ComboProduct);
	DDX_Control(pDX, IDC_SPIN_VOLUME, SpinVolume);
	DDX_Control(pDX, IDC_LIST_CART, ListCart);
	//DDX_Text(pDX, IDC_EDIT_SUM, CartSum);
	DDX_CBString(pDX, IDC_COMBO_BRAND, BrandName);
	DDX_CBString(pDX, IDC_COMBO_PRODUCT, ProductName);
}


BEGIN_MESSAGE_MAP(OrderDLG, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &OrderDLG::OnBnClickedButtonAdd)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_VOLUME, &OrderDLG::OnDeltaposSpinVolume)
	ON_CBN_SELCHANGE(IDC_COMBO_BRAND, &OrderDLG::OnCbnSelchangeComboBrand)
	ON_CBN_SELCHANGE(IDC_COMBO_PRODUCT, &OrderDLG::OnCbnSelchangeComboProduct)
END_MESSAGE_MAP()


// OrderDLG message handlers


BOOL OrderDLG::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	SpinVolume.SetRange(0, 200);
	ComboProduct.EnableWindow(FALSE);
	ComboBrand.AddString(_T("서울지점"));
	ComboBrand.AddString(_T("대전지점"));
	CDatabase* DBLink = new CDatabase;
	DBLink->OpenEx(_T("DSN=Localhost")); //여기도 수정하셔야합니다.
	CRecordset* DBSet = new CRecordset(DBLink);
	CString temp = _T("SELECT * FROM customer_info");
	if (DBSet->Open(CRecordset::dynaset, temp))
	{
		while (!DBSet->IsEOF())
		{
			CString comment = _T("고객번호 : ");
			CString ttemp;
			DBSet->GetFieldValue(short(0), ttemp);
			comment += ttemp;
			comment += _T(" 고객 이름 : ");
			DBSet->GetFieldValue(short(1), ttemp);
			comment += ttemp;
			ComboCustomer.AddString(comment);
			DBSet->MoveNext();
		}
	}
	DBSet->Close();
	DBLink->Close();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void OrderDLG::OnBnClickedButtonAdd()
{
	// TODO: Add your control notification handler code here
	CString SQL = _T("SELECT (case when stock_volume > ");
	CString a(std::to_string(SpinValue).c_str());
	SQL += a;
	SQL += _T(" then 'ok' else 'no' end) FROM branch_stock WHERE branch_name = '"); // Update로 확인하는것보다 이런식으로 확인하는게 더 안전해서 바꿉니다.
	SQL += BrandName;
	SQL += _T("'");
	SQL += _T(" AND product_name = '");
	SQL += ProductName;
	SQL += _T("'");
	//SQL += _T(" LOCK IN SHARE MODE"); // 쉐어모드
	CDatabase * DBLink = new CDatabase;
	DBLink->OpenEx(_T("DSN=Localhost")); //바꾸셔야합니다.
	CRecordset* DBSet = new CRecordset(DBLink);
	DBSet->Open(CRecordset::dynamic, SQL);
	CString IsOk;
	DBSet->GetFieldValue(short(0), IsOk);
	if (IsOk != "ok")
	{
		AfxMessageBox(_T("재고가 부족합니다!"));
	}
	else
	{
		CString custom;
		CRecordset* cDBSet = new CRecordset(DBLink);
		cDBSet->Open(CRecordset::dynamic, _T("SELECT * FROM customer_info"));
		cDBSet->GetFieldValue(short(0), custom);
		OrderSql.push_back(std::make_pair(ProductName,SpinValue));
		BranchCus.push_back(std::make_pair(BrandName, custom));
		ListCart.AddString(BrandName + _T(" ") + ProductName + _T(" ") + std::to_string(SpinValue).c_str() + _T("개"));
		cDBSet->Close();
		delete cDBSet;
	}
	DBSet->Close();
	DBLink->Close();
	delete DBSet, DBLink;
}


void OrderDLG::OnDeltaposSpinVolume(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: Add your control notification handler code here
	if (pNMUpDown->iDelta < 0)
		SpinValue--;
	else
		SpinValue++;
	if (SpinValue < 0)
		SpinValue = 0;
	SetDlgItemInt(IDC_EDIT_VOLUME, SpinValue);
	*pResult = 0;
}


void OrderDLG::OnCbnSelchangeComboBrand()
{
	// TODO: Add your control notification handler code here
	ComboProduct.EnableWindow(TRUE);
	UpdateData();
	ComboProduct.ResetContent();
	CDatabase* DBLink = new CDatabase;
	DBLink->OpenEx(_T("DSN=Localhost")); //여기도 수정하셔야합니다.
	CString qtemp = _T("SELECT product_name FROM branch_stock WHERE branch_name = '");
	qtemp += BrandName;
	qtemp += _T("'");
	CRecordset* aDBSet = new CRecordset(DBLink);
	if (aDBSet->Open(CRecordset::dynaset, qtemp))
	{
		while (!aDBSet->IsEOF())
		{
			CString ttemp;
			aDBSet->GetFieldValue(short(0), ttemp);
			ComboProduct.AddString(ttemp);
			aDBSet->MoveNext();
		}
	}
	aDBSet->Close();
	DBLink->Close();
}


void OrderDLG::OnCbnSelchangeComboProduct()
{
	// TODO: Add your control notification handler code here
	UpdateData();
}
