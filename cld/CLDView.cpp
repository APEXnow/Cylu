// CLDView.cpp : implementation of the CCLDView class
//
// CLD - Cylu Level Designer
// (C) 1996-2026 Paul S.J.Millard
//

#include "stdafx.h"
#include "CLD.h"

#include "CLDDoc.h"
#include "CLDView.h"
#include "SelectObjDlg.h"
#include "ParseDlg.h"
#include "levelcheck.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define COLOR_FLOOR				RGB( 255, 255, 255 )
#define COLOR_GRID				RGB( 205, 205, 215 )
#define COLOR_HOVER				RGB( 255, 140, 0 )
#define COLOR_HIGHLIGHT			RGB( 255, 0, 0 )
#define COLOR_PREVIEW			RGB( 255, 230, 0 )

/////////////////////////////////////////////////////////////////////////////
// CCLDView

IMPLEMENT_DYNCREATE(CCLDView, CScrollView)

BEGIN_MESSAGE_MAP(CCLDView, CScrollView)
	ON_COMMAND(IDM_PLACE_OBJECT, &CCLDView::OnPlaceObject)
	ON_UPDATE_COMMAND_UI(IDM_PLACE_OBJECT, &CCLDView::OnUpdatePlaceObject)
	ON_COMMAND(IDM_DELETE_OBJECT, &CCLDView::OnDeleteObject)
	ON_UPDATE_COMMAND_UI(IDM_DELETE_OBJECT, &CCLDView::OnUpdateDeleteObject)
	ON_COMMAND(IDM_DRAW_LINE, &CCLDView::OnDrawLine)
	ON_UPDATE_COMMAND_UI(IDM_DRAW_LINE, &CCLDView::OnUpdateDrawLine)
	ON_COMMAND(IDM_DRAW_BOX, &CCLDView::OnDrawBox)
	ON_UPDATE_COMMAND_UI(IDM_DRAW_BOX, &CCLDView::OnUpdateDrawBox)
	ON_COMMAND(IDM_SELECT_OBJECT, &CCLDView::OnSelectObject)
	ON_UPDATE_COMMAND_UI(IDM_SELECT_OBJECT, &CCLDView::OnUpdateSelectObject)
	ON_COMMAND(IDM_SCREEN_GRID, &CCLDView::OnScreenGrid)
	ON_UPDATE_COMMAND_UI(IDM_SCREEN_GRID, &CCLDView::OnUpdateScreenGrid)
	ON_COMMAND(IDM_OBJECT_ATTRIBUTES, &CCLDView::OnObjectAttributes)
	ON_UPDATE_COMMAND_UI(IDM_OBJECT_ATTRIBUTES, &CCLDView::OnUpdateObjectAttributes)
	ON_COMMAND(IDM_ZOOM_IN, &CCLDView::OnZoomIn)
	ON_UPDATE_COMMAND_UI(IDM_ZOOM_IN, &CCLDView::OnUpdateZoomIn)
	ON_COMMAND(IDM_ZOOM_OUT, &CCLDView::OnZoomOut)
	ON_UPDATE_COMMAND_UI(IDM_ZOOM_OUT, &CCLDView::OnUpdateZoomOut)
	ON_COMMAND(IDM_ATTRIB_UP, &CCLDView::OnAttribUp)
	ON_COMMAND(IDM_ATTRIB_DOWN, &CCLDView::OnAttribDown)
	ON_COMMAND(IDM_PARSE_LEVEL, &CCLDView::OnParseLevel)
	ON_UPDATE_COMMAND_UI(IDM_PARSE_LEVEL, &CCLDView::OnUpdateParseLevel)
	ON_COMMAND(IDM_TEST_LEVEL, &CCLDView::OnTestLevel)
	ON_UPDATE_COMMAND_UI(IDM_TEST_LEVEL, &CCLDView::OnUpdateTestLevel)
	ON_COMMAND_RANGE(IDM_OBJECT_FIRST, IDM_OBJECT_LAST, &CCLDView::OnPaletteObject)
	ON_UPDATE_COMMAND_UI_RANGE(IDM_OBJECT_FIRST, IDM_OBJECT_LAST, &CCLDView::OnUpdatePaletteObject)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_CELL, &CCLDView::OnUpdateIndicatorCell)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_SELECTED, &CCLDView::OnUpdateIndicatorSelected)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSELEAVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_CAPTURECHANGED()
	ON_WM_ERASEBKGND()
	ON_WM_SETCURSOR()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCLDView construction/destruction

CCLDView::CCLDView()
{
	m_nEditMode = EDITMODE_PLACE;
	m_nSelectedObject = OBJECT_WALL;
	m_nSelectedAttrib = 0;
	m_nZoom = DEFAULT_ZOOM;
	m_bDrawScreenGrid = FALSE;
	m_bShowAttribs = FALSE;

	m_bDragging = FALSE;
	m_bErasing = FALSE;
	m_bStrokeChanged = FALSE;
	m_bShapeFilled = FALSE;
	m_bTrackingMouse = FALSE;
	m_ptDragStart = CPoint( -1, -1 );
	m_ptLastCell = CPoint( -1, -1 );
	m_ptHover = CPoint( -1, -1 );
	m_ptHighlight = CPoint( -1, -1 );

	if( !m_cTiles.LoadBitmap( IDB_TILES ) )	//	Load the 1996 map tiles
		AfxMessageBox( _T("Failed to load map bitmaps into memory") );
}

