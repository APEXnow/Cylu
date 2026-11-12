/******************************************************************

	Project:	Cylu
	File:		cmaze.hpp
	Author:		Paul Millard
	Original:	Unknown
	Notes:		Class definition.
				Class function definitions for CMaze.

******************************************************************/

#include "stdafx.h"
#include "..\\cylu\\cmaze.hpp"

CMaze::CMaze()
{
	InitialiseMembers();
}

CMaze::CMaze( int nWidth, int nHeight )
{
	InitialiseMembers();
	Allocate( nWidth, nHeight );
}

CMaze::~CMaze()
{
	Deallocate();
}

void CMaze::InitialiseMembers( void )
{
	m_hgblMaze = NULL;
	m_lpCell = NULL;
	m_nWidth = NULL;
	m_nHeight = NULL;
	m_lpCFileMaze = NULL;
}

void CMaze::Deallocate( void )
{
	if( m_hgblMaze )
	{
		UnlockMaze();
		GlobalFree( m_hgblMaze );
		InitialiseMembers();
	}
}

BOOL CMaze::Allocate( int nWidth, int nHeight )
{
	BOOL bRet = FALSE;

	long lMemRequired = sizeof( CELL ) * ( nWidth * nHeight );

	Deallocate();

	m_hgblMaze = GlobalAlloc( GHND, lMemRequired );
	if( m_hgblMaze )
	{
		m_nWidth = nWidth;
		m_nHeight = nHeight;
		if( LockMaze() )
			bRet = TRUE;
		else
			Deallocate();
	}

	return bRet;
}

BOOL CMaze::CreateMaze( int nWidth, int nHeight )
{
	BOOL bRet = Allocate( nWidth, nHeight );
	return bRet;
}

void CMaze::DestroyMaze( void )
{
	Deallocate();
}

BOOL CMaze::SetCell( int nXPos, int nYPos, int nFlag, int nValue )
{
	BOOL bRet = FALSE;
	CELL *lpCell;

	if( m_hgblMaze )
	{
		if( (nXPos>=0) && (nXPos<m_nWidth) &&
			(nYPos>=0) && (nYPos<m_nHeight) )
		{
			LockMaze();
			lpCell = m_lpCell;
			lpCell+=( (nYPos * m_nWidth) + nXPos );
			switch( nFlag )
			{
				case CELL_OBJECT:
					lpCell->m_nObject = nValue;
					break;
				case CELL_ATTRIBUTE:
					lpCell->m_nAttribute = nValue;
					break;
			}
			bRet = TRUE;
		}
	}

	return bRet;
}

int CMaze::GetCell( int nXPos, int nYPos, int nFlag )
{
	int nRet = 0;
	CELL *lpCell;

	if( m_hgblMaze )
	{
		if( (nXPos>=0) && (nXPos<m_nWidth) &&
			(nYPos>=0) && (nYPos<m_nHeight) )
		{
			LockMaze();
			lpCell = m_lpCell;
			lpCell+=( (nYPos * m_nWidth) + nXPos );
			switch( nFlag )
			{
				case CELL_OBJECT:
					nRet = lpCell->m_nObject;
					break;
				case CELL_ATTRIBUTE:
					nRet = lpCell->m_nAttribute;
					break;
			}
		}
	}

	return nRet;
}

void CMaze::GetMazeSize( int *lpnWidth, int *lpnHeight )
{
	*lpnWidth = m_nWidth;
	*lpnHeight = m_nHeight;
}

BOOL CMaze::LoadMaze( CString cstrMazeName )
{
	BOOL bRet = FALSE;

	int nFileError = NULL;
	int nWidth;
	int nHeight;
	int nLoopx;
	int nLoopy;
	int nBuff;
	int nOpenError;
	int nFileID;

	Deallocate();

	if( OpenFile( FILE_LOAD, cstrMazeName, &nOpenError ) )
	{
		nFileError+=Read( (int*)&nFileID, sizeof( int ) );
		nFileError+=Read( (int*)&nWidth, sizeof( int ) );
		nFileError+=Read( (int*)&nHeight, sizeof( int ) );

		if( nFileID!=FILE_ID )
			AfxMessageBox( "This file is not a CLD maze file.", MB_ICONEXCLAMATION );
		else
		{
			if( Allocate( nWidth, nHeight ) )
			{
				for( nLoopy=0; nLoopy<nHeight; nLoopy++ )
				{
					for( nLoopx=0; nLoopx<nWidth; nLoopx++ )
					{
						nFileError+=Read( (PVOID)&nBuff, sizeof( int ) );
						SetCell( nLoopx, nLoopy, CELL_OBJECT, nBuff );
						nFileError+=Read( (PVOID)&nBuff, sizeof( int ) );
						SetCell( nLoopx, nLoopy, CELL_ATTRIBUTE, nBuff );
					}
				}
				if( !nFileError )
					bRet = TRUE;
				else
					Deallocate();
			}
		}
		CloseFile();
	}
	else
		AfxMessageBox( "Failed to load CLD maze file.", MB_ICONEXCLAMATION );

	return bRet;
}

