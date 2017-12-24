http://www.gamesthatwerent.com/2015/02/martin-piper-assets/

To get the racing game running:

<Mount: ProjectX.d64>
LOAD”DEMO2.BA”,8 <return>
RUN <return>
<You will see syntax errors, this is normal>
SYS2048

The file “RDOB1.D” contains the road side objects. All you need to do to see them is:
1. Extract all the files in the disk image to the PC. Use D64 Editor or something similar.
1. Remove the first two bytes of the “RDOB1.D” file (the old C64 header) and add the bytes 00 01 16 00. This tells CharPad that the map file is 0x100 bytes wide and 0x16 bytes high.
2. Run CharPad (I used 1.8 rev 3)
3. File->Import Data
4. Import: Character set -> Locate -> “CHRS”
5. Import: Map -> Locate -> “RDOB1.D” with the extra header "RDOB1.D - Copy.PRG"
6. Exit the import window and click on Map.

(See below for download of the map file)

For using the Road Side Block editor

LOAD “BLOCKEDIT.BA”,8
RUN

This runs the road side block editor.
The keys used are:
Cursor up/down/left/right (Changes the position within the RDOB1.D map file)
Q/W/O/P
/ and ; (Changes the height of the object)
Z and X (Changes the width of the object)
+ and – (Edits different objects used in the game, press + several times and you’ll see what I mean.)
Left arrow (Copies the current object to the next object, so it can be edited)
F1 and F7

I’m not too sure what QWOP, F1 and F7 do yet. I think they might have something to do with defining left or right hand track side objects…

Each object definition is 8 bytes long starting at 124*256 0x7c00:
0 = char x pos in the map
1 = char y pos in the map
2 = char width
3 = char height
4 = left road side hotspot
5 = right road side hotspot
6/7 = Seem to be unused



Memory map:
$4000 - $4fff = trackconfig2.d
$5000 - $5732 = trackoft.d
$5800 - $52ff = cornoft.d
$7800 - $78ff = trckcornpos.d
$7900 - $79ff = trckypos.d
$7a00 - $7bff = sideobjtab2.d
$7c00 - $71ff = blocktable.d
$8000 - $95ff = rdob1.d

Internals:
The rendered track is comprised of 0-35 entries (the "road rows") for Y position and X position to represent hills and corners.
Depending on the hill and corner these different positions are rendered into the screen with minimal overlapping.
In other other words the screen render for the track does not overdraw, it skips to the next drawn Y position.


trackoftfig
Uses $5000 (trackoft.d) (toftl/tofth) the steering (slide left/right) (skd) to offset the calculated x pos with a perspective effect.
	skd = 0		; Right steer (road to the left)
	skd = 50	; Middle (Road in the middle)
	skd = 100	; Left steer (road to the right)



1670 trckplot !
	Uses trckoft as the track



Track corner and hill/valley, this range can be observed to read and advance dependant on speed.
These are constantly read for each rendered frame even when there is no speed.
$7800 - $78ff = Corner (tablecorn) (trckcornpos.d) $12 is middle, $0 is right, $25 seems to be far left
$7900 - $79ff = Valley or hill (tabletrck) (trckypos.d) $0 is big valley, $10 crest of hill, $15 = level, $1a-$29 cresting with the distant other side coming up, 
	Values $16-$19 seem to not be used and these correspond to the "READY." prompt at $4398
* The read values for the frame are then fed into where and read from what tables?


Corner (tablecorn) ($7800-) is used as an index into $5800 (cornoft.d)
Routine: cornoftfig
Each line represents a different severity of corner index, as read from $7800-
$5800 - contains rows (at 40 chars each row) for 36 chars for the x screen position of the middle of the road



Valley or hill (tabletrck) ($7900-) is used as an index into the Road rows Y Pos ($4000-$47ff) and Row rows width ($4800-$4fff)
Different Y position, different road width and the horizon position are used to produce a variety of hills, crests and dips.

Road rows Y Pos ($4000-$47ff)
Columns 0-35 = Char screen Y pos for each row
Column 36 = $ff (* Why?) Just an obvious eyeball break for the data?
Column 37 = Y Split pos
Column 38 = ?, but it is filled in with something for the hill crest? (No apparent change when tweaked.)
				This compares Y pos with the column? > 2460 l10lo cpy ypost+38
				Some kind of screen row drawing logic test?
				* Due to: 2460 l10lo cpy ypost+38
					Column 38 seems to be an index, the logic is:
					If the column index is >= than column index in "Column 38" then continue to draw
						* TODO: Below mentioned ymax optimisation available here, set ymax to max height
					Otherwise compare the current column Y pos+1 with the Y pos from the "Column 38" index
					If that is greater than or equal to the Y pos then don't draw the road row, skip to the track object draw

Track side object plotting
	* For the track side object plot, first objsprint is called with Y set to the currently drawn road row
	* This indexes into objhorizl and objhorizr
	* Zero index means no object for this position
	* Then pertaby is added for the road row offset
		This ranges from 0 to 8, meaning we have up to 9 object size steps to define for the perspective
	* Then the left/right positions are offset with the relevant hotspots
	* There is no Y hotspot offset, although there certainly could be later on
	* Eventually the object is drawn with blockplot



Row rows width ($4800-$4fff)
Columns 0-35 = Char width
Column 36 = $ff (* Why?)





Left and right track object patterns. $7a00 - $7bff = sideobjtab2.d
$7a00 - $7aff = Left
$7b00 - $7bff = Right
Depending on speed, and when the counter steps, the next object is read from the list and placed into a buffer for left and right objects.
objhorizl and objhorizr are the left and right track side buffers
This buffer is scrolled at a different rate dependant on speed. This produces an effect of objects moving past the player.
$13 = Right lamp post object
