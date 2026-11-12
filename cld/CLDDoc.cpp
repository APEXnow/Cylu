// CLDDoc.cpp : implementation of the CCLDDoc class
//
// CLD - Cylu Level Designer
// (C) 1996-2026 Paul S.J.Millard
//

#include "stdafx.h"
#include "CLD.h"

#include "CLDDoc.h"
#include "NewMazeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CCLDDoc

IMPLEMENT_DYNCREATE(CCLDDoc, CDocument)

BEGIN_MESSAGE_MAP(CCLDDoc, CDocument)
	ON_COMMAND(IDM_NEW_DIMENSIONS, &CCLDDoc::OnNewDimensions)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, &CCLDDoc::OnUpdateFileSave)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_AS, &CCLDDoc::OnUpdateFileSaveAs)
	ON_COMMAND(ID_EDIT_UNDO, &CCLDDoc::OnEditUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, &CCLDDoc::OnUpdateEditUndo)
	ON_COMMAND(ID_EDIT_REDO, &CCLDDoc::OnEditRedo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REDO, &CCLDDoc::OnUpdateEditRedo)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCLDDoc construction/destruction

CCLDDoc::CCLDDoc()
{
}

CCLDDoc::~CCLDDoc()
{
}

/*
	Function:		CreateBorderedMaze
	Description:	New empty level with a wall all the way round, as the
					1996 version did
*/
void CCLDDoc::CreateBorderedMaze( int nWidth, int nHeight )
{
	int nLoopx, nLoopy;

	if( !m_cMaze.CreateMaze( nWidth, nHeight ) )
		return;

	for( nLoopx=0; nLoopx<nWidth; nLoopx++ )
	{
		m_cMaze.SetCell( nLoopx, 0, CELL_OBJECT, OBJECT_WALL );
		m_cMaze.SetCell( nLoopx, nHeight-1, CELL_OBJECT, OBJECT_WALL );
	}
	for( nLoopy=0; nLoopy<nHeight; nLoopy++ )
	{
		m_cMaze.SetCell( 0, nLoopy, CELL_OBJECT, OBJECT_WALL );
		m_cMaze.SetCell( nWidth-1, nLoopy, CELL_OBJECT, OBJECT_WALL );
	}
}

BOOL CCLDDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())		//	Calls DeleteContents
		return FALSE;

	CreateBorderedMaze( DEFAULT_LEVEL_WIDTH, DEFAULT_LEVEL_HEIGHT );

	if( !m_cMaze.ValidMaze() )
	{
		AfxMessageBox( _T("Maze too large for memory allocation."), MB_ICONINFORMATION );
		return FALSE;
	}

	return TRUE;
}

void CCLDDoc::DeleteContents()
{
	m_cMaze.DestroyMaze();
	m_Undo.clear();
	m_Redo.clear();

	CDocument::DeleteContents();
}

/////////////////////////////////////////////////////////////////////////////
// CCLDDoc serialization

void CCLDDoc::Serialize(CArchive& ar)
{
	//	Levels are read and written by CMaze (see OnOpenDocument and
	//	OnSaveDocument) so the file format stays the one the game uses
	UNREFERENCED_PARAMETER( ar );
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
	CMaze cLoaded;
	FILE *pFile;
	BOOL bRead;
	CString cstrMessage;

	pFile = _tfopen( lpszPathName, _T("rb") );
	if( !pFile )
	{
		cstrMessage.Format( _T("Unable to open %s"), lpszPathName );
		AfxMessageBox( cstrMessage, MB_ICONEXCLAMATION );
		return FALSE;
	}

	bRead = cLoaded.ReadMaze( pFile );
	fclose( pFile );

	if( !bRead )
	{
		cstrMessage.Format( _T("%s is not a Cylu level (.CLD) file, or it is damaged."), lpszPathName );
		AfxMessageBox( cstrMessage, MB_ICONEXCLAMATION );
		return FALSE;
	}

	DeleteContents();
	m_cMaze = cLoaded;
	SetModifiedFlag( FALSE );

	return TRUE;
}

/*
	Function:		SaveMazeTo
	Description:	Writes the level in the game's .CLD format.  Used by
					Save and by Test in Cylu.
*/
BOOL CCLDDoc::SaveMazeTo( LPCTSTR lpszPathName )
{
	FILE *pFile;
	BOOL bRet;

	pFile = _tfopen( lpszPathName, _T("wb") );
	if( !pFile )
		return FALSE;

	bRet = m_cMaze.WriteMaze( pFile );

	if( fclose( pFile ) != 0 )
		bRet = FALSE;

	return bRet;
}

BOOL CCLDDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	CString cstrMessage;

	if( !SaveMazeTo( lpszPathName ) )
	{
		cstrMessage.Format( _T("Unable to save %s"), lpszPathName );
		AfxMessageBox( cstrMessage, MB_ICONEXCLAMATION );
		return FALSE;
	}

	SetModifiedFlag( FALSE );
	return TRUE;
}

