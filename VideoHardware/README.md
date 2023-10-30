Video Hardware
==============

This project contains C64 code that interfaces with the user port 24 bit address and extension video hardware in this project: https://github.com/martinpiper/BombJack


To build and run
================


The assembler used is an updated version of ACME which can use Python as a built-in scripting language for assembly and data parsing or generation. Since this ACME is 32 bit (x86) code then it needs the 32 bit Python DLL to be included. To access most script functionality the Python SDK will need to be installed and accessible via the PATH environment. I installed Python 3.9.0 (final) 32 bit (x86), which is accessible via the environment variable %LOCALAPPDATA%\Programs\Python


To convert the artwork assets used in the demo. This does not need to be run every time, only when the assets are changed. Run:

	* assets\convert.bat
	* assets\convert3.bat
	* assets\convert4.bat
	* assets\convert5.bat
	* assets\convert6.bat
	* assets\convert7.bat
	* etc...
	
To build the code, execute tests, and run one of the examples by running one of these:

	* BuildIt.bat
	* BuildIt3.bat
	* BuildIt3Anim.bat
	* BuildIt3MultiplexTimer.bat
	* BuildIt4_8.bat
	* BuildIt4.bat
	* BuildIt5.bat
	* BuildIt6.bat
	* BuildIt7.bat
	* etc...


Map editor
==========

The map files in assets\*.tmx and *.tsx use this editor: https://www.mapeditor.org/2020/04/14/tiled-1-3-4-released.html

	* The map needs to be expoerted as an image, which is then converted into tile data using convert.bat
		Menu->File->Export as image: assets\map.png

	
Editing tests
=============

Tests for this projects are authored with BDD6502, which is a framework built around Cucumber that executes readable test cases.

To open the editor, run: FeatureEditor.bat



Source code repositories
========================

	Source: https://github.com/martinpiper/C64Public
	Source: https://github.com/martinpiper/BombJack
	Release: https://github.com/martinpiper/BDD6502/releases
	Release: https://github.com/martinpiper/ImageToBitplane/releases
	Corretto java corretto-1.8.0_252 can be used for better performance



Credits
=======

	Code:
		Me :)

	Artwork:
		https://opengameart.org/content/rpg-town-pixel-art-assets
		https://opengameart.org/content/colored-16x16-fantasy-tileset
		https://opengameart.org/content/cloud-set
		https://www.spriters-resource.com/arcade/arkanoid/
		https://codetapper.com/amiga/sprite-tricks/shadow-of-the-beast/
		https://www.oldgames.sk/docs/sotb/index.html
		https://www.spriters-resource.com/fullview/73177/?source=genre
	Music:
		https://modarchive.org/index.php?request=view_by_moduleid&query=188017
		http://amigamuseum.emu-france.info/Fichiers/sites/sotb/Fichiers/Musiques.html