CCLDView::~CCLDView()
{
}

/////////////////////////////////////////////////////////////////////////////
// CCLDView diagnostics

#ifdef _DEBUG
void CCLDView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CCLDView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CCLDDoc* CCLDView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCLDDoc)));
	return (CCLDDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// Scrolling, zoom and coordinates

void CCLDView::UpdateScrollSizes( void )
{
	SIZE sizeMaze;
	CSize sizeTotal;

	GetDocument()->GetMazeSize( &sizeMaze );
	sizeTotal.cx = sizeMaze.cx * CellSize();
	sizeTotal.cy = sizeMaze.cy * CellSize();

	SetScrollSizes( MM_TEXT, sizeTotal, sizeDefault, CSize( CellSize(), CellSize() ) );
}

void CCLDView::OnInitialUpdate()
{
	m_ptHover = CPoint( -1, -1 );
	m_ptHighlight = CPoint( -1, -1 );
	m_bDragging = FALSE;

	UpdateScrollSizes();
	CScrollView::OnInitialUpdate();
}

void CCLDView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	UNREFERENCED_PARAMETER( pSender );
	UNREFERENCED_PARAMETER( lHint );
	UNREFERENCED_PARAMETER( pHint );

	UpdateScrollSizes();
	Invalidate( FALSE );
}

/*
	Function:		PointToCell
	Inputs:			ptClient			- Point in client coordinates
					ptCell				- Receives the level cell
	Output:			TRUE if the point is on the level

	Description:	Converts a mouse position to a level cell.  ptCell is
					set even when the point is off the level.
*/
BOOL CCLDView::PointToCell( CPoint ptClient, CPoint &ptCell )
{
	CClientDC dc( this );
	SIZE sizeMaze;
	int nCell = CellSize();

	OnPrepareDC( &dc );
	dc.DPtoLP( &ptClient );

	ptCell.x = ptClient.x >= 0 ? ptClient.x / nCell : -1;
	ptCell.y = ptClient.y >= 0 ? ptClient.y / nCell : -1;

	GetDocument()->GetMazeSize( &sizeMaze );

	return ptCell.x >= 0 && ptCell.y >= 0 && ptCell.x < sizeMaze.cx && ptCell.y < sizeMaze.cy;
}

void CCLDView::InvalidateCell( CPoint ptCell )
{
	CClientDC dc( this );
	CRect rc;
	int nCell = CellSize();

	if( ptCell.x < 0 || ptCell.y < 0 )
		return;

	rc.SetRect( ptCell.x * nCell, ptCell.y * nCell, ( ptCell.x + 1 ) * nCell, ( ptCell.y + 1 ) * nCell );
	rc.InflateRect( 3, 3 );			//	Room for the hover frame

	OnPrepareDC( &dc );
	dc.LPtoDP( &rc );
	InvalidateRect( &rc, FALSE );
}

void CCLDView::ScrollToCell( CPoint ptCell )
{
	CRect rcClient;
	CSize sizeTotal = GetTotalSize();
	CPoint ptPos;
	int nCell = CellSize();

	GetClientRect( &rcClient );

	ptPos.x = ptCell.x * nCell + nCell / 2 - rcClient.Width() / 2;
	ptPos.y = ptCell.y * nCell + nCell / 2 - rcClient.Height() / 2;

	ptPos.x = min( ptPos.x, (int)sizeTotal.cx - rcClient.Width() );
	ptPos.y = min( ptPos.y, (int)sizeTotal.cy - rcClient.Height() );
	ptPos.x = max( ptPos.x, 0 );
	ptPos.y = max( ptPos.y, 0 );

	ScrollToPosition( ptPos );
}

/*
	Function:		SetZoom
	Description:	Changes the tile size, keeping the cell in the middle of
					the window in the middle
*/
void CCLDView::SetZoom( int nZoom )
{
	CRect rcClient;
	CPoint ptCentre;

	if( nZoom < MIN_ZOOM ) nZoom = MIN_ZOOM;
	if( nZoom > MAX_ZOOM ) nZoom = MAX_ZOOM;
	if( nZoom == m_nZoom )
		return;

	GetClientRect( &rcClient );
	ptCentre = GetScrollPosition() + rcClient.CenterPoint();
	ptCentre.x /= CellSize();
	ptCentre.y /= CellSize();

	m_nZoom = nZoom;
	UpdateScrollSizes();
	ScrollToCell( ptCentre );
	Invalidate( FALSE );
}

/////////////////////////////////////////////////////////////////////////////
// Drawing

BOOL CCLDView::OnEraseBkgnd(CDC* pDC)
{
	UNREFERENCED_PARAMETER( pDC );
	return TRUE;		//	OnDraw paints everything, so there's no flicker
}

