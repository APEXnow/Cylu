// SelectObjDlg.cpp : implementation file
//
// CLD - Cylu Level Designer
// (C) 1996-2026 Paul S.J.Millard
//

#include "stdafx.h"
#include "CLD.h"
#include "SelectObjDlg.h"
#include "levelcheck.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CSelectObjDlg dialog

CSelectObjDlg::CSelectObjDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSelectObjDlg::IDD, pParent)
{
	m_nSelectedObject = OBJECT_WALL;
	m_nSelectedAttrib = 0;
}

void CSelectObjDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_OBJECT_COMBO, m_ctrlObjectCombo);
	DDX_Control(pDX, IDC_ATTRIB_SPIN, m_ctrlAttribSpin);
}

BEGIN_MESSAGE_MAP(CSelectObjDlg, CDialog)
	ON_CBN_SELCHANGE(IDC_OBJECT_COMBO, &CSelectObjDlg::OnSelchangeObjectCombo)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSelectObjDlg message handlers

BOOL CSelectObjDlg::OnInitDialog()
{
	int nObject;

	CDialog::OnInitDialog();

	//	The list is in object number order, as it was in 1996
	for( nObject=0; nObject<OBJECT_COUNT; nObject++ )
		m_ctrlObjectCombo.AddString( CString( ObjectName( nObject ) ) );

	if( m_nSelectedObject < 0 || m_nSelectedObject >= OBJECT_COUNT )
		m_nSelectedObject = OBJECT_WALL;

	m_ctrlObjectCombo.SetCurSel( m_nSelectedObject );

	UpdateAttribute();
	m_ctrlAttribSpin.SetPos32( m_nSelectedAttrib );

	return TRUE;  // return TRUE unless you set the focus to a control
}

/*
	Function:		UpdateAttribute
	Description:	Shows what the attribute means for the chosen object and
					limits the spin control to the values the game uses
*/
void CSelectObjDlg::UpdateAttribute( void )
{
	int nObject = m_ctrlObjectCombo.GetCurSel();
	int nMax = AttributeMax( nObject );
	const char *szName = AttributeName( nObject );
	CString cstrHint;
	BOOL bError = FALSE;
	int nPos;

	if( szName )
		cstrHint.Format( _T("%hs (0 to %d)"), szName, nMax );
	else
		cstrHint = _T("Not used by this object");

	SetDlgItemText( IDC_ATTRIB_HINT, cstrHint );

	nPos = m_ctrlAttribSpin.GetPos32( &bError );
	m_ctrlAttribSpin.SetRange32( 0, nMax );
	if( bError || nPos > nMax )
		m_ctrlAttribSpin.SetPos32( bError ? 0 : nMax );

	GetDlgItem( IDC_ATTRIB_EDIT )->EnableWindow( nMax > 0 );
	m_ctrlAttribSpin.EnableWindow( nMax > 0 );
}

void CSelectObjDlg::OnSelchangeObjectCombo()
{
	UpdateAttribute();
}

void CSelectObjDlg::OnOK()
{
	BOOL bError = FALSE;
	int nMax;

	m_nSelectedObject = m_ctrlObjectCombo.GetCurSel();
	if( m_nSelectedObject == CB_ERR )
		m_nSelectedObject = OBJECT_WALL;

	nMax = AttributeMax( m_nSelectedObject );
	m_nSelectedAttrib = m_ctrlAttribSpin.GetPos32( &bError );
	if( bError || m_nSelectedAttrib < 0 )
		m_nSelectedAttrib = 0;
	if( m_nSelectedAttrib > nMax )
		m_nSelectedAttrib = nMax;

	CDialog::OnOK();
}
