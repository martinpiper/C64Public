# Microsoft Developer Studio Project File - Name="IRQDisk" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) External Target" 0x0106

CFG=IRQDisk - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "IRQDisk.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "IRQDisk.mak" CFG="IRQDisk - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "IRQDisk - Win32 Release" (based on "Win32 (x86) External Target")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "IRQDisk"
# PROP Scc_LocalPath "."
# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Cmd_Line "NMAKE /f IRQDisk.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "IRQDisk.exe"
# PROP BASE Bsc_Name "IRQDisk.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Cmd_Line "BuildIt.bat"
# PROP Rebuild_Opt ""
# PROP Target_File "IRQDisk.exe"
# PROP Bsc_Name ""
# PROP Target_Dir ""
# Begin Target

# Name "IRQDisk - Win32 Release"

!IF  "$(CFG)" == "IRQDisk - Win32 Release"

!ENDIF 

# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\BuildIt.bat
# End Source File
# Begin Source File

SOURCE=.\IRQDisk.a
# End Source File
# Begin Source File

SOURCE=.\IRQDiskDriveCode.a
# End Source File
# Begin Source File

SOURCE=.\TestIRQDisk.a
# End Source File
# Begin Source File

SOURCE=.\TestIRQDisk2.a
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\stdlib\BASICEntry900.a
# End Source File
# Begin Source File

SOURCE=..\stdlib\Initialise.a
# End Source File
# Begin Source File

SOURCE=.\IRQDiskC128.a
# End Source File
# Begin Source File

SOURCE=.\IRQDiskC16.a
# End Source File
# Begin Source File

SOURCE=.\IRQDiskC64.a
# End Source File
# Begin Source File

SOURCE=.\IRQDiskVars.a
# End Source File
# Begin Source File

SOURCE=.\IRQDiskVIC20.a
# End Source File
# Begin Source File

SOURCE=..\stdlib\stdlib.a
# End Source File
# Begin Source File

SOURCE=.\stdlib1541.a
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
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
# End Target
# End Project
