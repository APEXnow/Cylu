// CLDView.h : interface of the CCLDView class
//
// CLD - Cylu Level Designer
// (C) 1996-2026 Paul S.J.Millard
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

#define EDITMODE_PLACE			0
#define EDITMODE_DELETE			1
#define EDITMODE_LINE			2
#define EDITMODE_BOX			3

#define MIN_ZOOM				1
#define MAX_ZOOM				5
#define DEFAULT_ZOOM			2

class CCLDDoc;

class CCLDView : public CScrollView
{
protected: // create from serialization only
	CCLDView();
	DECLARE_DYNCREATE(CCLDView)

// Attributes
public:
	CCLDDoc* GetDocument() const;

	int m_nSelectedObject;			//	Selected object for editing
	int m_nSelectedAttrib;			//	Selected attribute for object
	int m_nEditMode;				//	Current edit mode
	int m_nZoom;					//	Tile size is BITMAP_WIDTH * m_nZoom
	BOOL m_bDrawScreenGrid;			//	Flag for cell grid drawing
	BOOL m_bShowAttribs;			//	Show attribute numbers

	CBitmap m_cTiles;				//	The 1996 tiles, one per object (res\tiles.bmp)

	//	Mouse state
	BOOL m_bDragging;				//	Left or right button held in the view
	BOOL m_bErasing;				//	Right button stroke, or delete mode
	BOOL m_bStrokeChanged;			//	Anything changed during this stroke
	BOOL m_bShapeFilled;			//	Shift held when a box was started
	CPoint m_ptDragStart;			//	Cell the stroke started on
	CPoint m_ptLastCell;			//	Last cell painted during a stroke
	CPoint m_ptHover;				//	Cell under the mouse, or (-1,-1)
	CPoint m_ptHighlight;			//	Cell picked from the Parse Level list
	BOOL m_bTrackingMouse;			//	TrackMouseEvent active (for OnMouseLeave)

// Operations
public:
	int CellSize( void ) const		{ return BITMAP_WIDTH * m_nZoom; }

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual void OnInitialUpdate();
protected:
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);

// Implementation
public:
	virtual ~CCLDView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	void UpdateScrollSizes( void );
	BOOL PointToCell( CPoint ptClient, CPoint &ptCell );
	void InvalidateCell( CPoint ptCell );
	void ScrollToCell( CPoint ptCell );
	void SetZoom( int nZoom );

	void DrawTile( CDC *pDC, CDC *pTileDC, int nObject, int nX, int nY );
	void DrawScreenGrid( CDC *pDC, const CRect &rcCells );
	void ShowAttribs( CDC *pDC, const CRect &rcCells );

	void GetShapeCells( CPoint ptFrom, CPoint ptTo, int nMode, BOOL bFilled, std::vector<CPoint> &Cells );
	void PaintCell( CPoint ptCell, BOOL bErase );
	void PaintLine( CPoint ptFrom, CPoint ptTo, BOOL bErase );
	void PickCell( CPoint ptCell );
	void SetSelection( int nObject, int nAttrib );
	void LaunchGame( LPCTSTR szExe, LPCTSTR szParamFormat, LPCTSTR szProject );

// Generated message map functions
protected:
	afx_msg void OnUpdatePlaceObject(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDeleteObject(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDrawLine(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDrawBox(CCmdUI* pCmdUI);
	afx_msg void OnDeleteObject();
	afx_msg void OnPlaceObject();
	afx_msg void OnDrawLine();
	afx_msg void OnDrawBox();
	afx_msg void OnSelectObject();
	afx_msg void OnUpdateSelectObject(CCmdUI* pCmdUI);
	afx_msg void OnScreenGrid();
	afx_msg void OnUpdateScreenGrid(CCmdUI* pCmdUI);
	afx_msg void OnObjectAttributes();
	afx_msg void OnUpdateObjectAttributes(CCmdUI* pCmdUI);
	afx_msg void OnZoomIn();
	afx_msg void OnUpdateZoomIn(CCmdUI* pCmdUI);
	afx_msg void OnZoomOut();
	afx_msg void OnUpdateZoomOut(CCmdUI* pCmdUI);
	afx_msg void OnAttribUp();
	afx_msg void OnAttribDown();
	afx_msg void OnParseLevel();
	afx_msg void OnUpdateParseLevel(CCmdUI* pCmdUI);
	afx_msg void OnTestLevel();
	afx_msg void OnTestLevel96();
	afx_msg void OnUpdateTestLevel(CCmdUI* pCmdUI);
	afx_msg void OnPaletteObject(UINT nID);
	afx_msg void OnUpdatePaletteObject(CCmdUI* pCmdUI);
	afx_msg void OnUpdateIndicatorCell(CCmdUI* pCmdUI);
	afx_msg void OnUpdateIndicatorSelected(CCmdUI* pCmdUI);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnCaptureChanged(CWnd *pWnd);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	DECLARE_MESSAGE_MAP()

	void BeginStroke( CPoint ptCell, BOOL bErase, BOOL bShift );
	void EndStroke( CPoint ptCell );
};

#ifndef _DEBUG  // debug version in CLDView.cpp
inline CCLDDoc* CCLDView::GetDocument() const
   { return reinterpret_cast<CCLDDoc*>(m_pDocument); }
#endif

/////////////////////////////////////////////////////////////////////////////
