/***************************************************************************
                          gfx.cpp  -  description
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

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>

#include "./gfx.h"
#include "./platform.h"

#define MAX_SPRITES				512

static ALLEGRO_BITMAP *s_pSprites[ MAX_SPRITES ];
static int s_nSprites = 0;

/*
	Function:		LoadSpriteData
	Inputs:			szManifest			- Path to manifest.txt
	Output:			true on success

	Description:	Loads every sprite listed in the manifest.  Each line is
					"<index> <NAME> <relative path>", the path being "-" for
					entries that are not images.
*/
bool LoadSpriteData( const char *szManifest, char *szError, int nErrorSize )
{
	char szLine[ 512 ];
	char szName[ 128 ];
	char szFile[ 256 ];
	char szDir[ 256 ];
	char szPath[ 512 ];
	const char *pSlash;
	int nIndex;
	FILE *pFile;

	FreeSpriteData();

	//	Paths in the manifest are relative to the manifest's own folder
	strncpy( szDir, szManifest, sizeof( szDir ) - 1 );
	szDir[ sizeof( szDir ) - 1 ] = '\0';
	pSlash = strrchr( szDir, '/' );
	if( pSlash )
		szDir[ pSlash - szDir + 1 ] = '\0';
	else
		szDir[ 0 ] = '\0';

	pFile = fopen( szManifest, "r" );
	if( !pFile )
	{
		snprintf( szError, nErrorSize, "Cannot open %s", szManifest );
		return false;
	}

	while( fgets( szLine, sizeof( szLine ), pFile ) )
	{
		if( sscanf( szLine, "%d %127s %255s", &nIndex, szName, szFile ) != 3 )
			continue;

		if( nIndex < 0 || nIndex >= MAX_SPRITES )
			continue;

		if( nIndex >= s_nSprites )
			s_nSprites = nIndex + 1;

		if( strstr( szFile, ".png" ) == NULL )
			continue;

		snprintf( szPath, sizeof( szPath ), "%s%s", szDir, szFile );
		s_pSprites[ nIndex ] = al_load_bitmap( szPath );

		if( !s_pSprites[ nIndex ] )
		{
			snprintf( szError, nErrorSize, "Cannot load %s", szPath );
			fclose( pFile );
			return false;
		}
	}

	fclose( pFile );

	if( s_nSprites == 0 )
	{
		snprintf( szError, nErrorSize, "No sprites listed in %s", szManifest );
		return false;
	}

	return true;
}

void FreeSpriteData( void )
{
	int i;

	for( i=0; i<MAX_SPRITES; i++ )
	{
		if( s_pSprites[i] )
			al_destroy_bitmap( s_pSprites[i] );
		s_pSprites[i] = NULL;
	}

	s_nSprites = 0;
}

ALLEGRO_BITMAP *GetSprite( int nIndex )
{
	if( nIndex < 0 || nIndex >= s_nSprites )
		return NULL;

	return s_pSprites[ nIndex ];
}

int SpriteWidth( ALLEGRO_BITMAP *pSprite )
{
	return pSprite ? al_get_bitmap_width( pSprite ) : 0;
}

int SpriteHeight( ALLEGRO_BITMAP *pSprite )
{
	return pSprite ? al_get_bitmap_height( pSprite ) : 0;
}

void DrawSprite( ALLEGRO_BITMAP *pSprite, int nX, int nY )
{
	if( pSprite )
		al_draw_bitmap( pSprite, (float)nX, (float)nY, 0 );
}

/*
	Equivalent of draw_lit_rle_sprite with set_trans_blender( 0,0,0,0 ):
	the sprite is blended towards black by nShade/255
*/
void DrawLitSprite( ALLEGRO_BITMAP *pSprite, int nX, int nY, int nShade )
{
	float fLight;

	if( !pSprite )
		return;

	if( nShade <= 0 )
	{
		al_draw_bitmap( pSprite, (float)nX, (float)nY, 0 );
		return;
	}

	if( nShade > 255 )
		nShade = 255;

	fLight = 1.0f - ( (float)nShade / 255.0f );
	al_draw_tinted_bitmap( pSprite, al_map_rgba_f( fLight, fLight, fLight, 1.0f ), (float)nX, (float)nY, 0 );
}

