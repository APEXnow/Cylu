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
#include <io.h>
#ifdef _DOS
#include <unistd.h>
#endif

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

int CMaze::Allocate( int nWidth, int nHeight )
{
	int bRet = false;

	long lMemRequired = sizeof( CELL ) * ( nWidth * nHeight );

	Deallocate();

	m_lpCell = (CELL*)malloc( lMemRequired );
	if( m_lpCell )
	{
		m_nWidth = nWidth;
		m_nHeight = nHeight;
		bRet = true;
	}

	return bRet;
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

int CMaze::GetCell( int nXPos, int nYPos, int nFlag )
{
	int nRet = 0;
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

int CMaze::LoadMaze( char *lpszMazeName, void *pExtraData, int nSize )
{
	int bRet = false;

	int nFileDescriptor = FILE_ID;
	int nWidth;
	int nHeight;
	int nLoopx;
	int nLoopy;
	int nBuff;
	FILE *pFileID;

	if( _access( lpszMazeName, 0 )!=0 )
		goto function_exit;

	Deallocate();

	pFileID = fopen( lpszMazeName, "rb" );

	if( pFileID )
	{
		fread( (int*)&nFileDescriptor, sizeof( int ), 1, pFileID );
		fread( (int*)&nWidth, sizeof( int ), 1, pFileID );
		fread( (int*)&nHeight, sizeof( int ), 1, pFileID );

		if( nFileDescriptor!=FILE_ID )
		{
			printf( "Not a CLD descriptor\n" );
			return bRet;
		}
		else
		{
			if( Allocate( nWidth, nHeight ) )
			{
				for( nLoopy=0; nLoopy<nHeight; nLoopy++ )
				{
					for( nLoopx=0; nLoopx<nWidth; nLoopx++ )
					{
						fread( (void*)&nBuff, sizeof( int ), 1, pFileID );
						SetCell( nLoopx, nLoopy, CELL_OBJECT, nBuff );

						fread( (void*)&nBuff, sizeof( int ), 1, pFileID );
						SetCell( nLoopx, nLoopy, CELL_ATTRIBUTE, nBuff );
					}
				}
			}
		}

		//	Check if there is extra data to be loaded
		if( pExtraData )
			fread( pExtraData, nSize, 1, pFileID );

		bRet = true;

		fclose( pFileID );
		pFileID = 0;
	}
	else
		printf( "Failed to load CLD maze file\n" );

function_exit:

	return bRet;
}

int CMaze::SaveMaze( char *lpszMazeName, void *pExtraData, int nSize )
{
	int bRet = false;

	int nFileDescriptor = FILE_ID;
	int nLoopx;
	int nLoopy;
	int nBuff;
	FILE *pFileID;

	pFileID = fopen( lpszMazeName, "wb" );

	if( pFileID )
	{
		fwrite( (int*)&nFileDescriptor, sizeof( int ), 1, pFileID );
		fwrite( (int*)&m_nWidth, sizeof( int ), 1, pFileID );
		fwrite( (int*)&m_nHeight, sizeof( int ), 1, pFileID );

		for( nLoopy=0; nLoopy<m_nHeight; nLoopy++ )
		{
			for( nLoopx=0; nLoopx<m_nWidth; nLoopx++ )
			{
				nBuff = GetCell( nLoopx, nLoopy, CELL_OBJECT );
				fwrite( (void*)&nBuff, sizeof( int ), 1, pFileID );

				nBuff = GetCell( nLoopx, nLoopy, CELL_ATTRIBUTE );
				fwrite( (void*)&nBuff, sizeof( int ), 1, pFileID );
			}
		}

		//	Check if extra data is needed saving
		if(	pExtraData )
			fwrite( pExtraData, nSize, 1, pFileID );

		fclose( pFileID );
		pFileID = 0;

		bRet = true;
	}

	return bRet;
}

int CMaze::ValidMaze( void )
{
	int bRet = false;

	if( m_lpCell )
		bRet = true;

	return bRet;
}

int CMaze::AcquireFromDataFile( DATAFILE *pFile, int nEntry )
{
	int bRet = false;

	int nFileDescriptor = FILE_ID;
	int nWidth;
	int nHeight;
	int nLoopx;
	int nLoopy;
	int *pBuff;

	Deallocate();

	if( pFile )
	{
		pBuff = (int*)pFile[ nEntry ].dat;

		nFileDescriptor = *pBuff++;
		nWidth = *pBuff++;
		nHeight = *pBuff++;

		if( nFileDescriptor!=FILE_ID )
		{
			printf( "Not a CLD descriptor\n" );
			return bRet;
		}
		else
		{
			if( Allocate( nWidth, nHeight ) )
			{
				for( nLoopy=0; nLoopy<nHeight; nLoopy++ )
				{
					for( nLoopx=0; nLoopx<nWidth; nLoopx++ )
					{
						SetCell( nLoopx, nLoopy, CELL_OBJECT, *pBuff++ );
						SetCell( nLoopx, nLoopy, CELL_ATTRIBUTE, *pBuff++ );
					}
				}
			}
		}

		bRet = true;
	}
	else
		printf( "Failed to load CLD maze file\n" );

	return bRet;
}
