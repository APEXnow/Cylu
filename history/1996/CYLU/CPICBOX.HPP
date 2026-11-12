/******************************************************************

	Project:	Cylu
	File:		cpicbox.hpp
	Author:		Paul Millard
	Original:	Unknown
	Notes:		This class imports the bitmaps by creating arrays
				of the CAnimate class which itself handles the
				images for an animated sprite

******************************************************************/

#ifndef _CPICBOX_HPP_
#define _CPICBOX_HPP_

#include "canimate.hpp"				//	Must be included for CPictureBox
									//	Class data members

#define _NUM_FIELD_STRINGS			10
#define _RECORD_DELIMITER			','

class CPictureBox : public CObject
{
//	Data members
private:
	CFile* m_lpScriptFile;			//	Script file pointer
	CString m_cstrFields[ _NUM_FIELD_STRINGS ];	// Field strings
	CString m_cstrScriptFileName;	//	Name of script file

	int m_nSequences;				//	Total number of CAnimate objects
									//	allocated by CPictureBox class
	HGLOBAL m_hgblCAnimate;			//	Mem handle to CAnimate objects
	CAnimate **m_lppCAnimate;			//	Pointer to CAnimate objects
	
//	Constructors
public:
	int GetAllocatedFilms( void );
	CPictureBox( void );
	~CPictureBox( void );

	//	Create CAnimate objects by reading sprite script file
	BOOL CreateFilms( CString cstrScriptName );
	BOOL DestroyFilms( void );
	int LockFilms( void );
	void UnlockFilms( void );
	BOOL RollFilms( void );
	CBitmap* GetFrame( BOOL bImage, int nSequence );

//	Public function members

//	Private function members
private:
	BOOL ReadRecord();
	CString GetFieldString( int nIndex );
	BOOL FieldStringUsed( int nIndex );
	BOOL OpenScriptFile( CString cstrFileName, int *lpError );
	void CloseScriptFile();
	BOOL ReadStringFromFile( CString cstrFileName, CString* lpcstrText );
	BOOL FindNextRecord();
	BOOL FindText( CString cstrText, int nIndex );
	CFile* GetScriptFile();
	void SetScriptFile( CFile* lpCFile );
	void ClearFieldStrings();
	void FilterBuffer( CString cstrBuffer );
	void SetScriptFileName( CString cstrName );
	CString GetScriptFileName();
	int ReadByte( BYTE *lpByte, CFile *lpCFile );
	BOOL CreateSpecificFilm( void );
	BOOL AllocateFilms( void );
	int FieldAnimationType( int nField );
};

#endif
