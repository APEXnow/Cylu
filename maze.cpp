/***************************************************************************
                          maze.cpp  -  description
                             -------------------
    begin                : Sun Nov 18 2001
    copyright            : (C) 2001 by Paul S.J.Millard
    email                : apex@apexnow.co.uk
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "maze.h"

CMaze::CMaze()
{
	InitialiseMembers();
}

CMaze::CMaze( int nWidth, int nHeight )
{
	InitialiseMembers();
	Allocate( nWidth, nHeight );
}

CMaze::CMaze( const CMaze &Other )
{
	InitialiseMembers();
	CopyFrom( Other );
}

CMaze& CMaze::operator=( const CMaze &Other )
{
	if( this != &Other )
		CopyFrom( Other );

	return *this;
}

CMaze::~CMaze()
{
	Deallocate();
}

void CMaze::InitialiseMembers( void )
{
	m_lpCell = 0;
	m_nWidth = 0;
	m_nHeight = 0;
}

void CMaze::Deallocate( void )
{
	if( m_lpCell )
	{
		free( m_lpCell );
		InitialiseMembers();
	}
}

/*
	Function:		Allocate
	Description:	Allocates a maze of the given size with every cell set
					to floor (object 0, attribute 0)
*/
int CMaze::Allocate( int nWidth, int nHeight )
{
	int bRet = false;

	Deallocate();

	if( nWidth < 1 || nWidth > MAX_WIDTH || nHeight < 1 || nHeight > MAX_HEIGHT )
		return bRet;

	m_lpCell = (CELL*)calloc( (size_t)nWidth * (size_t)nHeight, sizeof( CELL ) );
	if( m_lpCell )
	{
		m_nWidth = nWidth;
		m_nHeight = nHeight;
		bRet = true;
	}

	return bRet;
}

/*
	Function:		CopyFrom
	Description:	Makes this maze an exact copy of another
*/
int CMaze::CopyFrom( const CMaze &Other )
{
	if( !Other.m_lpCell )
	{
		Deallocate();
		return true;
	}

	if( !Allocate( Other.m_nWidth, Other.m_nHeight ) )
		return false;

	memcpy( m_lpCell, Other.m_lpCell, sizeof( CELL ) * (size_t)m_nWidth * (size_t)m_nHeight );

	return true;
}

int CMaze::CreateMaze( int nWidth, int nHeight )
{
	int bRet = Allocate( nWidth, nHeight );
	return bRet;
}

void CMaze::DestroyMaze( void )
{
	Deallocate();
}

int CMaze::SetCell( int nXPos, int nYPos, int nFlag, int nValue )
{
	int bRet = false;
	CELL *lpCell;

	if( m_lpCell )
	{
		if( (nXPos>=0) && (nXPos<m_nWidth) &&
			(nYPos>=0) && (nYPos<m_nHeight) )
		{
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
			bRet = true;
		}
	}

	return bRet;
}

int CMaze::GetCell( int nXPos, int nYPos, int nFlag ) const
{
	int nRet = 0;
	const CELL *lpCell;

	if( m_lpCell )
	{
		if( (nXPos>=0) && (nXPos<m_nWidth) &&
			(nYPos>=0) && (nYPos<m_nHeight) )
		{
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

void CMaze::GetMazeSize( int *lpnWidth, int *lpnHeight ) const
{
	*lpnWidth = m_nWidth;
	*lpnHeight = m_nHeight;
}

/*
	Function:		LoadMaze
	Inputs:			lpszMazeName		- File name
					pExtraData			- Optional buffer for data stored
										  after the maze (saved games)
					nSize				- Size of pExtraData
	Output:			true on success

	Description:	Loads a .CLD level.  The format, unchanged since 1996, is
					three ints (FILE_ID, width, height) followed by an
					object and attribute int for each cell, row by row.
*/
int CMaze::LoadMaze( const char *lpszMazeName, void *pExtraData, int nSize )
{
	int bRet = false;
	FILE *pFileID;

	pFileID = fopen( lpszMazeName, "rb" );
	if( !pFileID )
	{
		printf( "Failed to load CLD maze file\n" );
		return bRet;
	}

	bRet = ReadMaze( pFileID, pExtraData, nSize );

	fclose( pFileID );

	return bRet;
}

/*
	Function:		ReadMaze
	Description:	Reads a level from an open file (see LoadMaze).  The maze
					is left unchanged if the file is not a valid level.
*/
int CMaze::ReadMaze( FILE *pFileID, void *pExtraData, int nSize )
{
	int nHeader[3];
	int nCount;
	CMaze Loaded;

	if( !pFileID )
		return false;

	if( fread( nHeader, sizeof( int ), 3, pFileID ) != 3 )
		return false;

	if( nHeader[0] != FILE_ID )
	{
		printf( "Not a CLD descriptor\n" );
		return false;
	}

	if( !Loaded.Allocate( nHeader[1], nHeader[2] ) )
		return false;

	//	CELL is two ints (object, attribute), matching the file layout
	nCount = nHeader[1] * nHeader[2];
	if( fread( Loaded.m_lpCell, sizeof( CELL ), (size_t)nCount, pFileID ) != (size_t)nCount )
		return false;

	//	Check if there is extra data to be loaded
	if( pExtraData && nSize > 0 )
	{
		if( fread( pExtraData, (size_t)nSize, 1, pFileID ) != 1 )
			return false;
	}

	return CopyFrom( Loaded );
}

int CMaze::SaveMaze( const char *lpszMazeName, void *pExtraData, int nSize )
{
	int bRet = false;
	FILE *pFileID;

	pFileID = fopen( lpszMazeName, "wb" );

	if( pFileID )
	{
		bRet = WriteMaze( pFileID, pExtraData, nSize );

		if( fclose( pFileID ) != 0 )
			bRet = false;
	}

	return bRet;
}

/*
	Function:		WriteMaze
	Description:	Writes the level to an open file (see LoadMaze for the format)
*/
int CMaze::WriteMaze( FILE *pFileID, void *pExtraData, int nSize ) const
{
	int nHeader[3];
	size_t nCount;

	if( !pFileID || !m_lpCell )
		return false;

	nHeader[0] = FILE_ID;
	nHeader[1] = m_nWidth;
	nHeader[2] = m_nHeight;

	if( fwrite( nHeader, sizeof( int ), 3, pFileID ) != 3 )
		return false;

	nCount = (size_t)m_nWidth * (size_t)m_nHeight;
	if( fwrite( m_lpCell, sizeof( CELL ), nCount, pFileID ) != nCount )
		return false;

	//	Check if extra data is needed saving
	if(	pExtraData && nSize > 0 )
	{
		if( fwrite( pExtraData, (size_t)nSize, 1, pFileID ) != 1 )
			return false;
	}

	return true;
}

int CMaze::ValidMaze( void ) const
{
	int bRet = false;

	if( m_lpCell )
		bRet = true;

	return bRet;
}
