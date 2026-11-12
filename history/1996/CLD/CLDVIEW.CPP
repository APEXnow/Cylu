// CLDView.cpp : implementation of the CCLDView class
//

#include "stdafx.h"
#include "CLD.h"

#include "CLDDoc.h"
#include "CLDView.h"
#include "slctdlg.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCLDView

IMPLEMENT_DYNCREATE(CCLDView, CScrollView)

BEGIN_MESSAGE_MAP(CCLDView, CScrollView)
	//{{AFX_MSG_MAP(CCLDView)
	ON_UPDATE_COMMAND_UI(IDM_PLACE_OBJECT, OnUpdatePlaceObject)
	ON_UPDATE_COMMAND_UI(IDM_DELETE_OBJECT, OnUpdateDeleteObject)
	ON_COMMAND(IDM_DELETE_OBJECT, OnDeleteObject)
	ON_COMMAND(IDM_PLACE_OBJECT, OnPlaceObject)
	ON_COMMAND(IDM_SELECT_OBJECT, OnSelectObject)
	ON_UPDATE_COMMAND_UI(IDM_SELECT_OBJECT, OnUpdateSelectObject)
	ON_COMMAND(IDM_SCREEN_GRID, OnScreenGrid)
	ON_UPDATE_COMMAND_UI(IDM_SCREEN_GRID, OnUpdateScreenGrid)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONDOWN()
	ON_UPDATE_COMMAND_UI(IDM_OBJECT_ATTRIBUTES, OnUpdateObjectAttributes)
	ON_COMMAND(IDM_OBJECT_ATTRIBUTES, OnObjectAttributes)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCLDView construction/destruction

CCLDView::CCLDView()
{
	m_hgblCBitmaps = NULL;
	m_nEditMode = EDITMODE_PLACE;
	m_nSelectedObject = OBJECT_WALL;
	m_nSelectedAttrib = NULL;
	m_bDrawScreenGrid = FALSE;
	m_bShowAttribs = FALSE;
	if( InitialiseBitmaps() )	//	Allocate memory and load bitmaps
		AfxMessageBox( "Failed to load map bitmaps into memory\n" );
}

CCLDView::~CCLDView()
{
	ReleaseBitmaps();		//	Unload and destroy memory for bitmaps
}

BOOL CCLDView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CCLDView diagnostics

#ifdef _DEBUG
void CCLDView::AssertValid() const
{
	CView::AssertValid();
}

void CCLDView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CCLDDoc* CCLDView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCLDDoc)));
	return (CCLDDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCLDView message handlers

BOOL CCLDView::PreTranslateMessage(MSG* pMsg)
{
	return CScrollView::PreTranslateMessage(pMsg);
}

void CCLDView::OnInitialUpdate() 
{
	CScrollView::OnInitialUpdate();
	
	CCLDDoc *pDoc = (CCLDDoc*)GetDocument();

	SIZE szScrollSizes;
	if( pDoc )
	{
		pDoc->GetMazeSize( &szScrollSizes );
		szScrollSizes.cx*=BITMAP_WIDTH;
		szScrollSizes.cy*=BITMAP_HEIGHT;
		SetScrollSizes( MM_TEXT, szScrollSizes );
	}
}

void CCLDView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	CCLDDoc *pDoc = (CCLDDoc*)GetDocument();

	SIZE szScrollSizes;
	CRect rcClient;

	if( pDoc )
	{
		pDoc->GetMazeSize( &szScrollSizes );
		szScrollSizes.cx*=BITMAP_WIDTH;
		szScrollSizes.cy*=BITMAP_HEIGHT;
		SetScrollSizes( MM_TEXT, szScrollSizes );
	}
}

