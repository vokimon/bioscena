# Microsoft Developer Studio Generated NMAKE File, Based on Bioscena.dsp
!IF "$(CFG)" == ""
CFG=Bioscena - Win32 Debug
!MESSAGE No configuration specified. Defaulting to Bioscena - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "Bioscena - Win32 Release" && "$(CFG)" !=\
 "Bioscena - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Bioscena.mak" CFG="Bioscena - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Bioscena - Win32 Release" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "Bioscena - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Bioscena - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\Bioscena.exe"

!ELSE 

ALL : "$(OUTDIR)\Bioscena.exe"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\AppInfo.obj"
	-@erase "$(INTDIR)\Biotop.obj"
	-@erase "$(INTDIR)\BiotopToroidal.obj"
	-@erase "$(INTDIR)\Color.obj"
	-@erase "$(INTDIR)\Encaix.obj"
	-@erase "$(INTDIR)\GeneradorMascares.obj"
	-@erase "$(INTDIR)\MainBio1.obj"
	-@erase "$(INTDIR)\Missatger.obj"
	-@erase "$(INTDIR)\RandomStream.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(OUTDIR)\Bioscena.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D\
 "_MBCS" /Fp"$(INTDIR)\Bioscena.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD\
 /c 
CPP_OBJS=.\Release/
CPP_SBRS=.
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Bioscena.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /subsystem:console /incremental:no\
 /pdb:"$(OUTDIR)\Bioscena.pdb" /machine:I386 /out:"$(OUTDIR)\Bioscena.exe" 
LINK32_OBJS= \
	"$(INTDIR)\AppInfo.obj" \
	"$(INTDIR)\Biotop.obj" \
	"$(INTDIR)\BiotopToroidal.obj" \
	"$(INTDIR)\Color.obj" \
	"$(INTDIR)\Encaix.obj" \
	"$(INTDIR)\GeneradorMascares.obj" \
	"$(INTDIR)\MainBio1.obj" \
	"$(INTDIR)\Missatger.obj" \
	"$(INTDIR)\RandomStream.obj"

"$(OUTDIR)\Bioscena.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Bioscena - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\Bioscena.exe" "$(OUTDIR)\Bioscena.bsc"

!ELSE 

ALL : "$(OUTDIR)\Bioscena.exe" "$(OUTDIR)\Bioscena.bsc"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\AppInfo.obj"
	-@erase "$(INTDIR)\AppInfo.sbr"
	-@erase "$(INTDIR)\Biotop.obj"
	-@erase "$(INTDIR)\Biotop.sbr"
	-@erase "$(INTDIR)\BiotopToroidal.obj"
	-@erase "$(INTDIR)\BiotopToroidal.sbr"
	-@erase "$(INTDIR)\Color.obj"
	-@erase "$(INTDIR)\Color.sbr"
	-@erase "$(INTDIR)\Encaix.obj"
	-@erase "$(INTDIR)\Encaix.sbr"
	-@erase "$(INTDIR)\GeneradorMascares.obj"
	-@erase "$(INTDIR)\GeneradorMascares.sbr"
	-@erase "$(INTDIR)\MainBio1.obj"
	-@erase "$(INTDIR)\MainBio1.sbr"
	-@erase "$(INTDIR)\Missatger.obj"
	-@erase "$(INTDIR)\Missatger.sbr"
	-@erase "$(INTDIR)\RandomStream.obj"
	-@erase "$(INTDIR)\RandomStream.sbr"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\vc50.pdb"
	-@erase "$(OUTDIR)\Bioscena.bsc"
	-@erase "$(OUTDIR)\Bioscena.exe"
	-@erase "$(OUTDIR)\Bioscena.ilk"
	-@erase "$(OUTDIR)\Bioscena.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MLd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE"\
 /D "_MBCS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Bioscena.pch" /YX /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.\Debug/
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Bioscena.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\AppInfo.sbr" \
	"$(INTDIR)\Biotop.sbr" \
	"$(INTDIR)\BiotopToroidal.sbr" \
	"$(INTDIR)\Color.sbr" \
	"$(INTDIR)\Encaix.sbr" \
	"$(INTDIR)\GeneradorMascares.sbr" \
	"$(INTDIR)\MainBio1.sbr" \
	"$(INTDIR)\Missatger.sbr" \
	"$(INTDIR)\RandomStream.sbr"

