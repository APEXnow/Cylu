// SLCTDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSelectObjDlg dialog

class CSelectObjDlg : public CDialog
{
// Construction
public:
	CSelectObjDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSelectObjDlg)
	enum { IDD = IDD_SELECT_OBJECT };
	CSpinButtonCtrl	m_ctrlAttribSpin;
	CString	m_cstrAttribEdit;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSelectObjDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Attributes
public:
	int m_nSelectedObject;		//	Selected object from list box
	int m_nSelectedAttrib;		//	Selected Attribute number

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSelectObjDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeObjectCombo();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
