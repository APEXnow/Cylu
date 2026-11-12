/***************************************************************************
                          platform.cpp  -  description
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
#include <string.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_native_dialog.h>

#include "./platform.h"

#define KEY_BUFFER_SIZE			64

static ALLEGRO_DISPLAY *s_pDisplay = NULL;
static ALLEGRO_EVENT_QUEUE *s_pQueue = NULL;
static ALLEGRO_TIMER *s_pTimer = NULL;
static ALLEGRO_FONT *s_pFont = NULL;

static int s_nWidth = 640;					//	Virtual (game) resolution
static int s_nHeight = 480;
static bool s_bVsync = true;

static volatile int s_nTicks = 0;			//	Game clock
static int s_nLastPresented = 0;
static bool s_bQuit = false;

static bool s_bKeyDown[ ALLEGRO_KEY_MAX ];
static int s_nKeyBuf[ KEY_BUFFER_SIZE ];
static int s_nKeyHead = 0;
static int s_nKeyTail = 0;

/*
	Function:		PushKey
	Description:	Adds a key code to the circular key buffer
*/
static void PushKey( int nKeyCode )
{
	int nNext = ( s_nKeyTail + 1 ) % KEY_BUFFER_SIZE;

	if( nNext != s_nKeyHead )		//	Drop keys if the buffer is full
	{
		s_nKeyBuf[ s_nKeyTail ] = nKeyCode;
		s_nKeyTail = nNext;
	}
}

/*
	Function:		HandleEvent
	Description:	Updates clock, keyboard and window state for one event
*/
static void HandleEvent( const ALLEGRO_EVENT &Event )
{
	switch( Event.type )
	{
		case ALLEGRO_EVENT_TIMER:
			s_nTicks++;
			break;

		case ALLEGRO_EVENT_KEY_DOWN:
			if( Event.keyboard.keycode > 0 && Event.keyboard.keycode < ALLEGRO_KEY_MAX )
				s_bKeyDown[ Event.keyboard.keycode ] = true;
			break;

		case ALLEGRO_EVENT_KEY_UP:
			if( Event.keyboard.keycode > 0 && Event.keyboard.keycode < ALLEGRO_KEY_MAX )
				s_bKeyDown[ Event.keyboard.keycode ] = false;
			break;

		case ALLEGRO_EVENT_KEY_CHAR:
			//	KEY_CHAR includes auto repeat, which matches Allegro 4's readkey()
			if( ( Event.keyboard.keycode == ALLEGRO_KEY_ENTER && ( Event.keyboard.modifiers & ALLEGRO_KEYMOD_ALT ) ) ||
				Event.keyboard.keycode == ALLEGRO_KEY_F11 )
			{
				if( !Event.keyboard.repeat )
					PlatformToggleFullscreen();
				break;
			}

			if( Event.keyboard.keycode == ALLEGRO_KEY_PAD_ENTER )
				PushKey( ALLEGRO_KEY_ENTER );
			else
				PushKey( Event.keyboard.keycode );
			break;

		case ALLEGRO_EVENT_DISPLAY_CLOSE:
			s_bQuit = true;
			break;

		case ALLEGRO_EVENT_DISPLAY_RESIZE:
			al_acknowledge_resize( Event.display.source );
			break;

		case ALLEGRO_EVENT_DISPLAY_SWITCH_OUT:
			//	Avoid keys getting stuck down when focus is lost
			memset( s_bKeyDown, 0, sizeof( s_bKeyDown ) );
			break;
	}
}

/*
	Function:		ChooseWindowScale
	Description:	Largest whole number scale of the game screen that fits
					comfortably on the primary monitor
*/
static int ChooseWindowScale( void )
{
	ALLEGRO_MONITOR_INFO Info;
	int nScale = 1;

	if( al_get_monitor_info( 0, &Info ) )
	{
		int nMonW = Info.x2 - Info.x1;
		int nMonH = Info.y2 - Info.y1;

		while( s_nWidth * ( nScale + 1 ) <= nMonW * 9 / 10 &&
			   s_nHeight * ( nScale + 1 ) <= nMonH * 9 / 10 )
			nScale++;
	}

	return nScale;
}

