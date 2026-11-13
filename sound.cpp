/***************************************************************************
                          sound.cpp  -  description
                             -------------------
    begin                : Fri Nov 13 2026
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

#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

#include "./sound.h"

#define SOUND_FOLDER			"data/sounds/"
#define SOUND_VOICES			16			//	Effects that can play at once

//	File names, in SoundEffect order
static const char *s_szFiles[ SND_COUNT ] =
{
	"step.wav",
	"bump.wav",
	"pickup.wav",
	"drop.wav",
	"deposit.wav",
	"forcefield_off.wav",
	"teleport.wav",
	"refuel.wav",
	"fuel_low.wav",
	"menu_move.wav",
	"menu_select.wav",
	"whoosh.wav",
	"game_over.wav",
	"win.wav"
};

static ALLEGRO_SAMPLE *s_pSamples[ SND_COUNT ];
static bool s_bInstalled = false;
static bool s_bMuted = false;

bool SoundInit( char *szError, int nErrorSize )
{
	char szPath[ 256 ];
	int i;

	for( i=0; i<SND_COUNT; i++ )
		s_pSamples[i] = NULL;

	if( !al_install_audio() )
	{
		snprintf( szError, nErrorSize, "No audio device" );
		return false;
	}

	if( !al_init_acodec_addon() || !al_reserve_samples( SOUND_VOICES ) )
	{
		al_uninstall_audio();
		snprintf( szError, nErrorSize, "Unable to start the audio mixer" );
		return false;
	}

	s_bInstalled = true;

	for( i=0; i<SND_COUNT; i++ )
	{
		snprintf( szPath, sizeof( szPath ), "%s%s", SOUND_FOLDER, s_szFiles[i] );
		s_pSamples[i] = al_load_sample( szPath );		//	NULL if missing: that effect is silent
	}

	return true;
}

void SoundShutdown( void )
{
	int i;

	if( !s_bInstalled )
		return;

	al_stop_samples();

	for( i=0; i<SND_COUNT; i++ )
	{
		if( s_pSamples[i] )
			al_destroy_sample( s_pSamples[i] );
		s_pSamples[i] = NULL;
	}

	al_uninstall_audio();
	s_bInstalled = false;
}

void SoundPlay( int nEffect )
{
	if( !s_bInstalled || s_bMuted || nEffect < 0 || nEffect >= SND_COUNT || !s_pSamples[ nEffect ] )
		return;

	al_play_sample( s_pSamples[ nEffect ], 1.0f, 0.0f, 1.0f, ALLEGRO_PLAYMODE_ONCE, NULL );
}

void SoundToggleMute( void )
{
	s_bMuted = !s_bMuted;

	if( s_bMuted && s_bInstalled )
		al_stop_samples();
}

bool SoundMuted( void )
{
	return s_bMuted;
}
