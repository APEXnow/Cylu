/***************************************************************************
                          main.cpp  -  description
                             -------------------
    begin                : Sun Nov 18 03:17:45 GMT 2001
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

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <ctype.h>
#include <limits.h>

#include <allegro5/allegro.h>

#include "./platform.h"
#include "./gfx.h"
#include "./sound.h"
#include "./cylu.h"
#include "./sprite.h"
#include "./anim.h"
#include "./cyluobjs.h"
#include "./error.h"

/*
	Define globals used throughout the game
*/

BOOL g_bSound = TRUE;
BOOL g_bRotate = TRUE;
BOOL g_bVsync = TRUE;

#ifndef PATH_MAX
#define PATH_MAX 4096
#endif
char g_szLevelFile[ PATH_MAX ] = { LEVEL_FILE };	//	Level to play (-level overrides)
#ifdef _DEBUG
BOOL g_bFullscreen = FALSE;				//	Start in a window when debugging
#else
BOOL g_bFullscreen = TRUE;
#endif

char szLastError[255] = { "" };

BOOL g_bInitialised = FALSE;			//	Used by initial debug text

CMaze *g_lpCMaze = NULL;				//	Level data

GameState g_enuGameState;				//	Game state
BOOL g_bMenu = FALSE;					//	Toggles main menu on and off
BOOL g_bEndGame = FALSE;				//	For end game sequence
ALLEGRO_BITMAP *g_pBackBuffer = NULL;			//	Rendering buffer
ALLEGRO_BITMAP *g_pCopy = NULL;					//	Copy used for end game sequence
int g_nTimer;							//	Global game timer
CAnim *g_pAnim = NULL;					//	Animation class
int g_nMenuPos = 0;						//	Menu cursor position
BOOL g_bMenuShown = FALSE;

BOOL g_bInstructionsShown = FALSE;
int g_nCurPage = 0;						//	Current page
int g_nTotalPages = 5;					//	Total pages

int g_nExamineTimer = 0;				//	Expiry time to remove examine text
int g_nDroidX = 2;						//	Current Droid X position
int g_nDroidY = 2;						//	Current Droid Y position
int g_nDroidXOrig = 0;					//	Original X droid position
int g_nDroidYOrig = 0;					//	Original Y droid position
int g_nDroidDir = DROID_NORTH;			//	Current droid direction
int g_nDroidDirOrig = DROID_NORTH;		//	Original droid direction
float g_fDroidXStep = 0;				//	Droid movement stepping
float g_fDroidYStep = 0;
float g_fShadeStep = 0;					//	Used by the compile function
BOOL g_bMoving = FALSE;					//	Movement status
int g_nObjects = 0;						//	Number of objects collected
int g_nMaxObjects = 0;					//	Total number of objects to collect
int g_nPaces = 0;						//	Number of paces
#ifndef _LOWFUEL_
int g_nFuel = FUEL_INIT;				//	Current fuel status
#else
int g_nFuel = 20;
#endif

int g_nAction = ACTION_NONE;			//	Action status

POCKET g_Pockets[5];					//	Pocket collections
int g_nPocket = 0;						//	Currently selected pocket

int g_nCurCmd = COMMAND_NONE;			//	Game command status

StatState g_enuStatState;				//	Current status of stats
int g_nStatEnd = 120;					//	Position of stats
int g_nStatPos = 120;					//	Current stats position

int g_nTextY = 2;

#ifdef _DESIGN_MODE_
int g_nWall = 0;						//	Used by design mode for selecting
										//	which wall to place
#endif

/*
	Declarations for use with bouncy text routines
*/

int g_nTextSprites[MAX_CHARACTERS];

int g_nGotoX[MAX_CHARACTERS];
int g_nGotoY[MAX_CHARACTERS];
int g_nGotoRotate[MAX_CHARACTERS];

int g_nCurX[MAX_CHARACTERS];
int g_nCurY[MAX_CHARACTERS];
int g_nCurRotate[MAX_CHARACTERS];

int g_nStepX[MAX_CHARACTERS];
int g_nStepY[MAX_CHARACTERS];
int g_nStepRotate[MAX_CHARACTERS];

int g_nStartHere = 0;
int g_nEndHere = 0;

/*
	Declarations for use with halo cursor
*/
CURSOR g_oCursors[100];					//	Storage of cursor positions
int g_nCursors = 0;						//	Number to select from

int g_nCursorGotoX[MAX_CURSORS];
int g_nCursorGotoY[MAX_CURSORS];

int g_nCursorCurX[MAX_CURSORS];
int g_nCursorCurY[MAX_CURSORS];

int g_nCursorStepX[MAX_CURSORS];
int g_nCursorStepY[MAX_CURSORS];

int	g_nCursorLine = 0;

/*
	Filter for selecting correct sprite based on object & attribute numbers
	
	The structure is defined as :	OBJECT_TYPE			-	Obtained from the level data
									OBJECT_ATTRIBUTE	-	Obtained from the level data
									SPRITE_LIST_ENTRY	-	Number in Sprite List to use for this object
*/
FILTER g_pFilter[] =	{ OBJECT_WALL, WALL_BORDER, 0,
						  OBJECT_WALL, 1, 1,
						  OBJECT_WALL, 2, 2,
						  OBJECT_WALL, 3, 3,
						  OBJECT_WALL, 4, 4,
						  OBJECT_WALL, 5, 5,
						  OBJECT_WALL, 6, 6,
						  OBJECT_WALL, 7, 7,
						  OBJECT_WALL, 8, 8,
						  OBJECT_WALL, 9, 9,
						  OBJECT_WALL, 10, 10,
						  OBJECT_WALL, 11, 11,
						  OBJECT_WALL, 12, 15,
						  OBJECT_WALL, 13, 12,
						  OBJECT_WALL, 14, 13,
						  OBJECT_WALL, 15, 14,

						  //	Game specific objects
						  OBJECT_SHIELD, -1, 16,
						  OBJECT_KEY, -1, 17,
						  OBJECT_TRANSPORT, -1, 18,
						  OBJECT_FUEL, -1, 19,
						  OBJECT_BASE, -1, 20,
						  OBJECT_CPU, -1, 21,

						  //	Objects to pick up
						  OBJECT_OBJECT, 0, 22,
						  OBJECT_OBJECT, 1, 23,
						  OBJECT_OBJECT, 2, 24,
						  OBJECT_OBJECT, 3, 25,
						  OBJECT_OBJECT, 4, 26,

						  //	Refuel at base
						  OBJECT_RFL, -1, 27,

						  //	Return to base
						  OBJECT_RTB, -1, 28,

						  //	Droid movement
						  OBJECT_DROID, DROID_NORTH, 29,
						  OBJECT_DROID, DROID_EAST, 30,
						  OBJECT_DROID, DROID_SOUTH, 31,
						  OBJECT_DROID, DROID_WEST, 32 };

/*
	Sprite list descriptor
	
	The structure is defined as	:	FRAME_COUNT		-	Number of frames the sprite animation has
									DATA_FILE_LIST	-	List of defines that point to the images from the datafile
									ANIMATION_SPEED	-	Speed of animation in Milliseconds
*/
int g_pSpriteList[] = 	{ 4, CYLU_LOGOC, CYLU_LOGOY, CYLU_LOGOL, CYLU_LOGOU, 20, // 0

						  1, CYLU_WALL0, STATIC, // 1
						  1, CYLU_WALL1, STATIC, // 2
						  1, CYLU_WALL2, STATIC, // 3
						  1, CYLU_WALL3, STATIC, // 4
						  1, CYLU_WALL4, STATIC, // 5
						  1, CYLU_WALL5, STATIC, // 6
						  1, CYLU_WALL6, STATIC, // 7
						  1, CYLU_WALL7, STATIC, // 8
						  1, CYLU_WALL8, STATIC, // 9
						  1, CYLU_WALL9, STATIC, // 10
						  1, CYLU_WALL10, STATIC, // 11
						  1, CYLU_WALL10, STATIC, // 12
						  1, CYLU_WALL10, STATIC, // 13

						  20, CYLU_WALLTOR0, CYLU_WALLTOR1, CYLU_WALLTOR2, CYLU_WALLTOR3, CYLU_WALLTOR4, CYLU_WALLTOR5, CYLU_WALLTOR6,
						  		CYLU_WALLTOR7, CYLU_WALLTOR8, CYLU_WALLTOR9, CYLU_WALLTORA, CYLU_WALLTORB, CYLU_WALLTORC, CYLU_WALLTORD,
						  		CYLU_WALLTORE, CYLU_WALLTORF, CYLU_WALLTORG, CYLU_WALLTORH, CYLU_WALLTORI, CYLU_WALLTORJ, 2, // 14

						  15, CYLU_FAN14, CYLU_FAN13, CYLU_FAN12, CYLU_FAN11, CYLU_FAN10, CYLU_FAN9, CYLU_FAN8, CYLU_FAN7, CYLU_FAN6,
						  	CYLU_FAN5, CYLU_FAN4, CYLU_FAN3, CYLU_FAN2, CYLU_FAN1, CYLU_FAN0, 2, // 15

						  13, CYLU_FIELD0, CYLU_FIELD1, CYLU_FIELD2, CYLU_FIELD3, CYLU_FIELD2, CYLU_FIELD1, CYLU_FIELD3, CYLU_FIELD0,
						  		CYLU_FIELD2, CYLU_FIELD3, CYLU_FIELD0, CYLU_FIELD2, CYLU_FIELD1, 4, // 16

						  4, CYLU_KEY0, CYLU_KEY1, CYLU_KEY2, CYLU_KEY3, 4, // 17
						  4, CYLU_TELEPORT0, CYLU_TELEPORT1, CYLU_TELEPORT2, CYLU_TELEPORT3, 5, // 18

						  18, CYLU_FUEL0, CYLU_FUEL1, CYLU_FUEL2, CYLU_FUEL3, CYLU_FUEL4, CYLU_FUEL5, CYLU_FUEL6,
						  		CYLU_FUEL7, CYLU_FUEL8, CYLU_FUEL9, CYLU_FUEL8, CYLU_FUEL7, CYLU_FUEL6,
						  		CYLU_FUEL5, CYLU_FUEL4, CYLU_FUEL3, CYLU_FUEL2, CYLU_FUEL1, 5, // 19

						  4, CYLU_COMPUTER0, CYLU_COMPUTER2, CYLU_COMPUTER1, CYLU_COMPUTER2, 30, // 20

						  1, CYLU_CPU, STATIC, // 21

						  11, CYLU_GLOBE0, CYLU_GLOBE1, CYLU_GLOBE2, CYLU_GLOBE3, CYLU_GLOBE4, CYLU_GLOBE5, CYLU_GLOBE6, CYLU_GLOBE7, CYLU_GLOBE8, CYLU_GLOBE9, CYLU_GLOBE10, 5, // 22

						  12, CYLU_NEWTON0, CYLU_NEWTON1, CYLU_NEWTON2, CYLU_NEWTON3, CYLU_NEWTON2, CYLU_NEWTON1, CYLU_NEWTON0, CYLU_NEWTON4, CYLU_NEWTON5, CYLU_NEWTON6, CYLU_NEWTON5, CYLU_NEWTON4, 4, // 23

						  18, CYLU_DISC0, CYLU_DISC1, CYLU_DISC2, CYLU_DISC3, CYLU_DISC4, CYLU_DISC5, CYLU_DISC6, CYLU_DISC7, CYLU_DISC8, CYLU_DISC9,
								CYLU_DISC10, CYLU_DISC11, CYLU_DISC12, CYLU_DISC13, CYLU_DISC14, CYLU_DISC15, CYLU_DISC16, CYLU_DISC17, 2, // 24

						  1, CYLU_PENCIL, STATIC, // 25

						  10, CYLU_SPHERE0, CYLU_SPHERE1, CYLU_SPHERE2, CYLU_SPHERE3, CYLU_SPHERE4, CYLU_SPHERE5, CYLU_SPHERE4, CYLU_SPHERE3, CYLU_SPHERE2, CYLU_SPHERE1, 2, // 26

						  12, CYLU_RFL11, CYLU_RFL10, CYLU_RFL9, CYLU_RFL8, CYLU_RFL7, CYLU_RFL6, CYLU_RFL5,
						  	CYLU_RFL4, CYLU_RFL3, CYLU_RFL2, CYLU_RFL1, CYLU_RFL0, 6, // 27
						  4, CYLU_RTB0, CYLU_RTB1, CYLU_RTB2, CYLU_RTB3, 4, // 28

						  1, CYLU_DROIDN0, STATIC, // 29	-	When droid is stationary
						  1, CYLU_DROIDE0, STATIC, // 30
						  1, CYLU_DROIDS0, STATIC, // 31
						  1, CYLU_DROIDW0, STATIC, // 32

						  2, CYLU_DROIDN0, CYLU_DROIDN1, 15, // 33	-	When droid is moving
						  2, CYLU_DROIDE0, CYLU_DROIDE1, 15, // 34
						  2, CYLU_DROIDS0, CYLU_DROIDS1, 15, // 35
						  2, CYLU_DROIDW0, CYLU_DROIDW1, 15, // 36
 						  -1 };

/*
	Function:		main
	Inputs:			argc			-	Count of command line arguments
					**argv			-	String array of command line arguments
	Output:			None

	Description:	Entry point for application
*/
int main( int argc, char **argv )
{
	int nRet = 0;
	char szWork[ 512 ];

	CmdLine( argc, argv);

	//	Seed random number generator
	srand( (unsigned int)time( NULL ) );

	nRet = Initialise();
	if( nRet )
		goto function_exit;

	//	Sprites converted from the original cylu.dat (see tools/extract_dat.py)
	if( !LoadSpriteData( SPRITE_MANIFEST, szLastError, sizeof( szLastError ) ) )
	{
		nRet = ERROR_DATAFILE;
		goto function_exit;
	}

	nRet = NewGame();	//	Initialise all globals
	if( nRet )
		goto function_exit;

	nRet = LoadSprites();
	if( nRet )
		goto function_exit;

	GfxTextfCentre( GRX_WIDTH/2, GRX_HEIGHT - 10, COLOR_WHITE, "Press any key to proceed" );

	while( !PlatformKeyPressed() && !PlatformQuitRequested() )
		PlatformPresent( g_pBackBuffer );
 	PlatformClearKeyBuf();

	if( PlatformQuitRequested() )
		goto function_exit;

#ifdef _DEBUG_
	goto function_exit;
#endif

	g_bInitialised = TRUE;

#ifndef _DESIGN_MODE_
	nRet = Intro();
	if( nRet )
		goto function_exit;
#endif

	nRet = GoCylu();

function_exit:

	if( nRet )
	{
		if( szLastError[0] )
			snprintf( szWork, sizeof( szWork ), "%s\n\n%s", ErrorText( nRet ), szLastError );
		else
			snprintf( szWork, sizeof( szWork ), "%s", ErrorText( nRet ) );

		PlatformErrorBox( "Cylu Error", szWork );
	}

	Terminate();

	return nRet;
}

