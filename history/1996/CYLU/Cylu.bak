# Microsoft Developer Studio Generated NMAKE File, Format Version 4.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

!IF "$(CFG)" == ""
CFG=Cylu - Win32 Debug
!MESSAGE No configuration specified.  Defaulting to Cylu - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "Cylu - Win32 Debug" && "$(CFG)" != "Cylu - Win32 Release"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "Cylu.mak" CFG="Cylu - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Cylu - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "Cylu - Win32 Release" (based on "Win32 (x86) Application")
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
# PROP Target_Last_Scanned "Cylu - Win32 Release"
MTL=mktyplib.exe
RSC=rc.exe
CPP=cl.exe

!IF  "$(CFG)" == "Cylu - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "WinDebug"
# PROP BASE Intermediate_Dir "WinDebug"
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "WinDebug"
# PROP Intermediate_Dir "WinDebug"
OUTDIR=.\WinDebug
INTDIR=.\WinDebug

ALL : "$(OUTDIR)\Cylu.exe" "$(OUTDIR)\Cylu.bsc"

CLEAN : 
	-@erase ".\WinDebug\vc40.pdb"
	-@erase ".\WinDebug\Cylu.pch"
	-@erase ".\WinDebug\vc40.idb"
	-@erase ".\WinDebug\Cylu.bsc"
	-@erase ".\WinDebug\stdafx.sbr"
	-@erase ".\WinDebug\cmaze.sbr"
	-@erase ".\WinDebug\csprite.sbr"
	-@erase ".\WinDebug\Cyluview.sbr"
	-@erase ".\WinDebug\general.sbr"
	-@erase ".\WinDebug\Cyludoc.sbr"
	-@erase ".\WinDebug\mainfrm.sbr"
	-@erase ".\WinDebug\canimate.sbr"
	-@erase ".\WinDebug\Cylu.sbr"
	-@erase ".\WinDebug\cpicbox.sbr"
	-@erase ".\WinDebug\Cylu.exe"
	-@erase ".\WinDebug\stdafx.obj"
	-@erase ".\WinDebug\cmaze.obj"
	-@erase ".\WinDebug\csprite.obj"
	-@erase ".\WinDebug\Cyluview.obj"
	-@erase ".\WinDebug\general.obj"
	-@erase ".\WinDebug\Cyludoc.obj"
	-@erase ".\WinDebug\mainfrm.obj"
	-@erase ".\WinDebug\canimate.obj"
	-@erase ".\WinDebug\Cylu.obj"
	-@erase ".\WinDebug\cpicbox.obj"
	-@erase ".\WinDebug\Cylu.res"
	-@erase ".\WinDebug\Cylu.ilk"
	-@erase ".\WinDebug\Cylu.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /MD /W3 /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /FR /Yu"stdafx.h" /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "\WING\INCLUDE" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fr /Yu"stdafx.h" /c
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /I "\WING\INCLUDE" /D "_DEBUG" /D\
 "WIN32" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fr"$(INTDIR)/"\
 /Fp"$(INTDIR)/Cylu.pch" /Yu"stdafx.h" /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\WinDebug/
CPP_SBRS=.\WinDebug/
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/Cylu.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Cylu.bsc" 
BSC32_SBRS= \
	"$(INTDIR)/stdafx.sbr" \
	"$(INTDIR)/cmaze.sbr" \
	"$(INTDIR)/csprite.sbr" \
	"$(INTDIR)/Cyluview.sbr" \
	"$(INTDIR)/general.sbr" \
	"$(INTDIR)/Cyludoc.sbr" \
	"$(INTDIR)/mainfrm.sbr" \
	"$(INTDIR)/canimate.sbr" \
	"$(INTDIR)/Cylu.sbr" \
	"$(INTDIR)/cpicbox.sbr"

"$(OUTDIR)\Cylu.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386
# SUBTRACT LINK32 /pdb:none
LINK32_FLAGS=/nologo /subsystem:windows /incremental:yes\
 /pdb:"$(OUTDIR)/Cylu.pdb" /debug /machine:I386 /out:"$(OUTDIR)/Cylu.exe" 
