/******************************************************************

	Project:	Cylu
	File:		cmaze.hpp
	Author:		Paul Millard
	Original:	Unknown
	Notes:		Class definition.
				The CMaze class defines the functions for creating,
				and handling the maze level used for Cylu.

******************************************************************/

#ifndef _CMAZE_HPP_
#define _CMAZE_HPP_

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

class CMaze : public CObject
{
//	Private Data Members
private:
	CFile* m_lpCFileMaze;			// Maze file pointer
	HGLOBAL m_hgblMaze;
	CELL *m_lpCell;
	int m_nWidth;
	int m_nHeight;
//	Public Member Functions
public:
	CMaze();					//	Constructor
	CMaze( int nWidth, int nHeight );
	~CMaze();					//	Destructor
//	Private Member Functions
private:
	void InitialiseMembers( void );
	BOOL Allocate( int nWidth, int nHeight );
	void Deallocate( void );
	BOOL OpenFile( int nMode, CString cstrFileName, int *lpError );
	void CloseFile( void );
	CFile* GetFile( void );
	void SetFile( CFile* lpCFile );
	int Read( PVOID pBuf, int nSize );
	int Write( PVOID pBuf, int nSize );
	BOOL LockMaze( void );
	void UnlockMaze( void );

//	Public Member Functions
public:
	BOOL CreateMaze( int nWidth, int nHeight );
	BOOL LoadMaze( CString cstrMazeName );
	BOOL SaveMaze( CString cstrMazeName );
	void DestroyMaze( void );
	BOOL SetCell( int nXPos, int nYPos, int nFlag, int nValue );
	int GetCell( int nXPos, int nYPos, int nFlag );
	void GetMazeSize( int *lpnWidth, int *lpnHeight );
	BOOL ValidMaze( void );
};

#endif
