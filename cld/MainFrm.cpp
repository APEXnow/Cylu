// MainFrm.cpp : implementation of the CMainFrame class
//
// CLD - Cylu Level Designer
// (C) 1996-2026 Paul S.J.Millard
//

#include "stdafx.h"
#include "CLD.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define TOOLBAR_IMAGE_SIZE		16

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	//	Show and hide the object palette from the View menu
	ON_COMMAND_EX(ID_VIEW_PALETTE, &CMainFrame::OnBarCheck)
	ON_UPDATE_COMMAND_UI(ID_VIEW_PALETTE, &CMainFrame::OnUpdateControlBarMenu)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CELL,		// cell under the mouse
	ID_INDICATOR_SELECTED,	// object being placed
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
}

CMainFrame::~CMainFrame()
{
}

/*
	Function:		CreateToolBar
	Description:	Main toolbar.  The images come from res\toolbar.bmp, one
					16x16 image per button (separators excluded), with
					magenta as the transparent colour.
*/
BOOL CMainFrame::CreateToolBar( void )
{
	static const UINT nButtons[] =
	{
		ID_FILE_NEW,
		ID_FILE_OPEN,
		ID_FILE_SAVE,
			ID_SEPARATOR,
		ID_EDIT_UNDO,
		ID_EDIT_REDO,
			ID_SEPARATOR,
		IDM_PLACE_OBJECT,
		IDM_DELETE_OBJECT,
		IDM_DRAW_LINE,
		IDM_DRAW_BOX,
			ID_SEPARATOR,
		IDM_SCREEN_GRID,
		IDM_OBJECT_ATTRIBUTES,
		IDM_ZOOM_IN,
		IDM_ZOOM_OUT,
			ID_SEPARATOR,
		IDM_PARSE_LEVEL,
		IDM_TEST_LEVEL
	};

	CBitmap cBitmap;

	if( !m_wndToolBar.CreateEx( this, TBSTYLE_FLAT,
		WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC ) )
		return FALSE;

	if( !cBitmap.LoadBitmap( IDB_TOOLBAR ) )
		return FALSE;

	if( !m_ilToolBar.Create( TOOLBAR_IMAGE_SIZE, TOOLBAR_IMAGE_SIZE, ILC_COLOR24 | ILC_MASK, 0, 20 ) )
		return FALSE;

	m_ilToolBar.Add( &cBitmap, RGB( 255, 0, 255 ) );

	m_wndToolBar.GetToolBarCtrl().SetImageList( &m_ilToolBar );
	m_wndToolBar.SetSizes( CSize( TOOLBAR_IMAGE_SIZE + 7, TOOLBAR_IMAGE_SIZE + 6 ), CSize( TOOLBAR_IMAGE_SIZE, TOOLBAR_IMAGE_SIZE ) );

	if( !m_wndToolBar.SetButtons( nButtons, _countof( nButtons ) ) )
		return FALSE;

	m_wndToolBar.SetWindowText( _T("Tools") );

	return TRUE;
}

/*
	Function:		CreatePalette
	Description:	Object palette, one button per object using the 20x20
					tiles drawn for CLD in 1996 (res\tiles.bmp)
*/
BOOL CMainFrame::CreatePalette( void )
{
	UINT nButtons[ OBJECT_COUNT - 1 ];
	CBitmap cBitmap;
	int i;

	for( i=0; i<OBJECT_COUNT-1; i++ )
		nButtons[i] = IDM_OBJECT_FIRST + 1 + i;		//	Wall (1) to Droid (12)

	if( !m_wndPalette.CreateEx( this, TBSTYLE_FLAT,
		WS_CHILD | WS_VISIBLE | CBRS_LEFT | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC,
		CRect( 0, 0, 0, 0 ), ID_VIEW_PALETTE ) )
		return FALSE;

	if( !cBitmap.LoadBitmap( IDB_TILES ) )
		return FALSE;

	if( !m_ilPalette.Create( BITMAP_WIDTH, BITMAP_HEIGHT, ILC_COLOR24, 0, OBJECT_COUNT ) )
		return FALSE;

	m_ilPalette.Add( &cBitmap, (CBitmap*)NULL );

	m_wndPalette.GetToolBarCtrl().SetImageList( &m_ilPalette );
	m_wndPalette.SetSizes( CSize( BITMAP_WIDTH + 7, BITMAP_HEIGHT + 6 ), CSize( BITMAP_WIDTH, BITMAP_HEIGHT ) );

	if( !m_wndPalette.SetButtons( nButtons, _countof( nButtons ) ) )
		return FALSE;

	m_wndPalette.SetWindowText( _T("Objects") );

	return TRUE;
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if( !CreateToolBar() )
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if( !CreatePalette() )
	{
		TRACE0("Failed to create object palette\n");
		return -1;
	}

	if( !m_wndStatusBar.Create( this ) ||
		!m_wndStatusBar.SetIndicators( indicators, _countof( indicators ) ) )
	{
		TRACE0("Failed to create status bar\n");
		return -1;
	}

	m_wndToolBar.EnableDocking( CBRS_ALIGN_ANY );
	m_wndPalette.EnableDocking( CBRS_ALIGN_ANY );
	EnableDocking( CBRS_ALIGN_ANY );
	DockControlBar( &m_wndToolBar, AFX_IDW_DOCKBAR_TOP );
	DockControlBar( &m_wndPalette, AFX_IDW_DOCKBAR_LEFT );

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;

	//	The 1996 version opened at 640x480 in the top left corner
	cs.cx = 1100;
	cs.cy = 800;

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG
