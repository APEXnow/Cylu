// CLD.h : main header file for the CLD application
//
// CLD - Cylu Level Designer
// Originally written in 1996 with Visual C++ 4 and MFC,
// brought back for Visual Studio 2026 in 2026.
// (C) 1996-2026 Paul S.J.Millard
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file
#endif

#define _CLD_

#include "resource.h"				//	main symbols
#include "../maze.h"				//	CMaze, shared with the game
#include "../objects.h"				//	Object numbers, shared with the game

//	Size of one map tile in the 1996 bitmaps, before zooming
#define BITMAP_WIDTH			20
#define BITMAP_HEIGHT			20

/////////////////////////////////////////////////////////////////////////////
// CCLDApp:
// See CLD.cpp for the implementation of this class
//

class CCLDApp : public CWinApp
{
public:
	CCLDApp();

// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CCLDApp theApp;