LINK32_OBJS= \
	"$(INTDIR)/stdafx.obj" \
	"$(INTDIR)/cmaze.obj" \
	"$(INTDIR)/csprite.obj" \
	"$(INTDIR)/Cyluview.obj" \
	"$(INTDIR)/general.obj" \
	"$(INTDIR)/Cyludoc.obj" \
	"$(INTDIR)/mainfrm.obj" \
	"$(INTDIR)/canimate.obj" \
	"$(INTDIR)/Cylu.obj" \
	"$(INTDIR)/cpicbox.obj" \
	"$(INTDIR)/Cylu.res"

"$(OUTDIR)\Cylu.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Cylu - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "WinRel"
# PROP BASE Intermediate_Dir "WinRel"
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "WinRel"
# PROP Intermediate_Dir "WinRel"
OUTDIR=.\WinRel
INTDIR=.\WinRel

ALL : "$(OUTDIR)\Cylu.exe" "$(OUTDIR)\Cylu.bsc"

CLEAN : 
	-@erase ".\WinRel\Cylu.bsc"
	-@erase ".\WinRel\canimate.sbr"
	-@erase ".\WinRel\Cylu.pch"
	-@erase ".\WinRel\mainfrm.sbr"
	-@erase ".\WinRel\Cyluview.sbr"
	-@erase ".\WinRel\Cylu.sbr"
	-@erase ".\WinRel\stdafx.sbr"
	-@erase ".\WinRel\csprite.sbr"
	-@erase ".\WinRel\cpicbox.sbr"
	-@erase ".\WinRel\cmaze.sbr"
	-@erase ".\WinRel\general.sbr"
	-@erase ".\WinRel\Cyludoc.sbr"
	-@erase ".\WinRel\Cylu.exe"
	-@erase ".\WinRel\cpicbox.obj"
	-@erase ".\WinRel\cmaze.obj"
	-@erase ".\WinRel\general.obj"
	-@erase ".\WinRel\Cyludoc.obj"
	-@erase ".\WinRel\canimate.obj"
	-@erase ".\WinRel\mainfrm.obj"
	-@erase ".\WinRel\Cyluview.obj"
	-@erase ".\WinRel\Cylu.obj"
	-@erase ".\WinRel\stdafx.obj"
	-@erase ".\WinRel\csprite.obj"
	-@erase ".\WinRel\Cylu.res"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /FR /Yu"stdafx.h" /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "\WING\INCLUDE" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fr /Yu"stdafx.h" /c
CPP_PROJ=/nologo /MD /W3 /GX /O2 /I "\WING\INCLUDE" /D "NDEBUG" /D "WIN32" /D\
 "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fr"$(INTDIR)/" /Fp"$(INTDIR)/Cylu.pch"\
 /Yu"stdafx.h" /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\WinRel/
CPP_SBRS=.\WinRel/
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/Cylu.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Cylu.bsc" 
BSC32_SBRS= \
	"$(INTDIR)/canimate.sbr" \
	"$(INTDIR)/mainfrm.sbr" \
	"$(INTDIR)/Cyluview.sbr" \
	"$(INTDIR)/Cylu.sbr" \
	"$(INTDIR)/stdafx.sbr" \
	"$(INTDIR)/csprite.sbr" \
	"$(INTDIR)/cpicbox.sbr" \
	"$(INTDIR)/cmaze.sbr" \
	"$(INTDIR)/general.sbr" \
	"$(INTDIR)/Cyludoc.sbr"

"$(OUTDIR)\Cylu.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 /nologo /subsystem:windows /machine:I386
# SUBTRACT LINK32 /pdb:none
LINK32_FLAGS=/nologo /subsystem:windows /incremental:no\
 /pdb:"$(OUTDIR)/Cylu.pdb" /machine:I386 /out:"$(OUTDIR)/Cylu.exe" 
