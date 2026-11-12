/***************************************************************************
                          maze.h  -  description
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

#ifndef _MAZE_H_
#define _MAZE_H_

#include <stddef.h>


#define CELL_OBJECT				0
#define CELL_ATTRIBUTE			1

#define FILE_ID					12345
#define MAX_WIDTH				100
#define MAX_HEIGHT				100

#define FILE_LOAD				0
#define FILE_SAVE				1

typedef struct tagCell
{
	int m_nObject;
	int m_nAttribute;
} CELL;

class CMaze
{
//	Private Data Members
private:
	CELL *m_lpCell;
	int m_nWidth;
	int m_nHeight;
//	Public Member Functions
public:
	CMaze();
	CMaze( int nWidth, int nHeight );
	~CMaze();					//	Destructor
//	Private Member Functions
private:
	void InitialiseMembers( void );
	int Allocate( int nWidth, int nHeight );
	void Deallocate( void );

//	Public Member Functions
public:
	int CreateMaze( int nWidth, int nHeight );
	int LoadMaze( const char *lpszMazeName, void *pExtraData = NULL, int nSize = 0 );
	int SaveMaze( const char *lpszMazeName, void *pExtraData = NULL, int nSize = 0 );
	void DestroyMaze( void );
	int SetCell( int nXPos, int nYPos, int nFlag, int nValue );
	int GetCell( int nXPos, int nYPos, int nFlag );
	void GetMazeSize( int *lpnWidth, int *lpnHeight );
	int ValidMaze( void );
};

#endif
