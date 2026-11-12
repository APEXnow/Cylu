// ParseDlg.h : header file
//
// CLD - Cylu Level Designer
// (C) 1996-2026 Paul S.J.Millard
//

#pragma once

#include "levelcheck.h"

/////////////////////////////////////////////////////////////////////////////
// CParseDlg dialog
//
// Shows the level statistics (as the 1996 version did) and the list of
// problems found by CheckLevel.  Choosing "Go To" closes the dialog with
// IDOK and m_nSelected set, so the view can scroll to that cell.

class CParseDlg : public CDialog
{
// Construction
public:
	CParseDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	enum { IDD = IDD_PARSER };
	CString	m_cstrStatistics;
	CListBox m_ctrlIssues;

// Attributes
public:
	const std::vector<LEVELISSUE> *m_pIssues;	//	Set before DoModal
	int m_nSelected;							//	Issue to go to, or -1

// Overrides
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnDblclkIssues();
	afx_msg void OnSelchangeIssues();
	DECLARE_MESSAGE_MAP()
};
