// NWMZDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CLD.h"
#include "NWMZDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNewMazeDlg dialog


CNewMazeDlg::CNewMazeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNewMazeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNewMazeDlg)
	m_nEditHeight = 0;
	m_nEditWidth = 0;
	//}}AFX_DATA_INIT
}


void CNewMazeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNewMazeDlg)
	DDX_Text(pDX, IDC_EDIT_HEIGHT, m_nEditHeight);
	DDX_Text(pDX, IDC_EDIT_WIDTH, m_nEditWidth);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNewMazeDlg, CDialog)
	//{{AFX_MSG_MAP(CNewMazeDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNewMazeDlg message handlers

BOOL CNewMazeDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_nEditWidth = DEFAULT_LEVEL_WIDTH;
	m_nEditHeight = DEFAULT_LEVEL_HEIGHT;

	UpdateData( FALSE );
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
