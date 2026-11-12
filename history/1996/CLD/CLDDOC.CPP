// CLDDoc.cpp : implementation of the CCLDDoc class
//

#include "stdafx.h"
#include "CLD.h"

#include "CLDDoc.h"
#include "nwmzdlg.h"
#include "..\\cylu\\cmaze.hpp"
#include "parsedlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCLDDoc

IMPLEMENT_DYNCREATE(CCLDDoc, CDocument)

BEGIN_MESSAGE_MAP(CCLDDoc, CDocument)
	//{{AFX_MSG_MAP(CCLDDoc)
	ON_COMMAND(IDM_NEW_DIMENSIONS, OnNewDimensions)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, OnUpdateFileSave)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_AS, OnUpdateFileSaveAs)
	ON_COMMAND(IDM_PARSE_LEVEL, OnParseLevel)
	ON_UPDATE_COMMAND_UI(IDM_PARSE_LEVEL, OnUpdateParseLevel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCLDDoc construction/destruction

CCLDDoc::CCLDDoc()
{
}

CCLDDoc::~CCLDDoc()
{
}

BOOL CCLDDoc::OnNewDocument()
{
	int nLoopx, nLoopy;

	if (!CDocument::OnNewDocument())
		return FALSE;

	if( !m_cMaze.CreateMaze(	DEFAULT_LEVEL_WIDTH,
								DEFAULT_LEVEL_HEIGHT ) )
	{
		AfxMessageBox( "Maze to large for memory allocation.", MB_ICONINFORMATION );
		return FALSE;
	}
	else
	{
		#ifdef FILL_LEVEL
		int nIndex = 0;
		for( nLoopx=0; nLoopx<DEFAULT_LEVEL_WIDTH-1; nLoopx++ )
		{
			for( nLoopy=0; nLoopy<DEFAULT_LEVEL_HEIGHT-1; nLoopy++ )
			{
				m_cMaze.SetCell( nLoopx, nLoopy, CELL_OBJECT, nIndex );
				nIndex++;
				if( nIndex>BITMAP_COUNT )
					nIndex=0;
			}
		}
		#endif

		for( nLoopx=0; nLoopx<DEFAULT_LEVEL_WIDTH; nLoopx++ )
		{
			m_cMaze.SetCell( nLoopx, 0, CELL_OBJECT, OBJECT_WALL );
			m_cMaze.SetCell( nLoopx, DEFAULT_LEVEL_HEIGHT-1, CELL_OBJECT, OBJECT_WALL );
		}
		for( nLoopy=0; nLoopy<DEFAULT_LEVEL_HEIGHT; nLoopy++ )
		{
			m_cMaze.SetCell( 0, nLoopy, CELL_OBJECT, OBJECT_WALL );
			m_cMaze.SetCell( DEFAULT_LEVEL_WIDTH-1, nLoopy, CELL_OBJECT, OBJECT_WALL );
		}
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CCLDDoc serialization

void CCLDDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CCLDDoc diagnostics

#ifdef _DEBUG
void CCLDDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CCLDDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCLDDoc commands

BOOL CCLDDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!GetMaze()->LoadMaze( lpszPathName ))
		return FALSE;

	UpdateAllViews( NULL );

	return TRUE;
}

BOOL CCLDDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	SetModifiedFlag( FALSE );
	return GetMaze()->SaveMaze( lpszPathName );
}

void CCLDDoc::GetMazeSize( SIZE *lpSize )
{
	CMaze* lpCMaze = GetMaze();
	int nWidth;
	int nHeight;
	lpCMaze->GetMazeSize( &nWidth, &nHeight );
	lpSize->cx = nWidth;
	lpSize->cy = nHeight;
}

