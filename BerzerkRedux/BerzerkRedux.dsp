# Microsoft Developer Studio Project File - Name="BerzerkRedux" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) External Target" 0x0106

CFG=BerzerkRedux - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "BerzerkRedux.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "BerzerkRedux.mak" CFG="BerzerkRedux - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "BerzerkRedux - Win32 Release" (based on "Win32 (x86) External Target")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "BerzerkRedux"
# PROP Scc_LocalPath "."
# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Cmd_Line "NMAKE /f BerzerkRedux.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "BerzerkRedux.exe"
# PROP BASE Bsc_Name "BerzerkRedux.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Cmd_Line "BuildIt.bat"
# PROP Rebuild_Opt ""
# PROP Target_File "BerzerkRedux.exe"
# PROP Bsc_Name ""
# PROP Target_Dir ""
# Begin Target

# Name "BerzerkRedux - Win32 Release"

!IF  "$(CFG)" == "BerzerkRedux - Win32 Release"

!ENDIF 

# Begin Group "Source Files"

# PROP Default_Filter "a"
# Begin Source File

SOURCE=.\BerzerkRedux.a
# End Source File
# Begin Source File

SOURCE=.\BerzerkReduxVars.a
# End Source File
# Begin Source File

SOURCE=.\ClearMisc.a
# End Source File
# Begin Source File

SOURCE=.\Data.a
# End Source File
# Begin Source File

SOURCE=.\EnterInitialsScreen.a
# End Source File
# Begin Source File

SOURCE=.\GameLogic.a
# End Source File
# Begin Source File

SOURCE=.\GameOver.a
# End Source File
# Begin Source File

SOURCE=.\IRQs.a
# End Source File
# Begin Source File

SOURCE=.\MazeRoom.a
# End Source File
# Begin Source File

SOURCE=.\Rand.a
# End Source File
# Begin Source File

SOURCE=.\Score.a
# End Source File
# Begin Source File

SOURCE=.\TitleScreen.a
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\stdlib\Initialise.a
# End Source File
# Begin Source File

SOURCE=..\IRQDisk\IRQDisk.a
# End Source File
# Begin Source File

SOURCE=..\IRQDisk\IRQDiskC64.a
# End Source File
# Begin Source File

SOURCE=..\IRQDisk\IRQDiskDriveCode.a
# End Source File
# Begin Source File

SOURCE=..\IRQDisk\IRQDiskVars.a
# End Source File
# Begin Source File

SOURCE=..\SFX\SFX.a
# End Source File
# Begin Source File

SOURCE=..\SpriteMultiplexor\SpriteMultiplexor.a
# End Source File
# Begin Source File

SOURCE=..\SpriteMultiplexor\SpriteMultiplexorVars.a
# End Source File
# Begin Source File

SOURCE=..\stdlib\stdlib.a
# End Source File
# Begin Source File

SOURCE=..\IRQDisk\stdlib1541.a
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\BerzerkLogo.prg
# End Source File
# Begin Source File

SOURCE=.\BerzerkMusic.prg
# End Source File
# Begin Source File

SOURCE=.\Chars.ctm
# End Source File
# Begin Source File

SOURCE=.\Sprites.spr
# End Source File
# Begin Source File

SOURCE=.\Sprites.spr.inf
# End Source File
# End Group
# Begin Source File

SOURCE=.\BuildIt.bat
# End Source File
# Begin Source File

SOURCE=..\CleanProject.bat
# End Source File
# Begin Source File

SOURCE=..\CleanProjectFully.bat
# End Source File
# Begin Source File

SOURCE=..\CleanProjectFullyWithAttrib.bat
# End Source File
# Begin Source File

SOURCE=..\DoBackup.bat
# End Source File
# Begin Source File

SOURCE=.\HowToBuildRelease.txt
# End Source File
# End Target
# End Project
