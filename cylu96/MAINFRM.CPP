// mainfrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "Cylu.h"

#include "mainfrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	
}

CMainFrame::~CMainFrame()
{
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style =	WS_DLGFRAME | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
	/*cs.x = 0;
	cs.y = 0;
	cs.cx = 640;
	cs.cy = 480;*/

	return CFrameWnd::PreCreateWindow(cs);
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

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	//	2026: in 1996 the window was 640x480 including its frame, which
	//	left about 640x438 for the game.  Modern frames are thicker, so
	//	size the client area to that instead and centre the window.
	CRect rcClientArea( 0, 0, 640, 438 );
	AdjustWindowRectEx( &rcClientArea, GetStyle(), TRUE, GetExStyle() );
	SetWindowPos( NULL, 0, 0, rcClientArea.Width(), rcClientArea.Height(), SWP_NOMOVE | SWP_NOZORDER );
	CenterWindow();

	SetWindowText( APP_TITLE );
	
	return 0;
}