/*
	Function:		CmdLine
	Inputs:			argc			- Number of arguments
					argv			- Pointer to array of strings
	Output:			Error code

	Description:	Scans the command line options
*/
static int StrICmp( const char *szA, const char *szB )
{
	while( *szA && tolower( (unsigned char)*szA ) == tolower( (unsigned char)*szB ) )
	{
		szA++;
		szB++;
	}

	return tolower( (unsigned char)*szA ) - tolower( (unsigned char)*szB );
}

int CmdLine( int argc, char **argv )
{
	int nRet = 0;
	int i;

	for( i=0; i<argc; i++ )
	{
		if( StrICmp( argv[i], "-NOSOUND" ) == 0 )
			g_bSound = FALSE;
		if( StrICmp( argv[i], "-NOROTATE" ) == 0 )
			g_bRotate = FALSE;
		if( StrICmp( argv[i], "-NOVSYNC" ) == 0 )
			g_bVsync = FALSE;
		if( StrICmp( argv[i], "-WINDOW" ) == 0 )
			g_bFullscreen = FALSE;
		if( StrICmp( argv[i], "-FULLSCREEN" ) == 0 )
			g_bFullscreen = TRUE;

		//	-level <file> plays a different level, e.g. one being tested
		//	from CLD.  Made absolute now, because PlatformInit changes the
		//	current folder to the one holding the executable.
		if( StrICmp( argv[i], "-LEVEL" ) == 0 && i+1 < argc )
		{
			i++;
#ifdef _WIN32
			if( !_fullpath( g_szLevelFile, argv[i], sizeof( g_szLevelFile ) ) )
#else
			if( !realpath( argv[i], g_szLevelFile ) )
#endif
				snprintf( g_szLevelFile, sizeof( g_szLevelFile ), "%s", argv[i] );
		}
	}

	return nRet;
}

/*
	Function:		Initialise
	Inputs:			None
	Output:			True 		- If successful
					False 		- On failure

	Description:	Initialises the allegro system and installs appropriate
					drivers required by the system.  Also intialises the
					global data file for cylu components
*/
int Initialise( void )
{
	int nRet = 0;
	int i;
	unsigned int nVersion;

#ifdef _DEBUG_
	char szChar[2] = { 0, 0 };
	int x, y, c;
#endif

	if( !PlatformInit( GRX_WIDTH, GRX_HEIGHT, g_bFullscreen ? true : false, g_bVsync ? true : false,
		szLastError, sizeof( szLastError ) ) )
	{
		nRet = ERROR_ALLEGRO;
		goto function_exit;
	}

#ifndef _NOBITS_
	if( InitGraphics() )
	{
		nRet = ERROR_INITGRAPHICS;
		goto function_exit;
	}

	for( i=0; i<GRX_WIDTH; i++ )
		RectFill( i, 0, i, GRX_HEIGHT, al_map_rgb( 0, 0, 255-(int)((float)i * ((float)256/(float)GRX_WIDTH)) ) );

	//	Main app ver
	for( i=0; i<TEXT_STEP; i++ )
	{
		RectFill( 0, i, GRX_WIDTH, i, al_map_rgb( 63, 63, i * (256/TEXT_STEP) ) );
		RectFill( 0, GRX_HEIGHT - (TEXT_STEP - i), GRX_WIDTH, GRX_HEIGHT - (TEXT_STEP - i), al_map_rgb( i * (256/TEXT_STEP), 63, 63 ) );
	}

#ifdef _DEBUG_
	x=0;
	y=0;
	for( c=32; c<256; c++ )
	{
		szChar[0] = c;
		GfxTextf( 10 + (x*10), (GRX_HEIGHT - 350)+(y*10), al_map_rgb( 255,255,255 ), szChar );
		x++;
		if( x==16 )
		{
			x=0;
			y++;
		}
	}
#endif

#ifdef _DESIGN_MODE_
	if( !g_bInitialised )
	{
		GfxTextf( 0, g_nTextY, COLOR_GREEN, "DESIGN MODE - Game functionality disabled" );
		g_nTextY = g_nTextY + TEXT_STEP*2;
	}
#endif

	if( !g_bInitialised )
	{
		GfxTextf( 0, g_nTextY, COLOR_WHITE, "%64s: Version %01d.%02d%c", " ", VER_MAJOR, VER_MINOR, VER_RELEASE );
		GfxTextf( 0, g_nTextY, COLOR_WHITE, " %s", VER_TITLE );
		g_nTextY = g_nTextY + (TEXT_STEP*2);
	}

#ifdef _SHAREWARE_
	if( !g_bInitialised )
	{
		GfxTextf( 0, g_nTextY, COLOR_GREEN, "UNREGISTERED VERSION - Limited playability" );
		g_nTextY = g_nTextY + TEXT_STEP*2;
	}
#endif

	if( !g_bInitialised )
	{
		GfxTextf( 0, g_nTextY, COLOR_WHITE, "Please wait - Initialising System" );
		g_nTextY = g_nTextY + TEXT_STEP;

		nVersion = al_get_allegro_version();

		g_nTextY = g_nTextY + TEXT_STEP;
		GfxTextf( 0, g_nTextY, COLOR_WHITE, "Platform: %s", PLATFORM_NAME );
		g_nTextY = g_nTextY + TEXT_STEP;
		GfxTextf( 0, g_nTextY, COLOR_WHITE, "Library: Allegro %d.%d.%d", nVersion >> 24, (nVersion >> 16) & 255, (nVersion >> 8) & 255 );
		g_nTextY = g_nTextY + (TEXT_STEP*2);

		//	Keyboard and timers are set up by PlatformInit
		GfxTextf( 0, g_nTextY, COLOR_WHITE, "Initialising Keyboard:" );
		GfxTextf( 300, g_nTextY, COLOR_GREEN, "Successful" );
		g_nTextY = g_nTextY + TEXT_STEP;

		GfxTextf( 0, g_nTextY, COLOR_WHITE, "Initialising System Timers:" );
		GfxTextf( 300, g_nTextY, COLOR_GREEN, "Successful" );
		g_nTextY = g_nTextY + TEXT_STEP;
	}

	//	Initialising sound engine
	if( g_bSound )
	{
		if( !g_bInitialised )
			GfxTextf( 0, g_nTextY, COLOR_WHITE, "Initialising Sound Engine:" );

		if( InitSound() )
		{
			//	No sound isn't fatal: carry on silently
			if( !g_bInitialised )
			{
				GfxTextf( 300, g_nTextY, COLOR_RED, "Not available (%s)", szLastError );
				g_nTextY = g_nTextY + TEXT_STEP;
			}
			szLastError[0] = '\0';
			g_bSound = FALSE;
		}
		else
		{
			if( !g_bInitialised )
			{
				GfxTextf( 300, g_nTextY, COLOR_GREEN, "Successful" );
				g_nTextY = g_nTextY + TEXT_STEP;
			}
		}
	}
#endif

function_exit:

	return nRet;
}

/*
	Function:		LoadLevel
	Inputs:			None
	Output:			True 		- If successful
					False 		- On failure

	Description:	Loads the level data from the global datafile
*/
int LoadLevel( void )
{
	int nRet = 0;

	//	Initialising sound engine
	if( !g_bInitialised )
		GfxTextf( 0, g_nTextY, COLOR_WHITE, "Testing Level Data:" );

	if( g_lpCMaze )
		delete g_lpCMaze;

	g_lpCMaze = new CMaze;

	if( !g_lpCMaze )
	{
		nRet = ERROR_LOADLEVEL;
		if( !g_bInitialised )
			GfxTextf( 300, g_nTextY, COLOR_RED, "Failed: Low Memory" );
		goto function_exit;
	}

	//	The level is stored in its own file (it used to be the LEVEL entry in cylu.dat)
	if( !g_lpCMaze->LoadMaze( g_szLevelFile ) )
	{
		nRet = ERROR_LOADLEVEL;
		snprintf( szLastError, sizeof( szLastError ), "Cannot load %s", g_szLevelFile );
		if( !g_bInitialised )
			GfxTextf( 300, g_nTextY, COLOR_RED, "Failed: Unable To Load Level Data" );
		goto function_exit;
	}

	if( !g_bInitialised )
	{
		GfxTextf( 300, g_nTextY, COLOR_GREEN, "Successful" );
		g_nTextY = g_nTextY + TEXT_STEP;
	}

function_exit:

	return nRet;
}

/*
	Function:		LoadSprites
	Inputs:			None
	Output:			True 		- If successful
					False 		- On failure

	Description:	Allocates memory and loads sprites into memory
*/
int LoadSprites( void )
{
	int nRet = 0;
	int i = 0;
	int *pFrameList;
	int nFrames;
	int nSpeed;

	if( g_pAnim )
		delete g_pAnim;

	if( !g_bInitialised )
		GfxTextf( 0, g_nTextY, COLOR_WHITE, "Loading Sprites:" );

	g_pAnim = new CAnim;

	if( !g_pAnim )
		nRet = ERROR_LOADSPRITES;

	while( g_pSpriteList[i]!=-1 )
	{
		nFrames = g_pSpriteList[i];
		nSpeed = g_pSpriteList[i+nFrames+1];
		pFrameList = g_pSpriteList+i+1;

		if( !g_pAnim->AddSprite( pFrameList, nFrames, nSpeed ) )
		{
			if( !g_bInitialised )
			{
				GfxTextf( 300, g_nTextY, COLOR_RED, "Failed: Low Memory" );
				g_nTextY = g_nTextY + TEXT_STEP;
				nRet = ERROR_LOWMEMORY;
				goto function_exit;
			}
		}

		i=i+(nFrames+2);
	}

	if( !g_bInitialised )
	{
		GfxTextf( 300, g_nTextY, COLOR_GREEN, "Successful" );
		g_nTextY = g_nTextY + TEXT_STEP;
	}

function_exit:

	return nRet;
}

/*
	Function:		Terminate
	Inputs:			None
	Output:			None

	Description:	Frees memory and closes the system down
*/
void Terminate( void )
{
	if( g_pAnim )
		delete g_pAnim;
	g_pAnim = NULL;

	if( g_lpCMaze )
		delete g_lpCMaze;
	g_lpCMaze = NULL;

	FreeSpriteData();

	if( g_pBackBuffer )
		al_destroy_bitmap( g_pBackBuffer );
	g_pBackBuffer = NULL;

	if( g_pCopy )
		al_destroy_bitmap( g_pCopy );
	g_pCopy = NULL;

	SoundShutdown();
	PlatformShutdown();
}

/*
	Function:		InitGraphics
	Inputs:			None
	Output:			Error Code

	Description:	Creates the 640x480 back buffer the game draws into.
					The window itself is created by PlatformInit and the
					back buffer is scaled to fit it in PlatformPresent.
*/
int InitGraphics( void )
{
	int nRet = 0;

	if( g_pBackBuffer )
		al_destroy_bitmap( g_pBackBuffer );

	//	Smooth scaling when the window is not an exact multiple of 640x480
	al_set_new_bitmap_flags( ALLEGRO_VIDEO_BITMAP | ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR );
	g_pBackBuffer = al_create_bitmap( GRX_WIDTH, GRX_HEIGHT );
	al_set_new_bitmap_flags( ALLEGRO_VIDEO_BITMAP );

	if( !g_pBackBuffer )
	{
		snprintf( szLastError, sizeof( szLastError ), "Unable to create %dx%d back buffer", GRX_WIDTH, GRX_HEIGHT );
		nRet = ERROR_INITGRAPHICS;
		goto function_exit;
	}

	al_set_target_bitmap( g_pBackBuffer );
	al_clear_to_color( al_map_rgb( 0, 0, 0 ) );

function_exit:

	return nRet;
}

/*
	Function:		InitSound
	Inputs:			None
	Output:			Error Code

	Description:	Initialises the sound engine and loads the effects
					from data/sounds (see sound.cpp).  -nosound skips this.
*/
int InitSound( void )
{
	int nRet = 0;

	if( !SoundInit( szLastError, sizeof( szLastError ) ) )
		nRet = ERROR_SOUND;

	return nRet;
}

/*
	Function:		Intro
	Inputs:			None
	Output:			Error code

	Description:	Does title screen intro
*/
int Intro( void )
{
	int nRet = 0;
	float fi, fj;
	float fStep = 5;
	int i, j;
	ALLEGRO_BITMAP *pBitmap;

	pBitmap = al_create_bitmap( GRX_WIDTH, GRX_HEIGHT );
	if( !pBitmap )
	{
		nRet = ERROR_LOWMEMORY;
		goto function_exit;
	}

	//	Prepare bitmap for blitting by ensuring title is on it
	al_set_target_bitmap( pBitmap );
	al_clear_to_color( al_map_rgb( 0, 0, 0 ) );
	ShowTitle( pBitmap );

	//	Each step is drawn over the previous frame, just as it was drawn
	//	straight to the screen under Allegro 4
	al_set_target_bitmap( g_pBackBuffer );

	if( ((rand()%100)+1) < ((rand()%100)+1) )
	{
		//	Interlaced lines slide in from both sides
		for( i=0; i<GRX_WIDTH; i=i+20 )
		{
			for( j=0; j<GRX_HEIGHT/2; j++ )
			{
				al_draw_bitmap_region( pBitmap, 0, (float)(j*2), GRX_WIDTH, 1, (float)((-GRX_WIDTH)+i), (float)(j*2), 0 );
				al_draw_bitmap_region( pBitmap, 0, (float)((j*2)+1), GRX_WIDTH, 1, (float)(GRX_WIDTH-i), (float)((j*2)+1), 0 );
			}
			PlatformPresent( g_pBackBuffer );

			if( PlatformQuitRequested() )
				break;
		}
	}
	else
	{
		//	Zoom out from the centre
		fj = 0;

		for( fi=0; fi<GRX_WIDTH/2; fi=fi+fStep )
		{
			al_draw_scaled_bitmap( pBitmap, 0, 0, GRX_WIDTH, GRX_HEIGHT,
				(float)(int)((GRX_WIDTH/2)-fi), (float)(int)((GRX_HEIGHT/2)-fj), (float)(int)(fi*2), (float)(int)(fj*2), 0 );
			fj = fj + ((float)GRX_HEIGHT / (float)GRX_WIDTH)*fStep;
			PlatformPresent( g_pBackBuffer );

			if( PlatformQuitRequested() )
				break;
		}
	}

	al_destroy_bitmap( pBitmap );

function_exit:

	return nRet;
}