/*
	Function:		DrawTile
	Description:	Draws one object's 1996 tile, scaled to the zoom
*/
void CCLDView::DrawTile( CDC *pDC, CDC *pTileDC, int nObject, int nX, int nY )
{
	int nCell = CellSize();
	CRect rc( nX * nCell, nY * nCell, ( nX + 1 ) * nCell, ( nY + 1 ) * nCell );

	if( nObject == OBJECT_FLOOR )
		return;

	if( nObject > OBJECT_FLOOR && nObject < OBJECT_COUNT )
	{
		pDC->StretchBlt( rc.left, rc.top, nCell, nCell, pTileDC,
			( nObject - 1 ) * BITMAP_WIDTH, 0, BITMAP_WIDTH, BITMAP_HEIGHT, SRCCOPY );
	}
	else
	{
		//	Unknown object number
		pDC->FillSolidRect( &rc, RGB( 255, 0, 255 ) );
		pDC->SetTextColor( RGB( 255, 255, 255 ) );
		pDC->DrawText( _T("?"), 1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE );
	}
}

void CCLDView::OnDraw(CDC* pDC)
{
	CCLDDoc *pDoc = GetDocument();
	SIZE sizeMaze;
	CRect rcClip, rcMaze, rcCells, rcDraw;
	CDC dcMem, dcTile;
	CBitmap bmMem;
	CBitmap *pOldMem, *pOldTile;
	int nCell = CellSize();
	int nLoopx, nLoopy;
	size_t i;

	if( !pDoc )
		return;

	pDC->GetClipBox( &rcClip );		//	Logical coordinates
	if( rcClip.IsRectEmpty() )
		return;

	//	Draw into a memory bitmap the size of the area being painted, with
	//	its origin moved so the same logical coordinates can be used
	dcMem.CreateCompatibleDC( pDC );
	bmMem.CreateCompatibleBitmap( pDC, rcClip.Width(), rcClip.Height() );
	pOldMem = dcMem.SelectObject( &bmMem );
	dcMem.SetViewportOrg( -rcClip.left, -rcClip.top );

	dcMem.FillSolidRect( &rcClip, ::GetSysColor( COLOR_APPWORKSPACE ) );

	pDoc->GetMazeSize( &sizeMaze );
	rcMaze.SetRect( 0, 0, sizeMaze.cx * nCell, sizeMaze.cy * nCell );

	if( rcDraw.IntersectRect( &rcClip, &rcMaze ) )
	{
		CMaze *pMaze = pDoc->GetMaze();

		dcMem.FillSolidRect( &rcDraw, COLOR_FLOOR );

		//	Cells touched by the paint area
		rcCells.left = rcDraw.left / nCell;
		rcCells.top = rcDraw.top / nCell;
		rcCells.right = min( (int)sizeMaze.cx - 1, ( rcDraw.right - 1 ) / nCell );
		rcCells.bottom = min( (int)sizeMaze.cy - 1, ( rcDraw.bottom - 1 ) / nCell );

		dcTile.CreateCompatibleDC( pDC );
		pOldTile = dcTile.SelectObject( &m_cTiles );
		dcMem.SetStretchBltMode( COLORONCOLOR );		//	Keep the pixels sharp
		dcMem.SetBkMode( TRANSPARENT );

		for( nLoopy=rcCells.top; nLoopy<=rcCells.bottom; nLoopy++ )
			for( nLoopx=rcCells.left; nLoopx<=rcCells.right; nLoopx++ )
				DrawTile( &dcMem, &dcTile, pMaze->GetCell( nLoopx, nLoopy, CELL_OBJECT ), nLoopx, nLoopy );

		//	Preview of a line or box being dragged out
		if( m_bDragging && !m_bErasing && ( m_nEditMode == EDITMODE_LINE || m_nEditMode == EDITMODE_BOX ) )
		{
			std::vector<CPoint> Cells;
			CBrush cBrush( COLOR_PREVIEW );

			GetShapeCells( m_ptDragStart, m_ptLastCell, m_nEditMode, m_bShapeFilled, Cells );
			for( i=0; i<Cells.size(); i++ )
			{
				CRect rcCell( Cells[i].x * nCell, Cells[i].y * nCell, ( Cells[i].x + 1 ) * nCell, ( Cells[i].y + 1 ) * nCell );

				dcMem.FillSolidRect( &rcCell, COLOR_FLOOR );
				DrawTile( &dcMem, &dcTile, m_nSelectedObject, Cells[i].x, Cells[i].y );
				dcMem.FrameRect( &rcCell, &cBrush );
			}
		}

		dcTile.SelectObject( pOldTile );

		if( m_bDrawScreenGrid )
			DrawScreenGrid( &dcMem, rcCells );
		if( m_bShowAttribs )
			ShowAttribs( &dcMem, rcCells );

		//	Cell picked in the Parse Level list
		if( m_ptHighlight.x >= 0 )
		{
			CRect rc( m_ptHighlight.x * nCell, m_ptHighlight.y * nCell, ( m_ptHighlight.x + 1 ) * nCell, ( m_ptHighlight.y + 1 ) * nCell );
			CBrush cBrush( COLOR_HIGHLIGHT );

			for( i=0; i<3; i++ )
			{
				dcMem.FrameRect( &rc, &cBrush );
				rc.InflateRect( 1, 1 );
			}
		}

		//	Cell under the mouse
		if( m_ptHover.x >= 0 && m_ptHover.x < sizeMaze.cx && m_ptHover.y >= 0 && m_ptHover.y < sizeMaze.cy )
		{
			CRect rc( m_ptHover.x * nCell, m_ptHover.y * nCell, ( m_ptHover.x + 1 ) * nCell, ( m_ptHover.y + 1 ) * nCell );
			CBrush cBrush( COLOR_HOVER );

			dcMem.FrameRect( &rc, &cBrush );
			rc.DeflateRect( 1, 1 );
			dcMem.FrameRect( &rc, &cBrush );
		}
	}

	pDC->BitBlt( rcClip.left, rcClip.top, rcClip.Width(), rcClip.Height(), &dcMem, rcClip.left, rcClip.top, SRCCOPY );

	dcMem.SelectObject( pOldMem );
}