void CCLDView::OnDraw(CDC* pDC) 
{
	CCLDDoc *pDoc = (CCLDDoc*)GetDocument();
	CPaintDC *cPaintDC = (CPaintDC*)pDC;
	RECT rcUpdate = cPaintDC->m_ps.rcPaint;
	int nLoopx;				//	For loops
	int nLoopy;
	int nXPos, nYPos;		//	Bitmap coordinates
	CDC CCompatibleDC;		//	Compatible memory DC
	SIZE sizeMaze;
	CBitmap **lppCBitmaps;	//	Memory pointer for bitmap pointers
	CBitmap *lpCBitmapOldCompat;
	int nCellObject;
	CMaze *lpCMaze;			//	Maze object
	int nXStart, nXFinish, nYStart, nYFinish;

	if( pDoc )
	{
		pDoc->GetMazeSize( &sizeMaze );
		if( ( sizeMaze.cx>0 ) && ( sizeMaze.cy>0 ) )
		{
			if( m_hgblCBitmaps )
			{
				//	Create a compatible DC from the client window DC
				CCompatibleDC.CreateCompatibleDC( pDC );
				if( !CCompatibleDC.GetSafeHdc() )	//	If failed
					TRACE( "CCLDView::OnDraw - Failed to create compatible DC\n" );
				else
				{
					lppCBitmaps = (CBitmap**)GlobalLock( m_hgblCBitmaps );
					pDoc->GetMazeSize( &sizeMaze );
					lpCMaze = pDoc->GetMaze();
					pDC->DPtoLP( &rcUpdate );
					nXStart = (rcUpdate.left / BITMAP_WIDTH );
					nYStart = (rcUpdate.top / BITMAP_HEIGHT );
					nXFinish = (rcUpdate.right / BITMAP_WIDTH )+1;
					nYFinish = (rcUpdate.bottom / BITMAP_HEIGHT )+1;
					if( nXFinish>sizeMaze.cx )
						nXFinish=sizeMaze.cx;
					if( nYFinish>sizeMaze.cy )
						nYFinish=sizeMaze.cy;
					for( nLoopy=nYStart; nLoopy<nYFinish; nLoopy++ )
					{
						for( nLoopx=nXStart; nLoopx<nXFinish; nLoopx++ )
						{
							nCellObject = lpCMaze->GetCell( nLoopx, nLoopy, CELL_OBJECT );
							if( OBJECT_FLOOR!=nCellObject )
							{
								lpCBitmapOldCompat = CCompatibleDC.SelectObject( lppCBitmaps[ nCellObject-1 ] );
								nXPos = nLoopx * BITMAP_WIDTH;
								nYPos = nLoopy * BITMAP_HEIGHT;
								pDC->BitBlt( nXPos, nYPos, BITMAP_WIDTH, BITMAP_HEIGHT, &CCompatibleDC, 0, 0, SRCCOPY );
								CCompatibleDC.SelectObject( lpCBitmapOldCompat );
							}
						}
					}
					CCompatibleDC.DeleteDC();
					GlobalUnlock( m_hgblCBitmaps );
					lppCBitmaps = NULL;
					if( m_bDrawScreenGrid )
						DrawScreenGrid( pDC );
					if( m_bShowAttribs )
						ShowAttribs( pDC );
				}
			}
			else
				TRACE( "CCLDView::OnDraw - No bitmaps to use\n" );
		}
	}
	else
		TRACE( "CCLDView::OnDraw - No CDocument associated with CScrollView\n" );
}

void CCLDView::DrawScreenGrid( CDC *pDC )
{
	CBrush cBrush( RGB( 0, 0, 255 ) );
	CCLDDoc *pDoc = (CCLDDoc*)GetDocument();
	CPaintDC *cPaintDC = (CPaintDC*)pDC;
	RECT rcUpdate = cPaintDC->m_ps.rcPaint;
	SIZE sizeMaze;
	int nLoopx;				//	For loops
	int nLoopy;
	CRgn rgn;
	CRect rc;
	CMaze *lpCMaze;			//	Maze object
	int nXStart, nXFinish, nYStart, nYFinish;
	int nHeight, nWidth;

	if( pDoc )
	{
		pDoc->GetMazeSize( &sizeMaze );
		if( ( sizeMaze.cx>0 ) && ( sizeMaze.cy>0 ) )
		{
			pDoc->GetMazeSize( &sizeMaze );
			lpCMaze = pDoc->GetMaze();
			pDC->DPtoLP( &rcUpdate );
			nXStart = (rcUpdate.left / BITMAP_WIDTH );
			nYStart = (rcUpdate.top / BITMAP_HEIGHT );
			nXFinish = (rcUpdate.right / BITMAP_WIDTH )+1;
			nYFinish = (rcUpdate.bottom / BITMAP_HEIGHT )+1;
			if( nXFinish>sizeMaze.cx )
				nXFinish=sizeMaze.cx;
			if( nYFinish>sizeMaze.cy )
				nYFinish=sizeMaze.cy;

			nWidth = sizeMaze.cx / PLAYAREA_WIDTH_STEP;
			nHeight = sizeMaze.cy / PLAYAREA_HEIGHT_STEP;

			for( nLoopy=0; nLoopy<nHeight; nLoopy++ )
			{
				for( nLoopx=0; nLoopx<nWidth; nLoopx++ )
				{
					rc.left = ( PLAYAREA_WIDTH_STEP * BITMAP_WIDTH ) * nLoopx;
					rc.top = ( PLAYAREA_HEIGHT_STEP * BITMAP_HEIGHT ) * nLoopy;
					rc.right = ( ( PLAYAREA_WIDTH_STEP * BITMAP_WIDTH ) * nLoopx ) + ( PLAYAREA_WIDTH * BITMAP_WIDTH );
					rc.bottom = ( ( PLAYAREA_HEIGHT_STEP * BITMAP_HEIGHT ) * nLoopy ) + ( PLAYAREA_HEIGHT * BITMAP_HEIGHT );
					rgn.CreateRectRgn( rc.left, rc.top, rc.right, rc.bottom );
					pDC->FrameRgn( &rgn, &cBrush, 4, 4 );
					rgn.DeleteObject();
				}
			}
		}
	}
	else
		TRACE( "CCLDView::OnDraw - No CDocument associated with CScrollView\n" );
}

