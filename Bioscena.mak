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
	-@erase "$(INTDIR)\Actuador.obj"
	-@erase "$(INTDIR)\Actuadors.obj"
	-@erase "$(INTDIR)\Agent.obj"
	-@erase "$(INTDIR)\Aleaturitzador.obj"
	-@erase "$(INTDIR)\AppInfo.obj"
	-@erase "$(INTDIR)\Biosistema.obj"
	-@erase "$(INTDIR)\Color.obj"
	-@erase "$(INTDIR)\Compatibilitat.obj"
	-@erase "$(INTDIR)\Comunitat.obj"
	-@erase "$(INTDIR)\Configuracio.obj"
	-@erase "$(INTDIR)\Cromosoma.obj"
	-@erase "$(INTDIR)\Detector.obj"
	-@erase "$(INTDIR)\EnergiaDisipable.obj"
	-@erase "$(INTDIR)\FuncioAgent.obj"
	-@erase "$(INTDIR)\Gen.obj"
	-@erase "$(INTDIR)\GeneradorMascares.obj"
	-@erase "$(INTDIR)\InfoOrganisme.obj"
	-@erase "$(INTDIR)\Iterador.obj"
	-@erase "$(INTDIR)\Itineraris.obj"
	-@erase "$(INTDIR)\LlistaEstatica.obj"
	-@erase "$(INTDIR)\MainBio1.obj"
	-@erase "$(INTDIR)\Missatger.obj"
	-@erase "$(INTDIR)\MultiAgent.obj"
	-@erase "$(INTDIR)\Organisme.obj"
	-@erase "$(INTDIR)\Posicionador.obj"
	-@erase "$(INTDIR)\RandomStream.obj"
	-@erase "$(INTDIR)\Substrat.obj"
	-@erase "$(INTDIR)\Taxo.obj"
	-@erase "$(INTDIR)\Taxonomista.obj"
	-@erase "$(INTDIR)\Temporitzador.obj"
	-@erase "$(INTDIR)\Topologia.obj"
	-@erase "$(INTDIR)\TopologiaToroidal.obj"
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
	"$(INTDIR)\Actuador.obj" \
	"$(INTDIR)\Actuadors.obj" \
	"$(INTDIR)\Agent.obj" \
	"$(INTDIR)\Aleaturitzador.obj" \
	"$(INTDIR)\AppInfo.obj" \
	"$(INTDIR)\Biosistema.obj" \
	"$(INTDIR)\Color.obj" \
	"$(INTDIR)\Compatibilitat.obj" \
	"$(INTDIR)\Comunitat.obj" \
	"$(INTDIR)\Configuracio.obj" \
	"$(INTDIR)\Cromosoma.obj" \
	"$(INTDIR)\Detector.obj" \
	"$(INTDIR)\EnergiaDisipable.obj" \
	"$(INTDIR)\FuncioAgent.obj" \
	"$(INTDIR)\Gen.obj" \
	"$(INTDIR)\GeneradorMascares.obj" \
	"$(INTDIR)\InfoOrganisme.obj" \
	"$(INTDIR)\Iterador.obj" \
	"$(INTDIR)\Itineraris.obj" \
	"$(INTDIR)\LlistaEstatica.obj" \
	"$(INTDIR)\MainBio1.obj" \
	"$(INTDIR)\Missatger.obj" \
	"$(INTDIR)\MultiAgent.obj" \
	"$(INTDIR)\Organisme.obj" \
	"$(INTDIR)\Posicionador.obj" \
	"$(INTDIR)\RandomStream.obj" \
	"$(INTDIR)\Substrat.obj" \
	"$(INTDIR)\Taxo.obj" \
	"$(INTDIR)\Taxonomista.obj" \
	"$(INTDIR)\Temporitzador.obj" \
	"$(INTDIR)\Topologia.obj" \
	"$(INTDIR)\TopologiaToroidal.obj"

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

ALL : "$(OUTDIR)\Bioscena.exe"

!ELSE 

