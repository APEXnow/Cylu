// NewMazeDlg.h : header file
//
// CLD - Cylu Level Designer
// (C) 1996-2026 Paul S.J.Millard
//

#pragma once

/////////////////////////////////////////////////////////////////////////////
// CNewMazeDlg dialog

class CNewMazeDlg : public CDialog
{
// Construction
public:
	CNewMazeDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	enum { IDD = IDD_NEWMAZE };
	int		m_nEditHeight;
	int		m_nEditWidth;

// Overrides
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};
