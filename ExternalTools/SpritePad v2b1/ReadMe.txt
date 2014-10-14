

SPRITE PAD 2.0 BETA RELEASE 1 (24/7/2014).

(C) Subchrist Software 2003-2014.

                

Welcome to Sprite Pad!


Sprite Pad is an image editor/animator for sprite images that are compatible
with the classic 1980's Commodore 64 home computer system.

Sprite Pad allows games developers writing for this classic platform to easily
and comfortably design sprites for their games using a modern PC and Windows 
desktop environment.

-------------------------------------------------------------------------------

IMPORTANT NOTES :- 

This version of Sprite Pad has been completely rewritten in .NET C++ and 
therefore requires some version of .NET (probably 3.5 or better) runtime 
libraries to be installed/present on your system.

In order with the new animation storage system, this version uses a new version 
of the SPD file format when saving projects...

Projects saved with this version are NOT backwards compatible with previous
versions of Sprite Pad.

You have been warned!.


-------------------------------------------------------------------------------

PROGRAM FEATURES


Amongst the many operations available in Sprite Pad, here are most of the main
features...           


Edit and animate up to 256 sprite images/animations per project.

Import data from RAW, PRG, BMP, SEUCK files, VICE snapshot files (VSF).

Rotation tool allows free rotate and animated sequence generation.

Supports high resolution and multi-colour sprite images.

Supports all basic clipboard operations.

Slide/Flip/Reflect/Negative.

Overlays (one per sprite).

Multiple Undo/Redo.
             
             
             
-------------------------------------------------------------------------------

USING SPRITE PAD


The best way to get to know Sprite Pad is just to start using it, it has been 
designed to be as intuitive as possible with very little learning curve.
             
The usual process is simply to start drawing sprites, one at a time until
you are happy with the set, then you can export them in a format suitable for 
your project.



-------------------------------------------------------------------------------

MULTI-COLOUR MODE AND OVERLAYS


In Sprite Pad, each sprite has two basic settings that will dictate it's 
appearance, "Multi-colour mode" and "Overlay Next Sprite".

With multi-colour mode enabled, the current sprite image (or current range) 
loses half of it's horizontal resolution but gains the use of 2 extra colours
which are common to all other multi-colour sprites.

The loss of half the horizontal resolution does not affect the visible size of
the sprite it just means that the pixels become twice as wide.
 

When "Overlay Next Sprite" is enabled, the sprite image immediately following 
the current one will be drawn over the top of the current sprite.

All editing operations whilst in this mode will only affect the current sprite
image and not the overlaid one.

This mode provides an easy means of using a second sprite to add extra
detail and colour, a popular technique in it's day despite the obvious
impact it has on the number of hardware sprites available as separate game
objects. 



-------------------------------------------------------------------------------

THE SHOOT'EM UP CONSTRUCTION KIT    
  

For non-programmers, the simplest way of using the sprite images you create
with Sprite Pad is to use them for game making with Palace Software's 1987 
"Shoot'em Up Construction Kit", aka SEUCK.

There are some new features in Sprite Pad 1.8 that make exporting sprites easy 
and flexible plus there is now direct support for exporting in a format 
compatible with the Shoot'em up Construction Kit.

If you choose the menu option...

  File->Import/Export->SEUCK->Export Sprite Data

The sprites will be exported in a file format compatible with the SEUCK.

The file will always contain exactly 127 sprites even if there is a lesser 
quantity at the time of the save, this is exactly how SEUCK saves sprites 
designed with it's own editor.

To successfully import the sprite file into the SEUCK, you will have to add
the sprite file to a D64 disk image and make sure that the sprite file name has
exactly 16 characters and ends in ".A" (without the quotes).

ie. "MYSPRITES     .A"

The D64 disk image may then be used with the SEUCK, either using a Commodore 64 
emulator or by transferring the D64 to a real floppy disk for use on a real
machine.



-------------------------------------------------------------------------------

SPRITEPAD KEYBOARD SHOTCUTS


New Project            - CTRL + N
Open Project           - CTRL + O
Save Project           - CTRL + S 

Select All             - CTRL + A
Delete                 - DEL
Cut                    - CTRL + X
Copy                   - CTRL + C
Paste                  - CTRL + V  
Paste Into             - CTRL + E

Slide Up               - CTRL + U
Slide Down             - CTRL + D
Slide Left             - CTRL + L
Slide Right            - CTRL + R

Flip Top to Bottom     - CTRL + F
Flip Left to Right     - CTRL + G

Reflect Top to Bottom  - CTRL + H
Reflect Left to Right  - CTRL + J

Negative               - CTRL + I

Undo                   - CTRL + Z
Redo                   - CTRL + ALT + Z

-------------------------------------------------------------------------------

THE COMMODORE 64 AND THE VIC-II CHIP 

                                                      
The Commodore 64 uses a graphics chip called the VIC-II, usually referred to as
the VIC chip (Video Interace Controller).

Amongst the features of the VIC chip, eight hardware "sprites" are available.

nb. Sprites are also known as Movable Object Blocks or "MOBs"

Each sprite is 24x21 pixels in size (high resolution mode) or 12x21 pixels
when in multi-colour mode.

Sprites can be placed anywhere on the visible screen with single pixel accuracy.


In high resolution mode, a sprite may only contain one unique colour.

In multicolour mode a sprite may contain up to 3 colours but at the expense of 
it's horizontal resolution which is converted from 24 to just 12 pixels across, 
however the pixels are stretched to compensate for the change meaning that the 
visible dimensions remain the same. 

The VIC chip can access up to 256 sprite images (16k) at any one time and 
selectively apply any of the available images to any or all of the eight
hardware sprites.

nb. Each sprite occupies 63 bytes of memory, 3 bytes per row for 21
rows, the VIC chip "sees" sprites every 64 bytes (not 63) so there is one 
"spare" byte associated with each sprite image in memory, the VIC chip itself 
does not directly use this spare byte but Sprite Pad uses it to store colour 
and mode information for each sprite image.

This information can be easily accessed by your software programs and fed to 
the VIC chip in order to set the sprite colour and mode registers.
                                              
The VIC chip has access to a palette of 16 colours, each sprite can be assigned 
any one of these colours.

When a sprite is set to use multi-colour mode, two more colours may be used but 
these two must be shared between all other multi-colour sprites.
                 
For further information about the Commodore 64, sprites and the VIC-II chip 
please track down a copy of the "Commodore 64 Programmer's Reference Guide" 
which is readily available on the world wide web in electronic form at... 

http://project64.c64.org


The latest version of Sprite Pad and all other Subchrist productions are 
available from our homepage at...

http://www.coder.myby.co.uk



















    