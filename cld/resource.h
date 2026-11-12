//{{NO_DEPENDENCIES}}
// Resource identifiers for CLD, the Cylu Level Designer
// Used by CLD.rc
//
// The numbers of the 1996 commands and dialogs are kept as they were.
//
#define IDD_ABOUTBOX                    100
#define IDR_MAINFRAME                   128
#define IDR_CLDTYPE                     129
#define IDD_NEWMAZE                     130
#define IDD_SELECT_OBJECT               145
#define IDD_PARSER                      147
#define IDB_TILES                       150
#define IDB_TOOLBAR                     151

#define IDC_EDIT_WIDTH                  1000
#define IDC_EDIT_HEIGHT                 1001
#define IDC_OBJECT_COMBO                1002
#define IDC_ATTRIB_SPIN                 1005
#define IDC_ATTRIB_EDIT                 1006
#define IDC_STATISTICS                  1007
#define IDC_ISSUES                      1008
#define IDC_ATTRIB_HINT                 1009

#define IDM_NEW_DIMENSIONS              32772
#define IDM_PLACE_OBJECT                32779
#define IDM_DELETE_OBJECT               32780
#define IDM_SELECT_OBJECT               32781
#define IDM_DRAW_BOX                    32782
#define IDM_DRAW_LINE                   32783
#define IDM_OBJECT_ATTRIBUTES           32784
#define IDM_PARSE_LEVEL                 32785
#define IDM_SCREEN_GRID                 32786
#define IDM_ZOOM_IN                     32790
#define IDM_ZOOM_OUT                    32791
#define IDM_TEST_LEVEL                  32792
#define IDM_ATTRIB_UP                   32793
#define IDM_ATTRIB_DOWN                 32794

// Object palette buttons, one per object number (1 = wall ... 12 = droid)
#define IDM_OBJECT_FIRST                33000
#define IDM_OBJECT_LAST                 33012

// The palette bar's ID doubles as its View menu command, so that
// CFrameWnd::OnBarCheck can show and hide it (range 0xE800 - 0xE8FF)
#define ID_VIEW_PALETTE                 0xE820

// Status bar panes
#define ID_INDICATOR_CELL               40001
#define ID_INDICATOR_SELECTED           40002

// Next default values for new objects
//
#ifdef APSTUDIO_INVOKED
#ifndef APSTUDIO_READONLY_SYMBOLS
#define _APS_NEXT_RESOURCE_VALUE        152
#define _APS_NEXT_COMMAND_VALUE         32795
#define _APS_NEXT_CONTROL_VALUE         1010
#define _APS_NEXT_SYMED_VALUE           103
#endif
#endif
