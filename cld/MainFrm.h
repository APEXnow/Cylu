// MainFrm.h : interface of the CMainFrame class
//
// CLD - Cylu Level Designer
// (C) 1996-2026 Paul S.J.Millard
//

#pragma once

class CMainFrame : public CFrameWnd
{
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CToolBar    m_wndToolBar;			//	File, edit modes and view options
	CToolBar    m_wndPalette;			//	One button per object, using the 1996 tiles
	CStatusBar  m_wndStatusBar;
	CImageList  m_ilToolBar;
	CImageList  m_ilPalette;

	BOOL CreateToolBar( void );
	BOOL CreatePalette( void );

// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()
};
