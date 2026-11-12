/***************************************************************************
                          levelcheck.h  -  description
                             -------------------
    begin                : Thu Nov 12 2026
    copyright            : (C) 1996-2026 by Paul S.J.Millard
    email                : apex@apexnow.co.uk
 ***************************************************************************/

/*
	Level checking for CLD, the descendant of the 1996 "Parse Level" command.

	Counts what is on the level and reports anything that would stop it
	playing properly in the current game.  Plain C++ with no MFC, so it can
	be tested on its own.
*/

#ifndef _LEVELCHECK_H_
#define _LEVELCHECK_H_

#include <string>
#include <vector>

#include "../maze.h"

#define ISSUE_ERROR				2		//	The level can't be completed
#define ISSUE_WARNING			1		//	Something will misbehave
#define ISSUE_INFO				0		//	Worth knowing

typedef struct tagLEVELSTATS
{
	int m_nObjects;				//	Objects to deposit in the computer
	int m_nFuel;
	int m_nChips;
	int m_nForceFields;
	int m_nTransporters;
	int m_nKeys;
	int m_nBases;
	int m_nDroids;
	int m_nRefuel;
	int m_nReturn;
	int m_nTrees;
	int m_nWalls;
} LEVELSTATS;

typedef struct tagLEVELISSUE
{
	int m_nSeverity;			//	ISSUE_*
	int m_nX;					//	Cell, or -1 if the issue isn't about one cell
	int m_nY;
	std::string m_strText;
} LEVELISSUE;

//	Returns the number of ISSUE_ERROR entries
int CheckLevel( const CMaze &Maze, LEVELSTATS &Stats, std::vector<LEVELISSUE> &Issues );

//	Display name for an object number, e.g. "Forcefield"
const char *ObjectName( int nObject );

//	What an object's attribute means, e.g. "Wall type", or NULL if unused
const char *AttributeName( int nObject );

//	Highest attribute the game understands for an object (0 if unused)
int AttributeMax( int nObject );

#endif