/*
	Function:		NewGame
	Inputs:			None
	Output:			Error code

	Description:	Sets up a new game
*/
int NewGame( void )
{
	int nRet = 0;
	int i;

	if( LoadLevel() )
	{
		nRet = ERROR_LOADLEVEL;
		goto function_exit;
	}

	g_bEndGame = FALSE;					//	Not end of game
	g_nExamineTimer = 0;				//	Expiry examine text timer
	g_nDroidX = 2;						//	Current Droid X position
	g_nDroidY = 2;						//	Current Droid Y position
	g_nDroidXOrig = 0;					//	Original X droid position
	g_nDroidYOrig = 0;					//	Original Y droid position
	g_nDroidDir = DROID_NORTH;			//	Current droid direction
	g_nDroidDirOrig = DROID_NORTH;		//	Original droid direction
	g_fDroidXStep = 0;					//	Droid movement stepping
	g_fDroidYStep = 0;
	g_bMoving = FALSE;					//	Movement status
	g_nObjects = 0;						//	Number of objects collected
	g_nPaces = 0;						//	Number of paces
	g_nMaxObjects = 0;					//	Total number of objects to collect
#ifndef _LOWFUEL_
	g_nFuel = FUEL_INIT;				//	Current fuel status
#else
	g_nFuel = 20;
#endif

	g_nAction = ACTION_NONE;			//	Action status

	g_nPocket = 0;						//	Currently selected pocket

	g_nCurCmd = COMMAND_NONE;			//	Game command status

	g_nStatEnd = 120;					//	Position of stats
	g_nStatPos = 120;					//	Current stats position

	/*
		Initialise the pockets - Carrying nothing!
	*/
	for( i=0; i<5; i++ )
		SetPocket( NO_OBJECT, NO_ATTRIB, i );

#ifdef _DESIGN_MODE_
	g_enuGameState = ePlay;
#endif

 	nRet = Parser();

function_exit:

	return nRet;
}

/*
	Function:		GoCylu
	Inputs:			None
	Output:			Error code

	Description:	Main game loop
*/
int GoCylu( void )
{
	int nRet = 0;
	int nKey;

#ifdef _NOBITS_
	cputs( "GoCylu : Successful\r\n" );
#endif

	while( g_enuGameState!=eExit )
	{
		//	Closing the window quits the game
		if( PlatformQuitRequested() )
		{
			g_enuGameState = eExit;
			break;
		}

		if( g_enuGameState!=ePause )
			g_nTimer = PlatformTicks();	//	Get game timer count

		/*
			Obtain key information
		*/
#ifndef _DESIGN_MODE_

		if( !g_bMoving )
		{
			if( g_nCurCmd == COMMAND_NONE )
			{
				//	Keys that do not need testing for release
				if( g_enuGameState==ePlay && !g_bMenu )
				{
					if( PlatformKeyDown( ALLEGRO_KEY_UP ) )
						g_nCurCmd = COMMAND_UP;

					if( PlatformKeyDown( ALLEGRO_KEY_DOWN ) )
						g_nCurCmd = COMMAND_DOWN;

					if( PlatformKeyDown( ALLEGRO_KEY_LEFT ) )
						g_nCurCmd = COMMAND_LEFT;

					if( PlatformKeyDown( ALLEGRO_KEY_RIGHT ) )
						g_nCurCmd = COMMAND_RIGHT;
				}

				//	Keys that are tested for release
				if( g_nCurCmd == COMMAND_NONE && PlatformKeyPressed() )
				{
					nKey = PlatformReadKey();

					if( g_enuGameState!=eEnd && g_enuGameState!=eCredits &&
						g_enuGameState!=eInstructions )
					{
						if( nKey == ALLEGRO_KEY_ESCAPE )
							g_bMenu = !g_bMenu;
					}

					if( nKey == ALLEGRO_KEY_ENTER )
						g_nCurCmd = COMMAND_EXECUTE;

					if( nKey == ALLEGRO_KEY_M )
						SoundToggleMute();

					if( g_bMenu )
					{
						if( nKey == ALLEGRO_KEY_UP )
							g_nCurCmd = COMMAND_UP;

						if( nKey == ALLEGRO_KEY_DOWN )
							g_nCurCmd = COMMAND_DOWN;
					}
					else
					{
						if( nKey == ALLEGRO_KEY_SPACE )
							g_nCurCmd = COMMAND_PICKUP;

						if( nKey == ALLEGRO_KEY_BACKSPACE )
							g_nCurCmd = COMMAND_DROP;

						if( nKey == ALLEGRO_KEY_X )
							g_nCurCmd = COMMAND_EXAMINE;

						if( nKey == ALLEGRO_KEY_TAB )
							g_nCurCmd = COMMAND_SELECT;
					}

					PlatformClearKeyBuf();
				}
			}
		}
#else
		//	Keys that are tested for release
		if( g_nCurCmd == COMMAND_NONE && PlatformKeyPressed() )
		{
			nKey = PlatformReadKey();

			if( nKey == ALLEGRO_KEY_UP )
				g_nCurCmd = COMMAND_UP;

			if( nKey == ALLEGRO_KEY_DOWN )
				g_nCurCmd = COMMAND_DOWN;

			if( nKey == ALLEGRO_KEY_LEFT )
				g_nCurCmd = COMMAND_LEFT;

			if( nKey == ALLEGRO_KEY_RIGHT )
				g_nCurCmd = COMMAND_RIGHT;

			if( nKey == ALLEGRO_KEY_ESCAPE )
				g_enuGameState = eExit;

			if( nKey == ALLEGRO_KEY_TAB )
				g_nCurCmd = COMMAND_BRICK;

			if( nKey == ALLEGRO_KEY_BACKSPACE )
				g_nCurCmd = COMMAND_DELETE;

			if( nKey == ALLEGRO_KEY_SPACE )
				g_nCurCmd = COMMAND_PLACE;

			PlatformClearKeyBuf();
		}
#endif
		/*
			Process key commands appropriately
		*/
		Process();

		/*
			Process droid movements
		*/
		Actions();

		ClearBackBuffer();

#ifndef _DESIGN_MODE_
		/*
			Compile current frame
		*/
		if( !g_bMenu )
		{
			switch( g_enuGameState )
			{
				case eTitle:
					ShowTitle();
					break;
				case ePlay:
					Compile();
					CompileStats();
					break;
				case eInstructions:
					ShowInstructions();
					break;
				case eWin:
					ShowWin();
					break;
				case eCredits:
					ShowTitle();
					ShowFade();
					ShowCredits();
					break;
				case eEnd:
					ShowEndGame();
					CompileStats();
					break;
			}
		}

		if( g_bMenu )
			ShowMenu();
		else
			g_bMenuShown = FALSE;
#else
		Compile();
#endif

		if( nRet )
		{
			g_enuGameState = eExit;
			break;
		}

		/*
			Render frame to screen
		*/
		nRet = Render();

		if( nRet )
		{
			g_enuGameState = eEnd;
			break;
		}
	}

#ifdef _DESIGN_MODE_
	if( !g_lpCMaze->SaveMaze( g_szLevelFile ) )
		nRet = ERROR_SAVEMAZE;
#endif

	return nRet;
}