/*
	Equivalent of rotate_sprite: rotates around the sprite's centre, with
	(nX, nY) being where the unrotated top left corner would be
*/
void RotateSprite( ALLEGRO_BITMAP *pSprite, int nX, int nY, int nAngle )
{
	float fCX, fCY;
	float fRadians;

	if( !pSprite )
		return;

	fCX = al_get_bitmap_width( pSprite ) / 2.0f;
	fCY = al_get_bitmap_height( pSprite ) / 2.0f;
	fRadians = (float)( nAngle % 256 ) * ( 2.0f * (float)ALLEGRO_PI / 256.0f );

	al_draw_rotated_bitmap( pSprite, fCX, fCY, nX + fCX, nY + fCY, fRadians, 0 );
}

void RectFill( int nX1, int nY1, int nX2, int nY2, ALLEGRO_COLOR Color )
{
	int t;

	if( nX2 < nX1 ) { t = nX1; nX1 = nX2; nX2 = t; }
	if( nY2 < nY1 ) { t = nY1; nY1 = nY2; nY2 = t; }

	al_draw_filled_rectangle( (float)nX1, (float)nY1, (float)( nX2 + 1 ), (float)( nY2 + 1 ), Color );
}

void Rect( int nX1, int nY1, int nX2, int nY2, ALLEGRO_COLOR Color )
{
	int t;

	if( nX2 < nX1 ) { t = nX1; nX1 = nX2; nX2 = t; }
	if( nY2 < nY1 ) { t = nY1; nY1 = nY2; nY2 = t; }

	al_draw_rectangle( nX1 + 0.5f, nY1 + 0.5f, nX2 + 0.5f, nY2 + 0.5f, Color, 1.0f );
}

/*
	Translucent filled rectangle, nAlpha 0..255 (Allegro 4 DRAW_MODE_TRANS)
*/
void RectFillTrans( int nX1, int nY1, int nX2, int nY2, ALLEGRO_COLOR Color, int nAlpha )
{
	float fAlpha = (float)nAlpha / 255.0f;

	//	Allegro 5 blends with premultiplied alpha by default
	Color.r *= fAlpha;
	Color.g *= fAlpha;
	Color.b *= fAlpha;
	Color.a = fAlpha;

	RectFill( nX1, nY1, nX2, nY2, Color );
}

void GfxText( const char *szText, int nX, int nY, ALLEGRO_COLOR Color )
{
	al_draw_text( PlatformFont(), Color, (float)nX, (float)nY, ALLEGRO_ALIGN_LEFT, szText );
}

void GfxTextCentre( const char *szText, int nX, int nY, ALLEGRO_COLOR Color )
{
	al_draw_text( PlatformFont(), Color, (float)nX, (float)nY, ALLEGRO_ALIGN_CENTRE, szText );
}

/*
	Text drawn over a solid background (Allegro 4 text_mode( colour ))
*/
void GfxTextBg( const char *szText, int nX, int nY, ALLEGRO_COLOR Color, ALLEGRO_COLOR Background )
{
	int nW = al_get_text_width( PlatformFont(), szText );
	int nH = al_get_font_line_height( PlatformFont() );

	al_draw_filled_rectangle( (float)nX, (float)nY, (float)( nX + nW ), (float)( nY + nH ), Background );
	GfxText( szText, nX, nY, Color );
}

void GfxTextf( int nX, int nY, ALLEGRO_COLOR Color, const char *szFormat, ... )
{
	char szBuf[ 512 ];
	va_list Args;

	va_start( Args, szFormat );
	vsnprintf( szBuf, sizeof( szBuf ), szFormat, Args );
	va_end( Args );

	GfxText( szBuf, nX, nY, Color );
}

void GfxTextfCentre( int nX, int nY, ALLEGRO_COLOR Color, const char *szFormat, ... )
{
	char szBuf[ 512 ];
	va_list Args;

	va_start( Args, szFormat );
	vsnprintf( szBuf, sizeof( szBuf ), szFormat, Args );
	va_end( Args );

	GfxTextCentre( szBuf, nX, nY, Color );
}

void CopyBitmap( ALLEGRO_BITMAP *pDest, ALLEGRO_BITMAP *pSource )
{
	ALLEGRO_BITMAP *pOldTarget = al_get_target_bitmap();

	al_set_target_bitmap( pDest );
	al_set_blender( ALLEGRO_ADD, ALLEGRO_ONE, ALLEGRO_ZERO );		//	Straight copy, like blit()
	al_draw_bitmap( pSource, 0, 0, 0 );
	al_set_blender( ALLEGRO_ADD, ALLEGRO_ONE, ALLEGRO_INVERSE_ALPHA );

	al_set_target_bitmap( pOldTarget );
}
