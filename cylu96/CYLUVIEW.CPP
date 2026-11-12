// Cyluview.cpp : implementation of the CCyluView class
//

#include "stdafx.h"
#include "Cylu.h"

#include "Cyludoc.h"
#include "Cyluview.h"
#ifdef _USE_WING_
#include <wing.h>
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CCyluView

IMPLEMENT_DYNCREATE(CCyluView, CView)

BEGIN_MESSAGE_MAP(CCyluView, CView)
	//{{AFX_MSG_MAP(CCyluView)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_KEYDOWN()
	ON_COMMAND(IDM_NEW_GAME, &CCyluView::OnNewGame)
	ON_COMMAND(IDM_PAUSE_GAME, &CCyluView::OnPauseGame)
	ON_COMMAND(IDM_END_GAME, &CCyluView::OnEndGame)
	ON_UPDATE_COMMAND_UI(IDM_END_GAME, &CCyluView::OnUpdateEndGame)
	ON_UPDATE_COMMAND_UI(IDM_PAUSE_GAME, &CCyluView::OnUpdatePauseGame)
	ON_WM_KEYUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCyluView construction/destruction

CCyluView::CCyluView()
{
	TRACE( _T("Status:   Begin init loop\n") );
	m_nGameState = GAMESTATE_MAINTITLE;

	m_bInitFailed = FALSE;

	if( !InitialiseVariables() )
	{
		m_bInitFailed = TRUE;
		TRACE( _T("Status:   Begin init loop failed\n") );
	}
	else
		TRACE( _T("Status:   Begin init loop successful\n") );
}

BOOL CCyluView::InitialiseVariables()
{
	BOOL bRet = FALSE;
	int nLoop;
	CCyluApp *lpApp = (CCyluApp*)AfxGetApp();	

#ifdef _USE_WING_
	HGLOBAL hgblRecommendDIB;
	BITMAPINFO *lpbi;
	DWORD dwMemSize;
#endif

	TRACE( _T("Status:   Initialising variables\n") );

	m_bOkToUse = FALSE;
	m_nVertStep = CELL_HEIGHT/5;
	m_nHorzStep = CELL_WIDTH/2;
	m_nGridWidth =	CELL_WIDTH + ( m_nHorzStep * (PLAYAREA_HEIGHT-1) ) +
					(m_nHorzStep * (PLAYAREA_WIDTH-1) );
	m_nGridHeight = CELL_HEIGHT + ( m_nVertStep * (PLAYAREA_WIDTH-1) ) +
					(m_nVertStep * (PLAYAREA_HEIGHT-1) );
	m_nGridCentre = (CELL_WIDTH * PLAYAREA_HEIGHT) / 2;

	m_nDroidHorzStep = m_nHorzStep/4;
	m_nDroidVertStep = m_nVertStep/4;

	m_pOldCBitmapBuffer = NULL;

	m_nSelPocket = 0;

	m_nGridTop = NULL;
	m_uiFrameTimer = NULL;
	m_hWinGDC = NULL;
	m_hWinGBitmap = NULL;
	m_nIndex = 0;

	m_nFuelStatus = INITIAL_FUEL_STATUS;
	m_nFuelDelay = 0;
	m_nObjectsCollected = 0;

	m_bStillMoving = FALSE;

	//	Reload level, resets all objects
	if( !lpApp->CreateMaze() )
	{
		TRACE( _T("Status:   Initialising Failed\n") );
		return bRet;
	}

	lpApp->SetDirection( NORTH );	//	Face north by default

	lpApp->SetScreenPos( CalculateScreenPos( ((CCyluApp*)AfxGetApp())->GetDroidPos(), AXIS_X|AXIS_Y ) );

	for( nLoop=0; nLoop<MAX_POCKETS; nLoop++ )
		lpApp->SetPocket( nLoop, 0, 0 );

	TRACE( _T("Status:   Initialising successful\n") );

	TRACE( _T("Status:   Loading sprites\n") );
	if( !CreateSprites() )
	{
		DestroySprites();
		TRACE( _T("Status:   Loading sprites failed\n") );
	}
	else
	{
#ifdef _USE_WING_
		m_hWinGDC = WinGCreateDC();
		if( !m_hWinGDC )
			AfxMessageBox( _T("CCyluView::CCyluView: Failed to create WinG DC\n") );
		else
		{
			dwMemSize = sizeof( BITMAPINFO )+(sizeof( DWORD )*3);
			hgblRecommendDIB = GlobalAlloc( GHND, dwMemSize );
			if( hgblRecommendDIB )
			{
				lpbi = (BITMAPINFO*)GlobalLock( hgblRecommendDIB );
				if( !WinGRecommendDIBFormat( lpbi ) )
					AfxMessageBox( _T("CCyluView::CCyluView: WinG could not recomment a DIB format\n") );
				else
				{
					GlobalUnlock( hgblRecommendDIB );
					GlobalFree( hgblRecommendDIB );
					hgblRecommendDIB = NULL;
					bRet = TRUE;
				}
			}
			else
				AfxMessageBox( _T("CCyluView::CCyluView: No free store for WinG bitmap\n") );
		}
#else
		bRet = TRUE;
#endif	//	_USE_WING
	}

	return bRet;
}

BOOL CCyluView::ResetVariables()
{
	BOOL bRet = TRUE;

	TRACE( _T("Status:   Resetting variables\n") );

	if( m_pOldCBitmapBuffer )
	{
		m_cdcBuffer.SelectObject( m_pOldCBitmapBuffer );
		m_pOldCBitmapBuffer = NULL;
	}

	if( m_cBitmapBuffer.m_hObject )
		m_cBitmapBuffer.DeleteObject();

	if( m_cdcBuffer.GetSafeHdc() )
		m_cdcBuffer.DeleteDC();

	DestroySprites();

#ifdef _USE_WING_
	if( m_hWinGDC )
	{
		DeleteDC( m_hWinGDC );
		m_hWinGDC = NULL;
	}
#endif

	TRACE( _T("Status:   Resetting variables successful\n") );

	return bRet;
}

CCyluView::~CCyluView()
{
	TRACE( _T("Status:   Shutdown init loop\n") );
	ResetVariables();
	TRACE( _T("Status:   Shutdown init loop successful\n "));
}

