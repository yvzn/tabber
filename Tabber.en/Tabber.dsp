# Microsoft Developer Studio Project File - Name="Tabber" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Tabber - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Tabber.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Tabber.mak" CFG="Tabber - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Tabber - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Tabber - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Tabber - Win32 Release"

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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x40c /d "NDEBUG"
# ADD RSC /l 0x40c /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "Tabber - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "obj"
# PROP Intermediate_Dir "obj"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x40c /d "_DEBUG"
# ADD RSC /l 0x40c /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib comctl32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "Tabber - Win32 Release"
# Name "Tabber - Win32 Debug"
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\tabber_large.ico
# End Source File
# Begin Source File

SOURCE=.\res\tabber_small.ico
# End Source File
# End Group
# Begin Group "Application Core"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\core\Application.cpp
# End Source File
# Begin Source File

SOURCE=.\src\core\Application.h
# End Source File
# Begin Source File

SOURCE=.\src\core\ApplicationSettings.cpp
# End Source File
# Begin Source File

SOURCE=.\src\core\ApplicationSettings.h
# End Source File
# End Group
# Begin Group "Data Model"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\data\ChordDefinitions.cpp
# End Source File
# Begin Source File

SOURCE=.\src\data\ChordDefinitions.h
# End Source File
# Begin Source File

SOURCE=.\src\data\ChordGroup.cpp
# End Source File
# Begin Source File

SOURCE=.\src\data\ChordGroup.h
# End Source File
# Begin Source File

SOURCE=.\src\data\GuitarChord.cpp
# End Source File
# Begin Source File

SOURCE=.\src\data\GuitarChord.h
# End Source File
# Begin Source File

SOURCE=.\src\data\TuningDefinitions.cpp
# End Source File
# Begin Source File

SOURCE=.\src\data\TuningDefinitions.h
# End Source File
# End Group
# Begin Group "User Interface"

# PROP Default_Filter ""
# Begin Group "Dialogs"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\ui\dialogs\AboutDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\src\ui\dialogs\AboutDialog.h
# End Source File
# Begin Source File

SOURCE=.\src\ui\dialogs\PrintProgressDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\src\ui\dialogs\PrintProgressDialog.h
# End Source File
# Begin Source File

SOURCE=.\src\ui\dialogs\PromptDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\src\ui\dialogs\PromptDialog.h
# End Source File
# End Group
# Begin Group "Toolkit"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\ui\tools\DocumentInterface.cpp
# End Source File
# Begin Source File

SOURCE=.\src\ui\tools\DocumentInterface.h
# End Source File
# Begin Source File

SOURCE=.\src\ui\tools\EditionActions.cpp
# End Source File
# Begin Source File

SOURCE=.\src\ui\tools\EditionActions.h
# End Source File
# Begin Source File

SOURCE=.\src\ui\tools\EditionModes.h
# End Source File
# Begin Source File

SOURCE=.\src\ui\tools\EditionToolkit.cpp
# End Source File
# Begin Source File

SOURCE=.\src\ui\tools\EditionToolkit.h
# End Source File
# Begin Source File

SOURCE=.\src\ui\tools\PrinterInterface.cpp
# End Source File
# Begin Source File

SOURCE=.\src\ui\tools\PrinterInterface.h
# End Source File
# Begin Source File

SOURCE=.\src\ui\tools\PrinterPager.cpp
# End Source File
# Begin Source File

SOURCE=.\src\ui\tools\PrinterPager.h
# End Source File
# Begin Source File

SOURCE=.\src\ui\tools\SettingsInterface.cpp
# End Source File
# Begin Source File

SOURCE=.\src\ui\tools\SettingsInterface.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\src\ui\ChordsTabControl.cpp
# End Source File
# Begin Source File

SOURCE=.\src\ui\ChordsTabControl.h
# End Source File
# Begin Source File

SOURCE=.\src\ui\ChordsTabControlPanel.cpp
# End Source File
# Begin Source File

SOURCE=.\src\ui\ChordsTabControlPanel.h
# End Source File
# Begin Source File

SOURCE=.\src\ui\ChordsToolbar.cpp
# End Source File
# Begin Source File

SOURCE=.\src\ui\ChordsToolbar.h
# End Source File
# Begin Source File

SOURCE=.\src\ui\EditArea.cpp
# End Source File
# Begin Source File

SOURCE=.\src\ui\EditArea.h
# End Source File
# Begin Source File

SOURCE=.\src\ui\MainToolbar.cpp
# End Source File
# Begin Source File

SOURCE=.\src\ui\MainToolbar.h
# End Source File
# Begin Source File

SOURCE=.\src\ui\MainWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\src\ui\MainWindow.h
# End Source File
# Begin Source File

SOURCE=.\src\ui\StaffToolbar.cpp
# End Source File
# Begin Source File

SOURCE=.\src\ui\StaffToolbar.h
# End Source File
# Begin Source File

SOURCE=.\src\ui\StatusBar.cpp
# End Source File
# Begin Source File

SOURCE=.\src\ui\StatusBar.h
# End Source File
# End Group
# Begin Group "Utilities"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\util\DebugWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\src\util\DebugWindow.h
# End Source File
# Begin Source File

SOURCE=.\src\util\DoubleNullStringTokenizer.cpp
# End Source File
# Begin Source File

SOURCE=.\src\util\DoubleNullStringTokenizer.h
# End Source File
# Begin Source File

SOURCE=.\src\util\DynamicPointerArray.cpp
# End Source File
# Begin Source File

SOURCE=.\src\util\DynamicPointerArray.h
# End Source File
# Begin Source File

SOURCE=.\src\util\IniFileSequentialParser.cpp
# End Source File
# Begin Source File

SOURCE=.\src\util\IniFileSequentialParser.h
# End Source File
# Begin Source File

SOURCE=.\src\util\MemoryWatcher.cpp
# End Source File
# Begin Source File

SOURCE=.\src\util\MemoryWatcher.h
# End Source File
# Begin Source File

SOURCE=.\src\util\NotifyMessage.cpp
# End Source File
# Begin Source File

SOURCE=.\src\util\NotifyMessage.h
# End Source File
# Begin Source File

SOURCE=.\src\util\RuntimeException.cpp
# End Source File
# Begin Source File

SOURCE=.\src\util\RuntimeException.h
# End Source File
# Begin Source File

SOURCE=.\src\util\String.cpp
# End Source File
# Begin Source File

SOURCE=.\src\util\String.h
# End Source File
# Begin Source File

SOURCE=.\src\util\System.cpp
# End Source File
# Begin Source File

SOURCE=.\src\util\System.h
# End Source File
# Begin Source File

SOURCE=.\src\util\utilities.cpp
# End Source File
# Begin Source File

SOURCE=.\src\util\utilities.h
# End Source File
# End Group
# Begin Group "Documentation"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Documentation.html
# End Source File
# Begin Source File

SOURCE=.\Licence.txt
# End Source File
# Begin Source File

SOURCE=.\Readme_.txt
# End Source File
# End Group
# Begin Source File

SOURCE=.\src\resource.h
# End Source File
# Begin Source File

SOURCE=.\src\resource.rc
# End Source File
# Begin Source File

SOURCE=.\src\winmain.cpp
# End Source File
# End Target
# End Project