/*
	Function:		DrawScreenGrid
	Description:	Lines between the cells.  (In 1996 this outlined the
					screens of the original game, which scrolled a screen
					at a time; the current game scrolls a cell at a time.)
*/
void CCLDView::DrawScreenGrid( CDC *pDC, const CRect &rcCells )
{
	CPen cPen( PS_SOLID, 1, COLOR_GRID );
	CPen *pOldPen = pDC->SelectObject( &cPen );
	int nCell = CellSize();
	int nLoop;

	for( nLoop=rcCells.left; nLoop<=rcCells.right+1; nLoop++ )
	{
		pDC->MoveTo( nLoop * nCell, rcCells.top * nCell );
		pDC->LineTo( nLoop * nCell, ( rcCells.bottom + 1 ) * nCell );
	}

	for( nLoop=rcCells.top; nLoop<=rcCells.bottom+1; nLoop++ )
	{
		pDC->MoveTo( rcCells.left * nCell, nLoop * nCell );
		pDC->LineTo( ( rcCells.right + 1 ) * nCell, nLoop * nCell );
	}

	pDC->SelectObject( pOldPen );
}

/*
	Function:		ShowAttribs
	Description:	Writes each object's attribute number on its tile, in
					the 1996 colours.  The droid shows its direction.
*/
void CCLDView::ShowAttribs( CDC *pDC, const CRect &rcCells )
{
	static const TCHAR *szDirections[] = { _T("N"), _T("E"), _T("S"), _T("W") };
	CMaze *pMaze = GetDocument()->GetMaze();
	int nCell = CellSize();
	CFont cFontNumber;
	CFont *pOldFont;
	COLORREF crText = RGB( 0, 0, 0 ), crShadow;
	CString cstrOutput;
	int nLoopx, nLoopy, nObject, nAttrib;
	BOOL bDraw;

	cFontNumber.CreateFont( -max( 9, nCell * 11 / 20 ), 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
		DEFAULT_PITCH | FF_SWISS, _T("Segoe UI") );

	pOldFont = pDC->SelectObject( &cFontNumber );
	pDC->SetBkMode( TRANSPARENT );

	for( nLoopy=rcCells.top; nLoopy<=rcCells.bottom; nLoopy++ )
	{
		for( nLoopx=rcCells.left; nLoopx<=rcCells.right; nLoopx++ )
		{
			nObject = pMaze->GetCell( nLoopx, nLoopy, CELL_OBJECT );
			nAttrib = pMaze->GetCell( nLoopx, nLoopy, CELL_ATTRIBUTE );
			bDraw = TRUE;
			crShadow = RGB( 0, 0, 0 );

			switch( nObject )
			{
				case OBJECT_WALL:
					crText = RGB( 255, 255, 255 );
					break;
				case OBJECT_SHIELD:
					crText = RGB( 80, 140, 255 );
					break;
				case OBJECT_KEY:
					crText = RGB( 0, 0, 0 );
					crShadow = RGB( 255, 255, 255 );
					break;
				case OBJECT_TRANSPORT:
					crText = RGB( 0, 255, 0 );
					break;
				case OBJECT_OBJECT:
					crText = RGB( 255, 60, 60 );
					break;
				case OBJECT_CPU:
					crText = RGB( 255, 255, 0 );
					break;
				case OBJECT_DROID:
					crText = RGB( 255, 0, 255 );
					break;
				default:
					bDraw = FALSE;
					break;
			}

			if( bDraw )
			{
				CRect rc( nLoopx * nCell, nLoopy * nCell, ( nLoopx + 1 ) * nCell, ( nLoopy + 1 ) * nCell );

				if( nObject == OBJECT_DROID && nAttrib >= DROID_NORTH && nAttrib <= DROID_WEST )
					cstrOutput = szDirections[ nAttrib ];
				else
					cstrOutput.Format( _T("%d"), nAttrib );

				rc.OffsetRect( 1, 1 );
				pDC->SetTextColor( crShadow );
				pDC->DrawText( cstrOutput, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOCLIP );
				rc.OffsetRect( -1, -1 );
				pDC->SetTextColor( crText );
				pDC->DrawText( cstrOutput, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOCLIP );
			}
		}
	}

	pDC->SelectObject( pOldFont );
}

/////////////////////////////////////////////////////////////////////////////
// Editing