/////////////////////////////////////////////////////////////////////////////
// CCyluView drawing

void CCyluView::OnDraw(CDC* pDC)
{
	CCyluDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
}

/////////////////////////////////////////////////////////////////////////////
// CCyluView diagnostics

#ifdef _DEBUG
void CCyluView::AssertValid() const
{
	CView::AssertValid();
}

void CCyluView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CCyluDoc* CCyluView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCyluDoc)));
	return (CCyluDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCyluView message handlers


BOOL CCyluView::OnEraseBkgnd(CDC* pDC) 
{
	CBrush cBrushBackGround;
	CRect rcClientArea;

	GetClientRect( &rcClientArea );
	cBrushBackGround.CreateSolidBrush( NULL );	//	Black brush
	pDC->FillRect( &rcClientArea, &cBrushBackGround );

	return TRUE;
}

void CCyluView::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	TRACE( _T("Method:   Painting\n") );
	switch( m_nGameState )
	{
		case GAMESTATE_MAINTITLE:
			DoMainTitle( &dc );
			break;
		case GAMESTATE_PAUSED:
		case GAMESTATE_GAMEPLAYING:	
			DrawPockets( &dc );
			break;
		case GAMESTATE_GAMEOVER:
			break;
	}
	TRACE( _T("Method:   Painting successful\n") );
}

BOOL CCyluView::CreateSprites( void )
{
	BOOL bRet = FALSE;
	CString cstrDebug;

	CCyluApp* lpApp = ((CCyluApp*)AfxGetApp());

	if( !lpApp->GetPictureBox()->CreateFilms( lpApp->m_cstrSSFName ) )
	{
		TRACE( _T("Status:   Sprite creation failed - Load sprites failed\n") );
		return bRet;
	}

	if( !lpApp->GetPictureBox()->LockFilms() )
	{
		TRACE( _T("Status:   Sprite creation failed - LockFilms failed\n") );
		return bRet;
	}

	bRet = TRUE;

	return bRet;
}

void CCyluView::DestroySprites( void )
{
	CCyluApp* lpApp = ((CCyluApp*)AfxGetApp());

	lpApp->GetPictureBox()->UnlockFilms();
	if( !lpApp->GetPictureBox()->DestroyFilms() )
	{
		TRACE( _T("Status:   Sprite deletion failed - DestroyFilms failed\n") );
	}
}

void CCyluView::SetupPlayArea( void )
{
	int nLoopx;
	int nLoopy;
	CCyluApp *lpApp = (CCyluApp*)AfxGetApp();
	CPoint pt = lpApp->GetScreenPos();
	int nWidth, nHeight;
	int nObject, nAttrib;
	lpApp->GetMaze()->GetMazeSize( &nWidth, &nHeight );

	for( nLoopy=0;nLoopy<PLAYAREA_HEIGHT;nLoopy++ )
	{
		for( nLoopx=0;nLoopx<PLAYAREA_WIDTH;nLoopx++ )
		{
			nObject = lpApp->GetMaze()->GetCell( pt.x + nLoopx, pt.y + nLoopy, CELL_OBJECT );
			nAttrib = lpApp->GetMaze()->GetCell( pt.x + nLoopx, pt.y + nLoopy, CELL_ATTRIBUTE );
			m_nPlayArea[nLoopx][nLoopy] = TranslateCell( nObject, nAttrib );
		}
	}
}

void CCyluView::DrawGameStats( void )
{
	CDC *dc = GetDC();
	CCyluApp *lpApp = (CCyluApp*)AfxGetApp();
	CFont cFont;
	CFont *lpOldCFont;
	COLORREF ForeColorOld;
	COLORREF BackColorOld;
	TCHAR szFuel[255];
	TCHAR szObjects[255];

	if( !cFont.CreatePointFont( 90, _T("System"), dc ) )
	{
		AfxMessageBox(	_T("Could not create stats font\n"),
						MB_ICONEXCLAMATION|MB_OK );
		return;
	}
	wsprintf( szFuel, _T("Fuel: %d    "), m_nFuelStatus );
	wsprintf( szObjects, _T("Objects: %d    "), m_nObjectsCollected );

	lpOldCFont = dc->SelectObject( &cFont );
	ForeColorOld = dc->SetTextColor( RGB( 255, 255, 0 ) );
	BackColorOld = dc->SetBkColor( RGB( 0, 0, 0 ) );

	dc->TextOut( 10, 5, szFuel, (int)_tcslen( szFuel ) );
	dc->TextOut( 10, 20, szObjects, (int)_tcslen( szObjects ) );

	dc->SetBkColor( BackColorOld );
	dc->SetTextColor( ForeColorOld );
	dc->SelectObject( lpOldCFont );

	ReleaseDC( dc );
}

