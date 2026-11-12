// SelectObjDlg.h : header file
//
// CLD - Cylu Level Designer
// (C) 1996-2026 Paul S.J.Millard
//

#pragma once

/////////////////////////////////////////////////////////////////////////////
// CSelectObjDlg dialog

class CSelectObjDlg : public CDialog
{
// Construction
public:
	CSelectObjDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	enum { IDD = IDD_SELECT_OBJECT };
	CComboBox		m_ctrlObjectCombo;
	CSpinButtonCtrl	m_ctrlAttribSpin;

// Attributes
public:
	int m_nSelectedObject;		//	Selected object from list box
	int m_nSelectedAttrib;		//	Selected Attribute number

// Overrides
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	void UpdateAttribute( void );

	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeObjectCombo();
	virtual void OnOK();
	DECLARE_MESSAGE_MAP()
};
