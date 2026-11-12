/***************************************************************************
                          anim.h  -  description
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

#ifndef _ANIM_H_
#define _ANIM_H_

#include "./cylu.h"
#include "./sprite.h"

class CAnim
{
private:
	//	Data members
	int m_nSprites;					//	Number of sprites memory
	CSprite **m_pSprites;			//	Array of sprite objects

public:
	//	Constructors & destructor
	CAnim();
	~CAnim();

	//	Member functions
	BOOL AddSprite( int *pSpriteList, int nFrames, int nSpeed );
	BOOL Free( void );

	//	Helper functions
	BOOL Advance( int nTimer );
	int GetFrame( int nSprite );
};

#endif