BOOL CCyluView::DoPlayAreaDisplay( void )
{
	BOOL bRet = FALSE;
	CCyluApp* lpCCyluApp = ((CCyluApp*)AfxGetApp());
	int nLoopx = 0;
	int nLoopy = 0;
	int nCellLeft;
	int nCellTop;
	int nClientWidth;
	int nClientHeight;
	int nCurrentCell;
	int nXScrnDrdPos;
	int nYScrnDrdPos;
	BYTE btDirection;
	int nShowFrame = 4-lpCCyluApp->GetMoveFrame();
	BOOL bLoopMethod = FALSE;
	BOOL bLoopDone = FALSE;	

	CPoint ptDroid = lpCCyluApp->GetDroidPos();
	CPoint ptScreen = lpCCyluApp->GetScreenPos();
	BOOL bDroidDone = FALSE;

	CDC *pCDC;
	CDC cdcCompatible2;

	CBitmap cBitmapBuffer;
	CBitmap cBitmapOutput;

	CBitmap *pOldSpriteCBitmap = NULL;
	
	CRect cRectClientArea;
	GetClientRect( &cRectClientArea );

	nClientWidth = cRectClientArea.Width();
	nClientHeight = cRectClientArea.Height();

	//	Get the DC for the client area
	pCDC = GetDC();

	//	Create a compatible DC from the client window DC
	if( !m_cdcBuffer.GetSafeHdc() )
	{
		if( !m_cdcBuffer.CreateCompatibleDC( pCDC ) )
		{
			TRACE( _T("Failed to generate compatible DC 1 from client's CDC\n") );
			return FALSE;
		}
	}

	if( !m_cBitmapBuffer.m_hObject )
		m_cBitmapBuffer.CreateCompatibleBitmap( pCDC, m_nGridWidth, m_nGridHeight );

	if( !m_cBitmapBuffer.m_hObject )
		TRACE( _T("Failed to create compatible bitmap\n") );
	else
	{
		//	DC for putting each sprite and mask into.
		cdcCompatible2.CreateCompatibleDC( pCDC );
		if( !m_pOldCBitmapBuffer )
			m_pOldCBitmapBuffer = m_cdcBuffer.SelectObject( &m_cBitmapBuffer );

		m_cdcBuffer.PatBlt( 0, 0, m_nGridWidth, m_nGridHeight, BLACKNESS );

		nXScrnDrdPos = ptDroid.x-ptScreen.x;
		nYScrnDrdPos = ptDroid.y-ptScreen.y;

		if( lpCCyluApp->GetMoveState()==MOVESTATE_MOVING )
		{
			btDirection = lpCCyluApp->GetDirection();
			if( (btDirection==NORTH) || (btDirection==SOUTH ) )
				bLoopMethod = TRUE;
			else
				bLoopMethod = FALSE;
		}

		while( !bLoopDone )
		{
			nCurrentCell = m_nPlayArea[nLoopx][nLoopy];
			if( nCurrentCell )
			{
				nCellLeft = m_nGridCentre + ((nLoopx * m_nHorzStep) - nLoopy * m_nHorzStep) - (CELL_WIDTH / 2);
				nCellTop = m_nGridTop + ((nLoopx * m_nVertStep) + nLoopy * m_nVertStep);

				if( !pOldSpriteCBitmap )
					pOldSpriteCBitmap = cdcCompatible2.SelectObject( lpCCyluApp->GetPictureBox()->GetFrame( MASK_IMAGE, nCurrentCell-1) );
				else
					cdcCompatible2.SelectObject( lpCCyluApp->GetPictureBox()->GetFrame( MASK_IMAGE, nCurrentCell-1) );

				m_cdcBuffer.BitBlt( nCellLeft, nCellTop, CELL_WIDTH, CELL_HEIGHT, &cdcCompatible2, 0, 0, SRCAND );
				cdcCompatible2.SelectObject( lpCCyluApp->GetPictureBox()->GetFrame( SPRITE_IMAGE, nCurrentCell-1) );
				m_cdcBuffer.BitBlt( nCellLeft, nCellTop, CELL_WIDTH, CELL_HEIGHT, &cdcCompatible2, 0, 0, SRCPAINT );
			}
			else
			{
				if( !bDroidDone )
				{
					if( ( ptScreen.x+nLoopx==ptDroid.x ) && ( ptScreen.y+nLoopy==ptDroid.y ) )
					{
						bDroidDone = TRUE;
						nCurrentCell = GetDroidFrame();
						nCellLeft = m_nGridCentre + ((nXScrnDrdPos * m_nHorzStep) - nYScrnDrdPos * m_nHorzStep) - (CELL_WIDTH / 2);
						nCellTop = m_nGridTop + ((nXScrnDrdPos * m_nVertStep) + nYScrnDrdPos * m_nVertStep);
						if( lpCCyluApp->GetMoveFrame()>0 )
						{
							switch( lpCCyluApp->GetDirection() )
							{
								case NORTH:
									nCellLeft-=m_nDroidHorzStep * nShowFrame;
									nCellTop+=m_nDroidVertStep * nShowFrame;
									break;
								case EAST:
									nCellLeft-=m_nDroidHorzStep * nShowFrame;
									nCellTop-=m_nDroidVertStep * nShowFrame;
									break;
								case SOUTH:
									nCellLeft+=m_nDroidHorzStep * nShowFrame;
									nCellTop-=m_nDroidVertStep * nShowFrame;
									break;
								case WEST:
									nCellLeft+=m_nDroidHorzStep * (4-lpCCyluApp->GetMoveFrame() );
									nCellTop+=m_nDroidVertStep * (4-lpCCyluApp->GetMoveFrame() );
									break;
							}
						}
						if( !pOldSpriteCBitmap )
							pOldSpriteCBitmap = cdcCompatible2.SelectObject( lpCCyluApp->GetPictureBox()->GetFrame( MASK_IMAGE, nCurrentCell) );
						else
							cdcCompatible2.SelectObject( lpCCyluApp->GetPictureBox()->GetFrame( MASK_IMAGE, nCurrentCell) );

						m_cdcBuffer.BitBlt( nCellLeft, nCellTop, CELL_WIDTH, CELL_HEIGHT, &cdcCompatible2, 0, 0, SRCAND );
						cdcCompatible2.SelectObject( lpCCyluApp->GetPictureBox()->GetFrame( SPRITE_IMAGE, nCurrentCell) );
						m_cdcBuffer.BitBlt( nCellLeft, nCellTop, CELL_WIDTH, CELL_HEIGHT, &cdcCompatible2, 0, 0, SRCPAINT );
					}
				}
			}
			if( !bLoopMethod )
			{
				nLoopx++;
				if( nLoopx==PLAYAREA_WIDTH )
				{
					nLoopx=0;
					nLoopy++;
					if( nLoopy==PLAYAREA_HEIGHT )
						bLoopDone = TRUE;
				}
			}
			else
			{
				nLoopy++;
				if( nLoopy==PLAYAREA_HEIGHT )
				{
					nLoopy=0;
					nLoopx++;
					if( nLoopx==PLAYAREA_WIDTH )
						bLoopDone = TRUE;
				}
			}
		}

		if( pOldSpriteCBitmap )
		{
			cdcCompatible2.SelectObject( pOldSpriteCBitmap );
			pCDC->BitBlt( (nClientWidth - m_nGridWidth) / 2, ((nClientHeight - m_nGridHeight) / 2)-70, m_nGridWidth, m_nGridHeight, &m_cdcBuffer, 0, 0, SRCCOPY );
			GdiFlush();
		}

		cdcCompatible2.DeleteDC();

		bRet = TRUE;
	}

	ReleaseDC( pCDC );

	return bRet;
}

