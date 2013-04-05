# Microsoft Developer Studio Project File - Name="IRQTape" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) External Target" 0x0106

CFG=IRQTape - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "IRQTape.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "IRQTape.mak" CFG="IRQTape - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "IRQTape - Win32 Release" (based on "Win32 (x86) External Target")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "IRQTape"
# PROP Scc_LocalPath "."
# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "IRQTape___Win32_Release"
# PROP BASE Intermediate_Dir "IRQTape___Win32_Release"
# PROP BASE Cmd_Line "NMAKE /f IRQTape.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "IRQTape.exe"
# PROP BASE Bsc_Name "IRQTape.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "IRQTape___Win32_Release"
# PROP Intermediate_Dir "IRQTape___Win32_Release"
# PROP Cmd_Line "..\acme.exe -v3 --msvc IRQTape1.a"
# PROP Rebuild_Opt ""
# PROP Bsc_Name ""
# PROP Target_Dir ""
# Begin Target

# Name "IRQTape - Win32 Release"

!IF  "$(CFG)" == "IRQTape - Win32 Release"

!ENDIF 

# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\IRQTape1.a
# End Source File
# Begin Source File

SOURCE=.\ScrollerMusicLoader.a
# End Source File
# Begin Source File

SOURCE=.\TapeLoaderCIA.a
# End Source File
# Begin Source File

SOURCE=.\TapeLoaderCIACommon.a
# End Source File
# Begin Source File

SOURCE=.\TapeLoaderCIAIRQ.a
# End Source File
# Begin Source File

SOURCE=.\TapeLoaderCIANovaish.a
# End Source File
# Begin Source File

SOURCE=.\TapeLoaderCIASmall.a
# End Source File
# Begin Source File

SOURCE=.\TurboTapeVars.a
# End Source File
# Begin Source File

SOURCE=.\TurboTapeWrite.a
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\stdlib\PETSCII.a
# End Source File
# Begin Source File

SOURCE=..\stdlib\stdlib.a
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\LotDComp.prg
# End Source File
# Begin Source File

SOURCE=.\LotDLoadMusic.prg
# End Source File
# Begin Source File

SOURCE=.\R.MUS8000.PRG
# End Source File
# Begin Source File

SOURCE=.\TestMultiplexor.prg
# End Source File
# End Group
# End Target
# End Project