/*
	Function:		Process
	Inputs:			None
	Output:			Error code

	Description:	Processes current command based on game state
*/
int Process( void )
{
	int nRet = 0;
	int nX, nY;
	int nObject, nAttrib, nObjectInFront;
	char szWork[10];

#ifndef _DESIGN_MODE_
	if( g_bMenu )
	{
		switch( g_nCurCmd )
		{
			case COMMAND_UP:
				SoundPlay( SND_MENU_MOVE );
				g_nMenuPos--;
				if( g_nMenuPos < 0 ) g_nMenuPos = 3;
				PlaceCursors( g_nMenuPos );
				g_nCurCmd = COMMAND_NONE;
				break;
			case COMMAND_DOWN:
				SoundPlay( SND_MENU_MOVE );
				g_nMenuPos++;
				if( g_nMenuPos > 3 ) g_nMenuPos = 0;
				PlaceCursors( g_nMenuPos );
				g_nCurCmd = COMMAND_NONE;
				break;
			case COMMAND_EXECUTE:
				SoundPlay( SND_MENU_SELECT );
				if( g_enuGameState != ePlay )
				{
					switch( g_nMenuPos )
					{
						case 0:
							g_nMenuPos = 0;
							g_enuGameState = ePlay;
							g_bMenu = FALSE;
							g_nCurCmd = COMMAND_NONE;
							g_nStatPos = g_nStatEnd;
							g_enuStatState = eUp;

							ResetBouncyText();
							PlaceBouncyText( "WELCOME", GRX_WIDTH/2, 60, 1 );
							g_nExamineTimer = g_nTimer + 360;

							break;
						case 1:
							g_nMenuPos = 0;
							g_bInstructionsShown = FALSE;
							g_enuGameState = eInstructions;
							g_bMenu = FALSE;
							g_nCurCmd = COMMAND_NONE;
							g_nStatPos = g_nStatEnd;
							g_enuStatState = eUp;
							break;
						case 2:
							g_nMenuPos = 0;
							g_enuGameState = eCredits;
							g_bMenu = FALSE;
							g_nCurCmd = COMMAND_NONE;
							break;
						case 3:
							g_nMenuPos = 0;
							g_enuGameState = eExit;
							g_nCurCmd = COMMAND_NONE;
							break;
					}
				}
				else
				{
					switch( g_nMenuPos )
					{
						case 0:
							g_nMenuPos = 0;
							g_bMenu = FALSE;
							g_nCurCmd = COMMAND_NONE;
							break;

						case 1:
							g_nMenuPos = 0;
							g_bMenu = FALSE;
							g_nCurCmd = COMMAND_NONE;		//	Don't also use the pocket object
#ifdef _SHAREWARE_
							ResetBouncyText();
							PlaceBouncyText( "NOT YET", GRX_WIDTH/2, 60, 1 );
							PlaceBouncyText( "AVAILABLE", GRX_WIDTH/2, 130, 1 );
							g_nExamineTimer = g_nTimer + 360;
#else
							if( SaveGame() )
							{
								ResetBouncyText();
								PlaceBouncyText( "FILE ERROR", GRX_WIDTH/2, 60, 1 );
								g_nExamineTimer = g_nTimer + 360;
							}
							else
							{
								ResetBouncyText();
								PlaceBouncyText( "GAME", GRX_WIDTH/2, 60, 1 );
								PlaceBouncyText( "SAVED", GRX_WIDTH/2, 130, 1 );
								g_nExamineTimer = g_nTimer + 360;
							}
#endif
							break;

						case 2:
							g_nMenuPos = 0;
							g_bMenu = FALSE;
							g_nCurCmd = COMMAND_NONE;		//	Don't also use the pocket object
#ifdef _SHAREWARE_
							ResetBouncyText();
							PlaceBouncyText( "NOT YET", GRX_WIDTH/2, 60, 1 );
							PlaceBouncyText( "AVAILABLE", GRX_WIDTH/2, 130, 1 );
							g_nExamineTimer = g_nTimer + 360;
#else
							if( LoadGame() )
							{
								ResetBouncyText();
								PlaceBouncyText( "FILE ERROR", GRX_WIDTH/2, 60, 1 );
								g_nExamineTimer = g_nTimer + 360;
							}
							else
							{
								ResetBouncyText();
								PlaceBouncyText( "GAME", GRX_WIDTH/2, 60, 1 );
								PlaceBouncyText( "LOADED", GRX_WIDTH/2, 130, 1 );
								g_nExamineTimer = g_nTimer + 360;
							}
#endif
							break;

						case 3:
							g_nMenuPos = 0;
							g_nFuel = 0;
							g_bMenu = FALSE;
							g_nCurCmd = COMMAND_NONE;
							break;
					}
				}
		}
	}

	switch( g_enuGameState )
	{
		case eCredits:
			switch( g_nCurCmd )
			{
				case COMMAND_EXECUTE:
					SoundPlay( SND_MENU_SELECT );
					g_bMenu = TRUE;
					g_enuGameState = eTitle;
					break;
			}
			break;
		case eInstructions:
			switch( g_nCurCmd )
			{
				case COMMAND_EXECUTE:
					SoundPlay( SND_MENU_SELECT );
					g_nCurPage++;
					if( g_nCurPage > g_nTotalPages-1 )
					{
						g_bMenu = TRUE;
						g_enuGameState = eTitle;
						g_nCurPage = 0;
					}
					break;
			}
			break;

		case eEnd:
			switch( g_nCurCmd )
			{
				case COMMAND_EXECUTE:
					g_enuGameState = eTitle;
					g_bMenu = TRUE;
				 	nRet = NewGame();
					break;
			}
			break;

		case ePlay:
			if( g_nCurCmd )
			{
				g_nAction = ACTION_NONE;

				switch( g_nCurCmd )
				{
					case COMMAND_UP:
						g_nDroidDir = DROID_NORTH;
						nX = 0;
						nY = -1;
						break;

					case COMMAND_DOWN:
						g_nDroidDir = DROID_SOUTH;
						nX = 0;
						nY = 1;
						break;

					case COMMAND_LEFT:
						g_nDroidDir = DROID_WEST;
						nX = -1;
						nY = 0;
						break;

					case COMMAND_RIGHT:
						g_nDroidDir = DROID_EAST;
						nX = 1;
						nY = 0;
						break;

					case COMMAND_SELECT:
						g_nAction = ACTION_SELECT;
						break;

					case COMMAND_PICKUP:
						g_nAction = ACTION_PICKUP;
						break;

					case COMMAND_DROP:
						g_nAction = ACTION_DROP;
						break;

					case COMMAND_EXAMINE:
						g_nAction = ACTION_EXAMINE;
						break;

					case COMMAND_EXECUTE:
						//	Get the status of the currently selected pocket
						GetPocket( nObject, nAttrib, g_nPocket );

						//	Do nothing if no object in pocket
						if( nObject==0 ) break;

						SetDirection( nX, nY );

						//	Check for front facing object
						nObjectInFront = g_lpCMaze->GetCell( g_nDroidX + nX, g_nDroidY + nY, CELL_OBJECT );

						switch( nObject )
						{
							case OBJECT_KEY:
								if( nObjectInFront == OBJECT_TRANSPORT )
								{
									nX = g_nDroidX;
									nY = g_nDroidY;
									Transport( nAttrib );
									if( g_nDroidX != nX || g_nDroidY != nY )
										SoundPlay( SND_TELEPORT );
									SetDirection( nX, nY );
								}
								break;

							case OBJECT_FUEL:
#ifdef _SHAREWARE_
								ResetBouncyText();
								PlaceBouncyText( "UNAVAILABLE", GRX_WIDTH/2, 60, 1 );
								g_nExamineTimer = g_nTimer + 360;
#else
								g_nFuel = FUEL_RENEW;
								SetPocket( NO_OBJECT, NO_ATTRIB, g_nPocket );
								SoundPlay( SND_REFUEL );
#endif
								break;

							case OBJECT_CPU:
								if( nObjectInFront == OBJECT_BASE )
								{
									DisableShield( nAttrib );
									SetPocket( NO_OBJECT, NO_ATTRIB, g_nPocket );
									SoundPlay( SND_FORCEFIELD_OFF );

									sprintf( szWork, "%d", nAttrib );

									//	Remove examine text timeout
									ResetBouncyText();
									PlaceBouncyText( "FORCEFIELD", GRX_WIDTH/2, 60, 1 );
									PlaceBouncyText( szWork, GRX_WIDTH/2, 140, 1 );
									PlaceBouncyText( "DISABLED", GRX_WIDTH/2, 220, 1 );
									g_nExamineTimer = g_nTimer + 360;
								}
								break;

							case OBJECT_OBJECT:
								if( nObjectInFront == OBJECT_BASE )
								{
									g_nObjects++;
									SetPocket( NO_OBJECT, NO_ATTRIB, g_nPocket );

									//	If all objects are deposited
									if( g_nObjects == g_nMaxObjects )
									{
										g_enuGameState = eWin;
										SoundPlay( SND_WIN );
									}
									else
										SoundPlay( SND_DEPOSIT );
								}
								break;

							case OBJECT_RFL:
#ifdef _SHAREWARE_
								ResetBouncyText();
								PlaceBouncyText( "UNAVAILABLE", GRX_WIDTH/2, 60, 1 );
								g_nExamineTimer = g_nTimer + 360;
#else
								if( nObjectInFront == OBJECT_BASE )
								{
									g_nFuel = FUEL_RENEW;
									SoundPlay( SND_REFUEL );
								}
#endif
								break;

							case OBJECT_RTB:
								g_lpCMaze->SetCell( g_nDroidX, g_nDroidY, CELL_OBJECT, 0 );
								g_lpCMaze->SetCell( g_nDroidX, g_nDroidY, CELL_ATTRIBUTE, 0 );

								g_nDroidX = g_nDroidXOrig;
								g_nDroidY = g_nDroidYOrig;
								g_nDroidDir = g_nDroidDirOrig;

								g_lpCMaze->SetCell( g_nDroidX, g_nDroidY, CELL_OBJECT, OBJECT_DROID );
								g_lpCMaze->SetCell( g_nDroidX, g_nDroidY, CELL_ATTRIBUTE, g_nDroidDir );
								SoundPlay( SND_TELEPORT );
								break;
						}

						break;

					default:
						goto function_exit;
						break;					//	Yeah!!
				}

#ifndef	_IGNORE_COLLISIONS_
				if( g_nAction == ACTION_NONE && g_nCurCmd != COMMAND_EXECUTE )
				{
					if( g_lpCMaze->GetCell( g_nDroidX + nX, g_nDroidY + nY, CELL_OBJECT ) )
					{
						static int nLastBump = -1000;

						g_lpCMaze->SetCell( g_nDroidX, g_nDroidY, CELL_ATTRIBUTE, g_nDroidDir );
						g_bMoving = FALSE;

						if( g_nTimer - nLastBump >= BUMP_SOUND_TICKS )
						{
							SoundPlay( SND_BUMP );
							nLastBump = g_nTimer;
						}
					}
					else
					{
#ifdef _NOBITS_
						cprintf( "X : %d\nY : %d\r\n", g_nDroidX + nX, g_nDroidY + nY );
#endif
						g_bMoving = TRUE;
					}
				}
#else
				g_bMoving = TRUE;
#endif
			}
			break;

		case eWin:					//	If you're lucky!!!
			switch( g_nCurCmd )
			{
				case COMMAND_EXECUTE:
					g_enuGameState = eTitle;
					g_bMenu = TRUE;
				 	nRet = NewGame();
					break;
			}
			break;
	}

#else
	if( g_nCurCmd )
	{
		g_nAction = ACTION_NONE;
		switch( g_nCurCmd )
		{
			case COMMAND_UP:
				g_nDroidY--;
				break;

			case COMMAND_DOWN:
				g_nDroidY++;
				break;

			case COMMAND_LEFT:
				g_nDroidX--;
				break;

			case COMMAND_RIGHT:
				g_nDroidX++;
				break;

			case COMMAND_BRICK:
				g_nWall++;
				if( g_nWall>15 ) g_nWall = 0;

				break;

			case COMMAND_DELETE:
				g_lpCMaze->SetCell(	g_nDroidX, g_nDroidY, CELL_OBJECT, 0 );
				g_lpCMaze->SetCell(	g_nDroidX, g_nDroidY, CELL_ATTRIBUTE, 0 );
				break;

			case COMMAND_PLACE:
				nObject =  g_lpCMaze->GetCell( g_nDroidX, g_nDroidY, CELL_OBJECT );

				if( nObject == OBJECT_WALL || nObject == NULL )
				{
					g_lpCMaze->SetCell(	g_nDroidX, g_nDroidY, CELL_OBJECT, OBJECT_WALL );
					g_lpCMaze->SetCell(	g_nDroidX, g_nDroidY, CELL_ATTRIBUTE, g_nWall );
				}
				break;
		}
	}
#endif

function_exit:
	g_nCurCmd = COMMAND_NONE;

	return nRet;
}

/*
	Function:		Actions
	Inputs:			None
	Output:			Error code

	Description:	Performs droid movement calculations, collisions and
					fuel consumption
					testing
*/
int Actions( void )
{
	int nRet = 0;
	static int nTimer = 0;
	static int nCurTimer = 0;
	static int nFuelTimer = 0;
	int nX, nY;
	int nObject, nAttrib;
	char szWork[10];

#ifndef _DESIGN_MODE_

	SetDirection( nX, nY );

	if( g_bMoving )
	{
		g_lpCMaze->SetCell( g_nDroidX, g_nDroidY, CELL_ATTRIBUTE, g_nDroidDir );

		nCurTimer = g_nTimer - nTimer;

		g_fDroidXStep = (nCurTimer * (SPRITE_XSUBDIV/SCROLL_SPEED));
		g_fDroidYStep = (nCurTimer * (SPRITE_YSUBDIV/SCROLL_SPEED));
		g_fShadeStep = (nCurTimer * (256/SCROLL_SPEED));

		switch( g_nDroidDir )
		{
			case DROID_NORTH:
				g_fDroidXStep = -g_fDroidXStep;
				g_fDroidYStep = g_fDroidYStep;
				break;
			case DROID_EAST:
				g_fDroidXStep = -g_fDroidXStep;
				g_fDroidYStep = -g_fDroidYStep;
				break;
			case DROID_SOUTH:
				g_fDroidXStep = g_fDroidXStep;
				g_fDroidYStep = -g_fDroidYStep;
				break;
			case DROID_WEST:
				g_fDroidXStep = g_fDroidXStep;
				g_fDroidYStep = g_fDroidYStep;
				break;
		}

		if( g_nTimer >= nTimer + SCROLL_SPEED )
		{
			g_fDroidXStep = 0;
			g_fDroidYStep = 0;
			g_fShadeStep = 0;

			g_nPaces++;
			SoundPlay( SND_STEP );

			g_lpCMaze->SetCell( g_nDroidX, g_nDroidY, CELL_OBJECT, 0 );

			SetDirection( nX, nY, TRUE );

			g_lpCMaze->SetCell( g_nDroidX, g_nDroidY, CELL_OBJECT, OBJECT_DROID );
			g_lpCMaze->SetCell( g_nDroidX, g_nDroidY, CELL_ATTRIBUTE, g_nDroidDir );

			g_bMoving = FALSE;
			nTimer = g_nTimer;
		}
	}
	else
		nTimer = g_nTimer;

	/*
		Update fuel status
	*/
	if( g_enuGameState == ePlay && !g_bMenu )
	{
		//	One unit every FUEL_SPEED_MS, the rate of the 1996 game
		if( g_nTimer >= nFuelTimer + FUEL_SPEED )
		{
			g_nFuel--;

			if( g_nFuel == FUEL_INIT / 6 || g_nFuel == FUEL_INIT / 12 || g_nFuel == FUEL_INIT / 24 )
				SoundPlay( SND_FUEL_LOW );		//	Warnings at 100, 50 and 25

			if( g_nFuel<1 )
			{
				g_bMenu = FALSE;
				g_enuGameState = eEnd;
				g_enuStatState = eDown;
				SoundPlay( SND_GAME_OVER );
				goto function_exit;
			}

			nFuelTimer = g_nTimer;
		}
	}

	if( g_enuStatState == eUp )
	{
		g_nStatPos = g_nStatPos - 5;
		if( g_nStatPos<=0 )
		{
			g_nStatPos = 0;
			g_enuStatState = eInView;
		}
	}

	if( g_enuStatState == eDown )
	{
		g_nStatPos = g_nStatPos + 5;
		if( g_nStatPos>g_nStatEnd )
		{
			g_nStatPos = g_nStatEnd;
			g_enuStatState = eOutOfView;
		}
	}

	switch( g_nAction )
	{
		case ACTION_SELECT:
			SoundPlay( SND_MENU_MOVE );
			g_nPocket++;
			if( g_nPocket>4 ) g_nPocket = 0;
			break;

		case ACTION_DROP:
			if( g_lpCMaze->GetCell( g_nDroidX + nX, g_nDroidY + nY, CELL_OBJECT ) == 0 )
			{
				GetPocket( nObject, nAttrib, g_nPocket );

				g_lpCMaze->SetCell( g_nDroidX + nX, g_nDroidY + nY, CELL_OBJECT, nObject );
				g_lpCMaze->SetCell( g_nDroidX + nX, g_nDroidY + nY, CELL_ATTRIBUTE, nAttrib );

				SetPocket( NO_OBJECT, NO_ATTRIB, g_nPocket );
				SoundPlay( SND_DROP );
			}
			break;

		case ACTION_PICKUP:
			/*
				Do nothing if object is already in pocket
			*/
			GetPocket( nObject, nAttrib, g_nPocket );
			if( nObject != -1 ) break;

			nObject = g_lpCMaze->GetCell( g_nDroidX + nX, g_nDroidY + nY, CELL_OBJECT );
			nAttrib = g_lpCMaze->GetCell( g_nDroidX + nX, g_nDroidY + nY, CELL_ATTRIBUTE );

			switch( nObject )
			{
				case OBJECT_KEY:
				case OBJECT_FUEL:
				case OBJECT_CPU:
				case OBJECT_OBJECT:
				case OBJECT_RFL:
				case OBJECT_RTB:
					SetPocket( nObject, nAttrib, g_nPocket );
					SoundPlay( SND_PICKUP );

					g_lpCMaze->SetCell( g_nDroidX + nX, g_nDroidY + nY, CELL_OBJECT, 0 );
					g_lpCMaze->SetCell( g_nDroidX + nX, g_nDroidY + nY, CELL_ATTRIBUTE, 0 );
					break;
			}
			break;

		case ACTION_EXAMINE:
			if(	g_nExamineTimer == 0 )
			{
				if( g_lpCMaze->GetCell( g_nDroidX + nX, g_nDroidY + nY, CELL_OBJECT ) == OBJECT_SHIELD )
				{
					//	Get the shield attribute number
					nAttrib = g_lpCMaze->GetCell( g_nDroidX + nX, g_nDroidY + nY, CELL_ATTRIBUTE );

					sprintf( szWork, "%d", nAttrib );

					//	Remove examine text timeout
					ResetBouncyText();
					PlaceBouncyText( "FORCEFIELD", GRX_WIDTH/2, 60, 1 );
					PlaceBouncyText( szWork, GRX_WIDTH/2, 140, 1 );
					g_nExamineTimer = g_nTimer + 360;
				}
				else
				{
					GetPocket( nObject, nAttrib, g_nPocket );

					switch( nObject )
					{
						case OBJECT_KEY:
							ResetBouncyText();
							PlaceBouncyText( "TELEPORTER", GRX_WIDTH/2, 60, 1 );
							PlaceBouncyText( "KEY", GRX_WIDTH/2, 140, 1 );
							g_nExamineTimer = g_nTimer + 360;
							break;

						case OBJECT_FUEL:
							ResetBouncyText();
							PlaceBouncyText( "FUEL", GRX_WIDTH/2, 60, 1 );
							g_nExamineTimer = g_nTimer + 360;
							break;

						case OBJECT_CPU:
							sprintf( szWork, "%d", nAttrib );
							ResetBouncyText();
							PlaceBouncyText( "CPU", GRX_WIDTH/2, 60, 1 );
							PlaceBouncyText( szWork, GRX_WIDTH/2, 140, 1 );
							g_nExamineTimer = g_nTimer + 360;
							break;

						case OBJECT_OBJECT:
							ResetBouncyText();
							PlaceBouncyText( "OBJECT", GRX_WIDTH/2, 60, 1 );
							g_nExamineTimer = g_nTimer + 360;
							break;

						case OBJECT_RFL:
							ResetBouncyText();
							PlaceBouncyText( "REFUEL AT", GRX_WIDTH/2, 60, 1 );
							PlaceBouncyText( "BASE", GRX_WIDTH/2, 140, 1 );
							g_nExamineTimer = g_nTimer + 360;
							break;

						case OBJECT_RTB:
							ResetBouncyText();
							PlaceBouncyText( "RETURN TO", GRX_WIDTH/2, 60, 1 );
							PlaceBouncyText( "BASE", GRX_WIDTH/2, 140, 1 );
							g_nExamineTimer = g_nTimer + 360;
							break;
					}
				}
			}

			break;
	}

#endif

	g_nAction = ACTION_NONE;

function_exit:

	return nRet;
}