LRESULT CCyluView::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	CCyluApp *lpApp = ((CCyluApp*)AfxGetApp());

	switch( message )
	{
		case WM_TIMER:
			DoTimerEvent();
			break;
	}
	
	return CView::WindowProc(message, wParam, lParam);
}

void CCyluView::CreateTimer( void )
{
	int nLoop = 1;
	while( !m_uiFrameTimer )
	{
		m_uiFrameTimer = SetTimer( nLoop, FRAME_RATE, NULL );
		nLoop++;
	}
}

void CCyluView::DestroyTimer( void )
{
	if( m_uiFrameTimer )
	{
		KillTimer( m_uiFrameTimer );
		m_uiFrameTimer = NULL;
	}
}

void CCyluView::DoTimerEvent( void )
{
	CCyluApp *lpApp = (CCyluApp*)AfxGetApp();
	int nMoveFrame;

	DrawGameStats();

	SetupPlayArea();

	if( GetGameState()!=GAMESTATE_PAUSED )
	{
		if( lpApp->GetMoveState()==MOVESTATE_MOVING )
		{
			nMoveFrame = lpApp->GetMoveFrame();
			if( m_bStillMoving )
			{
				if( nMoveFrame==3 )
				{
					nMoveFrame=0;
					lpApp->SetMoveState( MOVESTATE_STATIC );
				}
				else
					nMoveFrame++;
			}
			else
			{
				if( nMoveFrame==4 )
				{
					nMoveFrame=0;
					lpApp->SetMoveState( MOVESTATE_STATIC );
				}
				else
					nMoveFrame++;
			}
			lpApp->SetMoveFrame( nMoveFrame );
		}		
	}

	DoPlayAreaDisplay();

	if( GetGameState()!=GAMESTATE_PAUSED )
	{
		lpApp->GetPictureBox()->RollFilms();
		m_nFuelDelay++;
		if( m_nFuelDelay>FUEL_DELAY )
		{
			m_nFuelStatus--;
			m_nFuelDelay = 0;
		}
		if( m_nFuelStatus == 0 )
			DoGameOver();
	}
}

//	@@@ Fix palette problem
/*void CCyluView::CreatePalette( void )
{
	LOGPALETTE logpal;
	long lMemReq;
	LPBITMAPINFO lpbm;
	CCyluApp* lpCCyluApp = ((CCyluApp*)AfxGetApp());
	CBitmap *lpCBitmap = lpCCyluApp->GetPictureBox()->GetFrame( SPRITE_IMAGE, 0);
}

void CCyluView::DestroyPalette( void )
{

}*/

CPoint CCyluView::CalculateScreenPos( CPoint ptPos, int nAxis )
{
	CCyluApp *lpApp = (CCyluApp*)AfxGetApp();
	CPoint ptRet;
	CPoint ptOld = lpApp->GetScreenPosOld();
	CPoint ptNew = ptOld;

	if( nAxis & AXIS_X )
		ptNew.x = ((ptPos.x / PLAYAREA_WIDTH_STEP ) * PLAYAREA_WIDTH_STEP );
	if( nAxis & AXIS_Y )
		ptNew.y = ((ptPos.y / PLAYAREA_HEIGHT_STEP ) * PLAYAREA_HEIGHT_STEP );

	if( (ptOld!=ptNew) )// || (ptOld.y!=ptNew.y) )
	{
		ptRet = ptNew;
		lpApp->SetScreenPosOld( ptNew );
	}
	else
		ptRet =  ptOld;

	return ptRet;
}

int CCyluView::TranslateCell( int nObject, int nAttrib )
{
	int nRet = NULL;

	switch( nObject )
	{
		case OBJECT_FLOOR:
			nRet = NULL;
			break;
		case OBJECT_BASE:
			nRet = 1+0;
			break;
		case OBJECT_TREE:
			nRet = 1+1;
			break;
		case OBJECT_SHIELD:
			nRet = 1+2;
			break;
		case OBJECT_CPU:
			nRet = 1+3;
			break;
		case OBJECT_TRANSPORT:
			nRet = 1+4;
			break;
		case OBJECT_KEY:
			nRet = 1+5;
			break;
		case OBJECT_FUEL:
			nRet = 1+6;
			break;
		case OBJECT_RFL:
			nRet = 1+7;
			break;
		case OBJECT_RTB:
			nRet = 1+8;
			break;
		case OBJECT_OBJECT:
			nRet = (1+9)+nAttrib;
			break;
		case OBJECT_WALL:
			nRet = (1+23)+nAttrib;
			break;
	}

	return nRet;
}

