// Cylu.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Cylu.h"

#include "mainfrm.h"
#include "Cyludoc.h"
#include "Cyluview.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CCyluApp

BEGIN_MESSAGE_MAP(CCyluApp, CWinApp)
	//{{AFX_MSG_MAP(CCyluApp)
	ON_COMMAND(ID_APP_ABOUT, &CCyluApp::OnAppAbout)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCyluApp construction

CCyluApp::CCyluApp()
{
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CCyluApp object

CCyluApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CCyluApp initialization

BOOL CCyluApp::InitInstance()
{
	BOOL bRet = TRUE;
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

	//	2026: Enable3dControls() no longer exists - Windows draws 3D
	//	controls itself.  Settings are kept in the registry rather
	//	than an INI file in the Windows folder.
	SetRegistryKey( _T("Apex") );
	LoadStdProfileSettings(0);  // Load standard INI file options (including MRU)

	if( !InitialiseCylu() )		// Intialise main sprites etc.
		bRet = FALSE;

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CCyluDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CCyluView));
	AddDocTemplate(pDocTemplate);

	// create a new (empty) document
	OnFileNew();

	if (m_lpCmdLine[0] != '\0')
	{
		// TODO: add command line processing here
	}

	return bRet;
}

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//{{AFX_MSG(CAboutDlg)
		// No message handlers
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CCyluApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CCyluApp commands

void CCyluApp::DoMessageLoop( void )
{
	MSG	msg;
    if( PeekMessage(&msg,NULL,0,0,PM_REMOVE) )
    {
    	TranslateMessage(&msg);
    	DispatchMessage(&msg);
    }               
}

BOOL CCyluApp::CreatePictureBox( void )
{
	BOOL bRet = TRUE;

	if( GetPictureBox() )
	{
		delete GetPictureBox();
		m_lpCPictureBox = NULL;
	}

	m_lpCPictureBox = new CPictureBox;
	if( !m_lpCPictureBox )
		bRet = FALSE;

	return bRet;
}

BOOL CCyluApp::CreateMaze( void )
{
	BOOL bRet = TRUE;
	int nLoop;
	CString cstrLevelName = m_cstrLevelName;		//	2026: see InitialiseCylu

	if( GetMaze() )
	{
		delete GetMaze();
		m_lpCMaze = NULL;
	}

	m_lpCMaze = new CMaze;
	if( !m_lpCMaze )
		bRet = FALSE;

	if( !GetMaze()->LoadMaze( cstrLevelName ) )
	{
		GetMaze()->CreateMaze( DEFAULT_LEVEL_WIDTH, DEFAULT_LEVEL_HEIGHT );
		for( nLoop=0; nLoop<DEFAULT_LEVEL_WIDTH; nLoop++ )
		{
			GetMaze()->SetCell( nLoop, 0, CELL_OBJECT, OBJECT_WALL );
			GetMaze()->SetCell( nLoop, DEFAULT_LEVEL_HEIGHT, CELL_OBJECT, OBJECT_WALL );
			GetMaze()->SetCell( nLoop, 0, CELL_ATTRIBUTE, WALL_BORDER );
			GetMaze()->SetCell( nLoop, DEFAULT_LEVEL_HEIGHT, CELL_ATTRIBUTE, WALL_BORDER );
		}
		for( nLoop=0; nLoop<DEFAULT_LEVEL_HEIGHT; nLoop++ )
		{
			GetMaze()->SetCell( 0, nLoop, CELL_OBJECT, OBJECT_WALL );
			GetMaze()->SetCell( DEFAULT_LEVEL_WIDTH, nLoop, CELL_OBJECT, OBJECT_WALL );
			GetMaze()->SetCell( 0, nLoop, CELL_ATTRIBUTE, OBJECT_WALL );
			GetMaze()->SetCell( DEFAULT_LEVEL_WIDTH, nLoop, CELL_ATTRIBUTE, OBJECT_WALL );
		}
		AfxMessageBox( _T("Could not level, empty level created\n") );
	}
	else
		if( !ParseLevel() )
			AfxMessageBox( _T("Failed to parse level\n") );		

	return bRet;
}

void CCyluApp::DestroyPictureBox( void )
{
	if( GetPictureBox() )
	{
		delete GetPictureBox();
		m_lpCPictureBox = NULL;
	}
}

void CCyluApp::DestroyMaze( void )
{
	if( GetMaze() )
	{
		delete GetMaze();
		m_lpCMaze = NULL;
	}
}

CPictureBox* CCyluApp::GetPictureBox( void )
{
	return m_lpCPictureBox;
}

CMaze* CCyluApp::GetMaze( void )
{
	return m_lpCMaze;
}

int CCyluApp::ExitInstance() 
{
	ShutDownCylu();
	return CWinApp::ExitInstance();
}

