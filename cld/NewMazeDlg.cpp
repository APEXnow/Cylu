// NewMazeDlg.cpp : implementation file
//
// CLD - Cylu Level Designer
// (C) 1996-2026 Paul S.J.Millard
//

#include "stdafx.h"
#include "CLD.h"
#include "NewMazeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CNewMazeDlg dialog

CNewMazeDlg::CNewMazeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNewMazeDlg::IDD, pParent)
{
	m_nEditWidth = DEFAULT_LEVEL_WIDTH;
	m_nEditHeight = DEFAULT_LEVEL_HEIGHT;
}

void CNewMazeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_WIDTH, m_nEditWidth);
	DDV_MinMaxInt(pDX, m_nEditWidth, 3, MAX_WIDTH);
	DDX_Text(pDX, IDC_EDIT_HEIGHT, m_nEditHeight);
	DDV_MinMaxInt(pDX, m_nEditHeight, 3, MAX_HEIGHT);
}

BEGIN_MESSAGE_MAP(CNewMazeDlg, CDialog)
END_MESSAGE_MAP()