/*
	Function:		SetDirection
	Inputs:			nX					- Set direction vector
					nY					- Set direction vector
					bMove				- If true, the droid location is moved
	Output:			None

	Description:	Sets the direction vector depending on the droid
					facing angle
*/
void SetDirection( int &nX, int &nY, BOOL bMove )
{
	switch( g_nDroidDir )
	{
		case DROID_NORTH:
			nX = 0;
			nY = -1;
			if( bMove )	g_nDroidY--;
			break;
		case DROID_EAST:
			nX = 1;
			nY = 0;
			if( bMove ) g_nDroidX++;
			break;
		case DROID_SOUTH:
			nX = 0;
			nY = 1;
			if( bMove ) g_nDroidY++;
			break;
		case DROID_WEST:
			nX = -1;
			nY = 0;
			if( bMove ) g_nDroidX--;
			break;
	}
}

/*
*/
void ShowFade( void )
{
	Fade( g_pBackBuffer, 10, 10, GRX_WIDTH - 20, GRX_HEIGHT - 20 );
}

/*
	Function:		ShowTitle
	Inputs:			None
	Output:			Error code

	Description:	Compiles Title screen
*/
int ShowTitle( ALLEGRO_BITMAP *pCopy )
{
	int nRet = 0;
	ALLEGRO_BITMAP *pSprite;
	ALLEGRO_BITMAP *pOldTarget = al_get_target_bitmap();

	if( !pCopy )
		pCopy = g_pBackBuffer;

	al_set_target_bitmap( pCopy );

	pSprite = GetSprite( CYLU_TITLE );
	DrawSprite( pSprite, 0, 0 );

	/*
		Dont show the logo if instructions are shown because we
		show the bouncy text in the ShowInstructions function
	*/
	if( g_enuGameState != eInstructions )
	{
		pSprite = GetSprite( CYLU_LOGO );
		DrawSprite( pSprite, GRX_WIDTH / 2 - SpriteWidth( pSprite ) / 2, 30 );
	}

	if( !g_bMenu && g_enuGameState==eTitle )
	{
		GfxTextCentre( "Designed and programmed by Paul S.J.Millard", GRX_WIDTH / 2,
			130, al_map_rgb( 255,255,0 ) );

		GfxTextCentre( "Press <ESC> for menu", GRX_WIDTH / 2,
			GRX_HEIGHT - 35, al_map_rgb( 255,255,0 ) );
	}

	al_set_target_bitmap( pOldTarget );

	return nRet;
}

/*
	Function:		ShowMenu
	Inputs:			None
	Output:			Error code

	Description:	Compiles menu
*/
int ShowMenu( void )
{
	int nRet = 0;
	ALLEGRO_BITMAP *pBitmap;

	if( !g_bMenuShown )
	{
		switch( g_enuGameState )
		{
			case eTitle:
				ShowTitle();
				ShowFade();
				break;
			case ePlay:
				Compile();
				CompileStats();
				ShowFade();
				break;
			case eInstructions:
				ShowTitle();
				ShowFade();
				break;
			case eCredits:
				ShowTitle();
				ShowFade();
				break;
		}

		if( !g_pCopy )
			g_pCopy = al_create_bitmap( GRX_WIDTH, GRX_HEIGHT );

		pBitmap = GetSprite( CYLU_LOGO );
		DrawSprite( pBitmap, GRX_WIDTH / 2 - SpriteWidth( pBitmap ) / 2, 30 );

		CopyBitmap( g_pCopy, g_pBackBuffer );

		ResetBouncyText();

		if( g_enuGameState != ePlay )
		{
			PlaceBouncyText( "NEW GAME", GRX_WIDTH/2, 130, 1 );
			PlaceBouncyText( "HOW TO PLAY", GRX_WIDTH/2, 215, 1 );
			PlaceBouncyText( "CREDITS", GRX_WIDTH/2, 300, 1 );
			PlaceBouncyText( "QUIT", GRX_WIDTH/2, 385, 1 );
		}
		else
		{
			PlaceBouncyText( "CONTINUE", GRX_WIDTH/2, 130, 1 );
			PlaceBouncyText( "SAVE GAME", GRX_WIDTH/2, 215, 1 );
			PlaceBouncyText( "LOAD GAME", GRX_WIDTH/2, 300, 1 );
			PlaceBouncyText( "ABANDON", GRX_WIDTH/2, 385, 1 );
		}

		PlaceCursors( g_nMenuPos );

		g_bMenuShown = TRUE;
	}

	CopyBitmap( g_pBackBuffer, g_pCopy );
	DoBouncyText();
	DoCursors();

	return nRet;
}

/*
	Function:		ShowInstructions
	Inputs:			None
	Output:			Error code

	Description:	Compiles instructions
*/
int ShowInstructions( void )
{
	int nRet = 0;
	char szWork[32];
	ALLEGRO_BITMAP *pBitmap;
	int nSprite;

	if( !g_bInstructionsShown )
	{
		ShowTitle();
		ShowFade();

		if( !g_pCopy )
			g_pCopy = al_create_bitmap( GRX_WIDTH, GRX_HEIGHT );

		CopyBitmap( g_pCopy, g_pBackBuffer );

		ResetBouncyText();

		PlaceBouncyText( "HOW TO PLAY", GRX_WIDTH/2, 20, 1 );

		g_bInstructionsShown = TRUE;
	}

	CopyBitmap( g_pBackBuffer, g_pCopy );
	DoBouncyText();


	GfxTextCentre( "Designed and programmed by Paul S.J.Millard", GRX_WIDTH / 2,
		95, al_map_rgb( 255,255,0 ) );

	sprintf( szWork, "Page %d/%d", g_nCurPage+1, g_nTotalPages );
	GfxTextCentre( szWork, GRX_WIDTH/2, 115, al_map_rgb( 0, 255, 0 ) );

	switch( g_nCurPage )
	{
		case 0:
			pBitmap = GetSprite( CYLU_DROIDE0 );

			RectFill( 20, 120, 100, 220, COLOR_DGREY );
			DrawSprite( pBitmap, 20, 120 );

		//   xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

			GfxText( "Cylu the Otsan, the chosen one, enters the land of Vole to",
			120, 130, COLOR_YELLOW );
			GfxText( "participate in the final challenge.  The aim is simple,",
			120, 140, COLOR_YELLOW );
			GfxText( "the quest is long",
			120, 150, COLOR_YELLOW );
			GfxText( "The Otsan require a new leader, so a test has been devised",
			120, 160, COLOR_YELLOW );
			GfxText( "which is presented here.  You will require wits, speed,",
			120, 170, COLOR_YELLOW );
			GfxText( "cunning and logic to be worthy of the Otsan leader.",
			120, 180, COLOR_YELLOW );
			GfxText( "To complete the test, deposit all 24 objects into the",
			120, 190, COLOR_YELLOW );
			GfxText( "central computer.  This will require a knowledge of the",
			120, 200, COLOR_YELLOW );
			GfxText( "planet and certain areas are only accessible via the",
			120, 210, COLOR_YELLOW );
			GfxText( "teleportation network",
			120, 220, COLOR_YELLOW );

/*			Fade( g_pBackBuffer, 30, 280, 80, 100 );
			Fade( g_pBackBuffer, 150, 280, 80, 100 );
			Fade( g_pBackBuffer, 270, 280, 80, 100 );
			Fade( g_pBackBuffer, 390, 280, 80, 100 );
			Fade( g_pBackBuffer, 510, 280, 80, 100 );*/

			RectFill( 30, 280, 110, 380, COLOR_DGREY );
			RectFill( 150, 280, 230, 380, COLOR_DGREY );
			RectFill( 270, 280, 350, 380, COLOR_DGREY );
			RectFill( 390, 280, 470, 380, COLOR_DGREY );
			RectFill( 510, 280, 590, 380, COLOR_DGREY );

			GfxTextCentre( "These are a selection of objects you must collect",
			GRX_WIDTH / 2, 245, COLOR_GREEN );

			GfxTextCentre( "and deposit into the central computer",
			GRX_WIDTH / 2, 255, COLOR_GREEN );

			nSprite = Convert( OBJECT_OBJECT, 0 );
			pBitmap = GetSprite( g_pAnim->GetFrame( nSprite ) );
			DrawSprite( pBitmap, 30, 280 );

			nSprite = Convert( OBJECT_OBJECT, 1 );
			pBitmap = GetSprite( g_pAnim->GetFrame( nSprite ) );
			DrawSprite( pBitmap, 150, 280 );

			nSprite = Convert( OBJECT_OBJECT, 2 );
			pBitmap = GetSprite( g_pAnim->GetFrame( nSprite ) );
			DrawSprite( pBitmap, 270, 280 );

			nSprite = Convert( OBJECT_OBJECT, 3 );
			pBitmap = GetSprite( g_pAnim->GetFrame( nSprite ) );
			DrawSprite( pBitmap, 390, 280 );

			nSprite = Convert( OBJECT_OBJECT, 4 );
			pBitmap = GetSprite( g_pAnim->GetFrame( nSprite ) );
			DrawSprite( pBitmap, 510, 280 );

			g_pAnim->Advance( g_nTimer );
			break;

		case 1:
			//	Display the forcefield
			nSprite = Convert( OBJECT_SHIELD, 0 );
			pBitmap = GetSprite( g_pAnim->GetFrame( nSprite ) );

//			Fade( g_pBackBuffer, 20, 120, 80, 100 );
			RectFill( 20, 120, 100, 220, COLOR_DGREY );
			DrawSprite( pBitmap, 20, 120 );

			//	Display the chip
			nSprite = Convert( OBJECT_CPU, 0 );
			pBitmap = GetSprite( g_pAnim->GetFrame( nSprite ) );

//			Fade( g_pBackBuffer, 20, 230, 80, 100 );
			RectFill( 20, 230, 100, 330, COLOR_DGREY );
			DrawSprite( pBitmap, 20, 230 );

			//	Display	fuel cells
			nSprite = Convert( OBJECT_FUEL, 0 );
			pBitmap = GetSprite( g_pAnim->GetFrame( nSprite ) );

//			Fade( g_pBackBuffer, 20, 340, 80, 100 );
			RectFill( 20, 340, 100, 440, COLOR_DGREY );
			DrawSprite( pBitmap, 20, 340 );

		//   xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

			GfxText( "Forcefields prohibit access to areas of the planet",
			120, 170, COLOR_YELLOW );

			GfxText( "By depositing chips into the central computer, each",
			120, 280, COLOR_YELLOW );

			GfxText( "chip will disable designated forcefields around the planet",
			120, 290, COLOR_YELLOW );

			GfxText( "Collect and use fuel cells to maintain life support systems",
			120, 380, COLOR_YELLOW );
			break;

		case 2:
			//	Display the teleporter
			nSprite = Convert( OBJECT_TRANSPORT, 0 );
			pBitmap = GetSprite( g_pAnim->GetFrame( nSprite ) );

//			Fade( g_pBackBuffer, 20, 120, 80, 100 );
			RectFill( 20, 120, 100, 220, COLOR_DGREY );
			DrawSprite( pBitmap, 20, 120 );

			//	Display the teleport key
			nSprite = Convert( OBJECT_KEY, 0 );
			pBitmap = GetSprite( g_pAnim->GetFrame( nSprite ) );

//			Fade( g_pBackBuffer, 20, 230, 80, 100 );
			RectFill( 20, 230, 100, 330, COLOR_DGREY );
			DrawSprite( pBitmap, 20, 230 );

		//   xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

			GfxText( "Teleporters allow you access to areas of the planet via",
			120, 170, COLOR_YELLOW );

			GfxText( "the teleportation network",
			120, 180, COLOR_YELLOW );

			GfxText( "HINT: Ensure your way home!",
			120, 190, COLOR_YELLOW );

			GfxText( "Teleportation keys are assigned to designated pads of",
			120, 260, COLOR_YELLOW );

			GfxText( "the teleportation network.  To use, face a teleport pad",
			120, 270, COLOR_YELLOW );

			GfxText( "while holding a key and use it.  You will then be teleported",
			120, 280, COLOR_YELLOW );

			GfxText( "to the key's designated teleport pad",
			120, 290, COLOR_YELLOW );
			break;

		case 3:
			//	Display the	RFL object
			nSprite = Convert( OBJECT_RFL, 0 );
			pBitmap = GetSprite( g_pAnim->GetFrame( nSprite ) );

//			Fade( g_pBackBuffer, 20, 120, 80, 100 );
			RectFill( 20, 120, 100, 220, COLOR_DGREY );
			DrawSprite( pBitmap, 20, 120 );

			//	Display the	RTB object
			nSprite = Convert( OBJECT_RTB, 0 );
			pBitmap = GetSprite( g_pAnim->GetFrame( nSprite ) );

//			Fade( g_pBackBuffer, 20, 230, 80, 100 );
			RectFill( 20, 230, 100, 330, COLOR_DGREY );
			DrawSprite( pBitmap, 20, 230 );

		//   xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

			GfxText( "Special object.  Refuel at base allows you to replenish your",
			120, 160, COLOR_YELLOW );

			GfxText( "life support systems while facing the central computer.",
			120, 170, COLOR_YELLOW );

			GfxText( "Collect this object and keep hold of it incase you cannot",
			120, 180, COLOR_YELLOW );

			GfxText( "find fuel near by",
			120, 190, COLOR_YELLOW );

			GfxText( "Special object.  Return to base will ensure your safe return",
			120, 260, COLOR_YELLOW );

			GfxText( "to the central computer.  Keep hold of this during later",
			120, 270, COLOR_YELLOW );

			GfxText( "exploration of the planet to allow you to bring back",
			120, 280, COLOR_YELLOW );

			GfxText( "objects for deposit",
			120, 290, COLOR_YELLOW );

			break;

		case 4:
			GfxTextCentre( "Keys to use during the game",
			GRX_WIDTH / 2, 140, COLOR_BLUE );

			//	Display Keys
			GfxText( "<ARROW KEYS>         Use arrow keys to navigate the planet",
			70, 170, COLOR_YELLOW );

			GfxText( "<SPACE>              Pick up objects you are facing",
			70, 190, COLOR_YELLOW );

			GfxText( "<BACKSPACE>          Drop the currently selected object",
			70, 210, COLOR_YELLOW );

			GfxText( "<TAB>                Select one of your five pockets",
			70, 230, COLOR_YELLOW );

			GfxText( "<ENTER>              Use the object currently selected",
			70, 250, COLOR_YELLOW );

			GfxText( "<X>                  Examine the item you're carrying",
			70, 270, COLOR_YELLOW );
			GfxText( "                     Or examine the forcefield you're facing",
			70, 280, COLOR_GREEN );

			GfxText( "<M>                  Turn the sound on or off",
			70, 300, COLOR_YELLOW );

			//	Other details
			GfxText( "During the game, pressing <ESC> will bring up the menu.",
			70, 320, COLOR_YELLOW );
			GfxText( "Here you have the choice of saving your game, or loading",
			70, 330, COLOR_YELLOW );
			GfxText( "a previously saved game.",
			70, 340, COLOR_YELLOW );

			break;
	}

	g_pAnim->Advance( g_nTimer );

	GfxTextCentre( "Press <ENTER> to continue", GRX_WIDTH / 2,
		GRX_HEIGHT - 30, COLOR_CYAN );


	return nRet;
}

