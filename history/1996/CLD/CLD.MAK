# Microsoft Developer Studio Generated NMAKE File, Format Version 4.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

!IF "$(CFG)" == ""
CFG=CLD - Win32 Debug
!MESSAGE No configuration specified.  Defaulting to CLD - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "CLD - Win32 Release" && "$(CFG)" != "CLD - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "Cld.mak" CFG="CLD - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "CLD - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "CLD - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 
################################################################################
# Begin Project
# PROP Target_Last_Scanned "CLD - Win32 Debug"
RSC=rc.exe
CPP=cl.exe
MTL=mktyplib.exe

!IF  "$(CFG)" == "CLD - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
OUTDIR=.\Release
INTDIR=.\Release

ALL : "$(OUTDIR)\Cld.exe" "$(OUTDIR)\Cld.bsc"

CLEAN : 
	-@erase ".\Release\Cld.exe"
	-@erase ".\Release\SLCTDLG.obj"
	-@erase ".\Release\Cld.pch"
	-@erase ".\Release\NWMZDlg.obj"
	-@erase ".\Release\CLDDoc.obj"
	-@erase ".\Release\General.obj"
	-@erase ".\Release\CLDView.obj"
	-@erase ".\Release\MainFrm.obj"
	-@erase ".\Release\CLD.obj"
	-@erase ".\Release\StdAfx.obj"
	-@erase ".\Release\Cmaze.obj"
	-@erase ".\Release\CLD.res"
	-@erase ".\Release\ParseDlg.obj"
	-@erase ".\Release\Cld.bsc"
	-@erase ".\Release\CLD.sbr"
	-@erase ".\Release\StdAfx.sbr"
	-@erase ".\Release\MainFrm.sbr"
	-@erase ".\Release\CLDDoc.sbr"
	-@erase ".\Release\CLDView.sbr"
	-@erase ".\Release\NWMZDlg.sbr"
	-@erase ".\Release\Cmaze.sbr"
	-@erase ".\Release\SLCTDLG.sbr"
	-@erase ".\Release\General.sbr"
	-@erase ".\Release\ParseDlg.sbr"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /c
CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D\
 "_AFXDLL" /D "_MBCS" /FR"$(INTDIR)/" /Fp"$(INTDIR)/Cld.pch" /Yu"stdafx.h"\
 /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\Release/
CPP_SBRS=.\Release/
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
# ADD BASE RSC /l 0x809 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x809 /d "NDEBUG" /d "_AFXDLL"
RSC_PROJ=/l 0x809 /fo"$(INTDIR)/CLD.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Cld.bsc" 
BSC32_SBRS= \
	"$(INTDIR)/CLD.sbr" \
	"$(INTDIR)/StdAfx.sbr" \
	"$(INTDIR)/MainFrm.sbr" \
	"$(INTDIR)/CLDDoc.sbr" \
	"$(INTDIR)/CLDView.sbr" \
	"$(INTDIR)/NWMZDlg.sbr" \
	"$(INTDIR)/Cmaze.sbr" \
	"$(INTDIR)/SLCTDLG.sbr" \
	"$(INTDIR)/General.sbr" \
	"$(INTDIR)/ParseDlg.sbr"

"$(OUTDIR)\Cld.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386
LINK32_FLAGS=/nologo /subsystem:windows /incremental:no\
 /pdb:"$(OUTDIR)/Cld.pdb" /machine:I386 /out:"$(OUTDIR)/Cld.exe" 
LINK32_OBJS= \
	"$(INTDIR)/SLCTDLG.obj" \
	"$(INTDIR)/NWMZDlg.obj" \
	"$(INTDIR)/CLDDoc.obj" \
	"$(INTDIR)/General.obj" \
	"$(INTDIR)/CLDView.obj" \
	"$(INTDIR)/MainFrm.obj" \
	"$(INTDIR)/CLD.obj" \
	"$(INTDIR)/StdAfx.obj" \
	"$(INTDIR)/Cmaze.obj" \
	"$(INTDIR)/ParseDlg.obj" \
	"$(INTDIR)/CLD.res"

"$(OUTDIR)\Cld.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "CLD - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
OUTDIR=.\Debug
INTDIR=.\Debug

ALL : "$(OUTDIR)\Cld.exe" "$(OUTDIR)\Cld.bsc"