void CCLDView::ShowAttribs( CDC *pDC )
{
	CCLDDoc *pDoc = (CCLDDoc*)GetDocument();
	CPaintDC *cPaintDC = (CPaintDC*)pDC;
	RECT rcUpdate = cPaintDC->m_ps.rcPaint;
	SIZE sizeMaze;
	int nLoopx;				//	For loops
	int nLoopy;
	int nXPos, nYPos;		//	Bitmap coordinates
	CMaze *lpCMaze;			//	Maze object
	int nXStart, nXFinish, nYStart, nYFinish;
	CFont cFontNumber;
	CFont *lpCFont;
	UINT uiPrevTA;
	COLORREF prevColor;
	int prevBk, nCellObject;
	char szOutput[30];
	BOOL bDraw;

	if( pDoc )
	{
		pDoc->GetMazeSize( &sizeMaze );
		if( ( sizeMaze.cx>0 ) && ( sizeMaze.cy>0 ) )
		{
			pDoc->GetMazeSize( &sizeMaze );
			lpCMaze = pDoc->GetMaze();
			pDC->DPtoLP( &rcUpdate );
			nXStart = (rcUpdate.left / BITMAP_WIDTH );
			nYStart = (rcUpdate.top / BITMAP_HEIGHT );
			nXFinish = (rcUpdate.right / BITMAP_WIDTH )+1;
			nYFinish = (rcUpdate.bottom / BITMAP_HEIGHT )+1;
			if( nXFinish>sizeMaze.cx )
				nXFinish=sizeMaze.cx;
			if( nYFinish>sizeMaze.cy )
				nYFinish=sizeMaze.cy;

			//cFontNumber.CreatePointFont( 100, "Arial", pDC );
			cFontNumber.CreatePointFont( 9, "System", NULL );

			uiPrevTA = pDC->SetTextAlign( TA_CENTER );
			lpCFont = pDC->SelectObject( &cFontNumber );			
			prevColor = pDC->SetTextColor( RGB( 0,0,0 ) );
			prevBk = pDC->SetBkMode( TRANSPARENT );

			for( nLoopy=nYStart; nLoopy<nYFinish; nLoopy++ )
			{
				for( nLoopx=nXStart; nLoopx<nXFinish; nLoopx++ )
				{
					nCellObject = lpCMaze->GetCell( nLoopx, nLoopy, CELL_OBJECT );
					bDraw = TRUE;
					switch( nCellObject )
					{
						case OBJECT_WALL:
							pDC->SetTextColor( RGB( 255,255,255 ) );
							break;
						case OBJECT_SHIELD:
							pDC->SetTextColor( RGB( 0,0,255 ) );
							break;
						case OBJECT_KEY:
							pDC->SetTextColor( RGB( 0,0,0 ) );
							break;
						case OBJECT_TRANSPORT:
							pDC->SetTextColor( RGB( 0,255,0 ) );
							break;
						case OBJECT_OBJECT:
							pDC->SetTextColor( RGB( 255,0,0 ) );
							break;
						case OBJECT_CPU:
							pDC->SetTextColor( RGB( 255,255,0 ) );
							break;
						default:
							bDraw = FALSE;
							break;
					}
					if( bDraw )
					{
						wsprintf( szOutput, "%d", lpCMaze->GetCell( nLoopx, nLoopy, CELL_ATTRIBUTE ) );
						nXPos = nLoopx * BITMAP_WIDTH;
						nYPos = nLoopy * BITMAP_HEIGHT;
						pDC->TextOut( nXPos + (BITMAP_WIDTH/2), nYPos, szOutput, strlen( szOutput ) );
					}
				}
			}

			pDC->SetBkMode( prevBk );
			pDC->SetTextColor( prevColor );
			pDC->SelectObject( lpCFont );
			pDC->SetTextAlign( uiPrevTA );
		}
	}
	else
		TRACE( "CCLDView::OnDraw - No CDocument associated with CScrollView\n" );
}