/*
	Function:		ShowCredits
	Inputs:			None
	Output:			Error code

	Description:	Compiles Credits
*/
int ShowCredits( void )
{
	int nRet = 0;
	ALLEGRO_BITMAP *pBitmap;

	pBitmap = GetSprite( CYLU_LOGO );
	DrawSprite( pBitmap, GRX_WIDTH / 2 - SpriteWidth( pBitmap ) / 2, 30 );

	GfxTextCentre( "Designed and programmed by Paul S.J.Millard", GRX_WIDTH / 2,
		140, COLOR_RED );
	GfxTextCentre( "Original game concept by Geoff Foley", GRX_WIDTH / 2,
		160, COLOR_YELLOW );
	GfxTextCentre( "Original game published in 1985 by Firebird Software", GRX_WIDTH / 2,
		180, COLOR_YELLOW );
	GfxTextCentre( "For the Amstrad CPC series computer", GRX_WIDTH / 2,
		195, COLOR_YELLOW );

	GfxTextCentre( "Thanks goes to the following people", GRX_WIDTH / 2,
		230, COLOR_CYAN );
	GfxTextCentre( "DJ Delorie - For DJGPP v2.03", GRX_WIDTH / 2,
		250, COLOR_YELLOW );
	GfxTextfCentre( GRX_WIDTH / 2, 260, al_map_rgb( 255,255,0 ), "Shawn Hargreaves and the Allegro team for Allegro %d.%d.%d",
		al_get_allegro_version() >> 24, (al_get_allegro_version() >> 16) & 255, (al_get_allegro_version() >> 8) & 255 );
	GfxTextCentre( "The POV-Ray Team - for Persistence of Vision(TM) v3.02 (DOS)", GRX_WIDTH / 2,
		270, COLOR_YELLOW );
	GfxTextCentre( "Denis Olivier - For POVLab v4.00 SP4 Build 139", GRX_WIDTH / 2,
		280, COLOR_YELLOW );
	GfxTextCentre( "Robert Hohne - For RHIDE v1.4", GRX_WIDTH / 2,
		290, COLOR_YELLOW );

	GfxTextCentre( "And lastly but not least...", GRX_WIDTH / 2,
		320, COLOR_MAGENTA );
	GfxTextCentre( "Tanya, my loving wife, for all her support and encouragement", GRX_WIDTH / 2,
		330, COLOR_MAGENTA );

	GfxTextCentre( "Press <ENTER> to continue", GRX_WIDTH / 2,
		GRX_HEIGHT - 60, COLOR_YELLOW );


	return nRet;
}

/*
	Function:		ShowEndGame
	Inputs:			None
	Output:			Error code

	Description:	Compiles End of game sequence
*/
int ShowEndGame( void )
{
	int nRet = 0;

	ALLEGRO_BITMAP *pSprite;

	if( !g_bEndGame )
	{
		if( !g_pCopy )
			g_pCopy = al_create_bitmap( GRX_WIDTH, GRX_HEIGHT );

		Compile();		//	Done once
		Fade( g_pBackBuffer, 10, 10, GRX_WIDTH - 20, GRX_HEIGHT - 20 );
		CopyBitmap( g_pCopy, g_pBackBuffer );

		ResetBouncyText();
		PlaceBouncyText( "GAME OVER", GRX_WIDTH/2, 180, 1 );

		g_bEndGame = TRUE;
	}

	CopyBitmap( g_pBackBuffer, g_pCopy );
	pSprite = GetSprite( CYLU_LOGO );
	DrawSprite( pSprite, GRX_WIDTH / 2 - SpriteWidth( pSprite ) / 2, 30 );

	DoBouncyText();


	GfxTextCentre( "Your life support systems have failed",
		GRX_WIDTH/2, 300, COLOR_YELLOW );
	GfxTextCentre( "Better luck next time",
		GRX_WIDTH/2, 310, COLOR_YELLOW );

	GfxTextCentre( "Press <ENTER> to continue",
		GRX_WIDTH / 2, GRX_HEIGHT - 40, COLOR_YELLOW );

	return nRet;
}

/*
	Function:		ShowWin
	Inputs:			None
	Output:			Error code

	Description:	Show win result
*/
int ShowWin( void )
{
	int nRet = 0;

	ALLEGRO_BITMAP *pSprite;

	if( !g_bEndGame )
	{
		if( !g_pCopy )
			g_pCopy = al_create_bitmap( GRX_WIDTH, GRX_HEIGHT );

		Compile();		//	Done once
		Fade( g_pBackBuffer, 10, 10, GRX_WIDTH - 20, GRX_HEIGHT - 20 );
		CopyBitmap( g_pCopy, g_pBackBuffer );

		ResetBouncyText();
		PlaceBouncyText( "WELL DONE", GRX_WIDTH/2, 180, 1 );

		g_bEndGame = TRUE;
	}

	CopyBitmap( g_pBackBuffer, g_pCopy );
	pSprite = GetSprite( CYLU_LOGO );
	DrawSprite( pSprite, GRX_WIDTH / 2 - SpriteWidth( pSprite ) / 2, 30 );

	DoBouncyText();


	GfxTextCentre( "*** Congratulations!!! ***",
		GRX_WIDTH/2, 300, COLOR_YELLOW );
	GfxTextCentre( "You've successfully completed the test", GRX_WIDTH/2, 320,
		COLOR_YELLOW );
	GfxTextCentre( "The Otsan are very pleased and have rewarded you", GRX_WIDTH/2, 330,
		COLOR_YELLOW );
	GfxTextCentre( "Now you will take your place as leader", GRX_WIDTH/2, 340,
		COLOR_YELLOW );

	GfxTextCentre( "Enjoy the fruits of leadership for now, but you will", GRX_WIDTH/2, 360,
		COLOR_YELLOW );
	GfxTextCentre( "soon be faced with new and more challenging tasks...", GRX_WIDTH/2, 370,
		COLOR_YELLOW );

	GfxTextCentre( "Press <ENTER> to continue", GRX_WIDTH / 2, GRX_HEIGHT - 40,
		COLOR_YELLOW );

	return nRet;
}

/*
	Function:		ResetBouncyText

	Inputs:			None
	Output:			None

	Description:	Removes all text used by the bouncy text routines
*/
void ResetBouncyText( void )
{
	g_nStartHere = 0;
	g_nEndHere = 0;
	g_nCursors = 0;
	g_nExamineTimer = 0;
}

/*
	Function:		PlaceBouncyText
	Inputs:			szText				- Text string to show as bouncy text
					nX					- X position of string
					nY					- Y position of string
					nAllign				- 0 Left justified (default)
										  1 Centre justified
										  2 Right justified
	Output:			Error code

	Description:	Sets up a bouncy text string which can then be displayed
					using DoBouncyText.  Additional calls to this function
					sets up new strings including the ones previously
					setup via calls to this function

					Use ResetBouncyText to clear the text buffers
*/
int PlaceBouncyText( const char *szText, int nX, int nY, int nAllign )
{
	int nRet = 0;
	int nLen, nBouncyLen;
	int i, j;
	int nXPos;
	int nInitSprite = CYLU_FONT10;	//	Initial sprite ID for letter A
	int nOffset, nSpriteOffset;
	char c;

	const int nSize = 4;			//	Size of attractor
	const int nMass = 10;			//	Mass of attractor

	const int nSizeRotate = 1;		//	Rotate size
	const int nMassRotate = 45;		//	Rotate mass

	/*
		Determine length of string for use with justification calculations
	*/
	nBouncyLen = 0;
	for( i=0; i<(int)strlen( szText ); i++ )
	{
		c = szText[i];
		switch( c )
		{
			case ' ':
				nBouncyLen = nBouncyLen + 40;
				break;
			case 'I':
				nBouncyLen = nBouncyLen + 50;
				break;
			case 'M':
			case 'W':
				nBouncyLen = nBouncyLen + 65;
				break;
			default:
				nBouncyLen = nBouncyLen + 60;
				break;
		}
	}

	/*
		Determine string length excluding spaces
	*/
	nLen = 0;
	i = 0;
	do
	{
		if( szText[i]!=32 ) nLen++;
		i++;
	}
	while( szText[i]!='\0' );

	j=0;

	switch(	nAllign )
	{
		case 0:			//	Left justified
			nXPos = nX;
			break;
		case 1:			//	Centre justified
			nXPos = ( nX - (nBouncyLen/2) );
			break;
		case 2:			//	Right justified
			nXPos = ( nX - nBouncyLen );
			break;
		default:
			nXPos = ( nX - (nBouncyLen/2) );
			break;
	}

	g_nStartHere = g_nEndHere;
	g_nEndHere = g_nEndHere + nLen;

	//	Prevent overflow
	if( g_nEndHere > MAX_CHARACTERS-1 )
		g_nEndHere = MAX_CHARACTERS - 1;

	for( i=g_nStartHere; i<g_nEndHere; i++ )
	{
		/*
			Do text sprite analysis
		*/
		while( (c = szText[j])==32 )
		{
			j++;
			nXPos = nXPos + 40;
		}

		if( c>58 )
		{
			nOffset = 65;
			nSpriteOffset = 10;
		}
		else
		{
			nOffset = 48;
			nSpriteOffset = 0;
		}

		g_nTextSprites[i] = (nInitSprite+nSpriteOffset) + (c-nOffset);

		//	Origin for each letter to appear
		g_nCurX[i] = GRX_WIDTH/2;
		g_nCurY[i] = GRX_HEIGHT/2;

		g_nCurRotate[i] = 360;

		//	Splits the two words!
		g_nGotoX[i] = nXPos;

		g_nGotoRotate[i] = 0;

		g_nGotoY[i] = nY;

		if( g_nCurX[i] < g_nGotoX[i] )
			g_nStepX[i] = nMass;
		else
			g_nStepX[i] = -nMass;

		if( g_nCurY[i] < g_nGotoY[i] )
			g_nStepY[i] = nMass;
		else
			g_nStepY[i] = -nMass;

		if( g_nCurRotate[i] < g_nGotoRotate[i] )
			g_nStepRotate[i] = nMassRotate;
		else
			g_nStepRotate[i] = -nMassRotate;

		switch( c )
		{
			case 'I':
				nXPos = nXPos + 50;
				break;
			case 'M':
			case 'W':
				nXPos = nXPos + 65;
				break;
			default:
				nXPos = nXPos + 60;
				break;
		}

		j++;
	}

	//	The first line of a new set of bouncy text flies in with a whoosh
	if( g_nCursors == 0 )
		SoundPlay( SND_WHOOSH );

	/*
		Now apply random selections for each halo cursor to one
		of the attractor points for this text string
	*/
	g_oCursors[g_nCursors].m_nStart = g_nStartHere;
	g_oCursors[g_nCursors].m_nEnd = g_nEndHere;

	for( i=0; i<MAX_CURSORS; i++ )
	{
		j = g_nStartHere + (rand() % (g_nEndHere - g_nStartHere ));

		g_nCursorGotoX[i] = g_nGotoX[j];
		g_nCursorGotoY[i] = g_nGotoY[j];

		if( g_nCursors < 1 )
		{
			g_nCursorCurX[i] = GRX_WIDTH/2;
			g_nCursorCurY[i] = GRX_HEIGHT/2;

			g_nCursorStepX[i] = 0;//g_nStepX[j];
			g_nCursorStepY[i] = 0;//g_nStepY[j];
		}
	}

	g_nCursors++;

	return nRet;
}

/*
	Function:		PlaceCursors
	Inputs:			nLine				-	Bouncy text line position
	Output:			Error code

	Description:	Sets up the cursor positions based on a	bouncy
					text line position
*/
int PlaceCursors( int nLine )
{
	int nRet = 0;
	int nStart;
	int nEnd;
	int i, j, k;
	static int nLastLine = 0;

	nStart = g_oCursors[nLine].m_nStart;
	nEnd = g_oCursors[nLine].m_nEnd;
	g_nCursorLine = nLine;

	for( i=0; i<MAX_CURSORS; i++ )
	{
		j = nStart + (rand() % (nEnd - nStart ));

		g_nCursorGotoX[i] = g_nCurX[j];
		g_nCursorGotoY[i] = g_nCurY[j];

		if( nLine!=nLastLine )
		{
			for( k=nStart; k<nEnd; k++ )
			{
				g_nStepRotate[k] = rand() % 20;
				g_nCurRotate[k] = g_nCurRotate[k] + 10;
			}
			nLastLine = nLine;
		}
	}

	return nRet;
}