void CCLDDoc::GetMazeSize( SIZE *lpSize )
{
	int nWidth;
	int nHeight;

	m_cMaze.GetMazeSize( &nWidth, &nHeight );
	lpSize->cx = nWidth;
	lpSize->cy = nHeight;
}

/////////////////////////////////////////////////////////////////////////////
// Editing and undo

void CCLDDoc::BeginChange( void )
{
	m_Undo.push_back( m_cMaze );
	if( m_Undo.size() > MAX_UNDO )
		m_Undo.pop_front();
}

void CCLDDoc::EndChange( BOOL bChanged )
{
	if( !bChanged )
	{
		//	Nothing happened, so forget the snapshot taken by BeginChange
		if( !m_Undo.empty() )
			m_Undo.pop_back();
		return;
	}

	m_Redo.clear();
	SetModifiedFlag();
}

int CCLDDoc::SetCell( int nX, int nY, int nObject, int nAttrib )
{
	int nWidth, nHeight;
	int nLoopx, nLoopy;
	int nRet = CHANGE_NONE;

	m_cMaze.GetMazeSize( &nWidth, &nHeight );
	if( nX < 0 || nY < 0 || nX >= nWidth || nY >= nHeight )
		return CHANGE_NONE;

	if( m_cMaze.GetCell( nX, nY, CELL_OBJECT ) == nObject &&
		m_cMaze.GetCell( nX, nY, CELL_ATTRIBUTE ) == nAttrib )
		return CHANGE_NONE;

	//	There is only one droid, so placing it moves it
	if( nObject == OBJECT_DROID )
	{
		for( nLoopy=0; nLoopy<nHeight; nLoopy++ )
		{
			for( nLoopx=0; nLoopx<nWidth; nLoopx++ )
			{
				if( m_cMaze.GetCell( nLoopx, nLoopy, CELL_OBJECT ) == OBJECT_DROID &&
					( nLoopx != nX || nLoopy != nY ) )
				{
					m_cMaze.SetCell( nLoopx, nLoopy, CELL_OBJECT, OBJECT_FLOOR );
					m_cMaze.SetCell( nLoopx, nLoopy, CELL_ATTRIBUTE, 0 );
					nRet = CHANGE_OTHERS;
				}
			}
		}
	}

	m_cMaze.SetCell( nX, nY, CELL_OBJECT, nObject );
	m_cMaze.SetCell( nX, nY, CELL_ATTRIBUTE, nAttrib );

	if( nRet == CHANGE_NONE )
		nRet = CHANGE_CELL;

	return nRet;
}

void CCLDDoc::OnEditUndo()
{
	if( m_Undo.empty() )
		return;

	m_Redo.push_back( m_cMaze );
	m_cMaze = m_Undo.back();
	m_Undo.pop_back();

	SetModifiedFlag();
	UpdateAllViews( NULL );
}

void CCLDDoc::OnUpdateEditUndo(CCmdUI* pCmdUI)
{
	pCmdUI->Enable( CanUndo() );
}

void CCLDDoc::OnEditRedo()
{
	if( m_Redo.empty() )
		return;

	m_Undo.push_back( m_cMaze );
	m_cMaze = m_Redo.back();
	m_Redo.pop_back();

	SetModifiedFlag();
	UpdateAllViews( NULL );
}

void CCLDDoc::OnUpdateEditRedo(CCmdUI* pCmdUI)
{
	pCmdUI->Enable( CanRedo() );
}

void CCLDDoc::OnNewDimensions()
{
	int nResponse;
	CNewMazeDlg cdlgNewMaze( AfxGetMainWnd() );

	if( m_cMaze.ValidMaze() )
	{
		nResponse = AfxMessageBox( _T("This operation will replace the level with a new empty one (Undo will bring it back). Continue?"), MB_ICONQUESTION|MB_YESNO );
		if( IDYES!=nResponse )
			return;
	}

	if( IDOK==cdlgNewMaze.DoModal() )
	{
		BeginChange();
		CreateBorderedMaze( cdlgNewMaze.m_nEditWidth, cdlgNewMaze.m_nEditHeight );

		if( !m_cMaze.ValidMaze() )
		{
			AfxMessageBox( _T("Maze too large for memory allocation."), MB_ICONINFORMATION );
			m_cMaze = m_Undo.back();		//	Put the old level back
			m_Undo.pop_back();
			return;
		}

		EndChange( TRUE );
		UpdateAllViews( NULL );
	}
}

void CCLDDoc::OnUpdateFileSave(CCmdUI* pCmdUI)
{
	pCmdUI->Enable( m_cMaze.ValidMaze() );
}

void CCLDDoc::OnUpdateFileSaveAs(CCmdUI* pCmdUI)
{
	pCmdUI->Enable( m_cMaze.ValidMaze() );
}