BOOL CCyluApp::InitialiseCylu( void )
{
	BOOL bRet = TRUE;

	m_lpCPictureBox = NULL;
	m_lpCMaze = NULL;
	SetDirection( NORTH );
	SetMoveState( MOVESTATE_STATIC );
	SetMoveFrame( NULL );
	if( !CreatePictureBox() )
	{
		TRACE( _T("Unable to create CPictureBox object\n") );
		bRet = FALSE;
	}
	else
	{
		//	2026: CYLU.INI pointed at C:\CYLU\SPRITES.SSF and C:\CYLU\LEVEL.CLD.
		//	The files now live in a cylu96 folder next to the executable,
		//	and a level can be given on the command line instead.
		TCHAR szPath[ MAX_PATH ];
		CString cstrFolder;

		GetModuleFileName( NULL, szPath, MAX_PATH );
		cstrFolder = szPath;
		cstrFolder = cstrFolder.Left( cstrFolder.ReverseFind( _T('\\') ) + 1 ) + _T("cylu96\\");

		m_cstrSSFName = cstrFolder + _T("sprites.ssf");
		m_cstrLevelName = cstrFolder + _T("level.cld");

		if( __argc > 1 )
			m_cstrLevelName = __targv[1];
	}

	return bRet;
}

void CCyluApp::ShutDownCylu( void )
{
	DestroyPictureBox();
	DestroyMaze();
}

void CCyluApp::SetForceFieldPos( CPoint pt, int nIndex )
{
	if( (nIndex>=0) && (nIndex<MAX_FORCEFIELDS) )		//	2026: was <=
		m_cForcefields[ nIndex ]=pt;
}

CPoint CCyluApp::GetForceFieldPos( int nIndex )
{
	CPoint ptRet;

	if( (nIndex>=0) && (nIndex<MAX_FORCEFIELDS) )
		return m_cForcefields[ nIndex ];

	return ptRet;
}

void CCyluApp::SetTransporterPos( CPoint pt, int nIndex )
{
	if( (nIndex>=0) && (nIndex<MAX_TRANSPORTERS) )		//	2026: was <=
		m_cTransporters[ nIndex ]=pt;
}

CPoint CCyluApp::GetTransporterPos( int nIndex )
{
	CPoint ptRet;

	if( (nIndex>=0) && (nIndex<MAX_TRANSPORTERS) )
		return m_cTransporters[ nIndex ];

	return ptRet;
}

void CCyluApp::SetPocket( int nPocket, int nObject, int nAttrib )
{
	if( ( nPocket >=0) && ( nPocket<MAX_POCKETS ) )		//	2026: was <=
	{
		m_nPocketObject[ nPocket ]=nObject;
		m_nPocketAttrib[ nPocket ]=nAttrib;
	}
}

BOOL CCyluApp::GetPocket( int nPocket, int *lpnObject, int *lpnAttrib )
{
	BOOL bRet = FALSE;
	if( ( nPocket >=0) && ( nPocket<MAX_POCKETS ) )
	{
		*lpnObject=m_nPocketObject[ nPocket ];
		*lpnAttrib=m_nPocketAttrib[ nPocket ];
		bRet = TRUE;
	}
	
	return bRet;
}

BOOL CCyluApp::ParseLevel( void )
{
	BOOL bRet = FALSE;
	int nWidth, nHeight;
	int nLoopx, nLoopy;
	int nForcefield = 0;
	int nTransporter = 0;
	int nObject;
	int nAttrib;
	int nMaxObjects = 0;
	CPoint pt;

	if( GetMaze()->ValidMaze() )
	{
		GetMaze()->GetMazeSize( &nWidth, &nHeight );
		for( nLoopy=0; nLoopy<nHeight; nLoopy++ )
		{
			for( nLoopx=0; nLoopx<nWidth; nLoopx++ )
			{
				nObject = GetMaze()->GetCell( nLoopx, nLoopy, CELL_OBJECT );
				nAttrib = GetMaze()->GetCell( nLoopx, nLoopy, CELL_ATTRIBUTE );
				pt.x = nLoopx;
				pt.y = nLoopy;
				switch( nObject )
				{
					case OBJECT_OBJECT:
						nMaxObjects++;
						break;
					case OBJECT_SHIELD:
						SetForceFieldPos( pt, nAttrib );
						break;
					case OBJECT_TRANSPORT:
						SetTransporterPos( pt, nAttrib );
						break;
					case OBJECT_BASE:
						SetBasePos( pt );
						break;
					case OBJECT_DROID:
						SetStartPos( pt );
						SetDroidPos( pt );
						GetMaze()->SetCell( pt.x, pt.y, CELL_OBJECT, NULL );
						GetMaze()->SetCell( pt.x, pt.y, CELL_ATTRIBUTE, NULL );
						break;
				}
			}
		}
		SetMaxObjects( nMaxObjects );
		bRet = TRUE;
	}

	return bRet;
}
