/******************************************************************

	Project:	Cylu
	File:		csprite.cpp
	Author:		Paul Millard.
	Original:	Unknown
	Notes:		Function definitions for the CSprite class.

******************************************************************/

#include "stdafx.h"
#include "csprite.hpp"

//	Constructor
CSprite::CSprite()
{
	m_lpCBitmapSprite = NULL;
	m_lpCBitmapMask = NULL;
}

CSprite::CSprite( int nID )
{
	m_lpCBitmapSprite = NULL;
	m_lpCBitmapMask = NULL;
	SetSpriteIDs( nID );
}

//	Destructor
CSprite::~CSprite()
{
	ReleaseHandles();
}

//	Private methods

void CSprite::ReleaseHandles( void )
{
	if( m_lpCBitmapSprite )
	{
		if( m_lpCBitmapSprite->m_hObject )
			m_lpCBitmapSprite->DeleteObject();

		delete m_lpCBitmapSprite;
		m_lpCBitmapSprite = NULL;
	}
	if( m_lpCBitmapMask )
	{
		if( m_lpCBitmapMask->m_hObject )
			m_lpCBitmapMask->DeleteObject();

		delete m_lpCBitmapMask;
		m_lpCBitmapMask = NULL;
	}
}

BOOL CSprite::CreateHandles( void )
{
	BOOL bRet = TRUE;

	m_lpCBitmapSprite = new CBitmap;
	m_lpCBitmapMask = new CBitmap;

	if( ( !m_lpCBitmapSprite ) || ( !m_lpCBitmapMask ) )
		bRet = FALSE;

	return bRet;
}

BOOL CSprite::SetSpriteIDs( int nID )
{
	BOOL bRet = TRUE;
#ifdef _DEBUG
	char szWork[255];
#endif

	ReleaseHandles();
	if( !CreateHandles() )
	{
		ReleaseHandles();
		bRet = FALSE;
	}
	else
	{
		if( !m_lpCBitmapSprite->LoadBitmap( SPRITE_ID + nID ) )
		{
	#ifdef _DEBUG
			wsprintf( szWork, "Status:   Failed to load BITMAP resource ID:%d\n", (SPRITE_ID + nID) );
			TRACE( szWork );
	#endif
			bRet = FALSE;
		}
		if( !m_lpCBitmapMask->LoadBitmap( MASK_ID + nID ) )
		{
	#ifdef _DEBUG
			wsprintf( szWork, "Status:   Failed to load BITMAP resource ID:%d\n", (SPRITE_ID + nID) );
			TRACE( szWork );
	#endif
			bRet = FALSE;
		}
	}

	return bRet;
}

CBitmap* CSprite::GetCBitmap( BOOL bImage )
{
	CBitmap* lpCBitmapRet = NULL;

	switch( bImage )
	{
		case SPRITE_IMAGE:
			lpCBitmapRet = m_lpCBitmapSprite;
			break;
		case MASK_IMAGE:
			lpCBitmapRet = m_lpCBitmapMask;
			break;
	}

	return lpCBitmapRet;
}