ALL : "$(OUTDIR)\Bioscena.exe"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\Actuador.obj"
	-@erase "$(INTDIR)\Actuadors.obj"
	-@erase "$(INTDIR)\Agent.obj"
	-@erase "$(INTDIR)\Aleaturitzador.obj"
	-@erase "$(INTDIR)\AppInfo.obj"
	-@erase "$(INTDIR)\Biosistema.obj"
	-@erase "$(INTDIR)\Color.obj"
	-@erase "$(INTDIR)\Compatibilitat.obj"
	-@erase "$(INTDIR)\Comunitat.obj"
	-@erase "$(INTDIR)\Configuracio.obj"
	-@erase "$(INTDIR)\Cromosoma.obj"
	-@erase "$(INTDIR)\Detector.obj"
	-@erase "$(INTDIR)\EnergiaDisipable.obj"
	-@erase "$(INTDIR)\FuncioAgent.obj"
	-@erase "$(INTDIR)\Gen.obj"
	-@erase "$(INTDIR)\GeneradorMascares.obj"
	-@erase "$(INTDIR)\InfoOrganisme.obj"
	-@erase "$(INTDIR)\Iterador.obj"
	-@erase "$(INTDIR)\Itineraris.obj"
	-@erase "$(INTDIR)\LlistaEstatica.obj"
	-@erase "$(INTDIR)\MainBio1.obj"
	-@erase "$(INTDIR)\Missatger.obj"
	-@erase "$(INTDIR)\MultiAgent.obj"
	-@erase "$(INTDIR)\Organisme.obj"
	-@erase "$(INTDIR)\Posicionador.obj"
	-@erase "$(INTDIR)\RandomStream.obj"
	-@erase "$(INTDIR)\Substrat.obj"
	-@erase "$(INTDIR)\Taxo.obj"
	-@erase "$(INTDIR)\Taxonomista.obj"
	-@erase "$(INTDIR)\Temporitzador.obj"
	-@erase "$(INTDIR)\Topologia.obj"
	-@erase "$(INTDIR)\TopologiaToroidal.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\vc50.pdb"
	-@erase "$(OUTDIR)\Bioscena.exe"
	-@erase "$(OUTDIR)\Bioscena.ilk"
	-@erase "$(OUTDIR)\Bioscena.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MLd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE"\
 /D "_MBCS" /Fp"$(INTDIR)\Bioscena.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\"\
 /FD /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Bioscena.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)\Bioscena.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)\Bioscena.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\Actuador.obj" \
	"$(INTDIR)\Actuadors.obj" \
	"$(INTDIR)\Agent.obj" \
	"$(INTDIR)\Aleaturitzador.obj" \
	"$(INTDIR)\AppInfo.obj" \
	"$(INTDIR)\Biosistema.obj" \
	"$(INTDIR)\Color.obj" \
	"$(INTDIR)\Compatibilitat.obj" \
	"$(INTDIR)\Comunitat.obj" \
	"$(INTDIR)\Configuracio.obj" \
	"$(INTDIR)\Cromosoma.obj" \
	"$(INTDIR)\Detector.obj" \
	"$(INTDIR)\EnergiaDisipable.obj" \
	"$(INTDIR)\FuncioAgent.obj" \
	"$(INTDIR)\Gen.obj" \
	"$(INTDIR)\GeneradorMascares.obj" \
	"$(INTDIR)\InfoOrganisme.obj" \
	"$(INTDIR)\Iterador.obj" \
	"$(INTDIR)\Itineraris.obj" \
	"$(INTDIR)\LlistaEstatica.obj" \
	"$(INTDIR)\MainBio1.obj" \
	"$(INTDIR)\Missatger.obj" \
	"$(INTDIR)\MultiAgent.obj" \
	"$(INTDIR)\Organisme.obj" \
	"$(INTDIR)\Posicionador.obj" \
	"$(INTDIR)\RandomStream.obj" \
	"$(INTDIR)\Substrat.obj" \
	"$(INTDIR)\Taxo.obj" \
	"$(INTDIR)\Taxonomista.obj" \
	"$(INTDIR)\Temporitzador.obj" \
	"$(INTDIR)\Topologia.obj" \
	"$(INTDIR)\TopologiaToroidal.obj"

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
SOURCE=.\Actuador.cpp

!IF  "$(CFG)" == "Bioscena - Win32 Release"

DEP_CPP_ACTUA=\
	".\Actuador.h"\
	".\Agent.h"\
	".\BioIncludes.h"\
	".\Missatger.h"\
	".\Outputer.h"\
	".\Portable.h"\
	".\Posicionador.h"\
	".\RandomStream.h"\
	".\Substrat.h"\
	".\Topologia.h"\
	

