; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CMainFrame
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "CLD.h"
LastPage=0

ClassCount=8
Class1=CCLDApp
Class2=CCLDDoc
Class3=CCLDView
Class4=CMainFrame

ResourceCount=7
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Class5=CAboutDlg
Resource3=IDD_PARSER
Resource4=IDD_SELECT_OBJECT
Class6=CNewMazeDlg
Resource5=IDD_ABOUTBOX (English (U.S.))
Class7=CSelectObjDlg
Resource6=IDD_NEWMAZE
Class8=CParseDlg
Resource7=IDR_MAINFRAME (English (U.S.))

[CLS:CCLDApp]
Type=0
HeaderFile=CLD.h
ImplementationFile=CLD.cpp
Filter=N
BaseClass=CWinApp
VirtualFilter=AC
LastObject=CCLDApp

[CLS:CCLDDoc]
Type=0
HeaderFile=CLDDoc.h
ImplementationFile=CLDDoc.cpp
Filter=N
BaseClass=CDocument
VirtualFilter=DC
LastObject=CCLDDoc

[CLS:CCLDView]
Type=0
HeaderFile=CLDView.h
ImplementationFile=CLDView.cpp
Filter=C
LastObject=CCLDView
BaseClass=CScrollView
VirtualFilter=VWC

[CLS:CMainFrame]
Type=0
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=T
LastObject=CMainFrame



[CLS:CAboutDlg]
Type=0
HeaderFile=CLD.cpp
ImplementationFile=CLD.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308352
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889
Class=CAboutDlg

[MNU:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command3=ID_FILE_SAVE
Command4=ID_FILE_SAVE_AS
Command5=ID_FILE_MRU_FILE1
Command6=ID_APP_EXIT
Command10=ID_EDIT_PASTE
Command11=ID_VIEW_TOOLBAR
Command12=ID_APP_ABOUT
CommandCount=12
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command7=ID_EDIT_UNDO
Command8=ID_EDIT_CUT
Command9=ID_EDIT_COPY

[ACL:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command5=ID_EDIT_CUT
Command6=ID_EDIT_COPY
Command7=ID_EDIT_PASTE
Command8=ID_EDIT_UNDO
Command9=ID_EDIT_CUT
Command10=ID_EDIT_COPY
Command11=ID_EDIT_PASTE
Command12=ID_NEXT_PANE
CommandCount=13
Command4=ID_EDIT_UNDO
Command13=ID_PREV_PANE


[DLG:IDD_NEWMAZE]
Type=1
Class=CNewMazeDlg
ControlCount=8
Control1=IDC_EDIT_WIDTH,edit,1350631552
Control2=IDC_EDIT_HEIGHT,edit,1350631552
Control3=IDOK,button,1342242817
Control4=IDCANCEL,button,1342242816
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC,static,1342177298
Control7=IDC_STATIC,static,1342308352
Control8=IDC_STATIC,static,1342308352

[TB:IDR_MAINFRAME (English (U.S.))]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_EDIT_CUT
Command5=ID_EDIT_COPY
Command6=ID_EDIT_PASTE
Command7=ID_FILE_PRINT
Command8=ID_APP_ABOUT
CommandCount=8

[MNU:IDR_MAINFRAME (English (U.S.))]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_SAVE_AS
Command5=ID_APP_EXIT
Command6=IDM_NEW_DIMENSIONS
Command7=IDM_PLACE_OBJECT
Command8=IDM_DELETE_OBJECT
Command9=IDM_SELECT_OBJECT
Command10=IDM_DRAW_BOX
Command11=IDM_DRAW_LINE
Command12=IDM_PARSE_LEVEL
Command13=ID_VIEW_TOOLBAR
Command14=IDM_SCREEN_GRID
Command15=IDM_OBJECT_ATTRIBUTES
Command16=ID_APP_ABOUT
CommandCount=16

[ACL:IDR_MAINFRAME (English (U.S.))]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_EDIT_UNDO
Command5=ID_EDIT_CUT
Command6=ID_EDIT_COPY
Command7=ID_EDIT_PASTE
Command8=ID_EDIT_UNDO
Command9=ID_EDIT_CUT
Command10=ID_EDIT_COPY
Command11=ID_EDIT_PASTE
Command12=ID_NEXT_PANE
Command13=ID_PREV_PANE
CommandCount=13

[DLG:IDD_ABOUTBOX (English (U.S.))]
Type=1
Class=?
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[CLS:CNewMazeDlg]
Type=0
HeaderFile=NWMZDlg.h
ImplementationFile=NWMZDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CNewMazeDlg

[DLG:IDD_SELECT_OBJECT]
Type=1
Class=CSelectObjDlg
ControlCount=7
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_OBJECT_COMBO,combobox,1344339969
Control4=IDC_ATTRIB_EDIT,edit,1350633600
Control5=IDC_ATTRIB_SPIN,msctls_updown32,1342177338
Control6=IDC_STATIC,static,1342308352
Control7=IDC_STATIC,static,1342308352

[CLS:CSelectObjDlg]
Type=0
HeaderFile=SLCTDlg.h
ImplementationFile=SLCTDLG.cpp
BaseClass=CDialog
Filter=D
LastObject=CSelectObjDlg
VirtualFilter=dWC

[DLG:IDD_PARSER]
Type=1
Class=CParseDlg
ControlCount=3
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATISTICS,static,1350701056

[CLS:CParseDlg]
Type=0
HeaderFile=ParseDlg.h
ImplementationFile=ParseDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CParseDlg