void CCLDDoc::OnNewDimensions() 
{
	BOOL bResult;
	int nLoopx, nLoopy;
	int nResponse;
	CNewMazeDlg cdlgNewMaze( AfxGetMainWnd() );

	if( GetMaze()->ValidMaze() )
	{
		nResponse = AfxMessageBox(	"This operation will destroy the existing maze in memory, continue?", MB_ICONQUESTION|MB_YESNO );
		if( IDYES!=nResponse )
			return;
	}

	if( IDOK==cdlgNewMaze.DoModal() )
	{
		if( ( cdlgNewMaze.m_nEditWidth >=1 ) &&
			( cdlgNewMaze.m_nEditWidth <= MAX_WIDTH ) &&
			( cdlgNewMaze.m_nEditHeight >= 1) &&
			( cdlgNewMaze.m_nEditHeight <= MAX_HEIGHT ) )
		{
			bResult = m_cMaze.CreateMaze(	cdlgNewMaze.m_nEditWidth,
											cdlgNewMaze.m_nEditHeight );
			if( !bResult )
				AfxMessageBox( "Maze to large for memory allocation.", MB_ICONINFORMATION );
			else
			{
				#ifdef FILL_LEVEL
				int nIndex = 0;
				for( nLoopx=0; nLoopx<cdlgNewMaze.m_nEditWidth; nLoopx++ )
				{
					for( nLoopy=0; nLoopy<cdlgNewMaze.m_nEditHeight; nLoopy++ )
					{
						m_cMaze.SetCell( nLoopx, nLoopy, CELL_OBJECT, nIndex );
						nIndex++;
						if( nIndex>BITMAP_COUNT )
							nIndex=0;
					}
				}
				#endif

				for( nLoopx=0; nLoopx<cdlgNewMaze.m_nEditWidth; nLoopx++ )
				{
					m_cMaze.SetCell( nLoopx, 0, CELL_OBJECT, OBJECT_WALL );
					m_cMaze.SetCell( nLoopx, (cdlgNewMaze.m_nEditHeight-1), CELL_OBJECT, OBJECT_WALL );
				}
				for( nLoopy=0; nLoopy<cdlgNewMaze.m_nEditHeight; nLoopy++ )
				{
					m_cMaze.SetCell( 0, nLoopy, CELL_OBJECT, OBJECT_WALL );
					m_cMaze.SetCell( (cdlgNewMaze.m_nEditWidth-1), nLoopy, CELL_OBJECT, OBJECT_WALL );
				}
			}
			SetModifiedFlag();
			AfxGetMainWnd()->Invalidate( TRUE );
			UpdateAllViews(NULL, NULL, NULL);
		}
		else
			AfxMessageBox( "Invalid dimensions.", MB_ICONEXCLAMATION );
	}
}

void CCLDDoc::OnUpdateFileSave(CCmdUI* pCmdUI) 
{
	if( GetMaze()->ValidMaze() )
		pCmdUI->Enable( TRUE );
	else
		pCmdUI->Enable( FALSE );
}

void CCLDDoc::OnUpdateFileSaveAs(CCmdUI* pCmdUI) 
{
	if( GetMaze()->ValidMaze() )
		pCmdUI->Enable( TRUE );
	else
		pCmdUI->Enable( FALSE );
}

void CCLDDoc::OnParseLevel() 
{
	int nWidth, nHeight;
	int nLoopx, nLoopy, nObj;
	CParseDlg parseDlg;
	char szFormat[255];
	char szString[255];

	GetMaze()->GetMazeSize( &nWidth, &nHeight );

	int nFound[6];
	//	Index meaning
	//	0	Objects
	//	1	Fuel
	//	2	Chips
	//	3	ForceFields
	//	4	Transporters
	//	5	Keys

	strcpy( szFormat, "\n Objects:%d\n Fuel Cells:%d\n Chips:%d\n ForceFields:%d\n Transporters:%d\n Keys:%d\n" );

	//	Clear statistics array
	for( nLoopx=0; nLoopx<6; nLoopx++ )
		nFound[ nLoopx ]=0;

	for( nLoopy=0; nLoopy<nHeight; nLoopy++ )
	{
		for( nLoopx=0; nLoopx<nWidth; nLoopx++ )
		{
			nObj = GetMaze()->GetCell( nLoopx, nLoopy, CELL_OBJECT );
			switch( nObj )
			{
				case OBJECT_OBJECT:
					nFound[0]++;
					break;
				case OBJECT_FUEL:
					nFound[1]++;
					break;
				case OBJECT_CPU:
					nFound[2]++;
					break;
				case OBJECT_SHIELD:
					nFound[3]++;
					break;
				case OBJECT_TRANSPORT:
					nFound[4]++;
					break;
				case OBJECT_KEY:
					nFound[5]++;
					break;
			}
		}
	}
	wsprintf(	szString,
				szFormat,
				nFound[0],
				nFound[1],
				nFound[2],
				nFound[3],
				nFound[4],
				nFound[5] );

	parseDlg.m_cstrStatistics = szString;

	parseDlg.DoModal();
}

void CCLDDoc::OnUpdateParseLevel(CCmdUI* pCmdUI) 
{
	if( GetMaze()->ValidMaze() )
		pCmdUI->Enable( TRUE );
	else
		pCmdUI->Enable( FALSE );
}