/*
	Function:		GetShapeCells
	Description:	Cells covered by a line (Bresenham) or a box between two
					cells.  Boxes are outlines unless bFilled.
*/
void CCLDView::GetShapeCells( CPoint ptFrom, CPoint ptTo, int nMode, BOOL bFilled, std::vector<CPoint> &Cells )
{
	int x, y;

	Cells.clear();

	if( nMode == EDITMODE_BOX )
	{
		int nLeft = min( ptFrom.x, ptTo.x ), nRight = max( ptFrom.x, ptTo.x );
		int nTop = min( ptFrom.y, ptTo.y ), nBottom = max( ptFrom.y, ptTo.y );

		for( y=nTop; y<=nBottom; y++ )
			for( x=nLeft; x<=nRight; x++ )
				if( bFilled || x == nLeft || x == nRight || y == nTop || y == nBottom )
					Cells.push_back( CPoint( x, y ) );
		return;
	}

	//	Line
	int dx = abs( ptTo.x - ptFrom.x ), sx = ptFrom.x < ptTo.x ? 1 : -1;
	int dy = -abs( ptTo.y - ptFrom.y ), sy = ptFrom.y < ptTo.y ? 1 : -1;
	int nErr = dx + dy, nErr2;

	x = ptFrom.x;
	y = ptFrom.y;

	for( ;; )
	{
		Cells.push_back( CPoint( x, y ) );
		if( x == ptTo.x && y == ptTo.y )
			break;
		nErr2 = 2 * nErr;
		if( nErr2 >= dy ) { nErr += dy; x += sx; }
		if( nErr2 <= dx ) { nErr += dx; y += sy; }
	}
}

void CCLDView::PaintCell( CPoint ptCell, BOOL bErase )
{
	int nChange;

	if( bErase )
		nChange = GetDocument()->SetCell( ptCell.x, ptCell.y, OBJECT_FLOOR, 0 );
	else
		nChange = GetDocument()->SetCell( ptCell.x, ptCell.y, m_nSelectedObject, m_nSelectedAttrib );

	if( nChange == CHANGE_NONE )
		return;

	m_bStrokeChanged = TRUE;

	if( nChange == CHANGE_OTHERS )
		Invalidate( FALSE );			//	The droid moved
	else
		InvalidateCell( ptCell );
}

/*
	Function:		PaintLine
	Description:	Paints every cell between two points, so a fast mouse
					stroke doesn't leave gaps
*/
void CCLDView::PaintLine( CPoint ptFrom, CPoint ptTo, BOOL bErase )
{
	std::vector<CPoint> Cells;
	size_t i;

	GetShapeCells( ptFrom, ptTo, EDITMODE_LINE, FALSE, Cells );
	for( i=0; i<Cells.size(); i++ )
		PaintCell( Cells[i], bErase );
}

void CCLDView::SetSelection( int nObject, int nAttrib )
{
	int nMax = AttributeMax( nObject );

	if( nAttrib < 0 ) nAttrib = 0;
	if( nAttrib > nMax ) nAttrib = nMax;

	m_nSelectedObject = nObject;
	m_nSelectedAttrib = nAttrib;
}

/*
	Function:		PickCell
	Description:	Ctrl+click: take the object and attribute from the map
*/
void CCLDView::PickCell( CPoint ptCell )
{
	CMaze *pMaze = GetDocument()->GetMaze();
	int nObject = pMaze->GetCell( ptCell.x, ptCell.y, CELL_OBJECT );

	if( nObject <= OBJECT_FLOOR || nObject >= OBJECT_COUNT )
		return;

	SetSelection( nObject, pMaze->GetCell( ptCell.x, ptCell.y, CELL_ATTRIBUTE ) );

	if( m_nEditMode == EDITMODE_DELETE )
		m_nEditMode = EDITMODE_PLACE;
}

void CCLDView::BeginStroke( CPoint ptCell, BOOL bErase, BOOL bShift )
{
	m_bDragging = TRUE;
	m_bErasing = bErase;
	m_bStrokeChanged = FALSE;
	m_bShapeFilled = bShift;
	m_ptDragStart = ptCell;
	m_ptLastCell = ptCell;

	if( m_ptHighlight.x >= 0 )
	{
		InvalidateCell( m_ptHighlight );
		m_ptHighlight = CPoint( -1, -1 );
	}

	SetCapture();
	GetDocument()->BeginChange();

	if( bErase || m_nEditMode == EDITMODE_PLACE || m_nEditMode == EDITMODE_DELETE )
		PaintCell( ptCell, bErase );
	else
		InvalidateCell( ptCell );			//	Start of the line or box preview
}

void CCLDView::EndStroke( CPoint ptCell )
{
	std::vector<CPoint> Cells;
	size_t i;

	UNREFERENCED_PARAMETER( ptCell );

	if( !m_bDragging )
		return;

	//	Clear the flag first: ReleaseCapture sends WM_CAPTURECHANGED
	m_bDragging = FALSE;
	ReleaseCapture();

	if( !m_bErasing && ( m_nEditMode == EDITMODE_LINE || m_nEditMode == EDITMODE_BOX ) )
	{
		GetShapeCells( m_ptDragStart, m_ptLastCell, m_nEditMode, m_bShapeFilled, Cells );
		for( i=0; i<Cells.size(); i++ )
			PaintCell( Cells[i], FALSE );
		Invalidate( FALSE );
	}

	GetDocument()->EndChange( m_bStrokeChanged );
}