bool PlatformInit( int nWidth, int nHeight, bool bFullscreen, bool bVsync, char *szError, int nErrorSize )
{
	int nScale;

	s_nWidth = nWidth;
	s_nHeight = nHeight;
	s_bVsync = bVsync;

	memset( s_bKeyDown, 0, sizeof( s_bKeyDown ) );

	if( !al_init() )
	{
		snprintf( szError, nErrorSize, "al_init failed" );
		return false;
	}

	al_set_org_name( "Apex" );
	al_set_app_name( "Cylu" );

	if( !al_init_image_addon() || !al_init_font_addon() || !al_init_primitives_addon() )
	{
		snprintf( szError, nErrorSize, "Failed to initialise Allegro add-ons" );
		return false;
	}

	al_init_native_dialog_addon();		//	Optional, only used for error boxes

	if( !al_install_keyboard() )
	{
		snprintf( szError, nErrorSize, "Failed to install keyboard" );
		return false;
	}

	//	Run game data files from the folder the executable lives in
	ALLEGRO_PATH *pPath = al_get_standard_path( ALLEGRO_RESOURCES_PATH );
	if( pPath )
	{
		al_change_directory( al_path_cstr( pPath, ALLEGRO_NATIVE_PATH_SEP ) );
		al_destroy_path( pPath );
	}

	//	Frame pacing is done with the 70Hz game clock, so display vsync is off
	al_set_new_display_option( ALLEGRO_VSYNC, 2, ALLEGRO_SUGGEST );
	al_set_new_display_flags( ALLEGRO_WINDOWED | ALLEGRO_RESIZABLE );

	nScale = ChooseWindowScale();
	s_pDisplay = al_create_display( s_nWidth * nScale, s_nHeight * nScale );
	if( !s_pDisplay )
	{
		snprintf( szError, nErrorSize, "Unable to create a %dx%d window", s_nWidth * nScale, s_nHeight * nScale );
		return false;
	}

	al_set_window_title( s_pDisplay, "Cylu" );
	al_set_window_constraints( s_pDisplay, s_nWidth / 2, s_nHeight / 2, 0, 0 );
	al_apply_window_constraints( s_pDisplay, true );

	if( bFullscreen )
		PlatformToggleFullscreen();

	s_pFont = al_create_builtin_font();
	if( !s_pFont )
	{
		snprintf( szError, nErrorSize, "Failed to create built in font" );
		return false;
	}

	s_pTimer = al_create_timer( 1.0 / GAME_TICKS_PER_SECOND );
	s_pQueue = al_create_event_queue();
	if( !s_pTimer || !s_pQueue )
	{
		snprintf( szError, nErrorSize, "Failed to create timer" );
		return false;
	}

	al_register_event_source( s_pQueue, al_get_display_event_source( s_pDisplay ) );
	al_register_event_source( s_pQueue, al_get_keyboard_event_source() );
	al_register_event_source( s_pQueue, al_get_timer_event_source( s_pTimer ) );
	al_start_timer( s_pTimer );

	return true;
}

void PlatformShutdown( void )
{
	if( s_pTimer )
		al_destroy_timer( s_pTimer );
	if( s_pQueue )
		al_destroy_event_queue( s_pQueue );
	if( s_pFont )
		al_destroy_font( s_pFont );
	if( s_pDisplay )
		al_destroy_display( s_pDisplay );

	s_pTimer = NULL;
	s_pQueue = NULL;
	s_pFont = NULL;
	s_pDisplay = NULL;
}

void PlatformPumpEvents( void )
{
	ALLEGRO_EVENT Event;

	if( !s_pQueue )
		return;

	while( al_get_next_event( s_pQueue, &Event ) )
		HandleEvent( Event );
}

void PlatformPresent( ALLEGRO_BITMAP *pBackBuffer )
{
	ALLEGRO_EVENT Event;
	int nDispW, nDispH;
	float fScale, fW, fH;

	PlatformPumpEvents();

	//	Equivalent of vsync(): hold the frame until the next game tick
	if( s_bVsync )
	{
		while( s_nTicks == s_nLastPresented && !s_bQuit )
		{
			al_wait_for_event( s_pQueue, &Event );
			HandleEvent( Event );
		}
	}
	s_nLastPresented = s_nTicks;

	//	Scale the game screen to the window, keeping the 4:3 aspect
	al_set_target_backbuffer( s_pDisplay );
	al_clear_to_color( al_map_rgb( 0, 0, 0 ) );

	nDispW = al_get_display_width( s_pDisplay );
	nDispH = al_get_display_height( s_pDisplay );

	fScale = (float)nDispW / (float)s_nWidth;
	if( (float)nDispH / (float)s_nHeight < fScale )
		fScale = (float)nDispH / (float)s_nHeight;

	fW = s_nWidth * fScale;
	fH = s_nHeight * fScale;

	al_draw_scaled_bitmap( pBackBuffer, 0, 0, (float)s_nWidth, (float)s_nHeight,
		( nDispW - fW ) / 2, ( nDispH - fH ) / 2, fW, fH, 0 );

	al_flip_display();

	al_set_target_bitmap( pBackBuffer );
}

int PlatformTicks( void )
{
	return s_nTicks;
}

bool PlatformQuitRequested( void )
{
	return s_bQuit;
}

bool PlatformKeyDown( int nKeyCode )
{
	if( nKeyCode <= 0 || nKeyCode >= ALLEGRO_KEY_MAX )
		return false;

	return s_bKeyDown[ nKeyCode ];
}

bool PlatformKeyPressed( void )
{
	PlatformPumpEvents();
	return s_nKeyHead != s_nKeyTail;
}

int PlatformReadKey( void )
{
	int nKey = 0;

	PlatformPumpEvents();

	if( s_nKeyHead != s_nKeyTail )
	{
		nKey = s_nKeyBuf[ s_nKeyHead ];
		s_nKeyHead = ( s_nKeyHead + 1 ) % KEY_BUFFER_SIZE;
	}

	return nKey;
}

void PlatformClearKeyBuf( void )
{
	PlatformPumpEvents();
	s_nKeyHead = s_nKeyTail = 0;
}

void PlatformToggleFullscreen( void )
{
	if( !s_pDisplay )
		return;

	bool bFullscreen = ( al_get_display_flags( s_pDisplay ) & ALLEGRO_FULLSCREEN_WINDOW ) != 0;
	al_set_display_flag( s_pDisplay, ALLEGRO_FULLSCREEN_WINDOW, !bFullscreen );
}

void PlatformErrorBox( const char *szTitle, const char *szText )
{
	if( al_is_native_dialog_addon_initialized() )
		al_show_native_message_box( s_pDisplay, szTitle, szTitle, szText, NULL, ALLEGRO_MESSAGEBOX_ERROR );
	else
		fprintf( stderr, "%s: %s\n", szTitle, szText );
}

ALLEGRO_FONT *PlatformFont( void )
{
	return s_pFont;
}
