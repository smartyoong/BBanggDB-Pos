
// BBanggDB-PosDlg.h : header file
//

#pragma once
#include <map>
#include "AddProduct.h"
#include "OrderDLG.h"

// CBBanggDBPosDlg dialog
class CBBanggDBPosDlg : public CDialogEx
{
// Construction
public:
	CBBanggDBPosDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BBANGGDBPOS_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

private:
	CDatabase* DBLink; // database ��ũ DB���� �����Ͱ� ����ɼ� �ִ� ������ ���⼭ ����
	CRecordset* DBSet; // �ܼ� Select���� �̰ɷ� ����
	std::map<int, CString> MappingField;
public:
//getter
	CDatabase* GetDataBase() { return DBLink; }
	CRecordset* GetRecordSet() { return DBSet; }
// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	// �� ����Ȳ �˸���
	CListBox ListCurrentProductInfo;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void MakeMap();
	afx_msg void OnBnClickedButtonAddProduct();
	afx_msg void OnBnClickedButtonOrder();
};