"$(INTDIR)\Actuador.obj" : $(SOURCE) $(DEP_CPP_ACTUA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Bioscena - Win32 Debug"

DEP_CPP_ACTUA=\
	".\Actuador.h"\
	".\Agent.h"\
	".\BioIncludes.h"\
	".\Missatger.h"\
	".\Outputer.h"\
	".\Portable.h"\
	".\Posicionador.h"\
	".\RandomStream.h"\
	".\Substrat.h"\
	".\Topologia.h"\
	

"$(INTDIR)\Actuador.obj" : $(SOURCE) $(DEP_CPP_ACTUA) "$(INTDIR)"


!ENDIF 

SOURCE=.\Actuadors.cpp

!IF  "$(CFG)" == "Bioscena - Win32 Release"

DEP_CPP_ACTUAD=\
	".\Actuador.h"\
	".\Actuadors.h"\
	".\Agent.h"\
	".\BioIncludes.h"\
	".\Color.h"\
	".\Iterador.h"\
	".\Itineraris.h"\
	".\Missatger.h"\
	".\MultiAgent.h"\
	".\Outputer.h"\
	".\Portable.h"\
	".\Posicionador.h"\
	".\RandomStream.h"\
	".\Substrat.h"\
	".\Temporitzador.h"\
	".\Topologia.h"\
	".\TopologiaToroidal.h"\
	

"$(INTDIR)\Actuadors.obj" : $(SOURCE) $(DEP_CPP_ACTUAD) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Bioscena - Win32 Debug"

DEP_CPP_ACTUAD=\
	".\Actuador.h"\
	".\Actuadors.h"\
	".\Agent.h"\
	".\BioIncludes.h"\
	".\Color.h"\
	".\Iterador.h"\
	".\Itineraris.h"\
	".\Missatger.h"\
	".\MultiAgent.h"\
	".\Outputer.h"\
	".\Portable.h"\
	".\Posicionador.h"\
	".\RandomStream.h"\
	".\Substrat.h"\
	".\Temporitzador.h"\
	".\Topologia.h"\
	".\TopologiaToroidal.h"\
	

"$(INTDIR)\Actuadors.obj" : $(SOURCE) $(DEP_CPP_ACTUAD) "$(INTDIR)"


!ENDIF 

SOURCE=.\Agent.cpp

!IF  "$(CFG)" == "Bioscena - Win32 Release"

DEP_CPP_AGENT=\
	".\Actuador.h"\
	".\Actuadors.h"\
	".\Agent.h"\
	".\Aleaturitzador.h"\
	".\BioIncludes.h"\
	".\Color.h"\
	".\Iterador.h"\
	".\Itineraris.h"\
	".\Missatger.h"\
	".\MultiAgent.h"\
	".\Outputer.h"\
	".\Portable.h"\
	".\Posicionador.h"\
	".\RandomStream.h"\
	".\Substrat.h"\
	".\Temporitzador.h"\
	".\Topologia.h"\
	".\TopologiaToroidal.h"\
	

"$(INTDIR)\Agent.obj" : $(SOURCE) $(DEP_CPP_AGENT) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Bioscena - Win32 Debug"

DEP_CPP_AGENT=\
	".\Actuador.h"\
	".\Actuadors.h"\
	".\Agent.h"\
	".\Aleaturitzador.h"\
	".\BioIncludes.h"\
	".\Color.h"\
	".\Iterador.h"\
	".\Itineraris.h"\
	".\Missatger.h"\
	".\MultiAgent.h"\
	".\Outputer.h"\
	".\Portable.h"\
	".\Posicionador.h"\
	".\RandomStream.h"\
	".\Substrat.h"\
	".\Temporitzador.h"\
	".\Topologia.h"\
	".\TopologiaToroidal.h"\
	

"$(INTDIR)\Agent.obj" : $(SOURCE) $(DEP_CPP_AGENT) "$(INTDIR)"


!ENDIF 

SOURCE=.\Aleaturitzador.cpp

!IF  "$(CFG)" == "Bioscena - Win32 Release"

DEP_CPP_ALEAT=\
	".\Agent.h"\
	".\Aleaturitzador.h"\
	".\BioIncludes.h"\
	".\Color.h"\
	".\FuncioAgent.h"\
	".\Missatger.h"\
	".\MultiAgent.h"\
	".\Outputer.h"\
	".\Portable.h"\
	".\RandomStream.h"\
	".\Substrat.h"\
	".\Topologia.h"\
	

"$(INTDIR)\Aleaturitzador.obj" : $(SOURCE) $(DEP_CPP_ALEAT) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Bioscena - Win32 Debug"

DEP_CPP_ALEAT=\
	".\Agent.h"\
	".\Aleaturitzador.h"\
	".\BioIncludes.h"\
	".\Color.h"\
	".\FuncioAgent.h"\
	".\Missatger.h"\
	".\MultiAgent.h"\
	".\Outputer.h"\
	".\Portable.h"\
	".\RandomStream.h"\
	".\Substrat.h"\
	".\Topologia.h"\
	

"$(INTDIR)\Aleaturitzador.obj" : $(SOURCE) $(DEP_CPP_ALEAT) "$(INTDIR)"


!ENDIF 

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
	

"$(INTDIR)\AppInfo.obj" : $(SOURCE) $(DEP_CPP_APPIN) "$(INTDIR)"


!ENDIF 

SOURCE=.\Biosistema.cpp

!IF  "$(CFG)" == "Bioscena - Win32 Release"

DEP_CPP_BIOSI=\
	".\Agent.h"\
	".\BioIncludes.h"\
	".\Biosistema.h"\
	".\Color.h"\
	".\Comunitat.h"\
	".\EnergiaDisipable.h"\
	".\InfoOrganisme.h"\
	".\LlistaEstatica.h"\
	".\Missatger.h"\
	".\Organisme.h"\
	".\Outputer.h"\
	".\Portable.h"\
	".\RandomStream.h"\
	".\Substrat.h"\
	".\Taxo.h"\
	".\Taxonomista.h"\
	".\Topologia.h"\
	".\TopologiaToroidal.h"\
	

"$(INTDIR)\Biosistema.obj" : $(SOURCE) $(DEP_CPP_BIOSI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Bioscena - Win32 Debug"

DEP_CPP_BIOSI=\
	".\Agent.h"\
	".\BioIncludes.h"\
	".\Biosistema.h"\
	".\Comunitat.h"\
	".\EnergiaDisipable.h"\
	".\InfoOrganisme.h"\
	".\LlistaEstatica.h"\
	".\Missatger.h"\
	".\MultiAgent.h"\
	".\Organisme.h"\
	".\Outputer.h"\
	".\Portable.h"\
	".\RandomStream.h"\
	".\Substrat.h"\
	".\Taxo.h"\
	".\Taxonomista.h"\
	".\Topologia.h"\
	

"$(INTDIR)\Biosistema.obj" : $(SOURCE) $(DEP_CPP_BIOSI) "$(INTDIR)"


!ENDIF 

SOURCE=.\Color.cpp

!IF  "$(CFG)" == "Bioscena - Win32 Release"

DEP_CPP_COLOR=\
	".\BioIncludes.h"\
	".\Color.h"\
	".\Missatger.h"\
	".\Outputer.h"\
	".\Portable.h"\
	

"$(INTDIR)\Color.obj" : $(SOURCE) $(DEP_CPP_COLOR) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Bioscena - Win32 Debug"

DEP_CPP_COLOR=\
	".\BioIncludes.h"\
	".\Color.h"\
	".\Missatger.h"\
	".\Outputer.h"\
	".\Portable.h"\
	

"$(INTDIR)\Color.obj" : $(SOURCE) $(DEP_CPP_COLOR) "$(INTDIR)"


!ENDIF 

SOURCE=.\Compatibilitat.cpp

!IF  "$(CFG)" == "Bioscena - Win32 Release"

DEP_CPP_COMPA=\
	".\BioIncludes.h"\
	".\Compatibilitat.h"\
	".\GeneradorMascares.h"\
	".\Missatger.h"\
	".\Outputer.h"\
	".\Portable.h"\
	".\RandomStream.h"\
	

"$(INTDIR)\Compatibilitat.obj" : $(SOURCE) $(DEP_CPP_COMPA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Bioscena - Win32 Debug"

DEP_CPP_COMPA=\
	".\BioIncludes.h"\
	".\Compatibilitat.h"\
	".\GeneradorMascares.h"\
	".\Missatger.h"\
	".\Outputer.h"\
	".\Portable.h"\
	".\RandomStream.h"\
	

"$(INTDIR)\Compatibilitat.obj" : $(SOURCE) $(DEP_CPP_COMPA) "$(INTDIR)"


!ENDIF 

SOURCE=.\Comunitat.cpp

!IF  "$(CFG)" == "Bioscena - Win32 Release"

DEP_CPP_COMUN=\
	".\BioIncludes.h"\
	".\Comunitat.h"\
	".\Configuracio.h"\
	".\EnergiaDisipable.h"\
	".\InfoOrganisme.h"\
	".\LlistaEstatica.h"\
	".\Missatger.h"\
	".\Organisme.h"\
	".\Outputer.h"\
	".\Portable.h"\
	".\RandomStream.h"\
	".\Taxo.h"\
	".\Taxonomista.h"\
	

"$(INTDIR)\Comunitat.obj" : $(SOURCE) $(DEP_CPP_COMUN) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Bioscena - Win32 Debug"

DEP_CPP_COMUN=\
	".\BioIncludes.h"\
	".\Comunitat.h"\
	".\Configuracio.h"\
	".\EnergiaDisipable.h"\
	".\InfoOrganisme.h"\
	".\LlistaEstatica.h"\
	".\Missatger.h"\
	".\Organisme.h"\
	".\Outputer.h"\
	".\Portable.h"\
	".\RandomStream.h"\
	".\Taxo.h"\
	".\Taxonomista.h"\
	

"$(INTDIR)\Comunitat.obj" : $(SOURCE) $(DEP_CPP_COMUN) "$(INTDIR)"


!ENDIF 

SOURCE=.\Configuracio.cpp

!IF  "$(CFG)" == "Bioscena - Win32 Release"

DEP_CPP_CONFI=\
	".\BioIncludes.h"\
	".\Configuracio.h"\
	".\Missatger.h"\
	".\Outputer.h"\
	".\Portable.h"\
	

"$(INTDIR)\Configuracio.obj" : $(SOURCE) $(DEP_CPP_CONFI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Bioscena - Win32 Debug"

DEP_CPP_CONFI=\
	".\BioIncludes.h"\
	".\Configuracio.h"\
	".\Missatger.h"\
	".\Outputer.h"\
	".\Portable.h"\
	

"$(INTDIR)\Configuracio.obj" : $(SOURCE) $(DEP_CPP_CONFI) "$(INTDIR)"


!ENDIF 

SOURCE=.\Cromosoma.cpp

!IF  "$(CFG)" == "Bioscena - Win32 Release"

DEP_CPP_CROMO=\
	".\BioIncludes.h"\
	".\Cromosoma.h"\
	".\Missatger.h"\
	".\Outputer.h"\
	".\Portable.h"\
	".\RandomStream.h"\
	

"$(INTDIR)\Cromosoma.obj" : $(SOURCE) $(DEP_CPP_CROMO) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Bioscena - Win32 Debug"

DEP_CPP_CROMO=\
	".\BioIncludes.h"\
	".\Cromosoma.h"\
	".\Missatger.h"\
	".\Outputer.h"\
	".\Portable.h"\
	".\RandomStream.h"\
	

"$(INTDIR)\Cromosoma.obj" : $(SOURCE) $(DEP_CPP_CROMO) "$(INTDIR)"


!ENDIF 

SOURCE=.\Detector.cpp

!IF  "$(CFG)" == "Bioscena - Win32 Release"

DEP_CPP_DETEC=\
	".\BioIncludes.h"\
	".\Detector.h"\
	".\Missatger.h"\
	".\Outputer.h"\
	".\Portable.h"\
	".\Substrat.h"\
	

"$(INTDIR)\Detector.obj" : $(SOURCE) $(DEP_CPP_DETEC) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Bioscena - Win32 Debug"

DEP_CPP_DETEC=\
	".\BioIncludes.h"\
	".\Detector.h"\
	".\Missatger.h"\
	".\Outputer.h"\
	".\Portable.h"\
	".\Substrat.h"\
	

"$(INTDIR)\Detector.obj" : $(SOURCE) $(DEP_CPP_DETEC) "$(INTDIR)"


!ENDIF 

SOURCE=.\EnergiaDisipable.cpp

!IF  "$(CFG)" == "Bioscena - Win32 Release"

DEP_CPP_ENERG=\
	".\BioIncludes.h"\
	".\EnergiaDisipable.h"\
	".\Missatger.h"\
	".\Outputer.h"\
	".\Portable.h"\
	

"$(INTDIR)\EnergiaDisipable.obj" : $(SOURCE) $(DEP_CPP_ENERG) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Bioscena - Win32 Debug"

DEP_CPP_ENERG=\
	".\BioIncludes.h"\
	".\EnergiaDisipable.h"\
	".\Missatger.h"\
	".\Outputer.h"\
	".\Portable.h"\
	

"$(INTDIR)\EnergiaDisipable.obj" : $(SOURCE) $(DEP_CPP_ENERG) "$(INTDIR)"


!ENDIF 

SOURCE=.\FuncioAgent.cpp

!IF  "$(CFG)" == "Bioscena - Win32 Release"

DEP_CPP_FUNCI=\
	".\Agent.h"\
	".\BioIncludes.h"\
	".\Color.h"\
	".\FuncioAgent.h"\
	".\Missatger.h"\
	".\Outputer.h"\
	".\Portable.h"\
	".\RandomStream.h"\
	".\Substrat.h"\
	".\Topologia.h"\
	

"$(INTDIR)\FuncioAgent.obj" : $(SOURCE) $(DEP_CPP_FUNCI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Bioscena - Win32 Debug"

DEP_CPP_FUNCI=\
	".\Agent.h"\
	".\BioIncludes.h"\
	".\Color.h"\
	".\FuncioAgent.h"\
	".\Missatger.h"\
	".\Outputer.h"\
	".\Portable.h"\
	".\RandomStream.h"\
	".\Substrat.h"\
	".\Topologia.h"\
	

"$(INTDIR)\FuncioAgent.obj" : $(SOURCE) $(DEP_CPP_FUNCI) "$(INTDIR)"


!ENDIF 

SOURCE=.\Gen.cpp

!IF  "$(CFG)" == "Bioscena - Win32 Release"

DEP_CPP_GEN_C=\
	".\Gen.h"\
	

"$(INTDIR)\Gen.obj" : $(SOURCE) $(DEP_CPP_GEN_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Bioscena - Win32 Debug"


"$(INTDIR)\Gen.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\GeneradorMascares.cpp

!IF  "$(CFG)" == "Bioscena - Win32 Release"

DEP_CPP_GENER=\
	".\BioIncludes.h"\
	".\GeneradorMascares.h"\
	".\Missatger.h"\
	".\Outputer.h"\
	".\Portable.h"\
	

"$(INTDIR)\GeneradorMascares.obj" : $(SOURCE) $(DEP_CPP_GENER) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Bioscena - Win32 Debug"

DEP_CPP_GENER=\
	".\BioIncludes.h"\
	".\GeneradorMascares.h"\
	".\Missatger.h"\
	".\Outputer.h"\
	".\Portable.h"\
	

"$(INTDIR)\GeneradorMascares.obj" : $(SOURCE) $(DEP_CPP_GENER) "$(INTDIR)"


!ENDIF 

SOURCE=.\InfoOrganisme.cpp

!IF  "$(CFG)" == "Bioscena - Win32 Release"

DEP_CPP_INFOO=\
	".\BioIncludes.h"\
	".\EnergiaDisipable.h"\
	".\InfoOrganisme.h"\
	".\Missatger.h"\
	".\Organisme.h"\
	".\Outputer.h"\
	".\Portable.h"\
	

"$(INTDIR)\InfoOrganisme.obj" : $(SOURCE) $(DEP_CPP_INFOO) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Bioscena - Win32 Debug"

DEP_CPP_INFOO=\
	".\BioIncludes.h"\
	".\EnergiaDisipable.h"\
	".\InfoOrganisme.h"\
	".\Missatger.h"\
	".\Organisme.h"\
	".\Outputer.h"\
	".\Portable.h"\
	

"$(INTDIR)\InfoOrganisme.obj" : $(SOURCE) $(DEP_CPP_INFOO) "$(INTDIR)"


!ENDIF 

SOURCE=.\Iterador.cpp

!IF  "$(CFG)" == "Bioscena - Win32 Release"

DEP_CPP_ITERA=\
	".\Agent.h"\
	".\BioIncludes.h"\
	".\Color.h"\
	".\FuncioAgent.h"\
	".\Iterador.h"\
	".\Missatger.h"\
	".\MultiAgent.h"\
	".\Outputer.h"\
	".\Portable.h"\
	".\RandomStream.h"\
	".\Substrat.h"\
	".\Topologia.h"\
	

"$(INTDIR)\Iterador.obj" : $(SOURCE) $(DEP_CPP_ITERA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Bioscena - Win32 Debug"

DEP_CPP_ITERA=\
	".\Agent.h"\
	".\BioIncludes.h"\
	".\Color.h"\
	".\FuncioAgent.h"\
	".\Iterador.h"\
	".\Missatger.h"\
	".\MultiAgent.h"\
	".\Outputer.h"\
	".\Portable.h"\
	".\RandomStream.h"\
	".\Substrat.h"\
	".\Topologia.h"\
	

"$(INTDIR)\Iterador.obj" : $(SOURCE) $(DEP_CPP_ITERA) "$(INTDIR)"


!ENDIF 

SOURCE=.\Itineraris.cpp

!IF  "$(CFG)" == "Bioscena - Win32 Release"

DEP_CPP_ITINE=\
	".\Actuador.h"\
	".\Actuadors.h"\
	".\Agent.h"\
	".\Aleaturitzador.h"\
	".\BioIncludes.h"\
	".\Color.h"\
	".\Iterador.h"\
	".\Itineraris.h"\
	".\Missatger.h"\
	".\MultiAgent.h"\
	".\Outputer.h"\
	".\Portable.h"\
	".\Posicionador.h"\
	".\RandomStream.h"\
	".\Substrat.h"\
	".\Temporitzador.h"\
	".\Topologia.h"\
	".\TopologiaToroidal.h"\
	

"$(INTDIR)\Itineraris.obj" : $(SOURCE) $(DEP_CPP_ITINE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Bioscena - Win32 Debug"

DEP_CPP_ITINE=\
	".\Actuador.h"\
	".\Actuadors.h"\
	".\Agent.h"\
	".\Aleaturitzador.h"\
	".\BioIncludes.h"\
	".\Color.h"\
	".\Iterador.h"\
	".\Itineraris.h"\
	".\Missatger.h"\
	".\MultiAgent.h"\
	".\Outputer.h"\
	".\Portable.h"\
	".\Posicionador.h"\
	".\RandomStream.h"\
	".\Substrat.h"\
	".\Temporitzador.h"\
	".\Topologia.h"\
	".\TopologiaToroidal.h"\
	

"$(INTDIR)\Itineraris.obj" : $(SOURCE) $(DEP_CPP_ITINE) "$(INTDIR)"


!ENDIF 

SOURCE=.\LlistaEstatica.cpp

!IF  "$(CFG)" == "Bioscena - Win32 Release"

DEP_CPP_LLIST=\
	".\BioIncludes.h"\
	".\LlistaEstatica.h"\
	".\Missatger.h"\
	".\Outputer.h"\
	".\Portable.h"\
	

"$(INTDIR)\LlistaEstatica.obj" : $(SOURCE) $(DEP_CPP_LLIST) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Bioscena - Win32 Debug"

DEP_CPP_LLIST=\
	".\BioIncludes.h"\
	".\LlistaEstatica.h"\
	".\Missatger.h"\
	".\Outputer.h"\
	".\Portable.h"\
	

"$(INTDIR)\LlistaEstatica.obj" : $(SOURCE) $(DEP_CPP_LLIST) "$(INTDIR)"


!ENDIF 

SOURCE=.\MainBio1.cpp

!IF  "$(CFG)" == "Bioscena - Win32 Release"

DEP_CPP_MAINB=\
	".\Actuador.h"\
	".\Actuadors.h"\
	".\Agent.h"\
	".\Aleaturitzador.h"\
	".\AppInfo.h"\
	".\BioIncludes.h"\
	".\Biosistema.h"\
	".\Color.h"\
	".\Compatibilitat.h"\
	".\Comunitat.h"\
	".\EnergiaDisipable.h"\
	".\FuncioAgent.h"\
	".\GeneradorMascares.h"\
	".\InfoOrganisme.h"\
	".\Iterador.h"\
	".\Itineraris.h"\
	".\LlistaEstatica.h"\
	".\Missatger.h"\
	".\MultiAgent.h"\
	".\Organisme.h"\
	".\Outputer.h"\
	".\Portable.h"\
	".\Posicionador.h"\
	".\RandomStream.h"\
	".\Substrat.h"\
	".\Taxo.h"\
	".\Taxonomista.h"\
	".\Temporitzador.h"\
	".\Topologia.h"\
	".\TopologiaToroidal.h"\
	

"$(INTDIR)\MainBio1.obj" : $(SOURCE) $(DEP_CPP_MAINB) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Bioscena - Win32 Debug"

DEP_CPP_MAINB=\
	".\Actuador.h"\
	".\Actuadors.h"\
	".\Agent.h"\
	".\Aleaturitzador.h"\
	".\AppInfo.h"\
	".\BioIncludes.h"\
	".\Biosistema.h"\
	".\Color.h"\
	".\Compatibilitat.h"\
	".\Comunitat.h"\
	".\EnergiaDisipable.h"\
	".\FuncioAgent.h"\
	".\GeneradorMascares.h"\
	".\InfoOrganisme.h"\
	".\Iterador.h"\
	".\Itineraris.h"\
	".\LlistaEstatica.h"\
	".\Missatger.h"\
	".\MultiAgent.h"\
	".\Organisme.h"\
	".\Outputer.h"\
	".\Portable.h"\
	".\Posicionador.h"\
	".\RandomStream.h"\
	".\Substrat.h"\
	".\Taxo.h"\
	".\Taxonomista.h"\
	".\Temporitzador.h"\
	".\Topologia.h"\
	".\TopologiaToroidal.h"\
	

"$(INTDIR)\MainBio1.obj" : $(SOURCE) $(DEP_CPP_MAINB) "$(INTDIR)"


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
	

"$(INTDIR)\Missatger.obj" : $(SOURCE) $(DEP_CPP_MISSA) "$(INTDIR)"


!ENDIF 

SOURCE=.\MultiAgent.cpp

!IF  "$(CFG)" == "Bioscena - Win32 Release"

DEP_CPP_MULTI=\
	".\Actuador.h"\
	".\Actuadors.h"\
	".\Agent.h"\
	".\BioIncludes.h"\
	".\Color.h"\
	".\Iterador.h"\
	".\Itineraris.h"\
	".\Missatger.h"\
	".\MultiAgent.h"\
	".\Outputer.h"\
	".\Portable.h"\
	".\Posicionador.h"\
	".\RandomStream.h"\
	".\Substrat.h"\
	".\Temporitzador.h"\
	".\Topologia.h"\
	".\TopologiaToroidal.h"\
	

"$(INTDIR)\MultiAgent.obj" : $(SOURCE) $(DEP_CPP_MULTI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Bioscena - Win32 Debug"

DEP_CPP_MULTI=\
	".\Actuador.h"\
	".\Actuadors.h"\
	".\Agent.h"\
	".\BioIncludes.h"\
	".\Color.h"\
	".\Iterador.h"\
	".\Itineraris.h"\
	".\Missatger.h"\
	".\MultiAgent.h"\
	".\Outputer.h"\
	".\Portable.h"\
	".\Posicionador.h"\
	".\RandomStream.h"\
	".\Substrat.h"\
	".\Temporitzador.h"\
	".\Topologia.h"\
	".\TopologiaToroidal.h"\
	

"$(INTDIR)\MultiAgent.obj" : $(SOURCE) $(DEP_CPP_MULTI) "$(INTDIR)"


!ENDIF 

SOURCE=.\Organisme.cpp

!IF  "$(CFG)" == "Bioscena - Win32 Release"

DEP_CPP_ORGAN=\
	".\BioIncludes.h"\
	".\Color.h"\
	".\Compatibilitat.h"\
	".\EnergiaDisipable.h"\
	".\GeneradorMascares.h"\
	".\Missatger.h"\
	".\Organisme.h"\
	".\Outputer.h"\
	".\Portable.h"\
	".\RandomStream.h"\
	

"$(INTDIR)\Organisme.obj" : $(SOURCE) $(DEP_CPP_ORGAN) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Bioscena - Win32 Debug"

DEP_CPP_ORGAN=\
	".\BioIncludes.h"\
	".\Color.h"\
	".\Compatibilitat.h"\
	".\EnergiaDisipable.h"\
	".\GeneradorMascares.h"\
	".\Missatger.h"\
	".\Organisme.h"\
	".\Outputer.h"\
	".\Portable.h"\
	".\RandomStream.h"\
	

"$(INTDIR)\Organisme.obj" : $(SOURCE) $(DEP_CPP_ORGAN) "$(INTDIR)"


!ENDIF 

SOURCE=.\Posicionador.cpp

!IF  "$(CFG)" == "Bioscena - Win32 Release"

DEP_CPP_POSIC=\
	".\Agent.h"\
	".\BioIncludes.h"\
	".\Missatger.h"\
	".\Outputer.h"\
	".\Portable.h"\
	".\Posicionador.h"\
	".\RandomStream.h"\
	".\Substrat.h"\
	".\Topologia.h"\
	

"$(INTDIR)\Posicionador.obj" : $(SOURCE) $(DEP_CPP_POSIC) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Bioscena - Win32 Debug"

DEP_CPP_POSIC=\
	".\Agent.h"\
	".\BioIncludes.h"\
	".\Missatger.h"\
	".\Outputer.h"\
	".\Portable.h"\
	".\Posicionador.h"\
	".\RandomStream.h"\
	".\Substrat.h"\
	".\Topologia.h"\
	

"$(INTDIR)\Posicionador.obj" : $(SOURCE) $(DEP_CPP_POSIC) "$(INTDIR)"


!ENDIF 

SOURCE=.\RandomStream.cpp

!IF  "$(CFG)" == "Bioscena - Win32 Release"

DEP_CPP_RANDO=\
	".\BioIncludes.h"\
	".\Missatger.h"\
	".\Outputer.h"\
	".\Portable.h"\
	".\RandomStream.h"\
	

"$(INTDIR)\RandomStream.obj" : $(SOURCE) $(DEP_CPP_RANDO) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Bioscena - Win32 Debug"

DEP_CPP_RANDO=\
	".\BioIncludes.h"\
	".\Missatger.h"\
	".\Outputer.h"\
	".\Portable.h"\
	".\RandomStream.h"\
	

"$(INTDIR)\RandomStream.obj" : $(SOURCE) $(DEP_CPP_RANDO) "$(INTDIR)"


!ENDIF 

SOURCE=.\Substrat.cpp

!IF  "$(CFG)" == "Bioscena - Win32 Release"

DEP_CPP_SUBST=\
	".\BioIncludes.h"\
	".\Color.h"\
	".\encaix.h"\
	".\GeneradorMascares.h"\
	".\Missatger.h"\
	".\Outputer.h"\
	".\Portable.h"\
	".\RandomStream.h"\
	".\Substrat.h"\
	

"$(INTDIR)\Substrat.obj" : $(SOURCE) $(DEP_CPP_SUBST) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Bioscena - Win32 Debug"

DEP_CPP_SUBST=\
	".\BioIncludes.h"\
	".\Color.h"\
	".\encaix.h"\
	".\GeneradorMascares.h"\
	".\Missatger.h"\
	".\Outputer.h"\
	".\Portable.h"\
	".\RandomStream.h"\
	".\Substrat.h"\
	

"$(INTDIR)\Substrat.obj" : $(SOURCE) $(DEP_CPP_SUBST) "$(INTDIR)"


!ENDIF 

SOURCE=.\Taxo.cpp

!IF  "$(CFG)" == "Bioscena - Win32 Release"

DEP_CPP_TAXO_=\
	".\BioIncludes.h"\
	".\Missatger.h"\
	".\Outputer.h"\
	".\Portable.h"\
	".\Taxo.h"\
	

"$(INTDIR)\Taxo.obj" : $(SOURCE) $(DEP_CPP_TAXO_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Bioscena - Win32 Debug"

DEP_CPP_TAXO_=\
	".\BioIncludes.h"\
	".\Missatger.h"\
	".\Outputer.h"\
	".\Portable.h"\
	".\Taxo.h"\
	

"$(INTDIR)\Taxo.obj" : $(SOURCE) $(DEP_CPP_TAXO_) "$(INTDIR)"


!ENDIF 

SOURCE=.\Taxonomista.cpp

!IF  "$(CFG)" == "Bioscena - Win32 Release"

DEP_CPP_TAXON=\
	".\BioIncludes.h"\
	".\LlistaEstatica.h"\
	".\Missatger.h"\
	".\Outputer.h"\
	".\Portable.h"\
	".\Taxo.h"\
	".\Taxonomista.h"\
	

"$(INTDIR)\Taxonomista.obj" : $(SOURCE) $(DEP_CPP_TAXON) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Bioscena - Win32 Debug"

DEP_CPP_TAXON=\
	".\BioIncludes.h"\
	".\LlistaEstatica.h"\
	".\Missatger.h"\
	".\Outputer.h"\
	".\Portable.h"\
	".\Taxo.h"\
	".\Taxonomista.h"\
	

"$(INTDIR)\Taxonomista.obj" : $(SOURCE) $(DEP_CPP_TAXON) "$(INTDIR)"


!ENDIF 

SOURCE=.\Temporitzador.cpp

!IF  "$(CFG)" == "Bioscena - Win32 Release"

DEP_CPP_TEMPO=\
	".\Agent.h"\
	".\BioIncludes.h"\
	".\Color.h"\
	".\FuncioAgent.h"\
	".\Missatger.h"\
	".\MultiAgent.h"\
	".\Outputer.h"\
	".\Portable.h"\
	".\RandomStream.h"\
	".\Substrat.h"\
	".\Temporitzador.h"\
	".\Topologia.h"\
	

"$(INTDIR)\Temporitzador.obj" : $(SOURCE) $(DEP_CPP_TEMPO) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Bioscena - Win32 Debug"

DEP_CPP_TEMPO=\
	".\Agent.h"\
	".\BioIncludes.h"\
	".\Color.h"\
	".\FuncioAgent.h"\
	".\Missatger.h"\
	".\MultiAgent.h"\
	".\Outputer.h"\
	".\Portable.h"\
	".\RandomStream.h"\
	".\Substrat.h"\
	".\Temporitzador.h"\
	".\Topologia.h"\
	

"$(INTDIR)\Temporitzador.obj" : $(SOURCE) $(DEP_CPP_TEMPO) "$(INTDIR)"


!ENDIF 

SOURCE=.\Topologia.cpp

!IF  "$(CFG)" == "Bioscena - Win32 Release"

DEP_CPP_TOPOL=\
	".\BioIncludes.h"\
	".\Missatger.h"\
	".\Outputer.h"\
	".\Portable.h"\
	".\RandomStream.h"\
	".\Topologia.h"\
	

"$(INTDIR)\Topologia.obj" : $(SOURCE) $(DEP_CPP_TOPOL) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Bioscena - Win32 Debug"

DEP_CPP_TOPOL=\
	".\BioIncludes.h"\
	".\Missatger.h"\
	".\Outputer.h"\
	".\Portable.h"\
	".\RandomStream.h"\
	".\Topologia.h"\
	

"$(INTDIR)\Topologia.obj" : $(SOURCE) $(DEP_CPP_TOPOL) "$(INTDIR)"


!ENDIF 

SOURCE=.\TopologiaToroidal.cpp

!IF  "$(CFG)" == "Bioscena - Win32 Release"

DEP_CPP_TOPOLO=\
	".\BioIncludes.h"\
	".\Color.h"\
	".\Missatger.h"\
	".\Outputer.h"\
	".\Portable.h"\
	".\RandomStream.h"\
	".\Topologia.h"\
	".\TopologiaToroidal.h"\
	

"$(INTDIR)\TopologiaToroidal.obj" : $(SOURCE) $(DEP_CPP_TOPOLO) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Bioscena - Win32 Debug"

DEP_CPP_TOPOLO=\
	".\BioIncludes.h"\
	".\Color.h"\
	".\Missatger.h"\
	".\Outputer.h"\
	".\Portable.h"\
	".\RandomStream.h"\
	".\Topologia.h"\
	".\TopologiaToroidal.h"\
	

"$(INTDIR)\TopologiaToroidal.obj" : $(SOURCE) $(DEP_CPP_TOPOLO) "$(INTDIR)"


!ENDIF 


!ENDIF 

