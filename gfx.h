/***************************************************************************
                          gfx.h  -  description
                             -------------------
    begin                : Thu Nov 12 2026
    copyright            : (C) 2001-2026 by Paul S.J.Millard
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

/*
	Drawing helpers and sprite storage for Allegro 5.

	All drawing goes to the current Allegro 5 target bitmap (normally the
	game's back buffer).  Coordinates follow the Allegro 4 conventions the
	game was written with: rectangles are inclusive of both corners and
	text is positioned by its top left (or top centre) corner.
*/

#ifndef _GFX_H_
#define _GFX_H_

#include <allegro5/allegro.h>

//	Sprites converted from the old cylu.dat (see tools/extract_dat.py).
//	The manifest maps the CYLU_* numbers in cyluobjs.h to PNG files.
bool LoadSpriteData( const char *szManifest, char *szError, int nErrorSize );
void FreeSpriteData( void );
ALLEGRO_BITMAP *GetSprite( int nIndex );

int SpriteWidth( ALLEGRO_BITMAP *pSprite );
int SpriteHeight( ALLEGRO_BITMAP *pSprite );

//	Sprites (transparency comes from the PNG alpha channel)
void DrawSprite( ALLEGRO_BITMAP *pSprite, int nX, int nY );
void DrawLitSprite( ALLEGRO_BITMAP *pSprite, int nX, int nY, int nShade );	//	nShade 0..255 darkens towards black
void RotateSprite( ALLEGRO_BITMAP *pSprite, int nX, int nY, int nAngle );		//	nAngle in Allegro 4 units (256 = full turn)

//	Primitives
void RectFill( int nX1, int nY1, int nX2, int nY2, ALLEGRO_COLOR Color );
void Rect( int nX1, int nY1, int nX2, int nY2, ALLEGRO_COLOR Color );
void RectFillTrans( int nX1, int nY1, int nX2, int nY2, ALLEGRO_COLOR Color, int nAlpha );

//	Text using the built in 8x8 font
void GfxText( const char *szText, int nX, int nY, ALLEGRO_COLOR Color );
void GfxTextCentre( const char *szText, int nX, int nY, ALLEGRO_COLOR Color );
void GfxTextBg( const char *szText, int nX, int nY, ALLEGRO_COLOR Color, ALLEGRO_COLOR Background );
void GfxTextf( int nX, int nY, ALLEGRO_COLOR Color, const char *szFormat, ... );
void GfxTextfCentre( int nX, int nY, ALLEGRO_COLOR Color, const char *szFormat, ... );

//	Copy a whole bitmap onto another, leaving the target unchanged afterwards
void CopyBitmap( ALLEGRO_BITMAP *pDest, ALLEGRO_BITMAP *pSource );

#endif