void CCyluView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CPoint ptDroid = ((CCyluApp*)AfxGetApp())->GetDroidPos();
	CCyluApp *lpApp = (CCyluApp*)AfxGetApp();

	if( GetGameState()==GAMESTATE_GAMEPLAYING )
	{
		if( lpApp->GetMoveState()==MOVESTATE_STATIC )
		{
			m_bStillMoving = TRUE;
			switch( nChar )
			{
				case VK_UP:
					MoveDroid( NORTH );
					break;
				case VK_RIGHT:
					MoveDroid( EAST );
					break;
				case VK_DOWN:
					MoveDroid( SOUTH );
					break;
				case VK_LEFT:
					MoveDroid( WEST );
					break;
				case VK_SHIFT:
					CyclePocket();
					break;
				case 80:	//	P
					PickUpObject();
					break;
				case 68:	//	D
					DropObject();
					break;
				case 85:	//	U
					UseObject();
					break;
			}
		}
	}
	
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CCyluView::MoveDroid( BYTE btDirection )
{
	CCyluApp *lpApp = (CCyluApp*)AfxGetApp();
	CPoint ptFront;
	CPoint ptDroid = lpApp->GetDroidPos();
	CPoint ptScreen = lpApp->GetScreenPos();
	CMaze *lpCMaze = lpApp->GetMaze();
	int nFlags;

	lpApp->SetDirection( btDirection );

	switch( btDirection )
	{
		case NORTH:
			ptFront.x = ptDroid.x;
			ptFront.y = ptDroid.y - 1;
			nFlags = AXIS_Y;
			break;
		case EAST:
			ptFront.x = ptDroid.x + 1;
			ptFront.y = ptDroid.y;
			nFlags = AXIS_X;
			break;
		case SOUTH:
			ptFront.x = ptDroid.x;
			ptFront.y = ptDroid.y + 1;
			nFlags = AXIS_Y;
			break;
		case WEST:
			ptFront.x = ptDroid.x - 1;
			ptFront.y = ptDroid.y;
			nFlags = AXIS_X;
			break;
	}

	if( lpCMaze->GetCell( ptFront.x, ptFront.y, CELL_OBJECT )==OBJECT_FLOOR )
	{
		lpApp->SetMoveState( MOVESTATE_MOVING );
		lpApp->SetMoveFrame( 0 );
		switch( btDirection )
		{
			case NORTH:
				ptDroid.y--;
				break;
			case EAST:
				ptDroid.x++;
				break;
			case SOUTH:
				ptDroid.y++;
				break;
			case WEST:
				ptDroid.x--;
				break;
		}
		lpApp->SetDroidPos( ptDroid );
		if(	(ptDroid.x<ptScreen.x) ||
			(ptDroid.x>ptScreen.x+PLAYAREA_WIDTH_STEP) ||
			(ptDroid.y<ptScreen.y) ||
			(ptDroid.y>ptScreen.y+PLAYAREA_HEIGHT_STEP) )
			lpApp->SetScreenPos( CalculateScreenPos( ptDroid, nFlags ) );
	}
	else
	{
		switch( btDirection )
		{
			case NORTH:
				ptDroid.y--;
				break;
			case EAST:
				ptDroid.x++;
				break;
			case SOUTH:
				ptDroid.y++;
				break;
			case WEST:
				ptDroid.x--;
				break;
		}
		if(	(ptDroid.x<ptScreen.x) ||
			(ptDroid.x>ptScreen.x+PLAYAREA_WIDTH_STEP) ||
			(ptDroid.y<ptScreen.y) ||
			(ptDroid.y>ptScreen.y+PLAYAREA_HEIGHT_STEP) )
			lpApp->SetScreenPos( CalculateScreenPos( ptDroid, nFlags ) );
	}
}

int CCyluView::GetDroidFrame()
{
	int nStartFrame = NULL;
	int nMoveFrame;
	CCyluApp *lpApp = (CCyluApp*)AfxGetApp();

	BYTE btDirection = lpApp->GetDirection();
	nMoveFrame = lpApp->GetMoveFrame();

	switch( btDirection )
	{
		case NORTH:
			nStartFrame=ROBOT_FRAME;
			break;
		case EAST:
			nStartFrame=ROBOT_FRAME+2;
			break;
		case SOUTH:
			nStartFrame=ROBOT_FRAME+4;
			break;
		case WEST:
			nStartFrame=ROBOT_FRAME+6;
			break;
	}
	if( (nMoveFrame==1) || (nMoveFrame==3) )
		nStartFrame++;

	return nStartFrame;
}

void CCyluView::DrawPockets( CDC *pDC )
{
	CDC cMemDC;
	CCyluApp *lpApp = (CCyluApp*)AfxGetApp();
	CBitmap *pOldMemBitmap = NULL;
	CBrush cBrushBlack( RGB( 0, 0, 0 ) );
	CBitmap cBitmap;
	RECT rc;
	CRect rcTopLip;
	int nLoop;
	int nTop;
	int nLeft;
	int nObject, nAttrib;
	int nCell;

	GetClientRect( &rc );

	cBitmap.LoadBitmap( IDB_OBJECT_BORDER );
	cMemDC.CreateCompatibleDC( pDC );	
	nTop = rc.bottom - OBJECT_BORDER_OFFSET - 120;

	for( nLoop=0; nLoop<MAX_POCKETS; nLoop++ )
	{
		lpApp->GetPocket( nLoop, &nObject, &nAttrib );
		nCell = TranslateCell( nObject, nAttrib );

		if( !pOldMemBitmap )
			pOldMemBitmap = cMemDC.SelectObject( &cBitmap );
		else
			cMemDC.SelectObject( &cBitmap );

		nLeft = (rc.left + OBJECT_BORDER_OFFSET) + (nLoop * 100);
		pDC->BitBlt( nLeft, nTop, 100, 120, &cMemDC, 0, 0, SRCCOPY );

		if( nCell!=0 )
		{		
			cMemDC.SelectObject( lpApp->GetPictureBox()->GetFrame( SPRITE_IMAGE, nCell-1) );
			pDC->BitBlt( nLeft+10, nTop+10, CELL_WIDTH, CELL_HEIGHT, &cMemDC, 0, 0, SRCCOPY );
		}
	}

	cMemDC.SelectObject( pOldMemBitmap );

	cBitmap.DeleteObject();
	cBitmap.LoadBitmap( IDB_POCKET_SELECT );

	rcTopLip.SetRect(	(rc.left + OBJECT_BORDER_OFFSET),
						(nTop - 16),
						(rc.left + OBJECT_BORDER_OFFSET + (100 * MAX_POCKETS)),
						(nTop - 2 ) );

	nLeft = (rc.left + OBJECT_BORDER_OFFSET + 10 ) + (m_nSelPocket * 100);

	pDC->FillRect( &rcTopLip, &cBrushBlack );

	cMemDC.SelectObject( &cBitmap );
	pDC->BitBlt( nLeft, nTop-16, 80, 14, &cMemDC, 0, 0, SRCCOPY );

	cMemDC.SelectObject( pOldMemBitmap );
	cBitmap.DeleteObject();
}

void CCyluView::CyclePocket( void )
{
	m_nSelPocket++;

	if( m_nSelPocket==MAX_POCKETS )
		m_nSelPocket = 0;

	Invalidate( FALSE );
	UpdateWindow();
}

