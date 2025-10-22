Welcome to the generic scrolling game engine source code.

Download source from the github link. 
https://github.com/martinpiper/C64Public 
(Download zip) 

Unzip it somewhere.

Change to the Scroller directory. 
Run buildIt.bat

It creates a compressed Scroller.prg and Scroller.crt that can be run on a C64 or pretty much any C64 emulator.

ScrollEntry.a allows the 8-way multidirection scrolling demo or SEUCK games builds to be selected near the comment "Choose one of the below source includes for the main build options".

ScrollEntry.a, ScrollEntry_SEUCK.a and ScrollEntry_MultiDirection.a contain lots of documented tweak options to enable or disable (or define or undefine).
For example an enabled or defined option for Scroller_LinkedEnemyTest will look like: "Scroller_LinkedEnemyTest = 1"
A disabled or undefined option for Scroller_LinkedEnemyTest will look like: ";Scroller_LinkedEnemyTest = 1"
The ';' character is a comment character and causes that line to be ignored.
Thus to disable it or undefine it use a ';' character at the start of the line for that option.


ScrollerData.a contains lots of !bin commands near the top of the file for including various chunks of data
ScrollerDataMacro.a defines the SEUCK data inclusion macros +SEUCKFileData
Enable one of the !bin to include one of the game data files from the SEUCK directory.


To create your own p00 game data file use the Vice C64 emulator, detach all d64 files, in SEUCK choose storage and save all data.
Then reference the saved p00 data file on your PC by putting the path to the file in one of the !bin commands in the ScrollerData.a file.


* Useful VICE options
-remotemonitor $(ProjectDir)\Scroller.prg
-remotemonitor -truedrive -cartcrt $(ProjectDir)\Scroller.crt
-warp -remotemonitor -truedrive -cartcrt $(ProjectDir)\Scroller.crt


* To start SEUCK in Vice and allow SEUCK to save "*.p00" files that can be easily imported:
cd /d C:\Work\C64\SEUCK
cd /d C:\Work\C64\SEUCK\Debug1
cd /d C:\Work\C64\SEUCK\Debug2
"C:\Downloads\WinVICE-3.1-x86-r34062\WinVICE-3.1-x86-r34062\x64sc.exe" -remotemonitor -fs8savep00 +truedrive -virtualdev "C:\Work\C64\SEUCK\SEUCK.prg"
"C:\Downloads\WinVICE-3.1-x86-r34062\WinVICE-3.1-x86-r34062\x64sc.exe" -remotemonitor -fs8savep00 +truedrive -virtualdev "C:\Work\C64\SEUCK\SEUCKDiskBlank.prg"
"C:\Downloads\WinVICE-3.1-x86-r34062\WinVICE-3.1-x86-r34062\x64sc.exe" -remotemonitor -fs8savep00 +truedrive -virtualdev "C:\Work\C64\SEUCK\SEUCKDiskGame1.prg"
"C:\Downloads\WinVICE-3.1-x86-r34062\WinVICE-3.1-x86-r34062\x64sc.exe" -remotemonitor -fs8savep00 +truedrive -virtualdev "C:\Work\C64\SEUCK\SidewaysSEUCK.prg"
"C:\Downloads\WinVICE-3.1-x86-r34062\WinVICE-3.1-x86-r34062\x64sc.exe" -remotemonitor -fs8savep00 +truedrive -virtualdev "C:\Work\C64\SEUCK\sideways left.prg"
etc.
In SEUCK, press D for the "Storage" menu, then "Save all data".

Saving all data with "SEUCK.prg" and "SEUCKDiskBlank.prg" produces practically identical data in the p00 file.


Some of the old VSF snapshot files use: C:\Backups\OldMachines\OldMachineBackup\C\VICE\x64.exe


* The files "SEUCKDisk*.prg" were extracted from the original PAL disk using my expanded version of C1541 with the "-recover" command: https://github.com/martinpiper/DebuggingDetails/blob/main/SEUCK.txt




* SEUCK Redux videos

Background							https://youtu.be/FKcuB8lMOSQ
Levels								https://youtu.be/h9aFMf6X3dI
Sprites	and animations				https://youtu.be/VJbpLoU6OJ0
Attack waves - Part 1				https://youtu.be/60HR-yhPSy8
Attack waves - Part 2				https://youtu.be/ri5ATGKv71o
Enemy bits							https://youtu.be/NUciFe8t0gk
Player limitations					https://youtu.be/q8H3OIrFiXw
Sound effects and title screen		https://youtu.be/J_MVIATywr8
Horizontal scroll and enhancements	https://youtu.be/UENzjhOqlts
