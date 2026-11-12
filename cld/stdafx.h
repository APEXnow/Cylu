// stdafx.h : standard system and MFC include files
//
// CLD - Cylu Level Designer
// (C) 1996-2026 Paul S.J.Millard
//

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers
#endif

#include <SDKDDKVer.h>

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions (toolbars, status bar)
#include <afxcmn.h>			// MFC support for Windows Common Controls

#include <stdio.h>
#include <deque>
#include <vector>

// Use the modern (version 6) common controls for themed buttons and lists
#if defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