BOOL CCLDView::InitialiseBitmaps( void )
{
	BOOL bRet = FALSE;
	LONG lMemReq;			//	Determined memory requirements for
							//	allocating memory to store pointer array
	int nLoop = NULL;
	CBitmap **lppCBitmaps;	//	Memory pointer for bitmap pointers
	CString cstrIDName;		//	String to build Bitmap ID from resource
	char szWork[10];

	if( m_hgblCBitmaps )	//	Test for prev allocation, just to be sure
		ReleaseBitmaps();

	lMemReq = sizeof( CBitmap* ) * BITMAP_COUNT;

	m_hgblCBitmaps = GlobalAlloc( GHND, lMemReq );
	if( m_hgblCBitmaps )
	{
		lppCBitmaps = (CBitmap**)GlobalLock( m_hgblCBitmaps );
		for( nLoop=0; nLoop<BITMAP_COUNT; nLoop++ )
		{
			lppCBitmaps[ nLoop ]=new CBitmap;	//	Create bitmap object
			if( !lppCBitmaps[ nLoop ] )
			{
				nLoop=BITMAP_COUNT;
				GlobalUnlock( m_hgblCBitmaps );
				lppCBitmaps=NULL;
				ReleaseBitmaps();
			}
			else
			{
				cstrIDName = BITMAP_ID;
				wsprintf( szWork, "%03d", nLoop+1 );
				cstrIDName+= szWork;
				if( !lppCBitmaps[ nLoop ]->LoadBitmap( cstrIDName ) )
				{
					nLoop=BITMAP_COUNT;
					GlobalUnlock( m_hgblCBitmaps );
					lppCBitmaps=NULL;
					ReleaseBitmaps();
				}
			}
		}
	}
	
	return bRet;
}

void CCLDView::ReleaseBitmaps( void )
{
	int nLoop;
	CBitmap **lppCBitmaps;	//	Memory pointer for bitmap pointers
	if( m_hgblCBitmaps )
	{
		lppCBitmaps = (CBitmap**)GlobalLock( m_hgblCBitmaps );
		for( nLoop=0; nLoop<BITMAP_COUNT; nLoop++ )
		{
			if( lppCBitmaps[ nLoop ] )
			{
				if( lppCBitmaps[ nLoop ]->m_hObject )
					lppCBitmaps[ nLoop ]->DeleteObject();
				delete lppCBitmaps[ nLoop ];
				lppCBitmaps[ nLoop ]=NULL;
			}
		}
		GlobalUnlock( m_hgblCBitmaps );
		lppCBitmaps=NULL;
		GlobalFree( m_hgblCBitmaps );
		m_hgblCBitmaps = NULL;
	}
}

void CCLDView::OnUpdatePlaceObject(CCmdUI* pCmdUI) 
{
	CCLDDoc *pDoc = GetDocument();

	if( pDoc->GetMaze()->ValidMaze() )
	{
		pCmdUI->Enable( TRUE );
		if( EDITMODE_PLACE==m_nEditMode )
			pCmdUI->SetCheck( 1 );
		else
			pCmdUI->SetCheck( 0 );
	}
	else
	{
		pCmdUI->Enable( FALSE );
		pCmdUI->SetCheck( 0 );
	}
}