BOOL CMaze::SaveMaze( CString cstrMazeName )
{
	BOOL bRet = FALSE;

	int nFileError = NULL;
	int nLoopx;
	int nLoopy;
	int nBuff;
	int nOpenError;
	int nFileID = FILE_ID;

	if( m_hgblMaze )
	{
		if( OpenFile( FILE_SAVE, cstrMazeName, &nOpenError ) )
		{
			nFileError+=Write( (PVOID)&nFileID, sizeof( int ) );
			nFileError+=Write( (PVOID)&m_nWidth, sizeof( int ) );
			nFileError+=Write( (PVOID)&m_nHeight, sizeof( int ) );
			GetFile()->Flush();

			for( nLoopy=0; nLoopy<m_nHeight; nLoopy++ )
			{
				for( nLoopx=0; nLoopx<m_nWidth; nLoopx++ )
				{
					nBuff = GetCell( nLoopx, nLoopy, CELL_OBJECT );
					nFileError+=Write( (PVOID)&nBuff, sizeof( int ) );
					nBuff = GetCell( nLoopx, nLoopy, CELL_ATTRIBUTE );
					nFileError+=Write( (PVOID)&nBuff, sizeof( int ) );
				}
				GetFile()->Flush();
			}
			if( !nFileError )
				bRet = TRUE;

			CloseFile();
		}
		else
			AfxMessageBox( "Failed to save CLD Maze file.", MB_ICONEXCLAMATION );
	}

	return bRet;
}

BOOL CMaze::OpenFile( int nMode, CString cstrFileName, int *lpError )
{
    BOOL bRet = FALSE;
        
    CloseFile();
	UINT uiFlags;
    CFile *lpFile;
    CFileException cFileException;
    
	switch( nMode )
	{
		case FILE_LOAD:
			uiFlags = CFile::modeReadWrite | CFile::shareDenyNone | CFile::typeBinary;
			break;
		case FILE_SAVE:
			uiFlags = CFile::modeCreate | CFile::modeReadWrite | CFile::shareDenyNone | CFile::typeBinary;
			break;
		default:
			*lpError = NULL;
			return bRet;
	}

    lpFile = new CFile;
	if( !lpFile )
	{
		bRet = FALSE;
		*lpError = NULL;
	}
	else
	{
		SetFile( lpFile );
    
		GetFile()->Open( cstrFileName, uiFlags, &cFileException );
		if (cFileException.m_cause)
		{
			delete lpFile;
			SetFile( (CFile*)0 );
			*lpError = cFileException.m_cause;
			bRet = FALSE;
		}
		else
		{
			bRet = TRUE;
			*lpError = NULL;
		}
	}

    return bRet;
}

void CMaze::CloseFile()
{
    if (GetFile())
    {
		GetFile()->Flush();
		GetFile()->Close();
        delete GetFile();
        SetFile( (CFile*)0 );
    }
}

void CMaze::SetFile( CFile *lpCFile )
{
	m_lpCFileMaze = lpCFile;
}

CFile* CMaze::GetFile( void )
{
	return m_lpCFileMaze;
}

int CMaze::Read( PVOID pBuf, int nSize )
{
	int nRet = FALSE;

	if( GetFile() )
	{
		//  Catch any file exception if the read function fails.
		TRY
		{
			GetFile()->Read( pBuf, nSize );
		}
		CATCH( CFileException, cFileException )
		{
			nRet = cFileException->m_cause;
		}
		END_CATCH
		//  Return file exception value, if the function didn't fail, this
		//  would be FALSE.
	}

	return nRet;
}

int CMaze::Write( PVOID pBuf, int nSize )
{
	int nRet = FALSE;

	if( GetFile() )
	{
		//  Catch any file exception if the write function fails.
		TRY
		{
			GetFile()->Write( pBuf, nSize );
		}
		CATCH( CFileException, cFileException )
		{
			nRet = cFileException->m_cause;
		}
		END_CATCH
		//  Return file exception value, if the function didn't fail, this
		//  would be FALSE.
	}
	return nRet;
}

BOOL CMaze::ValidMaze( void )
{
	BOOL bRet = FALSE;

	if( m_hgblMaze )
		bRet = TRUE;

	return bRet;
}

BOOL CMaze::LockMaze( void )
{
	BOOL bRet = FALSE;

	if( !m_lpCell )
	{
		m_lpCell = (CELL*)GlobalLock( m_hgblMaze );
		if( m_lpCell )
			bRet = TRUE;
	}
	else
		bRet = TRUE;

	return bRet;
}

void CMaze::UnlockMaze( void )
{
	if( m_lpCell )
	{
		GlobalUnlock( m_hgblMaze );
		m_lpCell = NULL;
	}
}
