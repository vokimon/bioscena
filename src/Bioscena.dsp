# Microsoft Developer Studio Project File - Name="Bioscena" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 5.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=Bioscena - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Bioscena.mak".
!MESSAGE 
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

# Begin Project
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Bioscena - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD BASE RSC /l 0x403 /d "NDEBUG"
# ADD RSC /l 0x403 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386

!ELSEIF  "$(CFG)" == "Bioscena - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /G6 /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /FR /YX /FD /c
# ADD BASE RSC /l 0x403 /d "_DEBUG"
# ADD RSC /l 0x403 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /version:0.7 /subsystem:console /incremental:no /debug /machine:I386 /pdbtype:sept
# SUBTRACT LINK32 /verbose

!ENDIF 

# Begin Target

# Name "Bioscena - Win32 Release"
# Name "Bioscena - Win32 Debug"
# Begin Group "Documentacio"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Configuracio.txt
# End Source File
# Begin Source File

SOURCE=.\Dudas.txt
# End Source File
# Begin Source File

SOURCE=.\Idees
# End Source File
# Begin Source File

SOURCE=.\Observacions
# End Source File
# Begin Source File

SOURCE=.\ParametresProves
# End Source File
# Begin Source File

SOURCE=.\Sensomotriu.txt
# End Source File
# Begin Source File

SOURCE=.\TODO.txt
# End Source File
# Begin Source File

SOURCE=.\Vistes.txt
# End Source File
# End Group
# Begin Source File

SOURCE=.\Actuador.cpp
# End Source File
# Begin Source File

SOURCE=.\Actuador.h
# End Source File
# Begin Source File

SOURCE=.\Actuadors.cpp
# End Source File
# Begin Source File

SOURCE=.\Actuadors.h
# End Source File
# Begin Source File

SOURCE=.\Agent.cpp
# End Source File
# Begin Source File

SOURCE=.\Agent.h
# End Source File
# Begin Source File

SOURCE=.\AgentsLog.txt
# End Source File
# Begin Source File

SOURCE=.\Aleaturitzador.cpp
# End Source File
# Begin Source File

SOURCE=.\Aleaturitzador.h
# End Source File
# Begin Source File

SOURCE=.\AppInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\AppInfo.h
# End Source File
# Begin Source File

SOURCE=.\BioIncludes.h
# End Source File
# Begin Source File

SOURCE=.\Biosistema.cpp
# End Source File
# Begin Source File

SOURCE=.\Biosistema.h
# End Source File
# Begin Source File

SOURCE=.\build.h
# End Source File
# Begin Source File

SOURCE=.\Cariotip.cpp
# End Source File
# Begin Source File

SOURCE=.\Cariotip.h
# End Source File
# Begin Source File

SOURCE=.\Color.cpp
# End Source File
# Begin Source File

SOURCE=.\Color.h
# End Source File
# Begin Source File

SOURCE=.\Compatibilitat.cpp
# End Source File
# Begin Source File

SOURCE=.\Compatibilitat.h
# End Source File
# Begin Source File

SOURCE=.\Comunitat.cpp
# End Source File
# Begin Source File

SOURCE=.\Comunitat.h
# End Source File
# Begin Source File

SOURCE=.\Configuracio.cpp
# End Source File
# Begin Source File

SOURCE=.\Configuracio.h
# End Source File
# Begin Source File

SOURCE=.\Cromosoma.cpp
# End Source File
# Begin Source File

SOURCE=.\Cromosoma.h
# End Source File
# Begin Source File

SOURCE=.\Distribucio.cpp
# End Source File
# Begin Source File

SOURCE=.\Distribucio.h
# End Source File
# Begin Source File

SOURCE=.\EnergiaDisipable.cpp
# End Source File
# Begin Source File

SOURCE=.\EnergiaDisipable.h
# End Source File
# Begin Source File

SOURCE=.\FuncioAgent.cpp
# End Source File
# Begin Source File

SOURCE=.\FuncioAgent.h
# End Source File
# Begin Source File

SOURCE=.\Gen.cpp
# End Source File
# Begin Source File

SOURCE=.\Gen.h
# End Source File
# Begin Source File

SOURCE=.\GeneradorMascares.cpp
# End Source File
# Begin Source File

SOURCE=.\GeneradorMascares.h
# End Source File
# Begin Source File

SOURCE=.\Genotip.cpp
# End Source File
# Begin Source File

SOURCE=.\Genotip.h
# End Source File
# Begin Source File

SOURCE=.\Grafic.cpp
# End Source File
# Begin Source File

SOURCE=.\Grafic.h
# End Source File
# Begin Source File

SOURCE=.\InfoOrganisme.cpp
# End Source File
# Begin Source File

SOURCE=.\InfoOrganisme.h
# End Source File
# Begin Source File

SOURCE=.\Iterador.cpp
# End Source File
# Begin Source File

SOURCE=.\Iterador.h
# End Source File
# Begin Source File

SOURCE=.\Itineraris.cpp
# End Source File
# Begin Source File

SOURCE=.\Itineraris.h
# End Source File
# Begin Source File

SOURCE=.\MainBio1.cpp
# End Source File
# Begin Source File

SOURCE=.\Missatger.cpp
# End Source File
# Begin Source File

SOURCE=.\Missatger.h
# End Source File
# Begin Source File

SOURCE=.\MultiAgent.cpp
# End Source File
# Begin Source File

SOURCE=.\MultiAgent.h
# End Source File
# Begin Source File

SOURCE=.\MutacioCariotip.cpp
# End Source File
# Begin Source File

SOURCE=.\MutacioCariotip.h
# End Source File
# Begin Source File

SOURCE=.\MutacioCromosomica.cpp
# End Source File
# Begin Source File

SOURCE=.\MutacioCromosomica.h
# End Source File
# Begin Source File

SOURCE=.\MutacioGenica.cpp
# End Source File
# Begin Source File

SOURCE=.\MutacioGenica.h
# End Source File
# Begin Source File

SOURCE=.\Organisme.cpp
# End Source File
# Begin Source File

SOURCE=.\Organisme.h
# End Source File
# Begin Source File

SOURCE=.\Outputer.cpp
# End Source File
# Begin Source File

SOURCE=.\Outputer.h
# End Source File
# Begin Source File

SOURCE=.\Portable.h
# End Source File
# Begin Source File

SOURCE=.\Posicionador.cpp
# End Source File
# Begin Source File

SOURCE=.\Posicionador.h
# End Source File
# Begin Source File

SOURCE=.\Probabilitat.cpp
# End Source File
# Begin Source File

SOURCE=.\Probabilitat.h
# End Source File
# Begin Source File

SOURCE=.\RandomStream.cpp
# End Source File
# Begin Source File

SOURCE=.\RandomStream.h
# End Source File
# Begin Source File

SOURCE=.\Substrat.cpp
# End Source File
# Begin Source File

SOURCE=.\Substrat.h
# End Source File
# Begin Source File

SOURCE=.\Temporitzador.cpp
# End Source File
# Begin Source File

SOURCE=.\Temporitzador.h
# End Source File
# Begin Source File

SOURCE=.\Topologia.cpp
# End Source File
# Begin Source File

SOURCE=.\Topologia.h
# End Source File
# Begin Source File

SOURCE=.\TopologiaToroidal.cpp
# End Source File
# Begin Source File

SOURCE=.\TopologiaToroidal.h
# End Source File
# End Target
# End Project
