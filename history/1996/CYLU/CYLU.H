// Cylu.h : main header file for the CYLU application
//

#define _CYLU_

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "common.hpp"		//	Include common defines and directives for
							//	this application

#include "resource.h"       //	main symbols
#include "general.h"		//	General functions
#include "cpicbox.hpp"		//	CPictureBox class header
							//	This clsss is responsible for loading
							//	and maintaining the sprites for the
							//	game
#include "cmaze.hpp"		//	CMaze object that contains the game
							//	play maze

/////////////////////////////////////////////////////////////////////////////
// CCyluApp:
// See Cylu.cpp for the implementation of this class
//

class CCyluApp : public CWinApp
{
public:
	CPictureBox* GetPictureBox( void );
	CMaze* GetMaze( void );
	BOOL CreatePictureBox( void );
	BOOL CreateMaze( void );
	void DestroyPictureBox( void );
	void DestroyMaze( void );
	void DoMessageLoop( void );
	void SetScreenPos( CPoint pt )
		{ m_cScreenPos = pt; }
	CPoint GetScreenPos( void )
		{ return m_cScreenPos; }
	void SetScreenPosOld( CPoint pt )
		{ m_cScreenPosOld = pt; }
	CPoint GetScreenPosOld( void )
		{ return m_cScreenPosOld; }
	void SetBasePos( CPoint pt )
		{ m_cBasePos = pt; }
	CPoint GetBasePos( void )
		{ return m_cBasePos; }
	void SetDroidPos( CPoint pt )
		{ m_cDroidPos = pt; }
	CPoint GetDroidPos( void )
		{ return m_cDroidPos; }
	void SetStartPos( CPoint pt )
		{ m_cStartPos = pt; }
	CPoint GetStartPos( void )
		{ return m_cStartPos; }
	void SetDirection( BYTE btDir )
		{ m_btDirection = btDir; }
	BYTE GetDirection( void )
		{ return m_btDirection; }
	void SetMoveState( int nState )
		{ m_nMoveState = nState; }
	int GetMoveState( void )
		{ return m_nMoveState; }
	void SetMoveFrame( int nFrame )
		{ m_nMoveFrame = nFrame; }
	int GetMoveFrame( void )
		{ return m_nMoveFrame; }
	void SetForceFieldPos( CPoint, int );
	void SetMaxObjects( int nMax )
		{ m_nMaxObjects = nMax; }
	int GetMaxObjects( void )
		{ return m_nMaxObjects; }
	CPoint GetForceFieldPos( int );
	void SetTransporterPos( CPoint, int );
	CPoint GetTransporterPos( int );
	void SetPocket( int, int, int );
	BOOL GetPocket( int, int *, int * );
	BOOL ParseLevel( void );

	CCyluApp();

//	Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCyluApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

//	Implementation

	//{{AFX_MSG(CCyluApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
//	Private Data members.
	CPictureBox * m_lpCPictureBox;
	CMaze * m_lpCMaze;
	CPoint m_cScreenPos;
	CPoint m_cScreenPosOld;
	CPoint m_cStartPos;
	CPoint m_cDroidPos;
	CPoint m_cBasePos;
	CPoint m_cForcefields[MAX_FORCEFIELDS];
	CPoint m_cTransporters[MAX_TRANSPORTERS];
	int m_btDirection;
	int m_nMoveState;
	int m_nMoveFrame;
	int m_nPocketObject[5];
	int m_nPocketAttrib[5];
	int m_nMaxObjects;

public:
	CString m_cstrSSFName;

//	Private member functions
	void ShutDownCylu( void );
	BOOL InitialiseCylu( void );
};

/////////////////////////////////////////////////////////////////////////////
