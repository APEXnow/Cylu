/******************************************************************

	Project:	Cylu/CLD
	File:		common.hpp
	Author:		Paul Millard
	Original:	Unknown
	Notes:		Common defines and directives used for both Cylu
				and CLD (Cylu Level Designer.)

******************************************************************/

#ifdef _CLD_

//#define FILL_LEVEL

//	Global declarations for the program.

#define BITMAP_WIDTH			20
#define BITMAP_HEIGHT			20
#define BITMAP_COUNT			12
#define BITMAP_ID				_T("MAPOBJECT")
#define MAX_WALL_ATTRIBUTE		5

#define APP_TITLE				_T("CLD")
//#define FILL_LEVEL

#endif

#ifdef _CYLU_

//	Global declarations for the program.

#define GAMESTATE_MAINTITLE		0
#define GAMESTATE_GAMEPLAYING	1
#define GAMESTATE_GAMEOVER		2
#define GAMESTATE_PAUSED		3

#define NORTH					0
#define EAST					1
#define SOUTH					2
#define WEST					3

#define ROBOT_FRAME				15

#define MAX_MOVES				4

#define MOVESTATE_STATIC		0
#define MOVESTATE_MOVING		1

#define AXIS_X					0x1
#define AXIS_Y					0x2

#define	CELL_WIDTH				80
#define CELL_HEIGHT				100

#define OBJECT_BORDER_OFFSET	5

#define INITIAL_FUEL_STATUS		800
#define MAX_FUEL_STATUS			400
#define FUEL_DELAY				1

#define RESOURCE_SPRITES		10000
#define RESOURCE_MASKS			20000

#define FRAME_RATE				250		//	2026: milliseconds per tick, was 60 (too fast on modern PCs)

#define MAX_FORCEFIELDS			31
#define MAX_TRANSPORTERS		10
#define MAX_POCKETS				5

#define APP_TITLE				_T("Cylu")

#endif

//	Global declarations for all applications

//	Objects
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
#define OBJECT_TREE				11
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

//	Key attributes
//	The key attribute represents the key number for an associated
//	transporter

//	Transporter attributes
//	The transporter attribute has its own associated transporter number

//	Chip attributes
//	Each chip attribute has a number to an energy shield to disable

#define PLAYAREA_WIDTH			4
#define PLAYAREA_WIDTH_STEP		3
#define PLAYAREA_HEIGHT			6
#define PLAYAREA_HEIGHT_STEP	5

#define DEFAULT_LEVEL_WIDTH		52
#define DEFAULT_LEVEL_HEIGHT	66
