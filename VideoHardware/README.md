Video Hardware
==============

This project contains C64 code that interfaces with the user port 24 bit address and extension video hardware in this project: https://github.com/martinpiper/BombJack

Prerequisites
=============

	Source: https://github.com/martinpiper/C64Public
	Source: https://github.com/martinpiper/BombJack
	Release: https://github.com/martinpiper/BDD6502/releases
	Release: https://github.com/martinpiper/ImageToBitplane/releases


To build and run
================

To convert the artwork assets used in the demo. This does not need to be run every time, only when the assets are changed. Run:

	* assets\convert.bat
	
To build the code, execute tests, and run the example game. Run:

	* BuildIt.bat

	
Editing tests
=============

Tests for this projects are authored with BDD6502, which is a framework built around Cucumber that executes readable test cases.

To open the editor, run: FeatureEditor.bat


Credits
=======

	Code:
		Me :)

	Artwork:
		https://opengameart.org/content/rpg-town-pixel-art-assets
		https://opengameart.org/content/colored-16x16-fantasy-tileset
		https://opengameart.org/content/cloud-set
