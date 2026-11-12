// ParseDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CLD.h"
#include "ParseDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParseDlg dialog


CParseDlg::CParseDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CParseDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CParseDlg)
	m_cstrStatistics = _T("");
	//}}AFX_DATA_INIT
}


void CParseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CParseDlg)
	DDX_Text(pDX, IDC_STATISTICS, m_cstrStatistics);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CParseDlg, CDialog)
	//{{AFX_MSG_MAP(CParseDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParseDlg message handlers

BOOL CParseDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	UpdateData( FALSE );
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
