// Cyluview.h : interface of the CCyluView class
//
/////////////////////////////////////////////////////////////////////////////

class CCyluView : public CView
{
protected: // create from serialization only
	CCyluView();
	DECLARE_DYNCREATE(CCyluView)

// Attributes
public:
	CCyluDoc* GetDocument();

	BOOL m_bInitFailed;				//	Init flag

	//	Display mapping array, determines which sprite to display
	int m_nPlayArea[PLAYAREA_WIDTH][PLAYAREA_HEIGHT];
	int m_nGridCentre;				//	Centre of display grid
	int m_nGridTop;					//	Top of display grid
	int m_nGridLeft;				//	Left of display grid
	int m_nGridHeight;				//	Maximum height of display grid
	int m_nGridWidth;				//	Maximum width of display grid
	int m_nHorzStep;				//	Horizontal display step size
	int m_nVertStep;				//	Vertical display step size
	int m_nIndex;					//	Sprite number when building
									//	playarea
	BOOL m_bOkToUse;				//	True if sprites were created
									//	successfully
	CDC m_cdcBuffer;				//	Back buffer for game bitmaps
	CBitmap m_cBitmapBuffer;		//	Background buffer bitmap for disp
	CBitmap *m_pOldCBitmapBuffer;	//	Previous selected bitmap

	UINT_PTR m_uiFrameTimer;		//	Handle for frame display timer

	HDC m_hWinGDC;					//	WinG DC.
	HBITMAP m_hWinGBitmap;			//	WinG Back buffer bitmap
	CPalette m_cPal;				//	Application's palette
	HGLOBAL m_hgblPalette;			//	LOGPALETTE struct

	int m_nDroidHorzStep;
	int m_nDroidVertStep;

	int m_nSelPocket;

	int m_nGameState;				//	Current game status
	int m_nPrevState;				//	Previous state before any change

	BOOL m_bStillMoving;			//	Flag to determine if droid is
									//	still moving while key is pressed
									//	Set to false when key released

	int m_nFuelStatus;				//	Current Fuel status
	int m_nFuelDelay;				//	Delay between decrement
	int m_nObjectsCollected;		//	Number of objects collected.

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCyluView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL DestroyWindow();
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCyluView();

	BOOL CreateSprites( void );		//	Create handles and load images
	void DestroySprites( void );	//	Unload images and destroy handles
	void SetupPlayArea( void );		//	Setup sprite selection grid
	BOOL DoPlayAreaDisplay( void );	//	Display sprites onto screen.
	void CreateTimer( void );		//	Allocate timer for frame display
									//	updating
	void DestroyTimer( void );		//	Delete frame timer
	void CreatePalette( void );		//	Generate correct colours for app
	void DestroyPalette( void );	//	Destroy app's palette

	//	Get Screen draw position
	CPoint CalculateScreenPos( CPoint, int );
	int TranslateCell( int, int );	//	Translates cell to display bitmap
									//	number required

	//	Move droid in appropriate direction
	void MoveDroid( BYTE btDirection );
	int GetDroidFrame();			//	Get display frame for current
									//	droids state
	void DrawPockets( CDC * );		//	Draw Pocket bitmaps
	void CyclePocket( void );		//	Move pocket selector

	void DoMainTitle( CDC * );		//	Draw game title

	void DoGameOver( void );		//	Called when game is over
									//	Fuel empty or all objects
									//	collected

	BOOL InitialiseVariables();		//	Init variables
	BOOL ResetVariables();			//	Reset existing variables

	void SetGameState( int );		//	Game state functions
	int GetGameState( void );		//	...
	int PrevGameState( void );		//	...

	void DrawGameStats( void );		//	Display game statistics

	void DoTimerEvent( void );

	void PickUpObject( void );		//	Pickup an object
	void DropObject( void );		//	Drop an object
	void UseObject( void );			//	Use selected object

	void Refuel( BOOL );			//	Refuel droid
	void IssueObject( void );		//	Issue object to base computer
	void DisableShield( void );		//	Shutdown a forcefield
	void ReturnToBase( void );		//	Teleport to base computer
	void Teleport( void );			//	Teleport using a key and terminal

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CCyluView)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnNewGame();
	afx_msg void OnPauseGame();
	afx_msg void OnEndGame();
	afx_msg void OnUpdateEndGame(CCmdUI* pCmdUI);
	afx_msg void OnUpdatePauseGame(CCmdUI* pCmdUI);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in Cyluview.cpp
inline CCyluDoc* CCyluView::GetDocument()
   { return (CCyluDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////
