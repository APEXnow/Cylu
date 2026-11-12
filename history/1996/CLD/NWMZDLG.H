// NWMZDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CNewMazeDlg dialog

class CNewMazeDlg : public CDialog
{
// Construction
public:
	CNewMazeDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CNewMazeDlg)
	enum { IDD = IDD_NEWMAZE };
	int		m_nEditHeight;
	int		m_nEditWidth;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNewMazeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CNewMazeDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
