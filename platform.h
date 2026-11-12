/***************************************************************************
                          platform.h  -  description
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
	Allegro 5 platform layer.

	The game renders everything into a fixed 640x480 back buffer exactly as
	it did under Allegro 4.  This module owns the window, scales the back
	buffer to whatever size the window is, runs the 70Hz game clock (the
	old retrace_count), and keeps the keyboard state / key buffer that the
	game polls with PlatformKeyDown / PlatformKeyPressed / PlatformReadKey.
*/

#ifndef _PLATFORM_H_
#define _PLATFORM_H_

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>

#define GAME_TICKS_PER_SECOND	70			//	Same rate as Allegro 4's retrace_count

//	Start up Allegro, its add-ons, the window and the game clock.
//	Returns false and fills szError on failure.
bool PlatformInit( int nWidth, int nHeight, bool bFullscreen, bool bVsync, char *szError, int nErrorSize );
void PlatformShutdown( void );

//	Process pending window, keyboard and timer events without waiting
void PlatformPumpEvents( void );

//	Wait for the next game tick (if vsync is on), then scale the back buffer
//	to the window and flip.  Leaves the back buffer as the drawing target.
void PlatformPresent( ALLEGRO_BITMAP *pBackBuffer );

//	Game clock in 1/70th second ticks since start up
int PlatformTicks( void );

//	True once the user has closed the window
bool PlatformQuitRequested( void );

//	Keyboard
bool PlatformKeyDown( int nKeyCode );		//	Held state, e.g. ALLEGRO_KEY_UP
bool PlatformKeyPressed( void );			//	Anything in the key buffer?
int  PlatformReadKey( void );				//	Next buffered key code, 0 if none
void PlatformClearKeyBuf( void );

//	Toggle between windowed and full screen (also bound to Alt+Enter and F11)
void PlatformToggleFullscreen( void );

//	Show an error in a native message box
void PlatformErrorBox( const char *szTitle, const char *szText );

//	The 8x8 built in font, equivalent of Allegro 4's global 'font'
ALLEGRO_FONT *PlatformFont( void );

#endif
