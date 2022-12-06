#pragma once
#include "BBanggDB-PosDlg.h"

// AddProduct dialog

class AddProduct : public CDialogEx
{
	DECLARE_DYNAMIC(AddProduct)

public:
	AddProduct(CWnd* pParent = nullptr);   // standard constructor
	virtual ~AddProduct();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_ADD };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
	CComboBox ComboBrand;
	CComboBox ComboProduct;
	CSpinButtonCtrl SpinNumber;
	int SpinValue=0;
	CString ProductName;
	CString BrandName;
public:
	//getter
	CString GetBrand() { return BrandName; }
	CString GetProduct() { return ProductName; }
	int GetNumber() { return SpinValue; }
	afx_msg void OnDeltaposSpinNumber(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCbnSelchangeComboProduct();
	afx_msg void OnCbnSelchangeComboBrand();
	virtual BOOL OnInitDialog();
};