LINK32_OBJS= \
	"$(INTDIR)/cpicbox.obj" \
	"$(INTDIR)/cmaze.obj" \
	"$(INTDIR)/general.obj" \
	"$(INTDIR)/Cyludoc.obj" \
	"$(INTDIR)/canimate.obj" \
	"$(INTDIR)/mainfrm.obj" \
	"$(INTDIR)/Cyluview.obj" \
	"$(INTDIR)/Cylu.obj" \
	"$(INTDIR)/stdafx.obj" \
	"$(INTDIR)/csprite.obj" \
	"$(INTDIR)/Cylu.res"

"$(OUTDIR)\Cylu.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

MTL_PROJ=

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

# Name "Cylu - Win32 Debug"
# Name "Cylu - Win32 Release"

!IF  "$(CFG)" == "Cylu - Win32 Debug"

!ELSEIF  "$(CFG)" == "Cylu - Win32 Release"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\stdafx.cpp
DEP_CPP_STDAF=\
	".\stdafx.h"\
	

!IF  "$(CFG)" == "Cylu - Win32 Debug"

# ADD BASE CPP /Yc"stdafx.h"
# ADD CPP /Yc"stdafx.h"

BuildCmds= \
	$(CPP) /nologo /MDd /W3 /Gm /GX /Zi /Od /I "\WING\INCLUDE" /D "_DEBUG" /D\
 "WIN32" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fr"$(INTDIR)/"\
 /Fp"$(INTDIR)/Cylu.pch" /Yc"stdafx.h" /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c\
 $(SOURCE) \
	

"$(INTDIR)\stdafx.obj" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\stdafx.sbr" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\Cylu.pch" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Cylu - Win32 Release"

# ADD BASE CPP /Yc"stdafx.h"
# ADD CPP /Yc"stdafx.h"

BuildCmds= \
	$(CPP) /nologo /MD /W3 /GX /O2 /I "\WING\INCLUDE" /D "NDEBUG" /D "WIN32" /D\
 "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fr"$(INTDIR)/" /Fp"$(INTDIR)/Cylu.pch"\
 /Yc"stdafx.h" /Fo"$(INTDIR)/" /c $(SOURCE) \
	

"$(INTDIR)\stdafx.obj" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\stdafx.sbr" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\Cylu.pch" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Cylu.cpp
DEP_CPP_CYLU_=\
	".\stdafx.h"\
	".\Cylu.h"\
	".\Mainfrm.h"\
	".\Cyludoc.h"\
	".\Cyluview.h"\
	".\Common.hpp"\
	".\General.h"\
	".\Cpicbox.hpp"\
	".\Cmaze.hpp"\
	".\Canimate.hpp"\
	".\Csprite.hpp"\
	

"$(INTDIR)\Cylu.obj" : $(SOURCE) $(DEP_CPP_CYLU_) "$(INTDIR)"\
 "$(INTDIR)\Cylu.pch"

"$(INTDIR)\Cylu.sbr" : $(SOURCE) $(DEP_CPP_CYLU_) "$(INTDIR)"\
 "$(INTDIR)\Cylu.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\mainfrm.cpp
DEP_CPP_MAINF=\
	".\stdafx.h"\
	".\Cylu.h"\
	".\Mainfrm.h"\
	".\Common.hpp"\
	".\General.h"\
	".\Cpicbox.hpp"\
	".\Cmaze.hpp"\
	".\Canimate.hpp"\
	".\Csprite.hpp"\
	

"$(INTDIR)\mainfrm.obj" : $(SOURCE) $(DEP_CPP_MAINF) "$(INTDIR)"\
 "$(INTDIR)\Cylu.pch"

"$(INTDIR)\mainfrm.sbr" : $(SOURCE) $(DEP_CPP_MAINF) "$(INTDIR)"\
 "$(INTDIR)\Cylu.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Cyludoc.cpp
DEP_CPP_CYLUD=\
	".\stdafx.h"\
	".\Cylu.h"\
	".\Cyludoc.h"\
	".\Common.hpp"\
	".\General.h"\
	".\Cpicbox.hpp"\
	".\Cmaze.hpp"\
	".\Canimate.hpp"\
	".\Csprite.hpp"\
	

