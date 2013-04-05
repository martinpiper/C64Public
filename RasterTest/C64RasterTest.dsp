# Microsoft Developer Studio Project File - Name="C64RasterTest" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) External Target" 0x0106

CFG=C64RasterTest - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "C64RasterTest.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "C64RasterTest.mak" CFG="C64RasterTest - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "C64RasterTest - Win32 Release" (based on "Win32 (x86) External Target")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "C64RasterTest"
# PROP Scc_LocalPath "."
# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "C64RasterTest___Win32_Release"
# PROP BASE Intermediate_Dir "C64RasterTest___Win32_Release"
# PROP BASE Cmd_Line "NMAKE /f C64RasterTest.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "C64RasterTest.exe"
# PROP BASE Bsc_Name "C64RasterTest.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "C64RasterTest___Win32_Release"
# PROP Intermediate_Dir "C64RasterTest___Win32_Release"
# PROP Cmd_Line "..\acme.exe -v3 --msvc RasterTest.a"
# PROP Rebuild_Opt ""
# PROP Bsc_Name ""
# PROP Target_Dir ""
# Begin Target

# Name "C64RasterTest - Win32 Release"

!IF  "$(CFG)" == "C64RasterTest - Win32 Release"

!ENDIF 

# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\PartialBucketSort.a
# End Source File
# Begin Source File

SOURCE=.\QSort.a
# End Source File
# Begin Source File

SOURCE=.\RasterTest.a
# End Source File
# Begin Source File

SOURCE=.\RasterTest2.a
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\stdlib\BASICEntry80d.a
# End Source File
# Begin Source File

SOURCE=..\stdlib\Initialise.a
# End Source File
# Begin Source File

SOURCE=..\stdlib\stdlib.a
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
