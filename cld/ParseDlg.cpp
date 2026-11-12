// ParseDlg.cpp : implementation file
//
// CLD - Cylu Level Designer
// (C) 1996-2026 Paul S.J.Millard
//

#include "stdafx.h"
#include "CLD.h"
#include "ParseDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CParseDlg dialog

CParseDlg::CParseDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CParseDlg::IDD, pParent)
{
	m_pIssues = NULL;
	m_nSelected = -1;
}

void CParseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATISTICS, m_cstrStatistics);
	DDX_Control(pDX, IDC_ISSUES, m_ctrlIssues);
}

BEGIN_MESSAGE_MAP(CParseDlg, CDialog)
	ON_LBN_DBLCLK(IDC_ISSUES, &CParseDlg::OnDblclkIssues)
	ON_LBN_SELCHANGE(IDC_ISSUES, &CParseDlg::OnSelchangeIssues)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParseDlg message handlers

BOOL CParseDlg::OnInitDialog()
{
	CString cstrLine;
	CDC *pDC;
	CFont *pOldFont;
	int nWidest = 0;
	int nIndex;
	size_t i;

	CDialog::OnInitDialog();

	if( !m_pIssues || m_pIssues->empty() )
	{
		m_ctrlIssues.AddString( _T("No problems found. The level is ready to play.") );
		m_ctrlIssues.EnableWindow( FALSE );
	}
	else
	{
		pDC = m_ctrlIssues.GetDC();
		pOldFont = pDC->SelectObject( m_ctrlIssues.GetFont() );

		for( i=0; i<m_pIssues->size(); i++ )
		{
			const LEVELISSUE &Issue = (*m_pIssues)[i];
			LPCTSTR szLevel = Issue.m_nSeverity == ISSUE_ERROR ? _T("Error") :
							  Issue.m_nSeverity == ISSUE_WARNING ? _T("Warning") : _T("Note");

			if( Issue.m_nX >= 0 )
				cstrLine.Format( _T("%s  (%d,%d)  %hs"), szLevel, Issue.m_nX, Issue.m_nY, Issue.m_strText.c_str() );
			else
				cstrLine.Format( _T("%s  %hs"), szLevel, Issue.m_strText.c_str() );

			nIndex = m_ctrlIssues.AddString( cstrLine );
			m_ctrlIssues.SetItemData( nIndex, (DWORD_PTR)i );

			CSize cSize = pDC->GetTextExtent( cstrLine );
			if( cSize.cx > nWidest )
				nWidest = cSize.cx;
		}

		pDC->SelectObject( pOldFont );
		m_ctrlIssues.ReleaseDC( pDC );

		//	Let long lines scroll sideways
		m_ctrlIssues.SetHorizontalExtent( nWidest + 8 );
		m_ctrlIssues.SetCurSel( 0 );
	}

	OnSelchangeIssues();

	return TRUE;  // return TRUE unless you set the focus to a control
}

void CParseDlg::OnSelchangeIssues()
{
	BOOL bCanGo = FALSE;
	int nSel = m_ctrlIssues.GetCurSel();

	if( m_pIssues && nSel != LB_ERR && m_ctrlIssues.IsWindowEnabled() )
	{
		size_t nIssue = (size_t)m_ctrlIssues.GetItemData( nSel );
		if( nIssue < m_pIssues->size() && (*m_pIssues)[nIssue].m_nX >= 0 )
			bCanGo = TRUE;
	}

	GetDlgItem( IDOK )->EnableWindow( bCanGo );
}

void CParseDlg::OnDblclkIssues()
{
	if( GetDlgItem( IDOK )->IsWindowEnabled() )
		OnOK();
}

/*
	Function:		OnOK
	Description:	"Go To" - close and tell the view which issue to show
*/
void CParseDlg::OnOK()
{
	int nSel = m_ctrlIssues.GetCurSel();

	m_nSelected = -1;
	if( m_pIssues && nSel != LB_ERR )
		m_nSelected = (int)m_ctrlIssues.GetItemData( nSel );

	CDialog::OnOK();
}
