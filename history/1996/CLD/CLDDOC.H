// CLDDoc.h : interface of the CCLDDoc class
//
/////////////////////////////////////////////////////////////////////////////

#include "..\\cylu\\cmaze.hpp"

class CCLDDoc : public CDocument
{
protected: // create from serialization only
	CCLDDoc();
	DECLARE_DYNCREATE(CCLDDoc)

// Attributes
public:
	CMaze m_cMaze;				//	CMaze object

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCLDDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCLDDoc();
	CMaze* GetMaze( void )
		{	return &m_cMaze; }
	void GetMazeSize( SIZE *lpSize );
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CCLDDoc)
	afx_msg void OnNewDimensions();
	afx_msg void OnUpdateFileSave(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileSaveAs(CCmdUI* pCmdUI);
	afx_msg void OnParseLevel();
	afx_msg void OnUpdateParseLevel(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