/****************************************************************************
 *									    *
 *  FUNCTION   : CreateBIPalette(LPBITMAPINFOHEADER lpbi)		    *
 *									    *
 *  PURPOSE    : Given a Pointer to a BITMAPINFO struct will create a	    *
 *		 a GDI palette object from the color table.		    *
 *									    *
 *  RETURNS    : A handle to the palette.				    *
 *									    *
 ****************************************************************************/
/*BOOL CCyluView::CreateBIPalette(LPBITMAPINFOHEADER lpbi)
BOOL CCyluView::CreateBIPalette( CBitmap *lpCBitmap, CPalette *cNewPal )
{
    LOGPALETTE          *pPal;
    HPALETTE            hpal = NULL;
    WORD                nNumColors;
    BYTE                red;
    BYTE                green;
    BYTE                blue;
    WORD                i;
    RGBQUAD        FAR *pRgb;
	LPBITMAPINFOHEADER	lpbi;

	if( (!lpCBitmap) || ( !cNewPal ) )
		return;

	lpbi = lpCBitmap->

    if (!lpbi)
	return NULL;

    if (lpbi->biSize != sizeof(BITMAPINFOHEADER))
	return NULL;

    // Get a pointer to the color table and the number of colors in it
    pRgb = (RGBQUAD FAR *)((LPSTR)lpbi + (WORD)lpbi->biSize);
    nNumColors = DibNumColors(lpbi);

    if (nNumColors){
	// Allocate for the logical palette structure
        pPal = (LOGPALETTE*)LocalAlloc(LPTR,sizeof(LOGPALETTE) + nNumColors * sizeof(PALETTEENTRY));
	if (!pPal)
	    return NULL;

        pPal->palNumEntries = nNumColors;
	pPal->palVersion    = PALVERSION;

	// Fill in the palette entries from the DIB color table and
	// create a logical color palette.
	//
	for (i = 0; i < nNumColors; i++){
            pPal->palPalEntry[i].peRed   = pRgb[i].rgbRed;
            pPal->palPalEntry[i].peGreen = pRgb[i].rgbGreen;
            pPal->palPalEntry[i].peBlue  = pRgb[i].rgbBlue;
            pPal->palPalEntry[i].peFlags = (BYTE)0;
        }
        hpal = CreatePalette(pPal);
        LocalFree((HANDLE)pPal);
    }
    else if (lpbi->biBitCount == 24){
	// A 24 bitcount DIB has no color table entries so, set the number of
	// to the maximum value (256).
	//
	nNumColors = MAXPALETTE;
        pPal = (LOGPALETTE*)LocalAlloc(LPTR,sizeof(LOGPALETTE) + nNumColors * sizeof(PALETTEENTRY));
        if (!pPal)
	    return NULL;

	pPal->palNumEntries = nNumColors;
	pPal->palVersion    = PALVERSION;

	red = green = blue = 0;

	// Generate 256 (= 8*8*4) RGB combinations to fill the palette
	// entries.
	//
	for (i = 0; i < pPal->palNumEntries; i++){
            pPal->palPalEntry[i].peRed   = red;
            pPal->palPalEntry[i].peGreen = green;
            pPal->palPalEntry[i].peBlue  = blue;
            pPal->palPalEntry[i].peFlags = (BYTE)0;

	    if (!(red += 32))
		if (!(green += 32))
		    blue += 64;
        }
        hpal = CreatePalette(pPal);
        LocalFree((HANDLE)pPal);
    }
    return hpal;
}
*/

void CCyluView::DoMainTitle( CDC *pDC )
{
	CBitmap cBitmapLogo;
	CBitmap *pOldBitmap;
	CDC cMemDC;

	cMemDC.CreateCompatibleDC( pDC );
	cBitmapLogo.LoadBitmap( IDB_LOGO );

	pOldBitmap = cMemDC.SelectObject( &cBitmapLogo );

	pDC->BitBlt( 0, 0, 640, 480, &cMemDC, 0, 0, SRCCOPY );	

	cMemDC.SelectObject( pOldBitmap );
	cMemDC.DeleteDC();
}

void CCyluView::OnNewGame() 
{
	CCyluApp *lpApp = (CCyluApp*)AfxGetApp();
	int nResponse;

	if(	( GetGameState()==GAMESTATE_GAMEPLAYING ) ||
		( GetGameState()==GAMESTATE_PAUSED ) )
	{
		SetGameState( GAMESTATE_PAUSED );
		nResponse = AfxMessageBox(	_T("Restart a new game?"),
									MB_ICONQUESTION | MB_YESNO );
		if( nResponse!=IDYES )
		{
			SetGameState( PrevGameState() );
			return;
		}
	}

	DestroyTimer();
	ResetVariables();
	InitialiseVariables();
	SetGameState( GAMESTATE_GAMEPLAYING );
	if( GetSafeHwnd() )
	{
		CreateTimer();
		Invalidate( TRUE );
		UpdateWindow();
	}
}

void CCyluView::OnPauseGame() 
{
	if( GetGameState()!=GAMESTATE_PAUSED )
		SetGameState( GAMESTATE_PAUSED );
	else
		SetGameState( PrevGameState() );
}

void CCyluView::SetGameState( int nState )
{
	m_nPrevState = m_nGameState;
	m_nGameState = nState;
}

int CCyluView::GetGameState( void )
{
	return m_nGameState;
}

int CCyluView::PrevGameState( void )
{
	return m_nPrevState;
}

void CCyluView::OnEndGame() 
{
	int nResponse;

	SetGameState( GAMESTATE_PAUSED );
	nResponse = AfxMessageBox(	_T("End the current game?"),
								MB_ICONQUESTION | MB_YESNO );
	if( nResponse==IDYES )
	{
		DestroyTimer();
		SetGameState( GAMESTATE_MAINTITLE );
		ResetVariables();
		if( GetSafeHwnd() )
		{
			Invalidate( TRUE );
			UpdateWindow();
		}
	}
	else
		SetGameState( PrevGameState() );
}

void CCyluView::OnUpdateEndGame(CCmdUI* pCmdUI) 
{
	if( GetGameState()==GAMESTATE_GAMEPLAYING )
		pCmdUI->Enable( TRUE );
	else
		pCmdUI->Enable( FALSE );
}

