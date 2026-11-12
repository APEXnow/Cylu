/***************************************************************************
                          objects.h  -  description
                             -------------------
    begin                : Thu Nov 12 2026
    copyright            : (C) 1996-2026 by Paul S.J.Millard
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
	Level object numbers and attributes, shared by the game and CLD (the
	Cylu Level Designer).  This is the descendant of the 1996 common.hpp.

	Each cell of a .CLD level holds an object number and an attribute.
*/

#ifndef _OBJECTS_H_
#define _OBJECTS_H_

//	Objects
#define OBJECT_FLOOR			0
#define OBJECT_WALL				1		//	Attribute: wall type 0..MAX_WALL_TYPE
#define OBJECT_SHIELD			2		//	Attribute: forcefield number
#define OBJECT_KEY				3		//	Attribute: teleporter number it operates
#define OBJECT_TRANSPORT		4		//	Attribute: teleporter number
#define OBJECT_FUEL				5
#define OBJECT_BASE				6		//	Central computer
#define OBJECT_CPU				7		//	Attribute: forcefield number it disables
#define OBJECT_OBJECT			8		//	Attribute: object type 0..MAX_OBJECT_TYPE
#define OBJECT_RFL				9		//	Refuel at base
#define OBJECT_RTB				10		//	Return to base
#define OBJECT_TREE				11		//	1996 version only, not drawn by the current game
#define OBJECT_DROID			12		//	Attribute: starting direction (DROID_*)

#define OBJECT_COUNT			13

//	Attribute limits understood by the current game
#define MAX_WALL_TYPE			15
#define MAX_OBJECT_TYPE			4
#define MAX_ATTRIBUTE			30		//	Forcefield, chip, key and teleporter numbers

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

//	Droid attributes
#define DROID_NORTH				0
#define DROID_EAST				1
#define DROID_SOUTH				2
#define DROID_WEST				3
#define DROID_NONE				4

//	Default level size
#define DEFAULT_LEVEL_WIDTH		52
#define DEFAULT_LEVEL_HEIGHT	66

#endif