/*
	Function:		DoCursors
	Inputs:			None
	Output:			Error code

	Description:	Compiles cursor positions
*/
int DoCursors( void )
{
	int nRet = 0;
	int i;
	ALLEGRO_BITMAP *pBitmap;
	static int nRotate = 0;

	const int nSize = 20;			//	Size of attractor
	const int nMass = 40;			//	Mass of attractor
	const int nSpeed = 8;			//	Speed of movement

	for( i=0; i<MAX_CURSORS; i++ )
	{
		pBitmap = GetSprite( CYLU_STAR );

        if( g_nCursorCurX[i] < g_nCursorGotoX[i] - nSize || g_nCursorCurX[i] > g_nCursorGotoX[i] + nSize ||
 			g_nCursorCurY[i] < g_nCursorGotoY[i] - nSize || g_nCursorCurY[i] > g_nCursorGotoY[i] + nSize )
		{
            if( g_nCursorCurX[i] > g_nCursorGotoX[i] + nSize )
			{
                g_nCursorStepX[i] = g_nCursorStepX[i] - nSpeed;
                if( g_nCursorStepX[i] < -(nMass - (rand() % nMass)) ) g_nCursorStepX[i] = -nSpeed;//((rand() % nMass/3)+1);
			}
            else if( g_nCursorCurX[i] < g_nCursorGotoX[i] - nSize )
			{
                g_nCursorStepX[i] = g_nCursorStepX[i] + nSpeed;
                if( g_nCursorStepX[i] > (nMass - (rand() % nMass)) ) g_nCursorStepX[i] = nSpeed;//((rand() % nMass/3)+1);
            }

            if( g_nCursorCurY[i] > g_nCursorGotoY[i] + nSize )
			{
                g_nCursorStepY[i] = g_nCursorStepY[i] - nSpeed;
                if( g_nCursorStepY[i] < -(nMass - (rand() % nMass)) ) g_nCursorStepY[i] = -nSpeed;//-((rand() % nMass/3)+1);
			}
            else if( g_nCursorCurY[i] < g_nCursorGotoY[i] - nSize )
			{
                g_nCursorStepY[i] = g_nCursorStepY[i] + nSpeed;
                if( g_nCursorStepY[i] > (nMass - (rand() % nMass)) ) g_nCursorStepY[i] = nSpeed;//((rand() % nMass/3)+1);
            }
           	
            g_nCursorCurX[i] = g_nCursorCurX[i] + g_nCursorStepX[i];
            g_nCursorCurY[i] = g_nCursorCurY[i] + g_nCursorStepY[i];
        }
		else
		{
//            g_nCursorCurX[i] = g_nCursorGotoX[i];
//            g_nCursorCurY[i] = g_nCursorGotoY[i];
			PlaceCursors( g_nCursorLine );
        }

		if( g_bRotate )
			RotateSprite( pBitmap, g_nCursorCurX[i], g_nCursorCurY[i]-15, nRotate );
		else
			DrawSprite( pBitmap, g_nCursorCurX[i], g_nCursorCurY[i]-15 );

		nRotate = nRotate + 2;
		if( nRotate > 357 ) nRotate = 0;
	}

	return nRet;
}

/*
	Function:		DoBouncyText
	Inputs:			None
	Output:			Error code

	Description:	Constructs single frame state of all bouncing text
*/
int DoBouncyText( void )
{
	int nRet = 0;
	int i;
	ALLEGRO_BITMAP *pBitmap;

	const int nSize = 20;			//	Size of attractor
	const int nMass = 100;			//	Mass of attractor
	const int nSpeed = 5;			//	Speed of movement

	const int nSizeRotate = 5;		//	Rotate size
	const int nMassRotate = 60;		//	Rotate mass

	for( i=0; i<g_nEndHere; i++ )
	{
		pBitmap = GetSprite( g_nTextSprites[i] );

        if( g_nCurX[i] < g_nGotoX[i] - nSize || g_nCurX[i] > g_nGotoX[i] + nSize ||
 			g_nCurY[i] < g_nGotoY[i] - nSize || g_nCurY[i] > g_nGotoY[i] + nSize )
		{
            if( g_nCurX[i] > g_nGotoX[i] + nSize )
			{
                g_nStepX[i] = g_nStepX[i] - nSpeed;
                if( g_nStepX[i] < -(nMass - (rand() % nMass)) ) g_nStepX[i] = -nSpeed;//-((rand() % nMass/2)+1);
			}
            else if( g_nCurX[i] < g_nGotoX[i] - nSize )
			{
                g_nStepX[i] = g_nStepX[i] + nSpeed;
                if( g_nStepX[i] > (nMass - (rand() % nMass)) ) g_nStepX[i] = nSpeed;//((rand() % nMass/2)+1);
            }

            if( g_nCurY[i] > g_nGotoY[i] + nSize )
			{
                g_nStepY[i] = g_nStepY[i] - nSpeed;
                if( g_nStepY[i] < -(nMass - (rand() % nMass)) ) g_nStepY[i] = -nSpeed;//-((rand() % nMass/2)+1);
			}
            else if( g_nCurY[i] < g_nGotoY[i] - nSize )
			{
                g_nStepY[i] = g_nStepY[i] + nSpeed;
                if( g_nStepY[i] > (nMass - (rand() % nMass)) ) g_nStepY[i] = nSpeed;//((rand() % nMass/2)+1);
            }
           	
            g_nCurX[i] = g_nCurX[i] + g_nStepX[i];
            g_nCurY[i] = g_nCurY[i] + g_nStepY[i];
        }
		else
		{
            g_nCurX[i] = g_nGotoX[i];
            g_nCurY[i] = g_nGotoY[i];
        }

		if( g_nCurRotate[i] < g_nGotoRotate[i] - nSizeRotate || g_nCurRotate[i] > g_nGotoRotate[i] + nSizeRotate )
		{
            if( g_nCurRotate[i] > g_nGotoRotate[i] + nSizeRotate )
			{
                g_nStepRotate[i] = g_nStepRotate[i] - 1;
                if( g_nStepRotate[i] < -(nMassRotate - (rand() % nMassRotate)) ) g_nStepRotate[i] = -((rand() % nMassRotate/3)+1);
			}
            else if( g_nCurRotate[i] < g_nGotoRotate[i] - nSizeRotate )
			{
                g_nStepRotate[i] = g_nStepRotate[i] + 1;
                if( g_nStepRotate[i] > (nMassRotate - (rand() % nMassRotate)) ) g_nStepRotate[i] = ((rand() % nMassRotate/3)+1);
            }

			g_nCurRotate[i] = g_nCurRotate[i] + g_nStepRotate[i];
		}
		else
			g_nCurRotate[i] = g_nGotoRotate[i];

		if( g_bRotate )
			RotateSprite( pBitmap, g_nCurX[i], g_nCurY[i], g_nCurRotate[i] );
		else
			DrawSprite( pBitmap, g_nCurX[i], g_nCurY[i] );
	}

	return nRet;
}

/*
	Function:		Compile
	Inputs:			None
	Output:			Error code

	Description:	Compiles the current frame for display
*/
int Compile( void )
{
	int nRet = 0;
	ALLEGRO_BITMAP *pBitmap;
	int x,y;
	float fXPos, fYPos;
	float fShade;
	int nLineN;
	int nLineE;
	int nLineS;
	int nLineW;
	int nObject;
	int nAttrib;
	int nSprite;
	int nIfMoving = 0;
	int nMiddle;
	int nShade;
	int nLocx, nLocy;
	BOOL bDone = FALSE;

	nMiddle = GRX_WIDTH/2;


	//	Advance frames
	if( g_enuGameState==ePlay && !g_bMenu )
		g_pAnim->Advance( g_nTimer );

	if( g_bMoving )
	{
		nShade = 0;
		switch( g_nDroidDir )
		{
			case DROID_NORTH:
				nLineN = 1;
				nLineE = 0;
				nLineS = 0;
				nLineW = 0;
				break;
			case DROID_SOUTH:
				nLineN = 0;
				nLineE = 0;
				nLineS = 1;
				nLineW = 0;
				break;
			case DROID_EAST:
				nLineN = 0;
				nLineE = 1;
				nLineS = 0;
				nLineW = 0;
				break;
			case DROID_WEST:
				nLineN = 0;
				nLineE = 0;
				nLineS = 0;
				nLineW = 1;
				break;
			default:
				nLineN = 0;
				nLineE = 0;
				nLineS = 0;
				nLineW = 0;
				break;
		}
	}
	else
	{
		nLineN = 0;
		nLineE = 0;
		nLineS = 0;
		nLineW = 0;
	}

	if( g_nDroidDir == DROID_NORTH )
	{
		x=DISP_START;
		y=DISP_START-nLineN;
	}
	else if( g_nDroidDir == DROID_WEST )
	{
		x=DISP_START-nLineW;
		y=DISP_START;
	}
	else
	{
		x=y=DISP_START;
	}

	/*
		Render level frame onto back buffer
	*/
	while( !bDone )
	{
		nObject = g_lpCMaze->GetCell( x+g_nDroidX-DROID_OFFSETX, y+g_nDroidY-DROID_OFFSETY, CELL_OBJECT );

		//	Calculate sprite position
		fXPos = nMiddle + ((x * SPRITE_XSUBDIV) - (y * SPRITE_XSUBDIV) - SPRITE_XSUBDIV);
		fYPos = ((x * SPRITE_YSUBDIV) + (y * SPRITE_YSUBDIV));
		fShade = 0;

		if( nObject != OBJECT_DROID )
		{
			//	Scroll position
			fXPos = fXPos + (float)g_fDroidXStep;
			fYPos = fYPos + (float)g_fDroidYStep;
			fShade = fShade + (float)g_fShadeStep;
			nIfMoving = 0;
		}
		else
			if( g_bMoving )
				nIfMoving = 4;

		if( nObject )
		{
			nAttrib = g_lpCMaze->GetCell( x+g_nDroidX-DROID_OFFSETX, y+g_nDroidY-DROID_OFFSETY, CELL_ATTRIBUTE );
			nSprite = Convert( nObject, nAttrib );

			pBitmap = GetSprite( g_pAnim->GetFrame( nSprite+nIfMoving ) );

			switch( g_nDroidDir )
			{
				case DROID_NORTH:
				case DROID_SOUTH:
					nLocx = 0;
					nLocy = -(int)g_fDroidXStep;
					break;
				case DROID_EAST:
				case DROID_WEST:
					nLocx = (int)g_fDroidXStep;
					nLocy = 0;
					break;
			}

			if( g_bMoving )
			{
				nShade = 0;
				switch( g_nDroidDir )
				{
					case DROID_NORTH:
						nLineN = 1;
						nLineE = 0;
						nLineS = 0;
						nLineW = 0;
						if( y==DISP_START-nLineN )
							nShade = 255-(int)fShade;
						if( y==DISP_END+nLineS )
							nShade = (int)fShade;
						break;
					case DROID_SOUTH:
						nLineN = 0;
						nLineE = 0;
						nLineS = 1;
						nLineW = 0;
						if( y==DISP_START-nLineN )
							nShade = (int)fShade;
						if( y==DISP_END+nLineS )
							nShade = 255-(int)fShade;
						break;
					case DROID_EAST:
						nLineN = 0;
						nLineE = 1;
						nLineS = 0;
						nLineW = 0;
						if( x==DISP_START-nLineW )
							nShade = (int)fShade;
						if( x==DISP_END+nLineE )
							nShade = 255-(int)fShade;
						break;
					case DROID_WEST:
						nLineN = 0;
						nLineE = 0;
						nLineS = 0;
						nLineW = 1;
						if( x==DISP_START-nLineW )
							nShade = 255-(int)fShade;
						if( x==DISP_END+nLineE )
							nShade = (int)fShade;
						break;
				}
			}
			else
			{
				nLineN = 0;
				nLineE = 0;
				nLineS = 0;
				nLineW = 0;
				nShade = 0;
			}
			
			//	Limit shading range
			if( nShade>255 ) nShade = 255;
			if( nShade<0 ) nShade = 0;

			//	Draw me sprites!!
			DrawLitSprite( pBitmap, (int)fXPos, (int)fYPos, nShade );
		}

#ifdef _DESIGN_MODE_
		if( x == 4 && y == 4 )
		{
			pBitmap = GetSprite( CYLU_CURSOR );
			DrawSprite( pBitmap, (int)fXPos, (int)fYPos );
		}
#endif

		/*
			Increment appropriate itteration vars
		*/
		if( g_nDroidDir == DROID_EAST || g_nDroidDir == DROID_WEST )
		{
			x++;
			if( x>DISP_END+nLineE )
			{
				x=DISP_START-nLineW;
				y++;
				if( y>DISP_END+nLineS ) bDone = TRUE;
			}
		}
		else
		{
			y++;
			if( y>DISP_END+nLineS )
			{
				y=DISP_START-nLineN;
				x++;
				if( x>DISP_END+nLineE ) bDone = TRUE;
			}
		}
	}

#ifdef _DESIGN_MODE_
	nSprite = Convert( OBJECT_WALL, g_nWall );
	pBitmap = GetSprite( g_pAnim->GetFrame( nSprite ) );

	DrawSprite( pBitmap, 10, GRX_HEIGHT - 110 );
#endif

	return nRet;
}