void CCLDView::OnLButtonDown(UINT nFlags, CPoint point)
{
	CPoint ptCell;

	SetFocus();

	if( !m_bDragging && PointToCell( point, ptCell ) )
	{
		if( nFlags & MK_CONTROL )
			PickCell( ptCell );
		else
			BeginStroke( ptCell, m_nEditMode == EDITMODE_DELETE, ( nFlags & MK_SHIFT ) != 0 );
	}

	CScrollView::OnLButtonDown(nFlags, point);
}

void CCLDView::OnLButtonUp(UINT nFlags, CPoint point)
{
	CPoint ptCell;

	PointToCell( point, ptCell );
	EndStroke( ptCell );

	CScrollView::OnLButtonUp(nFlags, point);
}

/*
	The right button erases, as it did in 1996
*/
void CCLDView::OnRButtonDown(UINT nFlags, CPoint point)
{
	CPoint ptCell;

	SetFocus();

	if( !m_bDragging && PointToCell( point, ptCell ) )
		BeginStroke( ptCell, TRUE, FALSE );

	CScrollView::OnRButtonDown(nFlags, point);
}

void CCLDView::OnRButtonUp(UINT nFlags, CPoint point)
{
	CPoint ptCell;

	PointToCell( point, ptCell );
	EndStroke( ptCell );

	//	No context menu, so the base class isn't called
	UNREFERENCED_PARAMETER( nFlags );
}

void CCLDView::OnMouseMove(UINT nFlags, CPoint point)
{
	CPoint ptCell;
	SIZE sizeMaze;
	BOOL bOnLevel;

	bOnLevel = PointToCell( point, ptCell );

	if( !m_bTrackingMouse )
	{
		TRACKMOUSEEVENT tme = { sizeof( tme ), TME_LEAVE, m_hWnd, 0 };
		m_bTrackingMouse = TrackMouseEvent( &tme );
	}

	if( m_bDragging )
	{
		//	Keep strokes on the level even if the mouse leaves it
		GetDocument()->GetMazeSize( &sizeMaze );
		ptCell.x = max( 0, min( ptCell.x, (int)sizeMaze.cx - 1 ) );
		ptCell.y = max( 0, min( ptCell.y, (int)sizeMaze.cy - 1 ) );
		bOnLevel = TRUE;

		if( ptCell != m_ptLastCell )
		{
			if( m_bErasing || m_nEditMode == EDITMODE_PLACE || m_nEditMode == EDITMODE_DELETE )
				PaintLine( m_ptLastCell, ptCell, m_bErasing );
			else
				Invalidate( FALSE );		//	Redraw the shape preview

			m_ptLastCell = ptCell;
		}
	}

	if( !bOnLevel )
		ptCell = CPoint( -1, -1 );

	if( ptCell != m_ptHover )
	{
		InvalidateCell( m_ptHover );
		m_ptHover = ptCell;
		InvalidateCell( m_ptHover );
	}

	CScrollView::OnMouseMove(nFlags, point);
}

void CCLDView::OnMouseLeave()
{
	m_bTrackingMouse = FALSE;

	if( m_ptHover.x >= 0 && !m_bDragging )
	{
		InvalidateCell( m_ptHover );
		m_ptHover = CPoint( -1, -1 );
	}

	CScrollView::OnMouseLeave();
}

void CCLDView::OnCaptureChanged(CWnd *pWnd)
{
	//	Capture taken away mid-stroke (e.g. Alt+Tab): keep what was painted,
	//	but drop an unfinished line or box
	if( m_bDragging )
	{
		m_bDragging = FALSE;
		GetDocument()->EndChange( m_bStrokeChanged );
		Invalidate( FALSE );
	}

	CScrollView::OnCaptureChanged(pWnd);
}

BOOL CCLDView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	//	Ctrl+wheel zooms
	if( nFlags & MK_CONTROL )
	{
		SetZoom( m_nZoom + ( zDelta > 0 ? 1 : -1 ) );
		return TRUE;
	}

	return CScrollView::OnMouseWheel(nFlags, zDelta, pt);
}

BOOL CCLDView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	if( nHitTest == HTCLIENT && m_ptHover.x >= 0 )
	{
		::SetCursor( AfxGetApp()->LoadStandardCursor( IDC_CROSS ) );
		return TRUE;
	}

	return CScrollView::OnSetCursor(pWnd, nHitTest, message);
}

/////////////////////////////////////////////////////////////////////////////
// Edit modes

void CCLDView::OnPlaceObject()
{
	m_nEditMode = EDITMODE_PLACE;
}

void CCLDView::OnUpdatePlaceObject(CCmdUI* pCmdUI)
{
	pCmdUI->Enable( GetDocument()->GetMaze()->ValidMaze() );
	pCmdUI->SetCheck( m_nEditMode == EDITMODE_PLACE );
}

void CCLDView::OnDeleteObject()
{
	m_nEditMode = EDITMODE_DELETE;
}

void CCLDView::OnUpdateDeleteObject(CCmdUI* pCmdUI)
{
	pCmdUI->Enable( GetDocument()->GetMaze()->ValidMaze() );
	pCmdUI->SetCheck( m_nEditMode == EDITMODE_DELETE );
}

