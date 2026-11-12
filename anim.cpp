/***************************************************************************
                          anim.cpp  -  description
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

#include "anim.h"

/*
	Function:		CAnim
	Inputs:			None
	Output:			None

	Description:	Constructor
*/
CAnim::CAnim()
{
	m_nSprites = 0;
	m_pSprites = NULL;
}

/*
	Function:		~CAnim
	Inputs:			None
	Output:			None

	Description:	Destructor
*/
CAnim::~CAnim()
{
	if( m_pSprites )
		Free();
}

/*
	Function:		AddSprite
	Inputs:			pSpriteList			- List of sprite frames to add
					nFrames				- Number of frames in sprite list
	Output:			True				- If successful
					False				- On failure

	Description:	Adds the sprite list to the animation object
					Each sprite list instantiates a new CSprite object
					into the animation array
*/
BOOL CAnim::AddSprite( int *pSpriteList, int nFrames, int nSpeed )
{
	BOOL bRet = FALSE;
	CSprite *pSprite;
	CSprite **pNewList;

	//	Allocate memory for array of sprites
	if( !m_pSprites )
	{
		m_pSprites = (CSprite**)malloc( sizeof( CSprite* ) );

		if( !m_pSprites )
			goto function_exit;

		m_nSprites = 1;
	}
	else
	{
		pNewList = (CSprite**)realloc( m_pSprites, sizeof( CSprite* )*(m_nSprites+1) );

		if( !pNewList )
			goto function_exit;

		m_pSprites = pNewList;
		m_nSprites++;
	}

	m_pSprites[m_nSprites-1] = new CSprite;
	pSprite = m_pSprites[m_nSprites-1];

	pSprite->SetFrameList( pSpriteList, nFrames );
	pSprite->SetSpeed( nSpeed );

	bRet = TRUE;

function_exit:

	return bRet;
}

/*
	Function:		Free
	Inputs:			None
	Output:			True				- If successful
					False				- On failure

	Description:	Frees all memory of sprite list
*/
BOOL CAnim::Free( void )
{
	BOOL bRet = FALSE;
	int i;
	CSprite *pSprite;

	if( !m_pSprites )
		goto function_exit;

	for( i=0; i<m_nSprites; i++ )
	{
		pSprite = m_pSprites[i];
		delete pSprite;
	}

	//	Deallocate memory
	free( m_pSprites );
	bRet = TRUE;

function_exit:

	return bRet;
}

/*
	Function:		Advance
	Inputs:			nTimer				- Vsync timer obtained from Allegro
	Output:			True				- If successful
					False				- On failure

	Description:	Scans through the sprite list and advances the frames
					according to timer
*/
BOOL CAnim::Advance( int nTimer )
{
	BOOL bRet = FALSE;
	int i;
	CSprite *pSprite;

	if( !m_pSprites )
		goto function_exit;

	for( i=0; i<m_nSprites; i++ )
	{
		pSprite = m_pSprites[i];
		pSprite->Advance( nTimer );
	}

	bRet = TRUE;

function_exit:

	return bRet;
}

/*
	Function:		Free
	Inputs:			None
	Output:			True				- If successful
					False				- On failure

	Description:	Frees all memory of sprite list
*/
int CAnim::GetFrame( int nSprite )
{
	int nRet = -1;
	CSprite *pSprite;

	if( !m_pSprites )
		goto function_exit;

	if( nSprite>=0 && nSprite< m_nSprites )
	{
		pSprite = m_pSprites[ nSprite ];
		nRet = pSprite->GetFrame();
	}

function_exit:

	return nRet;
}
