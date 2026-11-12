/***************************************************************************
                          levelcheck.cpp  -  description
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

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <map>

#include "../objects.h"
#include "./levelcheck.h"

/*
	Function:		ObjectName
	Description:	Names used in CLD's menus, palette and reports.  The order
					matches the 1996 "Select Object" list.
*/
const char *ObjectName( int nObject )
{
	static const char *szNames[ OBJECT_COUNT ] =
	{
		"Floor", "Wall", "Forcefield", "Key", "Teleporter", "Fuel",
		"Base", "CPU", "Object", "Refuel At Base", "Return To Base",
		"Tree", "Droid"
	};

	if( nObject >= 0 && nObject < OBJECT_COUNT )
		return szNames[ nObject ];

	return "Unknown";
}

/*
	Function:		AddIssue
	Description:	Appends a formatted issue to the list
*/
const char *AttributeName( int nObject )
{
	switch( nObject )
	{
		case OBJECT_WALL:		return "Wall type";
		case OBJECT_SHIELD:		return "Forcefield number";
		case OBJECT_KEY:		return "Opens teleporter";
		case OBJECT_TRANSPORT:	return "Teleporter number";
		case OBJECT_CPU:		return "Disables forcefield";
		case OBJECT_OBJECT:		return "Object type";
		case OBJECT_DROID:		return "Facing (0 N, 1 E, 2 S, 3 W)";
	}

	return NULL;
}

int AttributeMax( int nObject )
{
	switch( nObject )
	{
		case OBJECT_WALL:		return MAX_WALL_TYPE;
		case OBJECT_OBJECT:		return MAX_OBJECT_TYPE;
		case OBJECT_DROID:		return DROID_WEST;
		case OBJECT_SHIELD:
		case OBJECT_KEY:
		case OBJECT_TRANSPORT:
		case OBJECT_CPU:		return MAX_ATTRIBUTE;
	}

	return 0;
}

static void AddIssue( std::vector<LEVELISSUE> &Issues, int nSeverity, int nX, int nY, const char *szFormat, ... )
{
	char szText[ 256 ];
	va_list Args;
	LEVELISSUE Issue;

	va_start( Args, szFormat );
	vsnprintf( szText, sizeof( szText ), szFormat, Args );
	va_end( Args );

	Issue.m_nSeverity = nSeverity;
	Issue.m_nX = nX;
	Issue.m_nY = nY;
	Issue.m_strText = szText;
	Issues.push_back( Issue );
}

typedef struct tagCELLPOS
{
	int m_nX;
	int m_nY;
} CELLPOS;

/*
	Function:		IsFree
	Description:	True if the droid could stand on the cell, as tested by
					Transport() in the game
*/
static bool IsFree( const CMaze &Maze, int nX, int nY, int nWidth, int nHeight )
{
	if( nX < 0 || nY < 0 || nX >= nWidth || nY >= nHeight )
		return false;

	return Maze.GetCell( nX, nY, CELL_OBJECT ) == OBJECT_FLOOR;
}

