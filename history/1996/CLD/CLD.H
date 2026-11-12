// CLD.h : main header file for the CLD application
//

#define _CLD_

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"				// main symbols
#include "..\\cylu\\general.h"		//	General functions
#include "..\\cylu\\cmaze.hpp"		//	CMaze object that contains the game
									//	play maze

#include "..\\cylu\\common.hpp"		//	Include common defines and dirextives
									//	for this application

/////////////////////////////////////////////////////////////////////////////
// CCLDApp:
// See CLD.cpp for the implementation of this class
//

class CCLDApp : public CWinApp
{
public:
	void DoMessageLoop( void );
	CCLDApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCLDApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CCLDApp)
	afx_msg void OnAppAbout();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////
//	Globals