void CCLDView::OnUpdateDeleteObject(CCmdUI* pCmdUI) 
{
	CCLDDoc *pDoc = GetDocument();

	if( pDoc->GetMaze()->ValidMaze() )
	{
		pCmdUI->Enable( TRUE );
		if( EDITMODE_DELETE==m_nEditMode )
			pCmdUI->SetCheck( 1 );
		else
			pCmdUI->SetCheck( 0 );
	}
	else
	{
		pCmdUI->Enable( FALSE );
		pCmdUI->SetCheck( 0 );
	}
}

void CCLDView::OnDeleteObject() 
{
	m_nEditMode = EDITMODE_DELETE;
}

void CCLDView::OnPlaceObject() 
{
	m_nEditMode = EDITMODE_PLACE;
}

void CCLDView::OnSelectObject() 
{
	CSelectObjDlg slctDlg;

	slctDlg.m_nSelectedObject = m_nSelectedObject;
	slctDlg.m_nSelectedAttrib = m_nSelectedAttrib;
	if( IDOK==slctDlg.DoModal() )
	{
		m_nSelectedObject = slctDlg.m_nSelectedObject;
		m_nSelectedAttrib = slctDlg.m_nSelectedAttrib;
	}
}

void CCLDView::OnUpdateSelectObject(CCmdUI* pCmdUI) 
{
	CCLDDoc *pDoc = GetDocument();

	if( pDoc->GetMaze()->ValidMaze() )
		pCmdUI->Enable( TRUE );
	else
		pCmdUI->Enable( FALSE );
}

void CCLDView::OnScreenGrid() 
{
	if( m_bDrawScreenGrid )
	{
		m_bDrawScreenGrid = FALSE;
		Invalidate( TRUE );
	}
	else
	{
		m_bDrawScreenGrid = TRUE;
		Invalidate( FALSE );
	}
	UpdateWindow();
}

void CCLDView::OnUpdateScreenGrid(CCmdUI* pCmdUI) 
{
	if( GetDocument()->GetMaze()->ValidMaze() )
	{
		pCmdUI->Enable( TRUE );
		if( m_bDrawScreenGrid )
			pCmdUI->SetCheck( 1 );
		else
			pCmdUI->SetCheck( 0 );
	}
	else
	{
		pCmdUI->Enable( FALSE );
		pCmdUI->SetCheck( 0 );
	}
}

void CCLDView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CPoint pt = point;
	CPoint ptScroll;
	int nXPos, nYPos;
	CSize sizeMaze;
	CCLDDoc *pDoc = (CCLDDoc*)GetDocument();
	CRect rc;

	ptScroll = GetDeviceScrollPosition();
	pt.x+=ptScroll.x;
	pt.y+=ptScroll.y;
	pDoc->GetMazeSize( &sizeMaze );
	nXPos = pt.x / BITMAP_WIDTH;
	nYPos = pt.y / BITMAP_HEIGHT;

	if(	(nXPos>=0) && (nXPos<sizeMaze.cx) &&
		(nYPos>=0) && (nYPos<sizeMaze.cy) )
	{
		switch( m_nEditMode )
		{
			case EDITMODE_PLACE:
				pDoc->GetMaze()->SetCell( nXPos, nYPos, CELL_OBJECT, m_nSelectedObject);
				pDoc->GetMaze()->SetCell( nXPos, nYPos, CELL_ATTRIBUTE, m_nSelectedAttrib);
				rc.left = point.x-BITMAP_WIDTH;
				rc.top = point.y-BITMAP_HEIGHT;
				rc.right = point.x+BITMAP_WIDTH;
				rc.bottom = point.y+BITMAP_HEIGHT;
				InvalidateRect( &rc, FALSE );
				UpdateWindow();
				break;
			case EDITMODE_DELETE:
				pDoc->GetMaze()->SetCell( nXPos, nYPos, CELL_OBJECT, OBJECT_FLOOR );
				pDoc->GetMaze()->SetCell( nXPos, nYPos, CELL_ATTRIBUTE, 0 );
				rc.left = point.x-BITMAP_WIDTH;
				rc.top = point.y-BITMAP_HEIGHT;
				rc.right = point.x+BITMAP_WIDTH;
				rc.bottom = point.y+BITMAP_HEIGHT;
				InvalidateRect( &rc, TRUE );
				UpdateWindow();
				break;
			case EDITMODE_LINE:
				SetCapture();
				break;
			case EDITMODE_BOX:
				SetCapture();
				break;
		}
		pDoc->SetModifiedFlag();
	}

	CScrollView::OnLButtonDown(nFlags, point);
}

