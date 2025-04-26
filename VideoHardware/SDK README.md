Video Hardware
==============

This SDK contains the minimal set of files needed to build, test, and execute emulated video hardware that uses 5V TTL chips. These chips were common in arcade machines before the 1990s and were used to create all manner of customised effects.

Currently this SDK requires Windows and Java 1.8 or better to run. Notably the assembler is C64\acme.exe and this is a Windows specific build.

This SDK contains sources and some built assets from these repositories:

	* https://github.com/martinpiper/C64Public
	* https://github.com/martinpiper/BombJack
	* https://github.com/martinpiper/BDD6502/releases
	* https://github.com/martinpiper/ImageToBitplane/releases


To build and run
================

To convert the artwork assets used in the demo. This does not need to be run every time, only when the assets are changed. Run:

	* C64\VideoHardware\assets\convert.bat
	* C64\VideoHardware\assets\convert3.bat
	
To build the code, execute tests, and run the example game. Run:

	* C64\VideoHardware\BuildIt.bat
	* C64\VideoHardware\BuildIt3.bat
	* C64\VideoHardware\BuildIt3Anim.bat


Documentation
=============

Documentation "README.md" files for each component are located in each sub-directory.


	* Demo1 : Top down RPG style demo with music
	* Demo3 : Arkanoid (bat and ball) style demo. Also shows the title screen animation.
	* Demo4 : Turrican demo, side scrolling platformer
	* Demo5 : Very simple APU demo showing horizontal scroll splits
	* Demo6 : Shadow of the beast, title and level 1 demo, with music
	* Demo7 : Side scrolling shooter, with APU multiplexed sprites
	* Demo9 : MegaWang intro and 3D flying demo
	* Demo10 : Dual sprite layer Street Fighter demo
	* Demo11 : Dual display, C64 running BASIC and MegaWang character screen
	* Demo12 : 3D vector demo
	* Demo13 : Using RGB565 colour mode with four palette banks, giving up to 1024 colours on screen at once
	* Demo14 : After Burner demo using Sprites4