"$(INTDIR)\Cyludoc.obj" : $(SOURCE) $(DEP_CPP_CYLUD) "$(INTDIR)"\
 "$(INTDIR)\Cylu.pch"

"$(INTDIR)\Cyludoc.sbr" : $(SOURCE) $(DEP_CPP_CYLUD) "$(INTDIR)"\
 "$(INTDIR)\Cylu.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Cyluview.cpp
DEP_CPP_CYLUV=\
	".\stdafx.h"\
	".\Cylu.h"\
	".\Cyludoc.h"\
	".\Cyluview.h"\
	".\Common.hpp"\
	".\General.h"\
	".\Cpicbox.hpp"\
	".\Cmaze.hpp"\
	".\Canimate.hpp"\
	".\Csprite.hpp"\
	

"$(INTDIR)\Cyluview.obj" : $(SOURCE) $(DEP_CPP_CYLUV) "$(INTDIR)"\
 "$(INTDIR)\Cylu.pch"

"$(INTDIR)\Cyluview.sbr" : $(SOURCE) $(DEP_CPP_CYLUV) "$(INTDIR)"\
 "$(INTDIR)\Cylu.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Cylu.rc
DEP_RSC_CYLU_R=\
	".\SPRITES\SPRITE\OBJECTS\Scpu.bmp"\
	".\SPRITES\SPRITE\OBJECTS\Sfuel.bmp"\
	".\SPRITES\SPRITE\OBJECTS\Srfl.bmp"\
	".\SPRITES\SPRITE\OBJECTS\Srtb.bmp"\
	".\SPRITES\MASK\OBJECTS\Mcpu.bmp"\
	".\SPRITES\MASK\OBJECTS\Mfuel.bmp"\
	".\SPRITES\MASK\OBJECTS\Mrfl.bmp"\
	".\SPRITES\MASK\OBJECTS\Mrtb.bmp"\
	".\SPRITES\SPRITE\walls\sw2.bmp"\
	".\SPRITES\SPRITE\walls\sw3.bmp"\
	".\SPRITES\SPRITE\walls\sw4.bmp"\
	".\SPRITES\SPRITE\walls\sw5.bmp"\
	".\SPRITES\SPRITE\WALLS\Stele0.bmp"\
	".\SPRITES\SPRITE\WALLS\Stele1.bmp"\
	".\SPRITES\SPRITE\WALLS\Stele2.bmp"\
	".\SPRITES\MASK\WALLS\mw2.bmp"\
	".\SPRITES\MASK\WALLS\mw3.bmp"\
	".\SPRITES\MASK\WALLS\mw4.bmp"\
	".\SPRITES\MASK\WALLS\mw5.bmp"\
	".\SPRITES\MASK\WALLS\mtele0.bmp"\
	".\SPRITES\MASK\WALLS\mtele1.bmp"\
	".\SPRITES\MASK\WALLS\mtele2.bmp"\
	".\SPRITES\SPRITE\WALLS\stree0.bmp"\
	".\SPRITES\SPRITE\WALLS\stree1.bmp"\
	".\SPRITES\SPRITE\WALLS\stree2.bmp"\
	".\SPRITES\SPRITE\WALLS\stree3.bmp"\
	".\SPRITES\MASK\WALLS\mtree0.bmp"\
	".\SPRITES\MASK\WALLS\mtree1.bmp"\
	".\SPRITES\MASK\WALLS\mtree2.bmp"\
	".\SPRITES\MASK\WALLS\mtree3.bmp"\
	".\SPRITES\SPRITE\OBJECTS\Ssphere.bmp"\
	".\SPRITES\MASK\OBJECTS\Msphere.bmp"\
	".\SPRITES\MASK\OBJECTS\mbook.bmp"\
	".\SPRITES\SPRITE\OBJECTS\sbook.bmp"\
	".\SPRITES\SPRITE\OBJECTS\scalc.bmp"\
	".\SPRITES\MASK\OBJECTS\mcalc.bmp"\
	".\SPRITES\MASK\OBJECTS\mdisk.bmp"\
	".\SPRITES\SPRITE\OBJECTS\sdisk.bmp"\
	".\SPRITES\SPRITE\OBJECTS\spencil.bmp"\
	".\SPRITES\MASK\OBJECTS\mpencil.bmp"\
	".\SPRITES\SPRITE\walls\sw6.bmp"\
	".\SPRITES\MASK\walls\mw6.bmp"\
	".\SPRITES\SPRITE\walls\sc.bmp"\
	".\SPRITES\SPRITE\walls\sy.bmp"\
	".\SPRITES\SPRITE\walls\sl.bmp"\
	".\SPRITES\SPRITE\walls\su.bmp"\
	".\SPRITES\MASK\walls\mc.bmp"\
	".\SPRITES\MASK\walls\my.bmp"\
	".\SPRITES\MASK\walls\ml.bmp"\
	".\SPRITES\MASK\walls\mu.bmp"\
	".\SPRITES\SPRITE\walls\scmptr1.bmp"\
	".\SPRITES\SPRITE\walls\scmptr2.bmp"\
	".\SPRITES\SPRITE\walls\scmptr3.bmp"\
	".\SPRITES\SPRITE\walls\scmptr4.bmp"\
	".\SPRITES\MASK\walls\mcmptr.bmp"\
	".\SPRITES\SPRITE\walls\Sw0.bmp"\
	".\SPRITES\MASK\walls\Mw0.bmp"\
	".\SPRITES\SPRITE\walls\Sw1.bmp"\
	".\SPRITES\MASK\walls\Mw1.bmp"\
	".\SPRITES\SPRITE\OBJECTS\skey.bmp"\
	".\SPRITES\MASK\OBJECTS\mkey.bmp"\
	".\SPRITES\SPRITE\OBJECTS\sbottle.bmp"\
	".\SPRITES\MASK\OBJECTS\mbottle.bmp"\
	".\SPRITES\SPRITE\WALLS\sfield0.bmp"\
	".\SPRITES\SPRITE\WALLS\sfield1.bmp"\
	".\SPRITES\SPRITE\WALLS\sfield2.bmp"\
	".\SPRITES\SPRITE\WALLS\sfield3.bmp"\
	".\SPRITES\MASK\WALLS\mfield0.bmp"\
	".\SPRITES\MASK\WALLS\mfield1.bmp"\
	".\SPRITES\MASK\WALLS\mfield2.bmp"\
	".\SPRITES\MASK\WALLS\mfield3.bmp"\
	".\SPRITES\MASK\WALLS\mw7.bmp"\
	".\SPRITES\SPRITE\walls\sw8.bmp"\
	".\SPRITES\MASK\walls\Mw8.bmp"\
	".\SPRITES\SPRITE\walls\SW7.BMP"\
	".\SPRITES\SPRITE\walls\sw9.bmp"\
	".\SPRITES\MASK\walls\mw9.bmp"\
	".\SPRITES\SPRITE\walls\sw10.bmp"\
	".\SPRITES\MASK\walls\mw10.bmp"\
	".\SPRITES\SPRITE\droid\Sdroidn0.bmp"\
	".\SPRITES\SPRITE\droid\Sdroidn1.bmp"\
	".\SPRITES\SPRITE\droid\Sdroide0.bmp"\
	".\SPRITES\SPRITE\droid\Sdroide1.bmp"\
	".\SPRITES\SPRITE\droid\Sdroids0.bmp"\
	".\SPRITES\SPRITE\droid\Sdroids1.bmp"\
	".\SPRITES\SPRITE\droid\Sdroidw0.bmp"\
	".\SPRITES\SPRITE\droid\Sdroidw1.bmp"\
	".\SPRITES\MASK\droid\Mdroidn0.bmp"\
	".\SPRITES\MASK\droid\Mdroidn1.bmp"\
	".\SPRITES\MASK\droid\Mdroide0.bmp"\
	".\SPRITES\MASK\droid\Mdroide1.bmp"\
	".\SPRITES\MASK\droid\Mdroids0.bmp"\
	".\SPRITES\MASK\droid\Mdroids1.bmp"\
	".\SPRITES\MASK\droid\Mdroidw0.bmp"\
	".\SPRITES\MASK\droid\Mdroidw1.bmp"\
	".\SPRITES\SPRITE\walls\sw11.bmp"\
	".\SPRITES\SPRITE\walls\sw12.bmp"\
	".\SPRITES\MASK\walls\mw11.bmp"\
	".\SPRITES\MASK\walls\mw12.bmp"\
	".\SPRITES\SPRITE\WALLS\sw13.bmp"\
	".\SPRITES\MASK\walls\mw13.bmp"\
	".\SPRITES\SPRITE\WALLS\sw14.bmp"\
	".\SPRITES\MASK\walls\mw14.bmp"\
	".\SPRITES\Objbord.bmp"\
	".\SPRITES\LOGO.bmp"\
	".\SPRITES\obsel.bmp"\
	".\res\Cylu.ico"\
	".\res\Cylu.rc2"\
	