int CheckLevel( const CMaze &Maze, LEVELSTATS &Stats, std::vector<LEVELISSUE> &Issues )
{
	int nWidth, nHeight;
	int x, y;
	int nObject, nAttrib;
	int nErrors = 0;
	int nBorderGaps = 0;
	int nFirstGapX = -1, nFirstGapY = -1;

	//	Numbered items, keyed by attribute, in the order the game scans them
	std::map< int, std::vector<CELLPOS> > ForceFields, Chips, Keys, Transporters;
	std::vector<CELLPOS> Droids;
	CELLPOS Pos;

	memset( &Stats, 0, sizeof( Stats ) );
	Issues.clear();

	if( !Maze.ValidMaze() )
	{
		AddIssue( Issues, ISSUE_ERROR, -1, -1, "There is no level to check" );
		return 1;
	}

	Maze.GetMazeSize( &nWidth, &nHeight );

	//	The game scans row by row (Parser, Transport), so this does too
	for( y=0; y<nHeight; y++ )
	{
		for( x=0; x<nWidth; x++ )
		{
			nObject = Maze.GetCell( x, y, CELL_OBJECT );
			nAttrib = Maze.GetCell( x, y, CELL_ATTRIBUTE );
			Pos.m_nX = x;
			Pos.m_nY = y;

			//	Anything on the edge must stop the droid walking off the map
			if( ( x == 0 || y == 0 || x == nWidth-1 || y == nHeight-1 ) && nObject == OBJECT_FLOOR )
			{
				if( nBorderGaps == 0 )
				{
					nFirstGapX = x;
					nFirstGapY = y;
				}
				nBorderGaps++;
			}

			switch( nObject )
			{
				case OBJECT_FLOOR:
					break;

				case OBJECT_WALL:
					Stats.m_nWalls++;
					if( nAttrib < 0 || nAttrib > MAX_WALL_TYPE )
						AddIssue( Issues, ISSUE_WARNING, x, y,
							"Wall type %d is not drawn by the game (types are 0 to %d)", nAttrib, MAX_WALL_TYPE );
					break;

				case OBJECT_SHIELD:
					Stats.m_nForceFields++;
					ForceFields[ nAttrib ].push_back( Pos );
					break;

				case OBJECT_KEY:
					Stats.m_nKeys++;
					Keys[ nAttrib ].push_back( Pos );
					break;

				case OBJECT_TRANSPORT:
					Stats.m_nTransporters++;
					Transporters[ nAttrib ].push_back( Pos );
					break;

				case OBJECT_FUEL:
					Stats.m_nFuel++;
					break;

				case OBJECT_BASE:
					Stats.m_nBases++;
					break;

				case OBJECT_CPU:
					Stats.m_nChips++;
					Chips[ nAttrib ].push_back( Pos );
					break;

				case OBJECT_OBJECT:
					Stats.m_nObjects++;
					if( nAttrib < 0 || nAttrib > MAX_OBJECT_TYPE )
						AddIssue( Issues, ISSUE_WARNING, x, y,
							"Object type %d is not drawn by the game (types are 0 to %d)", nAttrib, MAX_OBJECT_TYPE );
					break;

				case OBJECT_RFL:
					Stats.m_nRefuel++;
					break;

				case OBJECT_RTB:
					Stats.m_nReturn++;
					break;

				case OBJECT_TREE:
					Stats.m_nTrees++;
					break;

				case OBJECT_DROID:
					Stats.m_nDroids++;
					Droids.push_back( Pos );
					if( nAttrib < DROID_NORTH || nAttrib > DROID_WEST )
						AddIssue( Issues, ISSUE_WARNING, x, y,
							"The droid's direction %d is not valid (0 north, 1 east, 2 south, 3 west)", nAttrib );
					break;

				default:
					AddIssue( Issues, ISSUE_WARNING, x, y,
						"Unknown object number %d (blocks the droid but is not drawn)", nObject );
					break;
			}
		}
	}

	/*
		Things that stop the level being played or completed
	*/
	if( Stats.m_nDroids == 0 )
		AddIssue( Issues, ISSUE_ERROR, -1, -1, "There is no droid, so the game has no starting position" );
	else if( Stats.m_nDroids > 1 )
	{
		for( size_t i=0; i<Droids.size()-1; i++ )
			AddIssue( Issues, ISSUE_ERROR, Droids[i].m_nX, Droids[i].m_nY,
				"Extra droid: the game starts at the last droid (%d,%d) and this one just blocks the way",
				Droids.back().m_nX, Droids.back().m_nY );
	}

	if( Stats.m_nObjects == 0 )
		AddIssue( Issues, ISSUE_ERROR, -1, -1, "There are no objects to collect, so the level can never be won" );

	if( Stats.m_nBases == 0 && Stats.m_nObjects > 0 )
		AddIssue( Issues, ISSUE_ERROR, -1, -1, "There is no base computer to deposit objects in, so the level can never be won" );

	if( nBorderGaps > 0 )
		AddIssue( Issues, ISSUE_ERROR, nFirstGapX, nFirstGapY,
			"%d edge square%s %s floor, so the droid can walk off the map (first at %d,%d)",
			nBorderGaps, nBorderGaps == 1 ? "" : "s", nBorderGaps == 1 ? "is" : "are", nFirstGapX, nFirstGapY );

	/*
		Forcefields and the chips that disable them
	*/
	for( std::map< int, std::vector<CELLPOS> >::const_iterator it = ForceFields.begin(); it != ForceFields.end(); ++it )
	{
		if( Chips.find( it->first ) == Chips.end() )
			AddIssue( Issues, ISSUE_WARNING, it->second[0].m_nX, it->second[0].m_nY,
				"Forcefield %d has no CPU chip %d, so it can never be disabled", it->first, it->first );
	}

	for( std::map< int, std::vector<CELLPOS> >::const_iterator it = Chips.begin(); it != Chips.end(); ++it )
	{
		if( ForceFields.find( it->first ) == ForceFields.end() )
			AddIssue( Issues, ISSUE_INFO, it->second[0].m_nX, it->second[0].m_nY,
				"CPU chip %d has no forcefield %d to disable", it->first, it->first );

		if( it->second.size() > 1 )
			AddIssue( Issues, ISSUE_INFO, it->second[1].m_nX, it->second[1].m_nY,
				"There are %d CPU chips numbered %d", (int)it->second.size(), it->first );
	}

	/*
		Keys and teleporters
	*/
	for( std::map< int, std::vector<CELLPOS> >::const_iterator it = Keys.begin(); it != Keys.end(); ++it )
	{
		if( Transporters.find( it->first ) == Transporters.end() )
			AddIssue( Issues, ISSUE_WARNING, it->second[0].m_nX, it->second[0].m_nY,
				"Key %d has no teleporter %d to take the droid to", it->first, it->first );
	}

	for( std::map< int, std::vector<CELLPOS> >::const_iterator it = Transporters.begin(); it != Transporters.end(); ++it )
	{
		const CELLPOS &First = it->second[0];

		if( Keys.find( it->first ) == Keys.end() )
			AddIssue( Issues, ISSUE_INFO, First.m_nX, First.m_nY,
				"No key leads to teleporter %d", it->first );

		if( it->second.size() > 1 )
			AddIssue( Issues, ISSUE_WARNING, it->second[1].m_nX, it->second[1].m_nY,
				"There are %d teleporters numbered %d; key %d always goes to the first one (%d,%d)",
				(int)it->second.size(), it->first, it->first, First.m_nX, First.m_nY );

		//	Transport() puts the droid on a free square north, east, south or west
		if( !IsFree( Maze, First.m_nX, First.m_nY-1, nWidth, nHeight ) &&
			!IsFree( Maze, First.m_nX+1, First.m_nY, nWidth, nHeight ) &&
			!IsFree( Maze, First.m_nX, First.m_nY+1, nWidth, nHeight ) &&
			!IsFree( Maze, First.m_nX-1, First.m_nY, nWidth, nHeight ) )
			AddIssue( Issues, ISSUE_WARNING, First.m_nX, First.m_nY,
				"Teleporter %d has no free square next to it to arrive on", it->first );
	}

	/*
		Leftovers from the 1996 version
	*/
	if( Stats.m_nTrees > 0 )
	{
		for( y=0; y<nHeight; y++ )
			for( x=0; x<nWidth; x++ )
				if( Maze.GetCell( x, y, CELL_OBJECT ) == OBJECT_TREE )
				{
					AddIssue( Issues, ISSUE_WARNING, x, y,
						"%d tree%s from the 1996 game: the current game doesn't draw trees, so they are invisible walls",
						Stats.m_nTrees, Stats.m_nTrees == 1 ? "" : "s" );
					y = nHeight;
					break;
				}
	}

	for( size_t i=0; i<Issues.size(); i++ )
		if( Issues[i].m_nSeverity == ISSUE_ERROR )
			nErrors++;

	return nErrors;
}
