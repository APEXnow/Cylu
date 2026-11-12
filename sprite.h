/***************************************************************************
                          sprite.h  -  description
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

#ifndef _SPRITE_H_
#define _SPRITE_H_

class CSprite
{
private:

	//	Private data members

	int m_nFrames;					//	Number of frames for this sprite
	int *m_pnFrameList;				//	Pointer to frame list
	int m_nCurFrame;				//	Current frame being displayed
	int m_nSpeed;					//	Speed at which the sprite is animated
	int m_nTimer;					//	Sync timer for frame advancing

public:
	//	Constructors & destructor

	CSprite( void );
	~CSprite( void );

	//	Member functions
	BOOL SetFrameList( int *pFrameList, int nFrames );
	void SetSpeed( int nSpeed );
	void Advance( int nTimer );

	//	Helper functions
	int GetFrame( void );
};

#endif