void CCLDView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CPoint pt = point;
	CPoint ptScroll;
	int nXPos, nYPos;
	CSize sizeMaze;
	CCLDDoc *pDoc = (CCLDDoc*)GetDocument();

	ptScroll = GetDeviceScrollPosition();
	pt.x+=ptScroll.x;
	pt.y+=ptScroll.y;
	pDoc->GetMazeSize( &sizeMaze );
	nXPos = pt.x / BITMAP_WIDTH;
	nYPos = pt.y / BITMAP_HEIGHT;

	if(	(nXPos>=0) && (nXPos<sizeMaze.cx) &&
		(nYPos>=0) && (nYPos<sizeMaze.cy) )
	{
		switch( m_nEditMode )
		{
			case EDITMODE_PLACE:
				break;
			case EDITMODE_DELETE:
				break;
			case EDITMODE_LINE:
				ReleaseCapture();
				break;
			case EDITMODE_BOX:
				ReleaseCapture();
				break;
		}
	}
	
	CScrollView::OnLButtonUp(nFlags, point);
}

void CCLDView::OnMouseMove(UINT nFlags, CPoint point) 
{
	CPoint pt = point;
	CPoint ptScroll;
	int nXPos, nYPos;
	CSize sizeMaze;
	CCLDDoc *pDoc = (CCLDDoc*)GetDocument();

	ptScroll = GetDeviceScrollPosition();
	pt.x+=ptScroll.x;
	pt.y+=ptScroll.y;
	pDoc->GetMazeSize( &sizeMaze );
	nXPos = pt.x / BITMAP_WIDTH;
	nYPos = pt.y / BITMAP_HEIGHT;

	if(	(nXPos>=0) && (nXPos<sizeMaze.cx) &&
		(nYPos>=0) && (nYPos<sizeMaze.cy) )
	{
		switch( m_nEditMode )
		{
			case EDITMODE_PLACE:
				break;
			case EDITMODE_DELETE:
				break;
			case EDITMODE_LINE:
				break;
			case EDITMODE_BOX:
				break;
		}
	}
	
	CScrollView::OnMouseMove(nFlags, point);
}

void CCLDView::OnRButtonDown(UINT nFlags, CPoint point) 
{
	CPoint pt = point;
	CPoint ptScroll;
	int nXPos, nYPos;
	CSize sizeMaze;
	CCLDDoc *pDoc = (CCLDDoc*)GetDocument();
	CRect rc;

	ptScroll = GetDeviceScrollPosition();
	pt.x+=ptScroll.x;
	pt.y+=ptScroll.y;
	pDoc->GetMazeSize( &sizeMaze );
	nXPos = pt.x / BITMAP_WIDTH;
	nYPos = pt.y / BITMAP_HEIGHT;

	if(	(nXPos>=0) && (nXPos<sizeMaze.cx) &&
		(nYPos>=0) && (nYPos<sizeMaze.cy) )
	{
		pDoc->GetMaze()->SetCell( nXPos, nYPos, CELL_OBJECT, OBJECT_FLOOR );
		pDoc->GetMaze()->SetCell( nXPos, nYPos, CELL_ATTRIBUTE, 0 );
		rc.left = point.x-BITMAP_WIDTH;
		rc.top = point.y-BITMAP_HEIGHT;
		rc.right = point.x+BITMAP_WIDTH;
		rc.bottom = point.y+BITMAP_HEIGHT;
		InvalidateRect( &rc, TRUE );
		UpdateWindow();
		pDoc->SetModifiedFlag();
	}

	CScrollView::OnRButtonDown(nFlags, point);
}

void CCLDView::OnUpdateObjectAttributes(CCmdUI* pCmdUI) 
{
	if( GetDocument()->GetMaze()->ValidMaze() )
	{
		pCmdUI->Enable( TRUE );
		if( m_bShowAttribs )
			pCmdUI->SetCheck( 1 );
		else
			pCmdUI->SetCheck( 0 );
	}
	else
	{
		pCmdUI->Enable( FALSE );
		pCmdUI->SetCheck( 0 );
	}
}

void CCLDView::OnObjectAttributes() 
{
	if( m_bShowAttribs )
	{
		m_bShowAttribs = FALSE;
		Invalidate( TRUE );
	}
	else
	{
		m_bShowAttribs = TRUE;
		Invalidate( FALSE );
	}
	UpdateWindow();
}