CLEAN : 
	-@erase ".\Debug\vc40.pdb"
	-@erase ".\Debug\Cld.pch"
	-@erase ".\Debug\vc40.idb"
	-@erase ".\Debug\Cld.bsc"
	-@erase ".\Debug\CLDDoc.sbr"
	-@erase ".\Debug\CLDView.sbr"
	-@erase ".\Debug\MainFrm.sbr"
	-@erase ".\Debug\SLCTDLG.sbr"
	-@erase ".\Debug\Cmaze.sbr"
	-@erase ".\Debug\StdAfx.sbr"
	-@erase ".\Debug\General.sbr"
	-@erase ".\Debug\CLD.sbr"
	-@erase ".\Debug\NWMZDlg.sbr"
	-@erase ".\Debug\ParseDlg.sbr"
	-@erase ".\Debug\Cld.exe"
	-@erase ".\Debug\NWMZDlg.obj"
	-@erase ".\Debug\CLDDoc.obj"
	-@erase ".\Debug\CLDView.obj"
	-@erase ".\Debug\MainFrm.obj"
	-@erase ".\Debug\SLCTDLG.obj"
	-@erase ".\Debug\Cmaze.obj"
	-@erase ".\Debug\StdAfx.obj"
	-@erase ".\Debug\General.obj"
	-@erase ".\Debug\CLD.obj"
	-@erase ".\Debug\CLD.res"
	-@erase ".\Debug\ParseDlg.obj"
	-@erase ".\Debug\Cld.ilk"
	-@erase ".\Debug\Cld.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /c
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /D "_AFXDLL" /D "_MBCS" /FR"$(INTDIR)/" /Fp"$(INTDIR)/Cld.pch" /Yu"stdafx.h"\
 /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.\Debug/
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32 
# ADD BASE RSC /l 0x809 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x809 /d "_DEBUG" /d "_AFXDLL"
RSC_PROJ=/l 0x809 /fo"$(INTDIR)/CLD.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Cld.bsc" 
BSC32_SBRS= \
	"$(INTDIR)/CLDDoc.sbr" \
	"$(INTDIR)/CLDView.sbr" \
	"$(INTDIR)/MainFrm.sbr" \
	"$(INTDIR)/SLCTDLG.sbr" \
	"$(INTDIR)/Cmaze.sbr" \
	"$(INTDIR)/StdAfx.sbr" \
	"$(INTDIR)/General.sbr" \
	"$(INTDIR)/CLD.sbr" \
	"$(INTDIR)/NWMZDlg.sbr" \
	"$(INTDIR)/ParseDlg.sbr"

"$(OUTDIR)\Cld.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386
LINK32_FLAGS=/nologo /subsystem:windows /incremental:yes\
 /pdb:"$(OUTDIR)/Cld.pdb" /debug /machine:I386 /out:"$(OUTDIR)/Cld.exe" 
LINK32_OBJS= \
	"$(INTDIR)/NWMZDlg.obj" \
	"$(INTDIR)/CLDDoc.obj" \
	"$(INTDIR)/CLDView.obj" \
	"$(INTDIR)/MainFrm.obj" \
	"$(INTDIR)/SLCTDLG.obj" \
	"$(INTDIR)/Cmaze.obj" \
	"$(INTDIR)/StdAfx.obj" \
	"$(INTDIR)/General.obj" \
	"$(INTDIR)/CLD.obj" \
	"$(INTDIR)/ParseDlg.obj" \
	"$(INTDIR)/CLD.res"

"$(OUTDIR)\Cld.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

################################################################################
# Begin Target

# Name "CLD - Win32 Release"
# Name "CLD - Win32 Debug"

!IF  "$(CFG)" == "CLD - Win32 Release"

!ELSEIF  "$(CFG)" == "CLD - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\ReadMe.txt

!IF  "$(CFG)" == "CLD - Win32 Release"

!ELSEIF  "$(CFG)" == "CLD - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\CLD.cpp
DEP_CPP_CLD_C=\
	".\StdAfx.h"\
	".\CLD.h"\
	".\MainFrm.h"\
	".\CLDDoc.h"\
	".\CLDView.h"\
	".\..\\cylu\\general.h"\
	".\..\\cylu\\cmaze.hpp"\
	".\..\\cylu\\common.hpp"\
	

"$(INTDIR)\CLD.obj" : $(SOURCE) $(DEP_CPP_CLD_C) "$(INTDIR)"\
 "$(INTDIR)\Cld.pch"