BOOL CCyluView::DestroyWindow() 
{
	DestroyTimer();
	
	return CView::DestroyWindow();
}

void CCyluView::OnUpdatePauseGame(CCmdUI* pCmdUI) 
{
	if( ( GetGameState()==GAMESTATE_PAUSED ) ||
		( GetGameState()==GAMESTATE_GAMEPLAYING ) )
	{
		pCmdUI->Enable( TRUE );
		if( GetGameState()==GAMESTATE_PAUSED )
			pCmdUI->SetCheck( TRUE );
		else
			pCmdUI->SetCheck( FALSE );
	}
	else
		pCmdUI->Enable( FALSE );
}

void CCyluView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	m_bStillMoving = FALSE;

	CView::OnKeyUp(nChar, nRepCnt, nFlags);
}

void CCyluView::PickUpObject( void )
{
	CCyluApp *lpApp = (CCyluApp*)AfxGetApp();
	CPoint ptDroid = lpApp->GetDroidPos();
	BYTE btDirection = lpApp->GetDirection();
	int nObject, nAttrib;
	int nPocketObject, nPocketAttrib;
	BOOL bOkToGet = FALSE;

	switch( btDirection )
	{
		case NORTH:
			ptDroid.y--;
			break;
		case EAST:
			ptDroid.x++;
			break;
		case SOUTH:
			ptDroid.y++;
			break;
		case WEST:
			ptDroid.x--;
			break;
	}

	nObject = lpApp->GetMaze()->GetCell( ptDroid.x, ptDroid.y, CELL_OBJECT );
	nAttrib = lpApp->GetMaze()->GetCell( ptDroid.x, ptDroid.y, CELL_ATTRIBUTE );

	switch( nObject )
	{
		case OBJECT_KEY:
		case OBJECT_FUEL:
		case OBJECT_CPU:
		case OBJECT_OBJECT:
		case OBJECT_RFL:
		case OBJECT_RTB:
			bOkToGet = TRUE;
			break;
	}

	if( bOkToGet )
	{
		lpApp->GetPocket( m_nSelPocket, &nPocketObject, &nPocketAttrib );
		lpApp->GetMaze()->SetCell( ptDroid.x, ptDroid.y, CELL_OBJECT, nPocketObject );
		lpApp->GetMaze()->SetCell( ptDroid.x, ptDroid.y, CELL_ATTRIBUTE, nPocketAttrib );
		lpApp->SetPocket( m_nSelPocket, nObject, nAttrib );
		Invalidate( FALSE );
		UpdateWindow();
	}
}

void CCyluView::DropObject( void )
{
	CCyluApp *lpApp = (CCyluApp*)AfxGetApp();
	CPoint ptDroid = lpApp->GetDroidPos();
	BYTE btDirection = lpApp->GetDirection();
	int nObject;
	int nPocketObject, nPocketAttrib;
	BOOL bOkToGet = FALSE;

	switch( btDirection )
	{
		case NORTH:
			ptDroid.y--;
			break;
		case EAST:
			ptDroid.x++;
			break;
		case SOUTH:
			ptDroid.y++;
			break;
		case WEST:
			ptDroid.x--;
			break;
	}

	nObject = lpApp->GetMaze()->GetCell( ptDroid.x, ptDroid.y, CELL_OBJECT );

	if( nObject==OBJECT_FLOOR )
	{
		lpApp->GetPocket( m_nSelPocket, &nPocketObject, &nPocketAttrib );
		lpApp->GetMaze()->SetCell( ptDroid.x, ptDroid.y, CELL_OBJECT, nPocketObject );
		lpApp->GetMaze()->SetCell( ptDroid.x, ptDroid.y, CELL_ATTRIBUTE, nPocketAttrib );
		lpApp->SetPocket( m_nSelPocket, 0, 0 );
		Invalidate( FALSE );
		UpdateWindow();
	}
}

void CCyluView::UseObject( void )
{
	CCyluApp *lpApp = (CCyluApp*)AfxGetApp();
	CPoint ptDroid = lpApp->GetDroidPos();
	BYTE btDirection = lpApp->GetDirection();
	int nPocketObject, nPocketAttrib;
	int nObject, nAttrib;

	switch( btDirection )
	{
		case NORTH:
			ptDroid.y--;
			break;
		case EAST:
			ptDroid.x++;
			break;
		case SOUTH:
			ptDroid.y++;
			break;
		case WEST:
			ptDroid.x--;
			break;
	}

	nObject = lpApp->GetMaze()->GetCell( ptDroid.x, ptDroid.y, CELL_OBJECT );
	nAttrib = lpApp->GetMaze()->GetCell( ptDroid.x, ptDroid.y, CELL_ATTRIBUTE );

	lpApp->GetPocket( m_nSelPocket, &nPocketObject, &nPocketAttrib );
	if( nPocketObject!=0 )	//	Carrying an object
	{
		switch( nPocketObject )
		{
			case OBJECT_KEY:
				Teleport();
				break;
			case OBJECT_FUEL:
				Refuel( FALSE );
				break;
			case OBJECT_CPU:
				DisableShield();
				break;
			case OBJECT_OBJECT:
				IssueObject();
				break;
			case OBJECT_RFL:
				Refuel( TRUE );
				break;
			case OBJECT_RTB:
				ReturnToBase();
				break;
		}
	}
}

void CCyluView::Refuel( BOOL bTestForBase )
{
	CCyluApp *lpApp = (CCyluApp*)AfxGetApp();
	BOOL bRefuel = FALSE;
	int nPocketObject, nPocketAttrib;
	BYTE btDirection = lpApp->GetDirection();

	CPoint ptDroid = lpApp->GetDroidPos();
	CPoint ptStart = lpApp->GetStartPos();

	lpApp->GetPocket( m_nSelPocket, &nPocketObject, &nPocketAttrib );

	if( m_nFuelStatus < MAX_FUEL_STATUS )
	{
		if( bTestForBase )
		{
			if( ( ptDroid==ptStart ) &&
				( btDirection==NORTH ) )
			{
				if( nPocketObject==OBJECT_RFL )
					bRefuel = TRUE;
			}
		}
		else
		{
			lpApp->SetPocket( m_nSelPocket, 0, 0 );
			bRefuel = TRUE;
			Invalidate( FALSE );
			UpdateWindow();
		}
	}

	if( bRefuel )
	{
		if( m_nFuelStatus<MAX_FUEL_STATUS )
			m_nFuelStatus = MAX_FUEL_STATUS;
	}
}

