/***************************************************************************
                          cylu.h  -  description
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

#ifndef _CYLU_H_
#define _CYLU_H_

//	Disable if not debugging
//#define _SHAREWARE_

//#define _NOBITS_
//#define _IGNORE_COLLISIONS_
//#define _LOWFUEL_

#define _VSYNC_

//#define _DESIGN_MODE_

#include "./maze.h"			//	CMaze object that contains the game
							//	play maze

#define TEXT_STEP				12

#ifdef _DEBUG
#define GFX_MODE				GFX_GDI
#else
#define GFX_MODE				GFX_AUTODETECT_FULLSCREEN
#endif

#define GRX_WIDTH				640
#define GRX_HEIGHT				480
#define GRX_MAKECOL				makecol
#define GRX_RED					0
#define GRX_GREEN				0
#define GRX_BLUE				0

#define COLOR_BLACK				GRX_MAKECOL( 0, 0, 0 )
#define COLOR_DGREY				GRX_MAKECOL( 63, 63, 63 )

#define COLOR_GREY				GRX_MAKECOL( 127, 127, 127 )
#define COLOR_WHITE				GRX_MAKECOL( 255, 255, 255 )
#define COLOR_DRED				GRX_MAKECOL( 127, 0, 0 )
#define COLOR_RED				GRX_MAKECOL( 255, 0, 0 )
#define COLOR_ORANGE			GRX_MAKECOL( 255, 127, 0 )
#define COLOR_YELLOW			GRX_MAKECOL( 255, 255, 0 )
#define COLOR_DYELLOW			GRX_MAKECOL( 127, 127, 0 )
#define COLOR_DGREEN			GRX_MAKECOL( 0, 127, 0 )
#define COLOR_GREEN				GRX_MAKECOL( 0, 255, 0 )
#define COLOR_DBLUE				GRX_MAKECOL( 0, 0, 127 )
#define COLOR_BLUE				GRX_MAKECOL( 0, 0, 255 )
#define COLOR_CYAN				GRX_MAKECOL( 0, 255, 255 )
#define COLOR_INDIGO			GRX_MAKECOL( 127, 0, 127 )
#define COLOR_MAGENTA			GRX_MAKECOL( 255, 0, 255 )

#define CONFIG_FILE				"cylu.cfg"

#ifdef __DOS__
#define DATA_FILE_NAME24		"./cylu.dat"		//	Data file containing level data and sprites
#else
#define DATA_FILE_NAME24		"./cylu.dat"		//	Data file containing level data and sprites
#endif

#define LEVEL_FILE				"./level.cld"		//	For use with design mode compiled into application

#ifdef __DOS__
#define SAVE_GAME_FILE			"./savegame"	//	File to use for saving and loading game positions
#else
#define SAVE_GAME_FILE			"./savegame"
#endif

#define VER_RELEASE				' '
#define VER_MAJOR				1
#define VER_MINOR				6
#define VER_TITLE				"Cylu - Author: Paul S.J.Millard"

//	Application defines for use in source
#define BOOL					int

#define OBJECT_FLOOR			0
#define OBJECT_WALL				1
#define OBJECT_SHIELD			2
#define OBJECT_KEY				3
#define OBJECT_TRANSPORT		4
#define OBJECT_FUEL				5
#define OBJECT_BASE				6
#define OBJECT_CPU				7
#define OBJECT_OBJECT			8
#define OBJECT_RFL				9
#define OBJECT_RTB				10
#define OBJECT_DROID			12

//	Attributes

//	Walls, the attribute can range from 0 to MAX_WALL_ATTRIBUTE

//	Object attributes
#define ATTRIBUTE_SPHERE		0
#define ATTRIBUTE_BOTTLE		1
#define ATTRIBUTE_BOOK			2
#define ATTRIBUTE_PENCIL		3

#define ATTRIBUTE_DISK			4
#define ATTRIBUTE_CALCULATOR	5

//	Wall attributes
//	The attribute number associates a wall type
#define WALL_BORDER				0

#ifndef _DESIGN_MODE_

#define DISP_START				1
#define DISP_END				7

#define DROID_OFFSETX			((DISP_END - DISP_START) / 2) - 1
#define DROID_OFFSETY			DROID_OFFSETX

#else

#define DISP_START				-1
#define DISP_END				9

#define DROID_OFFSETX			((DISP_END - DISP_START) / 2) + 1
#define DROID_OFFSETY			DROID_OFFSETX

#endif

//	Droid attributes
#define DROID_NORTH				0
#define DROID_EAST				1
#define DROID_SOUTH				2
#define DROID_WEST				3
#define DROID_NONE				4

//#define SHADE					0.638
#ifndef _DESIGN_MODE_

#define SHADE					1.6
#define BRIGHTNESS				40

#else

//#define SHADE					0.9
#define SHADE					1.3
#define BRIGHTNESS				100

#endif

//	Key attributes
//	The key attribute represents the key number for an associated
//	transporter

//	Transporter attributes
//	The transporter attribute has its own associated transporter number

//	Chip attributes
//	Each chip attribute has a number to an energy shield to disable

#define DEFAULT_LEVEL_WIDTH		52
#define DEFAULT_LEVEL_HEIGHT	66

#define SPRITE_WIDTH			80
#define SPRITE_HEIGHT			100
#define SPRITE_XSUBDIV			40.0
#define SPRITE_YSUBDIV			20.0

#define STATIC					0

//	Scrolling speed, also determines speed of droid
#ifndef _IGNORE_COLLISIONS_
#define SCROLL_SPEED			35.0
#else
#define SCROLL_SPEED			10.0
#endif

#define COMMAND_NONE			0
#define COMMAND_UP				1
#define COMMAND_DOWN			2
#define COMMAND_LEFT			3
#define COMMAND_RIGHT			4
#define COMMAND_EXECUTE			5
#define COMMAND_SELECT			6
#define COMMAND_EXAMINE			7
#define COMMAND_PICKUP			8
#define COMMAND_DROP			9

#ifdef _DESIGN_MODE_
#define COMMAND_BRICK			10
#define COMMAND_DELETE			11
#define COMMAND_PLACE			12
#endif

#define ACTION_NONE				0
#define ACTION_SELECT			1
#define ACTION_PICKUP			2
#define ACTION_DROP				3
#define ACTION_EXAMINE			4

#define FUEL_INIT				600
#define FUEL_RENEW				400
#define FUEL_SPEED				15

#define NO_OBJECT				-1
#define NO_ATTRIB				-1

/*
	Defines for bouncy text routines
*/
#define MAX_CHARACTERS			1000