"$(INTDIR)\CLD.sbr" : $(SOURCE) $(DEP_CPP_CLD_C) "$(INTDIR)"\
 "$(INTDIR)\Cld.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\StdAfx.cpp
DEP_CPP_STDAF=\
	".\StdAfx.h"\
	

!IF  "$(CFG)" == "CLD - Win32 Release"

# ADD CPP /Yc"stdafx.h"

BuildCmds= \
	$(CPP) /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D\
 "_AFXDLL" /D "_MBCS" /FR"$(INTDIR)/" /Fp"$(INTDIR)/Cld.pch" /Yc"stdafx.h"\
 /Fo"$(INTDIR)/" /c $(SOURCE) \
	

"$(INTDIR)\StdAfx.obj" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\StdAfx.sbr" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\Cld.pch" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "CLD - Win32 Debug"

# ADD CPP /Yc"stdafx.h"

BuildCmds= \
	$(CPP) /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /D "_AFXDLL" /D "_MBCS" /FR"$(INTDIR)/" /Fp"$(INTDIR)/Cld.pch" /Yc"stdafx.h"\
 /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c $(SOURCE) \
	

"$(INTDIR)\StdAfx.obj" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\StdAfx.sbr" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\Cld.pch" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\MainFrm.cpp
DEP_CPP_MAINF=\
	".\StdAfx.h"\
	".\CLD.h"\
	".\MainFrm.h"\
	".\..\\cylu\\general.h"\
	".\..\\cylu\\cmaze.hpp"\
	".\..\\cylu\\common.hpp"\
	

"$(INTDIR)\MainFrm.obj" : $(SOURCE) $(DEP_CPP_MAINF) "$(INTDIR)"\
 "$(INTDIR)\Cld.pch"

"$(INTDIR)\MainFrm.sbr" : $(SOURCE) $(DEP_CPP_MAINF) "$(INTDIR)"\
 "$(INTDIR)\Cld.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\CLDDoc.cpp
DEP_CPP_CLDDO=\
	".\StdAfx.h"\
	".\CLD.h"\
	".\CLDDoc.h"\
	".\NWMZDlg.h"\
	".\..\\cylu\\cmaze.hpp"\
	".\ParseDlg.h"\
	".\..\\cylu\\general.h"\
	".\..\\cylu\\common.hpp"\
	

"$(INTDIR)\CLDDoc.obj" : $(SOURCE) $(DEP_CPP_CLDDO) "$(INTDIR)"\
 "$(INTDIR)\Cld.pch"

"$(INTDIR)\CLDDoc.sbr" : $(SOURCE) $(DEP_CPP_CLDDO) "$(INTDIR)"\
 "$(INTDIR)\Cld.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\CLDView.cpp
DEP_CPP_CLDVI=\
	".\StdAfx.h"\
	".\CLD.h"\
	".\CLDDoc.h"\
	".\CLDView.h"\
	".\SLCTDlg.h"\
	".\..\\cylu\\general.h"\
	".\..\\cylu\\cmaze.hpp"\
	".\..\\cylu\\common.hpp"\
	

"$(INTDIR)\CLDView.obj" : $(SOURCE) $(DEP_CPP_CLDVI) "$(INTDIR)"\
 "$(INTDIR)\Cld.pch"

"$(INTDIR)\CLDView.sbr" : $(SOURCE) $(DEP_CPP_CLDVI) "$(INTDIR)"\
 "$(INTDIR)\Cld.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\CLD.rc
DEP_RSC_CLD_R=\
	".\res\CLDDoc.ico"\
	".\res\Toolbar.bmp"\
	".\res\CLD.ico"\
	".\BITMAPS\BASE.bmp"\
	".\BITMAPS\CPU.bmp"\
	".\BITMAPS\FUEL.bmp"\
	".\BITMAPS\KEY.bmp"\
	".\BITMAPS\OBJECT.bmp"\
	".\BITMAPS\RFL.bmp"\
	".\BITMAPS\RTB.bmp"\
	".\BITMAPS\SHIELD.bmp"\
	".\BITMAPS\TRNSPORT.bmp"\
	".\BITMAPS\WALL.bmp"\
	".\BITMAPS\TREE.bmp"\
	".\BITMAPS\DROID.bmp"\
	".\res\CLD.rc2"\
	

