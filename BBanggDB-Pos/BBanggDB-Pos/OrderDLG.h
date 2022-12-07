#pragma once
#include "BBanggDB-Pos.h"
#include <vector>

// OrderDLG dialog

class OrderDLG : public CDialogEx
{
	DECLARE_DYNAMIC(OrderDLG)

public:
	OrderDLG(CWnd* pParent = nullptr);   // standard constructor
	virtual ~OrderDLG();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_ORDER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
	virtual BOOL OnInitDialog();
	CComboBox ComboCustomer;
	CComboBox ComboBrand;
	CComboBox ComboProduct;
	CSpinButtonCtrl SpinVolume;
	CListBox ListCart;
	//CString CartSum;
	int SpinValue =0;
	int totalvalue = 0;
public:
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnDeltaposSpinVolume(NMHDR* pNMHDR, LRESULT* pResult);
private:
	CString BrandName;
	CString ProductName;
	std::vector<std::pair<CString,int>> OrderSql;
	std::vector<std::pair<CString,CString>> BranchCus;
public:
	afx_msg void OnCbnSelchangeComboBrand();
	CString GetBrandName() { return BrandName; }
	CString GetProductName() { return ProductName; }
	int GetSpinNum() { return SpinValue; }
	afx_msg void OnCbnSelchangeComboProduct();
	std::vector<std::pair<CString,int>> GetSQL() { return OrderSql; }
	std::vector<std::pair<CString,CString>> GetBranchCus() { return BranchCus; }
};