/*
	Defines for halo cursors
*/
#define MAX_CURSORS				20

typedef struct tagFILTER
{
	int m_nObject;
	int m_nAttrib;
	int m_nSprite;
} FILTER;

enum GameState
{
	eTitle,
	eMenu,
	eCredits,
	eInstructions,
	ePlay,
	ePause,
	eEnd,
	eWin,
	eExit
};

enum StatState
{
	eOutOfView,
	eUp,
	eDown,
	eInView
};

typedef struct tagPOCKET
{
	int m_nObject;
	int m_nAttrib;
} POCKET;

typedef struct tagCURSOR
{
	int m_nStart;
	int m_nEnd;
} CURSOR;

typedef struct tagSTATS
{
	int m_nDroidX;
	int m_nDroidY;
	int m_nDroidDir;
	int m_nObjects;
	int m_nPaces;
	int m_nFuel;
	POCKET m_Pockets[5];
} STATS;

//	Main definition
int main( int argc, char **argv );
int CmdLine( int argc, char **argv );

int Initialise( void );
void Terminate( void );

int LoadLevel( void );
int Parser( void );
int Convert( int nObject, int nAttrib );

int CreateMaze( void );
CMaze* GetMaze( void );
void DestroyMaze( void );
int InitGraphics( void );
int InitSound( void );
int LoadSprites( void );

int Intro( void );
int GoCylu( void );
int NewGame( void );

int Process( void );
int Actions( void );

void ShowFade( void );
int ShowTitle( BITMAP *pCopy = NULL );
int ShowMenu( void );
int ShowInstructions( void );
int ShowWin( void );
int ShowCredits( void );
int ShowEndGame( void );

int Compile( void );
int CompileStats( void );
int ClearBackBuffer( void );
int Render( void );

void SetDirection( int &nX, int &nY, BOOL bMove = FALSE );

int SetPocket( int nObject, int nAttrib, int nPocket );
int GetPocket( int &nObject, int &nAttrib, int nPocket );

int DisableShield( int nShield );
int Transport( int nKeyAttrib );

void ResetBouncyText( void );
int PlaceBouncyText( char *szText, int nX, int nY, int nAllign = 0 );
int DoBouncyText( void );
int PlaceCursors( int nLine );
int DoCursors( void );
int Fade( BITMAP *pBitmap, int nX, int nY, int nWidth, int nHeight );

int SaveGame( void );
int LoadGame( void );

#endif