"$(INTDIR)\CLD.res" : $(SOURCE) $(DEP_RSC_CLD_R) "$(INTDIR)"
   $(RSC) $(RSC_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\NWMZDlg.cpp
DEP_CPP_NWMZD=\
	".\StdAfx.h"\
	".\CLD.h"\
	".\NWMZDlg.h"\
	".\..\\cylu\\general.h"\
	".\..\\cylu\\cmaze.hpp"\
	".\..\\cylu\\common.hpp"\
	

"$(INTDIR)\NWMZDlg.obj" : $(SOURCE) $(DEP_CPP_NWMZD) "$(INTDIR)"\
 "$(INTDIR)\Cld.pch"

"$(INTDIR)\NWMZDlg.sbr" : $(SOURCE) $(DEP_CPP_NWMZD) "$(INTDIR)"\
 "$(INTDIR)\Cld.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=\Projects\Source\Cylu\Cmaze.cpp

!IF  "$(CFG)" == "CLD - Win32 Release"

DEP_CPP_CMAZE=\
	".\..\Cylu\stdafx.h"\
	".\..\\cylu\\cmaze.hpp"\
	
NODEP_CPP_CMAZE=\
	".\..\Cylu\nXPos"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\Cmaze.obj" : $(SOURCE) $(DEP_CPP_CMAZE) "$(INTDIR)"\
 "$(INTDIR)\Cld.pch"
   $(BuildCmds)

"$(INTDIR)\Cmaze.sbr" : $(SOURCE) $(DEP_CPP_CMAZE) "$(INTDIR)"\
 "$(INTDIR)\Cld.pch"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "CLD - Win32 Debug"

DEP_CPP_CMAZE=\
	".\..\Cylu\stdafx.h"\
	".\..\\cylu\\cmaze.hpp"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\Cmaze.obj" : $(SOURCE) $(DEP_CPP_CMAZE) "$(INTDIR)"\
 "$(INTDIR)\Cld.pch"
   $(BuildCmds)

"$(INTDIR)\Cmaze.sbr" : $(SOURCE) $(DEP_CPP_CMAZE) "$(INTDIR)"\
 "$(INTDIR)\Cld.pch"
   $(BuildCmds)

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\SLCTDLG.cpp
DEP_CPP_SLCTD=\
	".\StdAfx.h"\
	".\CLD.h"\
	".\SLCTDlg.h"\
	".\..\\cylu\\general.h"\
	".\..\\cylu\\cmaze.hpp"\
	".\..\\cylu\\common.hpp"\
	

"$(INTDIR)\SLCTDLG.obj" : $(SOURCE) $(DEP_CPP_SLCTD) "$(INTDIR)"\
 "$(INTDIR)\Cld.pch"

"$(INTDIR)\SLCTDLG.sbr" : $(SOURCE) $(DEP_CPP_SLCTD) "$(INTDIR)"\
 "$(INTDIR)\Cld.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=\Projects\Source\Cylu\General.cpp
DEP_CPP_GENER=\
	".\..\Cylu\stdafx.h"\
	".\..\Cylu\general.h"\
	

BuildCmds= \
	$(CPP) $(CPP_PROJ) $(SOURCE) \
	

"$(INTDIR)\General.obj" : $(SOURCE) $(DEP_CPP_GENER) "$(INTDIR)"\
 "$(INTDIR)\Cld.pch"
   $(BuildCmds)

"$(INTDIR)\General.sbr" : $(SOURCE) $(DEP_CPP_GENER) "$(INTDIR)"\
 "$(INTDIR)\Cld.pch"
   $(BuildCmds)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\ParseDlg.cpp
DEP_CPP_PARSE=\
	".\StdAfx.h"\
	".\CLD.h"\
	".\ParseDlg.h"\
	".\..\\cylu\\general.h"\
	".\..\\cylu\\cmaze.hpp"\
	".\..\\cylu\\common.hpp"\
	

"$(INTDIR)\ParseDlg.obj" : $(SOURCE) $(DEP_CPP_PARSE) "$(INTDIR)"\
 "$(INTDIR)\Cld.pch"

"$(INTDIR)\ParseDlg.sbr" : $(SOURCE) $(DEP_CPP_PARSE) "$(INTDIR)"\
 "$(INTDIR)\Cld.pch"


# End Source File
# End Target
# End Project
################################################################################
################################################################################
# Section CLD : {B6805000-A509-11CE-A5B0-00AA006BBF16}
# 	1:20:CG_IDR_POPUP_CLDVIEW:102
# End Section
################################################################################
