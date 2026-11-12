// ParseDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CParseDlg dialog

class CParseDlg : public CDialog
{
// Construction
public:
	CParseDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CParseDlg)
	enum { IDD = IDD_PARSER };
	CString	m_cstrStatistics;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CParseDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CParseDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
