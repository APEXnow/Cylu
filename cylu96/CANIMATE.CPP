/******************************************************************

	Project:	Cylu
	File:		canimate.cpp
	Author:		Paul Millard
	Original:	Unknown
	Notes:		Function definitions for the CAnimate class.
				This class encapsulates the functions required to
				animate a single sprite which consists of a dynamic
				array of CSprite objects.

******************************************************************/

#include "stdafx.h"
#include "csprite.hpp"
#include "canimate.hpp"

CAnimate::CAnimate()
{
	m_hgblAnimation = NULL;
	m_lppCSprite = NULL;
	m_nFrames = NULL;
	m_nSlide = NULL;
	m_nType = ANIMATE_STATIC;
	m_bStatus = FALSE;
}

CAnimate::~CAnimate()
{
	DestroyAnimation();
}

BOOL CAnimate::CreateAnimation( int nFrames, int nType, int nID )
{
	BOOL bRet = TRUE;
	long lMemoryRequired;
	int nLoop;

	m_nFrames = nFrames;
	m_nType = nType;
	m_nSlide = NULL;

	lMemoryRequired = sizeof( CSprite* ) * m_nFrames;

	m_hgblAnimation = GlobalAlloc( GHND, lMemoryRequired );
	if( !m_hgblAnimation )
		bRet = FALSE;
	else
	{
		if( LockAnimation()==LOCKSTATUS_SUCCESS )
		{		
			for( nLoop=0; nLoop<m_nFrames; nLoop++ )
			{
				m_lppCSprite[ nLoop ] = new CSprite;
				if( m_lppCSprite[ nLoop ] )
				{
					if( !m_lppCSprite[ nLoop ]->SetSpriteIDs( nID + nLoop ) )
					{
						nLoop=m_nFrames;
						bRet = FALSE;
					}
				}
				else
				{
					bRet = FALSE;
					nLoop=m_nFrames;
				}
			}

			if( !bRet )
			{
				for( nLoop=0; nLoop<m_nFrames; nLoop++ )
				{
					if( m_lppCSprite[ nLoop ])
					{
						delete m_lppCSprite[ nLoop ];
						m_lppCSprite[ nLoop ]=(CSprite*)0;
					}
				}
				UnlockAnimation();
				GlobalFree( m_hgblAnimation );
				m_hgblAnimation = NULL;
			}
			else
				UnlockAnimation();

		}
		else
			bRet = FALSE;
	}

	return bRet;
}

void CAnimate::AdvanceFrame( void )
{
	switch( m_nType )
	{
		//	ANIMATE_STATIC:
		//	Do nothing, its a still frame.
		case ANIMATE_ROTATE:
			if( m_nSlide < (m_nFrames-1) )
				m_nSlide++;
			else
				m_nSlide = NULL;
			break;
		case ANIMATE_BOUNCE:
			if( m_bStatus == FALSE )
			{	//	Increasing animation slide number
				if( m_nSlide < (m_nFrames-1) )
					m_nSlide++;
				else
					m_bStatus = TRUE;
			}
			else
			{	//	Decreasing animation slide number
				if( m_nSlide > 0 )
					m_nSlide--;
				else
					m_bStatus = FALSE;
			}
			break;
	}
}

CBitmap* CAnimate::GetSprite( BOOL bImage )
{
	CBitmap* lpCBitmapRet = NULL;

	if( m_lppCSprite )
		lpCBitmapRet = m_lppCSprite[m_nSlide]->GetCBitmap( bImage );

	return lpCBitmapRet;
}

int CAnimate::LockAnimation( void )
{
	int nRet = LOCKSTATUS_FAILED;

	if( m_lppCSprite )
		nRet = LOCKSTATUS_LOCKED;
	else
	{
		if( m_hgblAnimation )
		{
			m_lppCSprite = (CSprite**)GlobalLock( m_hgblAnimation );
			if( m_lppCSprite )
				nRet = LOCKSTATUS_SUCCESS;
		}
	}

	return nRet;
}

void CAnimate::UnlockAnimation( void )
{
	if( m_lppCSprite )
	{
		if( m_hgblAnimation )
		{
			GlobalUnlock( m_hgblAnimation );
			m_lppCSprite = NULL;
		}
	}
}

BOOL CAnimate::DestroyAnimation( void )
{
	BOOL bRet = TRUE;
	int nLoop;

	if( m_hgblAnimation )
	{
		LockAnimation();
		for( nLoop=0; nLoop<m_nFrames; nLoop++ )
		{
			if( m_lppCSprite[ nLoop ] )
			{
				delete m_lppCSprite[ nLoop ];
				m_lppCSprite[ nLoop ]=(CSprite*)0;
			}
		}
		UnlockAnimation();
		GlobalFree( m_hgblAnimation );
		m_hgblAnimation = NULL;
	}
	else
		bRet = FALSE;

	return bRet;
}