void CCLDView::OnDrawLine()
{
	m_nEditMode = EDITMODE_LINE;
}

void CCLDView::OnUpdateDrawLine(CCmdUI* pCmdUI)
{
	pCmdUI->Enable( GetDocument()->GetMaze()->ValidMaze() );
	pCmdUI->SetCheck( m_nEditMode == EDITMODE_LINE );
}

void CCLDView::OnDrawBox()
{
	m_nEditMode = EDITMODE_BOX;
}

void CCLDView::OnUpdateDrawBox(CCmdUI* pCmdUI)
{
	pCmdUI->Enable( GetDocument()->GetMaze()->ValidMaze() );
	pCmdUI->SetCheck( m_nEditMode == EDITMODE_BOX );
}

void CCLDView::OnSelectObject()
{
	CSelectObjDlg slctDlg;

	slctDlg.m_nSelectedObject = m_nSelectedObject;
	slctDlg.m_nSelectedAttrib = m_nSelectedAttrib;
	if( IDOK==slctDlg.DoModal() )
	{
		SetSelection( slctDlg.m_nSelectedObject, slctDlg.m_nSelectedAttrib );
		if( m_nEditMode == EDITMODE_DELETE )
			m_nEditMode = EDITMODE_PLACE;
	}
}

void CCLDView::OnUpdateSelectObject(CCmdUI* pCmdUI)
{
	pCmdUI->Enable( GetDocument()->GetMaze()->ValidMaze() );
}

void CCLDView::OnPaletteObject(UINT nID)
{
	SetSelection( (int)( nID - IDM_OBJECT_FIRST ), m_nSelectedAttrib );

	if( m_nEditMode == EDITMODE_DELETE )
		m_nEditMode = EDITMODE_PLACE;
}

void CCLDView::OnUpdatePaletteObject(CCmdUI* pCmdUI)
{
	pCmdUI->Enable( GetDocument()->GetMaze()->ValidMaze() );
	pCmdUI->SetCheck( m_nSelectedObject == (int)( pCmdUI->m_nID - IDM_OBJECT_FIRST ) );
}

/*
	] and [ step the attribute (wall type, forcefield number...) up and down
*/
void CCLDView::OnAttribUp()
{
	if( m_nSelectedAttrib < AttributeMax( m_nSelectedObject ) )
		m_nSelectedAttrib++;
	else
		MessageBeep( MB_OK );
}

void CCLDView::OnAttribDown()
{
	if( m_nSelectedAttrib > 0 )
		m_nSelectedAttrib--;
	else
		MessageBeep( MB_OK );
}

/////////////////////////////////////////////////////////////////////////////
// View options

void CCLDView::OnScreenGrid()
{
	m_bDrawScreenGrid = !m_bDrawScreenGrid;
	Invalidate( FALSE );
}

void CCLDView::OnUpdateScreenGrid(CCmdUI* pCmdUI)
{
	pCmdUI->Enable( GetDocument()->GetMaze()->ValidMaze() );
	pCmdUI->SetCheck( m_bDrawScreenGrid );
}

void CCLDView::OnObjectAttributes()
{
	m_bShowAttribs = !m_bShowAttribs;
	Invalidate( FALSE );
}

void CCLDView::OnUpdateObjectAttributes(CCmdUI* pCmdUI)
{
	pCmdUI->Enable( GetDocument()->GetMaze()->ValidMaze() );
	pCmdUI->SetCheck( m_bShowAttribs );
}

void CCLDView::OnZoomIn()
{
	SetZoom( m_nZoom + 1 );
}

void CCLDView::OnUpdateZoomIn(CCmdUI* pCmdUI)
{
	pCmdUI->Enable( m_nZoom < MAX_ZOOM );
}

void CCLDView::OnZoomOut()
{
	SetZoom( m_nZoom - 1 );
}

void CCLDView::OnUpdateZoomOut(CCmdUI* pCmdUI)
{
	pCmdUI->Enable( m_nZoom > MIN_ZOOM );
}

/////////////////////////////////////////////////////////////////////////////
// Status bar

void CCLDView::OnUpdateIndicatorCell(CCmdUI* pCmdUI)
{
	CString cstrText;
	CMaze *pMaze = GetDocument()->GetMaze();
	int nObject, nAttrib;

	if( m_ptHover.x >= 0 && pMaze->ValidMaze() )
	{
		nObject = pMaze->GetCell( m_ptHover.x, m_ptHover.y, CELL_OBJECT );
		nAttrib = pMaze->GetCell( m_ptHover.x, m_ptHover.y, CELL_ATTRIBUTE );

		if( AttributeName( nObject ) )
			cstrText.Format( _T("%d, %d   %hs %d"), m_ptHover.x, m_ptHover.y, ObjectName( nObject ), nAttrib );
		else
			cstrText.Format( _T("%d, %d   %hs"), m_ptHover.x, m_ptHover.y, ObjectName( nObject ) );
	}

	pCmdUI->Enable();
	pCmdUI->SetText( cstrText );
}

