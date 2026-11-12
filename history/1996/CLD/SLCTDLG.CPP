// SLCTDLG.cpp : implementation file
//

#include "stdafx.h"
#include "CLD.h"
#include "SLCTDlg.h"
#include "..\\cylu\\general.h"		//	General functions

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSelectObjDlg dialog


CSelectObjDlg::CSelectObjDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSelectObjDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSelectObjDlg)
	m_cstrAttribEdit = _T("");
	//}}AFX_DATA_INIT
}


void CSelectObjDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSelectObjDlg)
	DDX_Control(pDX, IDC_ATTRIB_SPIN, m_ctrlAttribSpin);
	DDX_Text(pDX, IDC_ATTRIB_EDIT, m_cstrAttribEdit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSelectObjDlg, CDialog)
	//{{AFX_MSG_MAP(CSelectObjDlg)
	ON_CBN_SELCHANGE(IDC_OBJECT_COMBO, OnSelchangeObjectCombo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSelectObjDlg message handlers

BOOL CSelectObjDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_ctrlAttribSpin.SetRange( 0, 30 );
	m_ctrlAttribSpin.SetPos( 1 );//	Do this just to update buddy window.
	m_ctrlAttribSpin.SetPos( m_nSelectedAttrib );

	CComboBox *pObjCombo = (CComboBox*)GetDlgItem( IDC_OBJECT_COMBO );
	pObjCombo->SetCurSel( m_nSelectedObject );
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSelectObjDlg::OnSelchangeObjectCombo() 
{
	// TODO: Add your control notification handler code here
	
}

void CSelectObjDlg::OnOK() 
{
	CComboBox *pObjCombo = (CComboBox*)GetDlgItem( IDC_OBJECT_COMBO );
	m_nSelectedObject = pObjCombo->GetCurSel();

	m_nSelectedAttrib = m_ctrlAttribSpin.GetPos();
	
	CDialog::OnOK();
}