void CCyluView::DisableShield( void )
{
	CCyluApp *lpApp = (CCyluApp*)AfxGetApp();

	int nPocketObject, nPocketAttrib;
	BYTE btDirection = lpApp->GetDirection();

	CPoint ptDroid = lpApp->GetDroidPos();
	CPoint ptStart = lpApp->GetStartPos();
	CPoint ptShield;

	lpApp->GetPocket( m_nSelPocket, &nPocketObject, &nPocketAttrib );

	if( ( ptDroid==ptStart ) &&
		( btDirection==NORTH ) &&
		( nPocketObject==OBJECT_CPU ) )
	{
		lpApp->SetPocket( m_nSelPocket, 0, 0 );
		ptShield = lpApp->GetForceFieldPos( nPocketAttrib );
		lpApp->GetMaze()->SetCell( ptShield.x, ptShield.y, CELL_OBJECT, 0 );
		lpApp->GetMaze()->SetCell( ptShield.x, ptShield.y, CELL_ATTRIBUTE, 0 );
		Invalidate( FALSE );
		UpdateWindow();
	}
}

void CCyluView::IssueObject( void )
{
	CCyluApp *lpApp = (CCyluApp*)AfxGetApp();

	int nPocketObject, nPocketAttrib;
	BYTE btDirection = lpApp->GetDirection();

	CPoint ptDroid = lpApp->GetDroidPos();
	CPoint ptStart = lpApp->GetStartPos();

	lpApp->GetPocket( m_nSelPocket, &nPocketObject, &nPocketAttrib );

	if( ( ptDroid==ptStart ) &&
		( btDirection==NORTH ) &&
		( nPocketObject==OBJECT_OBJECT ) )
	{
		lpApp->SetPocket( m_nSelPocket, 0, 0 );
		m_nObjectsCollected++;
		if( m_nObjectsCollected==lpApp->GetMaxObjects() )
			DoGameOver();
		else
		{
			Invalidate( FALSE );
			UpdateWindow();
		}
	}
}

void CCyluView::ReturnToBase( void )
{
	CCyluApp *lpApp = (CCyluApp*)AfxGetApp();

	int nPocketObject, nPocketAttrib;
	CPoint ptStart = lpApp->GetStartPos();

	lpApp->GetPocket( m_nSelPocket, &nPocketObject, &nPocketAttrib );

	if( nPocketObject==OBJECT_RTB )
	{
		lpApp->SetDroidPos( ptStart );
		lpApp->SetScreenPos( CalculateScreenPos( ptStart, AXIS_X|AXIS_Y ) );
	}
}

void CCyluView::Teleport( void )
{
	CCyluApp *lpApp = (CCyluApp*)AfxGetApp();

	int nPocketObject, nPocketAttrib;
	BYTE btDirection = lpApp->GetDirection();
	int nObject;
	int nLoop;
	CPoint ptDroid = lpApp->GetDroidPos();
	CPoint ptTransport;

	lpApp->GetPocket( m_nSelPocket, &nPocketObject, &nPocketAttrib );

	if( nPocketObject==OBJECT_KEY )
	{
		switch( btDirection )
		{
			case NORTH:
				ptDroid.y--;
				break;
			case EAST:
				ptDroid.x++;
				break;
			case SOUTH:
				ptDroid.y++;
				break;
			case WEST:
				ptDroid.x--;
				break;
		}
		nObject = lpApp->GetMaze()->GetCell( ptDroid.x, ptDroid.y, CELL_OBJECT );
		if( nObject==OBJECT_TRANSPORT )
		{
			//	Get transport terminal position for corresponding key
			ptTransport = lpApp->GetTransporterPos( nPocketAttrib );
			
			//	Look for an empty square near teleporter
			for( nLoop=0; nLoop<4; nLoop++ )
			{
				switch( nLoop )
				{
					case 0:		//	EAST SQUARE
						ptDroid.x = ptTransport.x+1;
						ptDroid.y = ptTransport.y;
						nObject = lpApp->GetMaze()->GetCell( ptDroid.x, ptDroid.y, CELL_OBJECT );
						break;
					case 1:		//	SOUTH SQUARE
						ptDroid.x = ptTransport.x;
						ptDroid.y = ptTransport.y+1;
						nObject = lpApp->GetMaze()->GetCell( ptDroid.x, ptDroid.y, CELL_OBJECT );
						break;
					case 2:		//	WEST SQUARE
						ptDroid.x = ptTransport.x-1;
						ptDroid.y = ptTransport.y;
						nObject = lpApp->GetMaze()->GetCell( ptDroid.x, ptDroid.y, CELL_OBJECT );
						break;
					case 3:		//	NORTH SQUARE
						ptDroid.x = ptTransport.x;
						ptDroid.y = ptTransport.y-1;
						nObject = lpApp->GetMaze()->GetCell( ptDroid.x, ptDroid.y, CELL_OBJECT );
						break;
				}
				if( nObject==OBJECT_FLOOR )
				{
					nLoop = 4;
					lpApp->SetDroidPos( ptDroid );
					lpApp->SetScreenPos( CalculateScreenPos( ptDroid, AXIS_X|AXIS_Y ) );
				}
			}
		}
	}
}

void CCyluView::DoGameOver( void )
{
	CCyluApp *lpApp = (CCyluApp*)AfxGetApp();

	DestroyTimer();
	SetGameState( GAMESTATE_MAINTITLE );
	ResetVariables();

	if( GetSafeHwnd() )
	{
		Invalidate( TRUE );
		UpdateWindow();
	}

	if( m_nObjectsCollected==lpApp->GetMaxObjects() )
	{
		AfxMessageBox(	_T("Congratulations, you have completed the game\n"),
						MB_OK );
	}
	else
	{
		AfxMessageBox(	_T("Game Over\n\nYou ran out of fuel\n"),
						MB_OK );
	}
}