void CCLDView::OnUpdateIndicatorSelected(CCmdUI* pCmdUI)
{
	static const TCHAR *szModes[] = { _T("Place"), _T("Delete"), _T("Line"), _T("Box") };
	CString cstrText;

	if( m_nEditMode == EDITMODE_DELETE )
		cstrText = _T("Delete");
	else if( AttributeName( m_nSelectedObject ) )
		cstrText.Format( _T("%s: %hs %d"), szModes[ m_nEditMode ], ObjectName( m_nSelectedObject ), m_nSelectedAttrib );
	else
		cstrText.Format( _T("%s: %hs"), szModes[ m_nEditMode ], ObjectName( m_nSelectedObject ) );

	pCmdUI->Enable();
	pCmdUI->SetText( cstrText );
}

/////////////////////////////////////////////////////////////////////////////
// Parse Level and Test in Cylu

void CCLDView::OnParseLevel()
{
	CParseDlg parseDlg;
	LEVELSTATS Stats;
	std::vector<LEVELISSUE> Issues;
	int nErrors;

	nErrors = CheckLevel( *GetDocument()->GetMaze(), Stats, Issues );

	//	The 1996 statistics, plus the rest of the objects
	parseDlg.m_cstrStatistics.Format(
		_T("Objects: %d\nFuel Cells: %d\nChips: %d\nForceFields: %d\nTransporters: %d\nKeys: %d\n")
		_T("Droid: %d   Base: %d\nRefuel / Return: %d / %d\nTrees: %d\n\n%d error%s"),
		Stats.m_nObjects, Stats.m_nFuel, Stats.m_nChips, Stats.m_nForceFields, Stats.m_nTransporters,
		Stats.m_nKeys, Stats.m_nDroids, Stats.m_nBases, Stats.m_nRefuel, Stats.m_nReturn, Stats.m_nTrees,
		nErrors, nErrors == 1 ? _T("") : _T("s") );

	parseDlg.m_pIssues = &Issues;

	if( parseDlg.DoModal() == IDOK && parseDlg.m_nSelected >= 0 && parseDlg.m_nSelected < (int)Issues.size() )
	{
		const LEVELISSUE &Issue = Issues[ parseDlg.m_nSelected ];

		if( Issue.m_nX >= 0 )
		{
			m_ptHighlight = CPoint( Issue.m_nX, Issue.m_nY );
			ScrollToCell( m_ptHighlight );
			Invalidate( FALSE );
		}
	}
}

void CCLDView::OnUpdateParseLevel(CCmdUI* pCmdUI)
{
	pCmdUI->Enable( GetDocument()->GetMaze()->ValidMaze() );
}

/*
	Function:		OnTestLevel
	Description:	Saves the level to a temporary file and starts the game
					(cylu.exe, in the same folder as CLD) with -level
*/
void CCLDView::OnTestLevel()
{
	CCLDDoc *pDoc = GetDocument();
	LEVELSTATS Stats;
	std::vector<LEVELISSUE> Issues;
	TCHAR szPath[ MAX_PATH ];
	CString cstrFolder, cstrGame, cstrLevel, cstrParams, cstrMessage;
	HINSTANCE hResult;
	int nSlash;

	if( CheckLevel( *pDoc->GetMaze(), Stats, Issues ) > 0 )
	{
		if( AfxMessageBox( _T("Parse Level found errors that may stop this level playing.\n\nTest it anyway?"),
			MB_ICONQUESTION | MB_YESNO ) != IDYES )
			return;
	}

	//	The game is built to the same folder as CLD
	if( !GetModuleFileName( NULL, szPath, MAX_PATH ) )
		return;
	cstrFolder = szPath;
	nSlash = cstrFolder.ReverseFind( _T('\\') );
	cstrFolder = cstrFolder.Left( nSlash + 1 );
	cstrGame = cstrFolder + _T("cylu.exe");

	if( GetFileAttributes( cstrGame ) == INVALID_FILE_ATTRIBUTES )
	{
		cstrMessage.Format( _T("Couldn't find the game at\n%s\n\nBuild the Cylu project (same configuration) first."), (LPCTSTR)cstrGame );
		AfxMessageBox( cstrMessage, MB_ICONEXCLAMATION );
		return;
	}

	if( !GetTempPath( MAX_PATH, szPath ) )
		return;
	cstrLevel = szPath;
	cstrLevel += _T("cld_test.cld");

	if( !pDoc->SaveMazeTo( cstrLevel ) )
	{
		cstrMessage.Format( _T("Unable to write the test level to\n%s"), (LPCTSTR)cstrLevel );
		AfxMessageBox( cstrMessage, MB_ICONEXCLAMATION );
		return;
	}

	cstrParams.Format( _T("-window -level \"%s\""), (LPCTSTR)cstrLevel );

	hResult = ShellExecute( GetSafeHwnd(), _T("open"), cstrGame, cstrParams, cstrFolder, SW_SHOWNORMAL );
	if( (INT_PTR)hResult <= 32 )
	{
		cstrMessage.Format( _T("Unable to start\n%s"), (LPCTSTR)cstrGame );
		AfxMessageBox( cstrMessage, MB_ICONEXCLAMATION );
	}
}

void CCLDView::OnUpdateTestLevel(CCmdUI* pCmdUI)
{
	pCmdUI->Enable( GetDocument()->GetMaze()->ValidMaze() );
}