/*
	Function:		CompileStats
	Inputs:			None
	Output:			Error code

	Description:	Renders the inventory and stats to the backbuffer
*/
int CompileStats( void )
{
	int nRet = 0;
	ALLEGRO_BITMAP *pBitmap;
	int i;
	int nObject;
	int nAttrib;
	int nSprite;
	ALLEGRO_COLOR nColor;
	char szWork[20];

	/*
		Display stats
	*/
	Fade( g_pBackBuffer, 470, 370+g_nStatPos, GRX_WIDTH - 480, (GRX_HEIGHT - 380)+g_nStatPos );
	for( i=0; i<3; i++ )
	{
		Rect( 450 + i, (370 + i)+g_nStatPos, GRX_WIDTH - 10 - i, (GRX_HEIGHT - 10 - i)+g_nStatPos, al_map_rgb( 127, 0, 255 ) );
		Rect( 488 + i, (370 + i)+g_nStatPos, 488 + i, (GRX_HEIGHT - 10 - i)+g_nStatPos, al_map_rgb( 127, 0, 255 ) );
	}

	//	Draw fuel gauge
	RectFill( 460, (GRX_HEIGHT - 20 - (int)((((float)GRX_HEIGHT - 380 - 20) / (float)FUEL_INIT) * (float)g_nFuel))+g_nStatPos, 480, (GRX_HEIGHT - 20)+g_nStatPos, al_map_rgb(0,255,0 ) );
	Rect( 460, 380+g_nStatPos, 480, (GRX_HEIGHT - 20)+g_nStatPos, al_map_rgb( 255,255,255 ) );
	pBitmap = GetSprite( CYLU_FUELGAUGE );
	DrawSprite( pBitmap, 458, 380+g_nStatPos );

	/*
		Show inventory
	*/
	for( i=0; i<5; i++ )
	{
		GetPocket( nObject, nAttrib, i );

		if( i==g_nPocket )
		{
			nColor = al_map_rgb( 255, 255, 127 );
			//Fade( g_pBackBuffer, 5 + (i*90), 370+g_nStatPos, 80, 100 );
			Rect( (5 + (i*90))-3, (370+g_nStatPos)-3, 5 + (i*90) + 83, 473+g_nStatPos, nColor );
		}
		else
			nColor = al_map_rgb( 255, 0, 0 );

		if( nObject!=-1 )
		{
			nSprite = Convert( nObject, nAttrib );

			pBitmap = GetSprite( g_pAnim->GetFrame( nSprite ) );
			DrawSprite( pBitmap, 5 + (i*90), 370+g_nStatPos );
		}

		Rect( 5 + (i*90), 370+g_nStatPos, 5 + (i*90) + 80, 470+g_nStatPos, nColor );
	}

	//	Display	gauge titles
	pBitmap = GetSprite( CYLU_OBJECTS );
	DrawSprite( pBitmap, 497, 380+g_nStatPos );

	pBitmap = GetSprite( CYLU_PACES );
	DrawSprite( pBitmap, 518, 422+g_nStatPos );

	//	Display objects
	sprintf( szWork, "%d/%d", g_nObjects, g_nMaxObjects );
	GfxTextBg( szWork, 545, 408+g_nStatPos, al_map_rgb( 0, 255, 0 ), COLOR_BLACK );

	//	Display paces
	sprintf( szWork, "%d", g_nPaces );
	GfxTextBg( szWork, 555, 450+g_nStatPos, al_map_rgb( 0, 255, 0 ), COLOR_BLACK );

	//	Show examination text if required
	if(	g_nExamineTimer > 0 )
	{
		if( g_nTimer > g_nExamineTimer )
		{
			ResetBouncyText();
			g_nExamineTimer = 0;
		}
		else
			DoBouncyText();
	}

	return nRet;
}

/*
	Function:		ClearBackBuffer
	Inputs:			None
	Output:			Error code

	Description:	Clears the back buffer bitmap
*/
int ClearBackBuffer( void )
{
	int nRet = 0;

	if( !g_pBackBuffer )
	{
		nRet = ERROR_CLEARBACKBUFFER;
		goto function_exit;
	}

	al_set_target_bitmap( g_pBackBuffer );
	al_clear_to_color( GRX_MAKECOL( GRX_RED, GRX_GREEN, GRX_BLUE ) );

function_exit:

	return nRet;
}

/*
	Function:		Render
	Inputs:			None
	Output:			Error code

	Description:	Renders the backbuffer to the screen
*/
int Render( void )
{
	int nRet = 0;

	if( !g_pBackBuffer )
	{
		nRet = ERROR_RENDER;
		goto function_exit;
	}

	//	Waits for the next 70Hz game tick (unless -novsync was given),
	//	then scales the back buffer to the window
	PlatformPresent( g_pBackBuffer );

function_exit:

	return nRet;
}

/*
	Function:		Convert
	Inputs:			nObject				- Object number from maze
					nAttrib				- Attribute number from maze
	Output:			Sprite number

	Description:	Based on the provided object and attribute number, this
					function returns the sprite number from the sprite array
					to be used for rendering
*/
int Convert( int nObject, int nAttrib )
{
	int nSprite = -1;
	int i;
	FILTER Filter;

	for( i=0; i<(int)( sizeof( g_pFilter ) / sizeof( g_pFilter[0] ) ); i++ )
	{
		Filter = g_pFilter[i];

		if( Filter.m_nObject == nObject )
		{
			if( Filter.m_nAttrib!=-1 )
			{
				if( Filter.m_nAttrib == nAttrib )
				{
					nSprite = Filter.m_nSprite;
					break;
				}
			}
			else
			{
				nSprite = Filter.m_nSprite;
				break;
			}
		}
	}

	return nSprite;
}

/*
	Function:		Parser
	Inputs:			None
	Output:			Error number

	Description:	Parses the level and obtains game information such
					as the droid position, teleporter and chip information
*/
int Parser( void )
{
	int nRet = 0;
	int x, y;
	int nWidth, nHeight;
	int nObject, nAttrib;

	g_lpCMaze->GetMazeSize( &nWidth, &nHeight );

	for( y=0; y<nHeight; y++ )
	{
		for( x=0; x<nWidth; x++ )
		{
			nObject =  g_lpCMaze->GetCell( x, y, CELL_OBJECT );
			nAttrib =  g_lpCMaze->GetCell( x, y, CELL_ATTRIBUTE );

			switch( nObject )
			{
				case OBJECT_DROID:
					g_nDroidXOrig = g_nDroidX = x;
					g_nDroidYOrig = g_nDroidY = y;
					g_nDroidDirOrig = nAttrib;
					break;

				case OBJECT_OBJECT:
					g_nMaxObjects++;
					break;
			}
		}
	}

	return nRet;
}

/*
	Function:		Fade
	Inputs:			pBitmap				- Bitmap to render fading effect
					nX, nY				- Left and top position of rectangle
					nWidth, nHeight		- Width and height of rectangle
	Output:			Error number

	Description:	Renders a darkened rectangle onto the specified bitmap
*/
int Fade( ALLEGRO_BITMAP *pBitmap, int nX, int nY, int nWidth, int nHeight )
{
	int nRet = 0;

	ALLEGRO_BITMAP *pOldTarget = al_get_target_bitmap();

	al_set_target_bitmap( pBitmap );
	RectFillTrans( nX, nY, nX + nWidth, nY + nHeight, al_map_rgb( 0, 63, 127 ), 127 );
	al_set_target_bitmap( pOldTarget );

	return nRet;
}

/*
	Function:		SetPocket
	Inputs:			nObject				- Object identity
					nAttrib				- Attribute number
					nPocket				- Pocket number to assign
	Output:			Error number

	Description:	Assigns the specified object and attribute values to
					one of the five pockets
*/
int SetPocket( int nObject, int nAttrib, int nPocket )
{
	int nRet = 0;

	if( nPocket < 0 || nPocket > 4 )
		nRet = -1;

	g_Pockets[ nPocket ].m_nObject = nObject;
	g_Pockets[ nPocket ].m_nAttrib = nAttrib;

	return nRet;
}

/*
	Function:		GetPocket
	Inputs:			nOvject				- Object identity to return
					nAttrib				- Attribute number to return
					nPocket				- Pocket number to query
	Output:			Error number

	Description:	Returns the object and attribute values for the
					specified pocket number
*/
int GetPocket( int &nObject, int &nAttrib, int nPocket )
{
	int nRet = 0;

	if( nPocket < 0 || nPocket > 4 )
		return -1;

	nObject = g_Pockets[ nPocket ].m_nObject;
	nAttrib = g_Pockets[ nPocket ].m_nAttrib;

	return nRet;
}

/*
	Function:		DisableShield
	Inputs:			nShield				- Sheild number
	Output:			Error number

	Description:	Scans the level data and removes the shield that
					matches the shield number
*/
int DisableShield( int nShield )
{
	int nRet = 0;
	int x, y;
	int nWidth, nHeight;
	int nObject, nAttrib;

	g_lpCMaze->GetMazeSize( &nWidth, &nHeight );

	for( x=0; x<nWidth; x++ )
	{
		for( y=0; y<nHeight; y++ )
		{
			nObject = g_lpCMaze->GetCell( x, y, CELL_OBJECT );
			nAttrib = g_lpCMaze->GetCell( x, y, CELL_ATTRIBUTE );

			if( nObject == OBJECT_SHIELD && nAttrib == nShield )
			{
				g_lpCMaze->SetCell( x, y, CELL_OBJECT, 0 );
				g_lpCMaze->SetCell( x, y, CELL_ATTRIBUTE, 0 );
			}
		}
	}

	return nRet;
}

/*
	Function:		Transport
	Inputs:			nKeyAttrib			- Key attribute, points to a transporter
										  number
	Output:			Error number

	Description:	Scans the level data and removes the shield that
					matches the shield number
*/
int Transport( int nKeyAttrib )
{
	int nRet = 0;
	int x, y;
	int nWidth, nHeight;
	int nObject, nAttrib;

	g_lpCMaze->GetMazeSize( &nWidth, &nHeight );

	for( y=0; y<nHeight; y++ )
	{
		for( x=0; x<nWidth; x++ )
		{
			nObject = g_lpCMaze->GetCell( x, y, CELL_OBJECT );
			nAttrib = g_lpCMaze->GetCell( x, y, CELL_ATTRIBUTE );

			if( nObject == OBJECT_TRANSPORT && nAttrib == nKeyAttrib )
			{
				g_lpCMaze->SetCell( g_nDroidX, g_nDroidY, CELL_OBJECT, 0 );
				g_lpCMaze->SetCell( g_nDroidX, g_nDroidY, CELL_ATTRIBUTE, 0 );

				//	Check north
				if( g_lpCMaze->GetCell( x, y-1, CELL_OBJECT ) == 0 )
				{
					g_nDroidX = x;
					g_nDroidY = y-1;
					goto complete_movement;
				}

				//	Check east
				if( g_lpCMaze->GetCell( x+1, y, CELL_OBJECT ) == 0 )
				{
					g_nDroidX = x+1;
					g_nDroidY = y;
					goto complete_movement;
				}

				//	Check south
				if( g_lpCMaze->GetCell( x, y+1, CELL_OBJECT ) == 0 )
				{
					g_nDroidX = x;
					g_nDroidY = y+1;
					goto complete_movement;
				}

				//	Check west
				if( g_lpCMaze->GetCell( x-1, y, CELL_OBJECT ) == 0 )
				{
					g_nDroidX = x-1;
					g_nDroidY = y;
					goto complete_movement;
				}

complete_movement:

				g_lpCMaze->SetCell( g_nDroidX, g_nDroidY, CELL_OBJECT, OBJECT_DROID );
				g_lpCMaze->SetCell( g_nDroidX, g_nDroidY, CELL_ATTRIBUTE, g_nDroidDir );

				goto function_exit;
			}
		}
	}

function_exit:

	return nRet;
}

/*
	Function:		SaveGamePath
	Inputs:			None
	Output:			Full path of the save game file

	Description:	Saved games are kept in the user's application data
					folder (e.g. %APPDATA%\Apex\Cylu on Windows) so the game
					can be installed somewhere read-only such as Program Files
*/
const char *SaveGamePath( void )
{
	static char szPath[ 1024 ] = { "" };
	ALLEGRO_PATH *pPath;

	if( szPath[0] )
		return szPath;

	pPath = al_get_standard_path( ALLEGRO_USER_DATA_PATH );
	if( pPath )
	{
		al_make_directory( al_path_cstr( pPath, ALLEGRO_NATIVE_PATH_SEP ) );
		al_set_path_filename( pPath, SAVE_GAME_NAME );
		snprintf( szPath, sizeof( szPath ), "%s", al_path_cstr( pPath, ALLEGRO_NATIVE_PATH_SEP ) );
		al_destroy_path( pPath );
	}
	else
		snprintf( szPath, sizeof( szPath ), "%s", SAVE_GAME_NAME );

	return szPath;
}

/*
	Function:		SaveGame
	Inputs:			None
	Output:			Error number

	Description:	Saves the current game state to disk
*/
int SaveGame( void )
{
	int nRet = 0;
	int i;
	STATS usGameStats;

	//	Transfer current game stats to the structure
	usGameStats.m_nDroidX = g_nDroidX;
	usGameStats.m_nDroidY = g_nDroidY;
	usGameStats.m_nDroidDir = g_nDroidDir;
	usGameStats.m_nObjects = g_nObjects;
	usGameStats.m_nPaces = g_nPaces;
	usGameStats.m_nFuel = g_nFuel;

	for( i=0; i<5; i++ )
	{
		usGameStats.m_Pockets[i].m_nObject = g_Pockets[i].m_nObject;
		usGameStats.m_Pockets[i].m_nAttrib = g_Pockets[i].m_nAttrib;
	}

	if( !g_lpCMaze->SaveMaze( SaveGamePath(), &usGameStats, sizeof( STATS ) ) )
		nRet = ERROR_SAVEGAME;

	return nRet;
}

/*
	Function:		Transport
	Inputs:			None
	Output:			Error number

	Description:	Loads the last saved game state
*/
int LoadGame( void )
{
	int nRet = 0;
	int i;
	STATS usGameStats;

	if( !g_lpCMaze->LoadMaze( SaveGamePath(), &usGameStats, sizeof( STATS ) ) )
	{
		nRet = ERROR_LOADGAME;
		goto function_exit;
	}

	//	Transfer current game stats to the structure
	g_nDroidX = usGameStats.m_nDroidX;
	g_nDroidY = usGameStats.m_nDroidY;
	g_nDroidDir = usGameStats.m_nDroidDir;
	g_nObjects = usGameStats.m_nObjects;
	g_nPaces = usGameStats.m_nPaces;
	g_nFuel = usGameStats.m_nFuel;

	for( i=0; i<5; i++ )
	{
		g_Pockets[i].m_nObject = usGameStats.m_Pockets[i].m_nObject;
		g_Pockets[i].m_nAttrib = usGameStats.m_Pockets[i].m_nAttrib;
	}

function_exit:

	return nRet;
}