"$(OUTDIR)\Bioscena.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)\Bioscena.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)\Bioscena.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\AppInfo.obj" \
	"$(INTDIR)\Biotop.obj" \
	"$(INTDIR)\BiotopToroidal.obj" \
	"$(INTDIR)\Color.obj" \
	"$(INTDIR)\Encaix.obj" \
	"$(INTDIR)\GeneradorMascares.obj" \
	"$(INTDIR)\MainBio1.obj" \
	"$(INTDIR)\Missatger.obj" \
	"$(INTDIR)\RandomStream.obj"

"$(OUTDIR)\Bioscena.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

.c{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<


!IF "$(CFG)" == "Bioscena - Win32 Release" || "$(CFG)" ==\
 "Bioscena - Win32 Debug"
SOURCE=.\AppInfo.cpp

!IF  "$(CFG)" == "Bioscena - Win32 Release"

DEP_CPP_APPIN=\
	".\AppInfo.h"\
	".\build.h"\
	".\Missatger.h"\
	".\Outputer.h"\
	

"$(INTDIR)\AppInfo.obj" : $(SOURCE) $(DEP_CPP_APPIN) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Bioscena - Win32 Debug"

DEP_CPP_APPIN=\
	".\AppInfo.h"\
	".\build.h"\
	".\Missatger.h"\
	".\Outputer.h"\
	

"$(INTDIR)\AppInfo.obj"	"$(INTDIR)\AppInfo.sbr" : $(SOURCE) $(DEP_CPP_APPIN)\
 "$(INTDIR)"


!ENDIF 

SOURCE=.\Biotop.cpp

!IF  "$(CFG)" == "Bioscena - Win32 Release"

DEP_CPP_BIOTO=\
	".\BioIncludes.h"\
	".\Biotop.h"\
	".\Missatger.h"\
	".\Outputer.h"\
	".\portable.h"\
	".\RandomStream.h"\
	

"$(INTDIR)\Biotop.obj" : $(SOURCE) $(DEP_CPP_BIOTO) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Bioscena - Win32 Debug"

DEP_CPP_BIOTO=\
	".\BioIncludes.h"\
	".\Biotop.h"\
	".\Missatger.h"\
	".\Outputer.h"\
	".\portable.h"\
	".\RandomStream.h"\
	

"$(INTDIR)\Biotop.obj"	"$(INTDIR)\Biotop.sbr" : $(SOURCE) $(DEP_CPP_BIOTO)\
 "$(INTDIR)"


!ENDIF 

SOURCE=.\BiotopToroidal.cpp

!IF  "$(CFG)" == "Bioscena - Win32 Release"

DEP_CPP_BIOTOP=\
	".\BioIncludes.h"\
	".\Biotop.h"\
	".\BiotopToroidal.h"\
	".\Color.h"\
	".\Missatger.h"\
	".\Outputer.h"\
	".\portable.h"\
	".\RandomStream.h"\
	

"$(INTDIR)\BiotopToroidal.obj" : $(SOURCE) $(DEP_CPP_BIOTOP) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Bioscena - Win32 Debug"

DEP_CPP_BIOTOP=\
	".\BioIncludes.h"\
	".\Biotop.h"\
	".\BiotopToroidal.h"\
	".\Missatger.h"\
	".\Outputer.h"\
	".\portable.h"\
	".\RandomStream.h"\
	

"$(INTDIR)\BiotopToroidal.obj"	"$(INTDIR)\BiotopToroidal.sbr" : $(SOURCE)\
 $(DEP_CPP_BIOTOP) "$(INTDIR)"


!ENDIF 

SOURCE=.\Color.cpp

!IF  "$(CFG)" == "Bioscena - Win32 Release"

DEP_CPP_COLOR=\
	".\BioIncludes.h"\
	".\Color.h"\
	".\Missatger.h"\
	".\Outputer.h"\
	".\portable.h"\
	

"$(INTDIR)\Color.obj" : $(SOURCE) $(DEP_CPP_COLOR) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Bioscena - Win32 Debug"


"$(INTDIR)\Color.obj"	"$(INTDIR)\Color.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\Encaix.cpp

!IF  "$(CFG)" == "Bioscena - Win32 Release"

DEP_CPP_ENCAI=\
	".\BioIncludes.h"\
	".\Encaix.h"\
	".\GeneradorMascares.h"\
	".\Missatger.h"\
	".\Outputer.h"\
	".\portable.h"\
	

"$(INTDIR)\Encaix.obj" : $(SOURCE) $(DEP_CPP_ENCAI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Bioscena - Win32 Debug"

DEP_CPP_ENCAI=\
	".\BioIncludes.h"\
	".\Encaix.h"\
	".\GeneradorMascares.h"\
	".\Missatger.h"\
	".\Outputer.h"\
	".\portable.h"\
	

"$(INTDIR)\Encaix.obj"	"$(INTDIR)\Encaix.sbr" : $(SOURCE) $(DEP_CPP_ENCAI)\
 "$(INTDIR)"


!ENDIF 

SOURCE=.\GeneradorMascares.cpp

!IF  "$(CFG)" == "Bioscena - Win32 Release"

DEP_CPP_GENER=\
	".\BioIncludes.h"\
	".\GeneradorMascares.h"\
	".\Missatger.h"\
	".\Outputer.h"\
	".\portable.h"\
	

"$(INTDIR)\GeneradorMascares.obj" : $(SOURCE) $(DEP_CPP_GENER) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Bioscena - Win32 Debug"

DEP_CPP_GENER=\
	".\BioIncludes.h"\
	".\GeneradorMascares.h"\
	".\Missatger.h"\
	".\Outputer.h"\
	".\portable.h"\
	

"$(INTDIR)\GeneradorMascares.obj"	"$(INTDIR)\GeneradorMascares.sbr" : $(SOURCE)\
 $(DEP_CPP_GENER) "$(INTDIR)"


!ENDIF 

SOURCE=.\MainBio1.cpp

!IF  "$(CFG)" == "Bioscena - Win32 Release"

DEP_CPP_MAINB=\
	".\BioIncludes.h"\
	".\Biotop.h"\
	".\BiotopToroidal.h"\
	".\Color.h"\
	".\Encaix.h"\
	".\GeneradorMascares.h"\
	".\Missatger.h"\
	".\Outputer.h"\
	".\portable.h"\
	".\RandomStream.h"\
	

"$(INTDIR)\MainBio1.obj" : $(SOURCE) $(DEP_CPP_MAINB) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Bioscena - Win32 Debug"

DEP_CPP_MAINB=\
	".\BioIncludes.h"\
	".\Biotop.h"\
	".\BiotopToroidal.h"\
	".\Cella.h"\
	".\Encaix.h"\
	".\GeneradorMascares.h"\
	".\Missatger.h"\
	".\Outputer.h"\
	".\portable.h"\
	".\RandomStream.h"\
	

"$(INTDIR)\MainBio1.obj"	"$(INTDIR)\MainBio1.sbr" : $(SOURCE) $(DEP_CPP_MAINB)\
 "$(INTDIR)"


!ENDIF 

SOURCE=.\Missatger.cpp

!IF  "$(CFG)" == "Bioscena - Win32 Release"

DEP_CPP_MISSA=\
	".\Missatger.h"\
	".\Outputer.h"\
	

"$(INTDIR)\Missatger.obj" : $(SOURCE) $(DEP_CPP_MISSA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Bioscena - Win32 Debug"

DEP_CPP_MISSA=\
	".\Missatger.h"\
	".\Outputer.h"\
	

"$(INTDIR)\Missatger.obj"	"$(INTDIR)\Missatger.sbr" : $(SOURCE)\
 $(DEP_CPP_MISSA) "$(INTDIR)"


!ENDIF 

SOURCE=.\RandomStream.cpp

!IF  "$(CFG)" == "Bioscena - Win32 Release"

DEP_CPP_RANDO=\
	".\BioIncludes.h"\
	".\Missatger.h"\
	".\Outputer.h"\
	".\portable.h"\
	".\RandomStream.h"\
	

"$(INTDIR)\RandomStream.obj" : $(SOURCE) $(DEP_CPP_RANDO) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Bioscena - Win32 Debug"

DEP_CPP_RANDO=\
	".\BioIncludes.h"\
	".\Missatger.h"\
	".\Outputer.h"\
	".\portable.h"\
	".\RandomStream.h"\
	

"$(INTDIR)\RandomStream.obj"	"$(INTDIR)\RandomStream.sbr" : $(SOURCE)\
 $(DEP_CPP_RANDO) "$(INTDIR)"


!ENDIF 


!ENDIF 

