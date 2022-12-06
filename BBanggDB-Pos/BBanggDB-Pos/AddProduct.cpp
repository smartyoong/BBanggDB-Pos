// AddProduct.cpp : implementation file
//

#include "pch.h"
#include "BBanggDB-Pos.h"
#include "AddProduct.h"
#include "afxdialogex.h"


// AddProduct dialog

IMPLEMENT_DYNAMIC(AddProduct, CDialogEx)

AddProduct::AddProduct(CWnd* pParent /*=nullptr*/) :
	 CDialogEx(IDD_DIALOG_ADD, pParent)
	, ProductName(_T(""))
	, BrandName(_T(""))
{
}


AddProduct::~AddProduct()
{
}

void AddProduct::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_BRAND, ComboBrand);
	DDX_Control(pDX, IDC_COMBO_PRODUCT, ComboProduct);
	DDX_Control(pDX, IDC_SPIN_NUMBER, SpinNumber);
	DDX_CBString(pDX, IDC_COMBO_PRODUCT, ProductName);
	DDX_CBString(pDX, IDC_COMBO_BRAND, BrandName);
}


BEGIN_MESSAGE_MAP(AddProduct, CDialogEx)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_NUMBER, &AddProduct::OnDeltaposSpinNumber)
	ON_CBN_SELCHANGE(IDC_COMBO_PRODUCT, &AddProduct::OnCbnSelchangeComboProduct)
	ON_CBN_SELCHANGE(IDC_COMBO_BRAND, &AddProduct::OnCbnSelchangeComboBrand)
END_MESSAGE_MAP()


// AddProduct message handlers


void AddProduct::OnDeltaposSpinNumber(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: Add your control notification handler code here
	if (pNMUpDown->iDelta < 0)
		SpinValue--;
	else
		SpinValue++;
	if (SpinValue < 0)
		SpinValue = 0;
	SetDlgItemInt(IDC_EDIT_NUMBER, SpinValue);
	*pResult = 0;
}


void AddProduct::OnCbnSelchangeComboProduct()
{
	// TODO: Add your control notification handler code here
	UpdateData();
}


void AddProduct::OnCbnSelchangeComboBrand()
{
	// TODO: Add your control notification handler code here
	UpdateData();
}


BOOL AddProduct::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	SpinNumber.SetRange(0, 200);
	ComboBrand.AddString(_T("��������")); // DB�� �귣�� ���� �߰��� �޴� ���� ��� ���Ƿ� �̷��� �ۼ�
	ComboBrand.AddString(_T("��������"));
	CDatabase* DBLink = new CDatabase;
	DBLink->OpenEx(_T("DSN=Localhost"));
	CRecordset* DBSet = new CRecordset(DBLink);
	// ��ǰ īŻ����ũ���� ��ǰ����� ������
	DBSet->Open(CRecordset::dynamic, _T("SELECT product_name FROM bakery_catalog"));
	while (!DBSet->IsEOF())
	{
		CString temp;
		DBSet->GetFieldValue(short(0), temp);
		ComboProduct.AddString(temp);
		DBSet->MoveNext();
	}
	DBSet->Close();
	DBLink->Close();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}
