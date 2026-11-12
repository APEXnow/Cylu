/******************************************************************

	Project:	Cylu
	File:		csprite.hpp
	Author:		Paul Millard.
	Original:	Unknown
	Notes:		Class definition

******************************************************************/

#ifndef _CSPRITE_HPP_
#define _CSPRITE_HPP_

#define SPRITE_ID		10000
#define MASK_ID			20000
#define SPRITE_IMAGE	0
#define MASK_IMAGE		1

class CSprite : public CObject
{
//	Data members
private:
	CBitmap *m_lpCBitmapSprite;		//	Sprite bitmap
	CBitmap *m_lpCBitmapMask;		//	Sprite Mask

//	Constructors
public:
	CSprite();
	CSprite( int nID );
//	Destructors
	~CSprite();
//	Public function members
	BOOL SetSpriteIDs( int nID );
	CBitmap* GetCBitmap( BOOL bImage );
//	Private function members
private:
	BOOL CreateHandles( void );		//	Create CBitmap object
	void ReleaseHandles( void );	//	Destroy any CBitmap objects
};

#endif