"$(INTDIR)\Cylu.res" : $(SOURCE) $(DEP_RSC_CYLU_R) "$(INTDIR)"
   $(RSC) $(RSC_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\readme.txt

!IF  "$(CFG)" == "Cylu - Win32 Debug"

!ELSEIF  "$(CFG)" == "Cylu - Win32 Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\csprite.cpp
DEP_CPP_CSPRI=\
	".\stdafx.h"\
	".\Csprite.hpp"\
	

"$(INTDIR)\csprite.obj" : $(SOURCE) $(DEP_CPP_CSPRI) "$(INTDIR)"\
 "$(INTDIR)\Cylu.pch"

"$(INTDIR)\csprite.sbr" : $(SOURCE) $(DEP_CPP_CSPRI) "$(INTDIR)"\
 "$(INTDIR)\Cylu.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\canimate.cpp
DEP_CPP_CANIM=\
	".\stdafx.h"\
	".\Csprite.hpp"\
	".\Canimate.hpp"\
	

"$(INTDIR)\canimate.obj" : $(SOURCE) $(DEP_CPP_CANIM) "$(INTDIR)"\
 "$(INTDIR)\Cylu.pch"

"$(INTDIR)\canimate.sbr" : $(SOURCE) $(DEP_CPP_CANIM) "$(INTDIR)"\
 "$(INTDIR)\Cylu.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\cpicbox.cpp
DEP_CPP_CPICB=\
	".\stdafx.h"\
	".\Cylu.h"\
	".\Cpicbox.hpp"\
	".\Common.hpp"\
	".\General.h"\
	".\Cmaze.hpp"\
	".\Canimate.hpp"\
	".\Csprite.hpp"\
	

"$(INTDIR)\cpicbox.obj" : $(SOURCE) $(DEP_CPP_CPICB) "$(INTDIR)"\
 "$(INTDIR)\Cylu.pch"

"$(INTDIR)\cpicbox.sbr" : $(SOURCE) $(DEP_CPP_CPICB) "$(INTDIR)"\
 "$(INTDIR)\Cylu.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\general.cpp
DEP_CPP_GENER=\
	".\stdafx.h"\
	".\General.h"\
	

"$(INTDIR)\general.obj" : $(SOURCE) $(DEP_CPP_GENER) "$(INTDIR)"\
 "$(INTDIR)\Cylu.pch"

"$(INTDIR)\general.sbr" : $(SOURCE) $(DEP_CPP_GENER) "$(INTDIR)"\
 "$(INTDIR)\Cylu.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\cmaze.cpp
DEP_CPP_CMAZE=\
	".\stdafx.h"\
	".\..\\cylu\\cmaze.hpp"\
	

"$(INTDIR)\cmaze.obj" : $(SOURCE) $(DEP_CPP_CMAZE) "$(INTDIR)"\
 "$(INTDIR)\Cylu.pch"

"$(INTDIR)\cmaze.sbr" : $(SOURCE) $(DEP_CPP_CMAZE) "$(INTDIR)"\
 "$(INTDIR)\Cylu.pch"


# End Source File
# End Target
# End Project
################################################################################
