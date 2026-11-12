/******************************************************************

	Project:	Cylu
	File:		canimate.hpp
	Author:		Paul Millard
	Original:	Unknown
	Notes:		Class definition.
				This class encapsulates the functions required to
				animate a single sprite which consists of a dynamic
				array of CSprite objects.

******************************************************************/

#ifndef _CANIMATE_HPP_
#define _CANIMATE_HPP_

#ifndef _CSPRITE_HPP_
#include "csprite.hpp"
#endif

#define ANIMATE_STATIC			0	//	Single frame animation - still
#define ANIMATE_ROTATE			1	//	Repeat animatation 0,1,2,0,1,2
#define ANIMATE_BOUNCE			2	//	Bounce animation 0,1,2,3,2,1,0

#define LOCKSTATUS_FAILED		0	//	Unable to lock animation
#define LOCKSTATUS_SUCCESS		1	//	Successfully locked animation
#define LOCKSTATUS_LOCKED		2	//	Animation already locked

class CAnimate : public CObject
{
//	Data members
private:
	HGLOBAL m_hgblAnimation;		//	Memory handle for sprite objects
	CSprite **m_lppCSprite;			//	Pointer to an array of CSprite
									//	pointers
	int m_nFrames;					//	Number of frames for animation
	int m_nSlide;					//	Current display slide
	int m_nType;					//	Animation type
	BOOL m_bStatus;					//	Status of animation sequence.
									//	This member only applies to the
									//	bounce animation type
	
//	Constructors
public:
	CAnimate( void );
//	Destructors
	~CAnimate( void );
//	Public function members
	//	Create animation with the specified parameters
	BOOL CreateAnimation( int nFrames, int nType, int nID );
	BOOL DestroyAnimation( void );
	int LockAnimation( void );
	void UnlockAnimation( void );
	CBitmap* GetSprite( BOOL bImage );
	void AdvanceFrame( void );		//	Advance animation, if not still
//	Private function members
};

#endif
