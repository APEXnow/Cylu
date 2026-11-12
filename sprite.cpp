/***************************************************************************
                          sprite.cpp  -  description
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
#include <memory.h>
#include <allegro.h>

#include "cylu.h"
#include "sprite.h"


/*
	Function:		CSprite
	Inputs:			None
	Output:			None

	Description:	Constructor
*/
CSprite::CSprite( void )
{
	m_nFrames = 0;
	m_pnFrameList = NULL;
	m_nCurFrame = 0;
	m_nSpeed = 0;
	m_nTimer = 0;
}

/*
	Function:		~CSprite
	Inputs:			None
	Output:			None

	Description:	Destructor
*/
CSprite::~CSprite( void )
{
	if( m_pnFrameList )
		free( m_pnFrameList );
}

/*
	Function:		SetFrameList
	Inputs:			pFrameList		-	Pointer to an array of frame numbers
					nFrames			-	Number of frames in list
	Output:			True			-	If successful
					False			-	On failure

	Description:	Sets up the frame list for this sprite
*/
BOOL CSprite::SetFrameList( int *pFrameList, int nFrames )
{
	BOOL bRet = FALSE;

	m_pnFrameList = (int*)malloc( sizeof( int ) * nFrames );

	if( m_pnFrameList )
	{
		memcpy( m_pnFrameList, pFrameList, sizeof( int ) * nFrames );
		m_nFrames = nFrames;
		bRet = TRUE;
	}

	return bRet;
}

/*
	Function:		SetSpeed
	Inputs:			nSpeed			-	Speed of sprite animation
	Output:			None

	Description:	Sets the speed of the sprite animation
*/
void CSprite::SetSpeed( int nSpeed )
{
	m_nSpeed = nSpeed;
	m_nTimer = 0;
	m_nCurFrame = 0;
}

/*
	Function:		Advance
	Inputs:			nTimer			-	Current vsync count provided by
										Allegro
	Output:			None

	Description:	This is required to advance the sprite animation which
					is determined by the speed of the animation according
					to what the nTimer value is.
*/
void CSprite::Advance( int nTimer )
{
	if( m_nFrames<2 )				//	Static animation, do nothing!
		goto function_exit;

	if( !m_nTimer )
	{
		m_nTimer = nTimer;
		goto function_exit;
	}

	//	Now calculate frame advancing based on current timer and speed
	if( ( nTimer - m_nTimer ) >= m_nSpeed )
	{
		m_nCurFrame++;
		if( m_nCurFrame>=m_nFrames ) m_nCurFrame=0;

		m_nTimer = nTimer;
	}

function_exit:

	return;
}

/*
	Function:		GetFrame
	Inputs:			None
	Output:			Frame number in current animation

	Description:	This function returns the current frame number to
					be used for display
*/
int CSprite::GetFrame( void )
{
	return m_pnFrameList[m_nCurFrame];
}
