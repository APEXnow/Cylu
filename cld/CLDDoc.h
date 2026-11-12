// CLDDoc.h : interface of the CCLDDoc class
//
// CLD - Cylu Level Designer
// (C) 1996-2026 Paul S.J.Millard
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include "../maze.h"

#define MAX_UNDO				100		//	Undo steps kept

#define CHANGE_NONE				0
#define CHANGE_CELL				1
#define CHANGE_OTHERS			2

class CCLDDoc : public CDocument
{
protected: // create from serialization only
	CCLDDoc();
	DECLARE_DYNCREATE(CCLDDoc)

// Attributes
public:
	CMaze m_cMaze;				//	CMaze object, shared with the game

// Operations
public:
	CMaze* GetMaze( void )
		{	return &m_cMaze; }
	void GetMazeSize( SIZE *lpSize );

	//	Editing.  Call BeginChange before a group of SetCell calls (one
	//	mouse stroke) and EndChange after, so the group undoes in one step.
	void BeginChange( void );
	void EndChange( BOOL bChanged );

	//	Returns CHANGE_NONE, CHANGE_CELL, or CHANGE_OTHERS when placing a
	//	droid also removed the old one (there can only be one droid)
	int SetCell( int nX, int nY, int nObject, int nAttrib );

	BOOL CanUndo( void ) const	{ return !m_Undo.empty(); }
	BOOL CanRedo( void ) const	{ return !m_Redo.empty(); }

	BOOL SaveMazeTo( LPCTSTR lpszPathName );

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	virtual void DeleteContents();

// Implementation
public:
	virtual ~CCLDDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	std::deque<CMaze> m_Undo;		//	Levels before each change, newest last
	std::deque<CMaze> m_Redo;		//	Levels undone, newest last

	void CreateBorderedMaze( int nWidth, int nHeight );

// Generated message map functions
protected:
	afx_msg void OnNewDimensions();
	afx_msg void OnUpdateFileSave(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileSaveAs(CCmdUI* pCmdUI);
	afx_msg void OnEditUndo();
	afx_msg void OnUpdateEditUndo(CCmdUI* pCmdUI);
	afx_msg void OnEditRedo();
	afx_msg void OnUpdateEditRedo(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
