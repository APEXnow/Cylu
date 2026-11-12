// CLDView.h : interface of the CCLDView class
//
/////////////////////////////////////////////////////////////////////////////

#define EDITMODE_PLACE			0
#define EDITMODE_DELETE			1
#define EDITMODE_LINE			2
#define EDITMODE_BOX			3

class CCLDView : public CScrollView
{
protected: // create from serialization only
	CCLDView();
	DECLARE_DYNCREATE(CCLDView)

// Attributes
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CCLDDoc* GetDocument();
	int m_nCursorX;
	int m_nCursorY;
	int m_nSelectedObject;			//	Selected object for editing
	int m_nSelectedAttrib;			//	Selected attribute for object
	int m_nEditMode;				//	Current edit mode
	HGLOBAL m_hgblCBitmaps;			//	Memory pointer to CBitmaps
	BOOL m_bDrawScreenGrid;			//	Flag for screen grid drawing
	BOOL m_bShowAttribs;			//	Show attribute numbers

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCLDView)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	protected:
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void OnDraw(CDC* pDC);
	//}}AFX_VIRTUAL

// Implementation
	void ReleaseBitmaps( void );		//	Release memory used by bitmaps
	BOOL InitialiseBitmaps( void );		//	Allocate memory for bitmaps
	void DrawScreenGrid( CDC *pDC );	//	Draw Screen grid
	void ShowAttribs( CDC *pDC );		//	Show object attributes

public:
	virtual ~CCLDView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CCLDView)
	afx_msg void OnUpdatePlaceObject(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDeleteObject(CCmdUI* pCmdUI);
	afx_msg void OnDeleteObject();
	afx_msg void OnPlaceObject();
	afx_msg void OnSelectObject();
	afx_msg void OnUpdateSelectObject(CCmdUI* pCmdUI);
	afx_msg void OnScreenGrid();
	afx_msg void OnUpdateScreenGrid(CCmdUI* pCmdUI);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnUpdateObjectAttributes(CCmdUI* pCmdUI);
	afx_msg void OnObjectAttributes();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in CLDView.cpp
inline CCLDDoc* CCLDView::GetDocument()
   { return (CCLDDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////
