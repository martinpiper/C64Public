
	Music Studio V2.2 help.
	-----------------------


Music Studio 2.2 is a Windows-based SID music creator software. For an accurate C64 sound, it utilises the newest RESID-FP emulation available, both old (6581) and new (8580) SID chips. MS2 is capable of creating 1x speed tunes and alternate many SID chip parameters directly with various commands. Classic and new C64 sounds can be created with envelope parameters that can be set up in few simple steps. 

For proper initialization, run MusicStudio2\MusicStudio\MusicStudio.exe

Video demonstration: https://www.youtube.com/watch?v=3ApqDIbYPec



Loading and saving files:
-------------------------

Files with the extension msmus are music files.
Files with the extension msenv are envelopes/instruments.

This editor can read packed files saved by the original C64 version of Music Studio editor, either from raw PRGs starting at 0x4600 or VICE P00 files with the longer header data. When saving files it will use the new file format for the PC editor.

Most (not all!) GoatTracker files starting with the header bytes "GTC!" or "GTS5" can also be imported. This is to facilitate importing of block note and track data. The instruments and other effects need a lot of hand tweaking.

MIDI files can also be imported into tracker blocks. A dialog will be displayed allowing various options to be edited:
	Time multiplier. The MIDI to 50Hz frame time conversion factor.
	Block length. The length of each tracker block.
	MIDI Channel to track. By default all MIDI channels are ignored, signified by a 0 in the edit box. If the channel number edit box is set to be 1 then that MIDI channel will be imported to track 1.
	Release note after half duration. When the box is ticked then each note will include an automatic release after half its duration.
When importing MIDI files make sure that at least one MIDI channel is marked for importing otherwise you will not see any music.


The File->Rip SID... option will prompt for a SID or PRG file. The notes values in the SID file will then be entered into the tracker display. Envelope information is currently not supported.

It can also export data to C64 native PRG format as well as SID files using the menu "File->Export to C64". This exported data is heavily optimised compared to the wasted space of the C64 editor. Even unused portions of code from the player routine are removed, for example if the music uses fewer effects the code size will be reduced. This optimisation is due to using assembly source files and the ACME assembler included with this package.

- Output SID file: Uses the source file "HeaderSID.a". Choosing the menu option "View->Extended view" information about TITLE, AUTHOR and RELEASED can be set that will be included in the .sid file header, too.

- Output standalone PRG: Uses the source file "HeaderSelf.a" and will save a PRG that can be loaded which will play the music.

- Output just the player and data: Uses the source "HeaderPRG.a" with the input hex address and will save a relocated PRG starting at the hex address suitable for import into other programs.

Using sound effects in games is supported with 'Include sound effect code' option. Try to export a tune as 'Output standalone PRG' with sound FX option enabled and run on C64/emulator to see how it works. Envelopes also can be marked to export as effects with the 'Force used' option in the envelope editing section. 



Using relocated user modules:
-----------------------------

The player source code is included in "MusicPlayer2.a". This means it is also possible to use the temporary generated sources for the music data with the player source code and include that in other programs.
By default the player code uses the zero page locations $fb/$fc and preserves these whilst playing music.
The zero page address can be changed using the extended view opened by the menu option "View->Extended view".

By default the relocated player code has three jumps at the start of the file:
The first JMP uses the Accumulator to set the music track to play. The second JMP plays the music and must be called once per frame. The third JMP stops any currently playing music.


MusicPlayerInit
	Register A choses the song, starting at 0.
MusicPlayerPlay
MusicPlayerStop

If the sound effect playback code is enabled in the export option then there are two extra jumps:

MusicPlayerPlaySFX
	This has the effect of reserving the channel for sound effects.
	Register A = Note
	Register X = Channel
	Register Y = Envelope

The player code is heavily optimised for the effects used by the music.
The player source code is in ACME format and located in:
MusicStudio2Bin\MusicStudio2\MusicStudioConvert\MusicPlayer2.a
When the editor plays music or exports SID/C64 data the file MusicStudio2Bin\MusicStudio2\MusicStudio\t.a
will be created which will contain the required defines, link in the player with !source "MusicPlayer2.a",
then include the data for the music.


For example assuming relocated music was saved from $1000 then this code will play the music:

*=$c000
	; Turn off IRQs and initialise music track 0
	sei
	lda #$00
	jsr $1000
	; Now wait for the raster at $80
.loop
	lda #$80
.wait
	cmp $d012
	bne .wait
	; Play one frame of music and then loop around again
	jsr $1003
	jmp .loop



-----------------------
USING THE MUSIC EDITOR:
-----------------------


The music editor displays the tracks, blocks, envelopes and drum data to be edited. Track and block data can also be copy and pasted as text.
The Menu->Help->Configuration option allows the window update and ReSID parameters to be changed.
Early versions of the editor would create a new default document with some envelopes, blocks and arpeggios. The newer editor creates a completely blank file. The old default data can be loaded from the "OldDefault.msmus" file.


	The track editor:
	-----------------

Using the "delete" key on an empty cell will delete the cell. The "insert" key will insert a cell before the current cell.

- Track control codes:

'00'-'3F'	- Play specified block.
'40'-'7F'	- Repeats next block by 'xx-$40' times.
'80'-'EF'	- Play all following blocks transposed up by 'xx-$80' semitones. If the number is >= $b0 then the note is transposed down rather than up.
'FD'	- Stops track.
'FE'	- Stops all tracks.
'FF'	- Loops track to beginning of song.



- Using multiple songs:

To edit and play multiple songs finish the previous song with 'FD'-'FF' and continue to edit as normal.



- Track operation:

While editing and playing you may notice bright green markers on the tracks. These indicate the current offset of each track as it is being played.

For example:

Song 1      Song 2      Song3
00 01 02 ff 43 02 fd 82 01 41 03 fe     etc...

The first song would play blocks 00 then 01 then 02 and loop around.

Song two would play 02 four times and stop the track but not all the song.

Song three would play 01 transposed by two semitones and then play 03 twice still transposed then stop all the tracks, finishing the song.

The override start can be set for each track and this will force the track to start playing at the point. This is useful for quickly debugging a point in the middle or end of some music. This setting is not exported to SID or C64 files.





	The block editor:
	-----------------


The blocks are labelled 00 - 3f and are able to be played on any track and can be transposed by different values on different tracks.

To make music the blocks must be programmed with which sound to play and notes. An overview of commands can be seen on the next page.

Each block can be edited with the duration style block editor or a tracker style block editor.
The duration style block editor includes commands to control the duration and the tracker style uses the row and tempo for the block note timing.

To switch between the two editing modes use the button in the top left of the editor window which is called "Tracker block edit mode" or "DUR Block edit mode".
The editor remembers the last mode used to edit a block and will convert that block to the other style's format.
So for example if a block is created with the tracker style it will create the necessary duration style commands in the duration style view.
The duration style block first line will contain the comment ";Tracker block" to give a hint which block editor was used.

When a block is edited using the duration style the tracker view is updated with the necessary note spacing for the row.
At this point it is posible to switch to the tracker view and before editing any rows change the block's tempo so that the number of rows can be increaed or decreased without changing the block's time. Each row will then become the time shown by the tempo.
When editing a tracker style block the tempo can then be used to change the block's overall time without changing the number of rows.
Using the "delete" key on an empty tracker cell will delete the row making the block shorter. The "insert" key will insert a row before the current row. Using the "insert" key on the "END" row will add a row to the block.
The up and down arrow keys will move between rows and scrol the block view. Holding CTRL with an up or down key will scroll the view without changing the currently selected row.
Use "tab" to advance to the next cell.


Error reporting:
While duration block editing if the line is highlighted with a "E>" to the left then there was a parsing error with the command on that line.
While playing music the current block's line is highlighted to the left of the visible block.

While tracker block editing if the line is highlighted with a red row then there was a parsing error with the command or note on that line.
While playing music the current block's line is highlighted with a green row.


Duration block editing commands:
--------------------------------

These commands are only used during duration block editing:

DUR:08	- Sets the duration for all notes to eight (about a minim). This is equivalent to 16 frames because each duration count is two frames.

DTI:XX,YY - Sets the duration in terms of frames (not duration units like with DUR:XX). For XX frames with YY frames of release.
Example: DTI:80,10 will sets a note that lasts for 80 frames and will start the release after 10 frames
Example: DTI:80,70 will sets a note that lasts for 80 frames and will start the release after 70 frames
Example: DTI:40,20 will sets a note that lasts for 40 frames and will start the release after 20 frames. This sounds identical to DUR:20
Example: DTI:40,40 will sets a note that lasts for 40 frames and will start the release after 40 frames. Effectively this means the sound has no release.

--- - An extra sustain of the last note played.

+++ - A silent rest. This can be used without any note before it. This will also stop the current note unless hard reset (HRD command) is disabled before playing the note.

=== - A rest like +++ except it includes an immediate gate off. If sluring is enabled or hard restart disabled then the gate off will apply.

ENV:00	- Sets all notes afterwards to play envelope 00.

Tracker mode block editing commands:
------------------------------------

There are no duration commands since the timing for this block editing mode comes from the row and tempo for the block.

These appear in the first column instead of a note:

=== - Gate off, releases the current note.

+++ - A silent rest. This will also stop the current note unless hard reset (HRD command) is disabled before playing the note.

The instrument number is the second column and must contain a number if there is a note in the first column.

The third column contains any effects from the general block commands below separated by a space.
It is possible to set more than one effect on the same row.


General block commands:
-----------------------

- BASE COMMANDS: setting instruments, notes, durations, volume

C-1	- Plays the note C in octave 1.

C1	- Also plays the note C in octave 1. Shorter notation than C-1

D#2	- Plays the note D with a sharp in octave 2. (E flat)


VOL:XX	- This block command that allows you to set the volume. Setting a volume of 0 will stop the player. So valid number ranges are VOL:01 to VOL:0F



- GLIDE: bending notes up or down

GL:01,4	- Glides the last note played upwards (quit slowly) after counting four (half a minim).

GL:02,2	- Glides the last note played upwards (a bit faster) after counting two (quarter minim). As can be seen the larger the number the faster the glide.

GL:83,0	- Glides the note down very fast after no delay. This means glide down.



- ARPEGGIO: chord-like fast notes

ARP:XY - Controls arpeggio for the voice and is in the format XY where the first root note is unchanged, next the root note is transposed X semitones. Next the root note is transposed Y semitones. The note pattern then loops. Arpeggios will stay on during envelope changes until an "ARP:00" command is used to remove the arpeggio.

ERP:XX - Uses the extended arpeggio code to play an arpeggio from the extended arpeggio table, check "extended view".

ARS    - Stops any arpeggio.



- FILTERS: the EQ parameters of the channels

FLL:XX	- low pass filter: Controls the value sent to $d415 SIDFilterCutoffFreqLo. So FLL:10 will put $10 into $d415

FLH:XX	- high pass filter: does the same as FLL but for $d416 SIDFilterCutoffFreqHi

FLC:XX	- Does the same but for $d417 SIDFilterControl. So FLC:F7 will set filter resonance F with voices 0,1 and 2 active (bits 0/1/2 = 7).

FLP:XX	- Does the same with $d418 SIDVolumeFilter. So FLP:10 will set bit 4 which is the low pass filter. The lower nybble maps to the volume control, don't set these values, keep it at 0 for now.

FG:XX,YY - Filter glide controls the filter frequency in a sinus pattern. XX controls the step size. YY controls the speed of the sinus pattern. If YY is 01 then the change will be slow, 02 is faster, 03 is even faster and so on. If YY has $80 added then the initial sinus pattern is falling instead of rising. For example $81 will fall slowly, $82 will fall faster etc.



- HARD RESTART: Some SID versions have a bug where a voice will not always trigger a new note attack cleanly and instead a quiet click will be heard instead.
A hard restart resets the SID voice generator before a note is played to help ensure the note attack cleanly starts. By default a hard restart is enabled for all voices.
The hard restart AttackDecay SustainRelease Waveform and frame time for each voice can be edited with the extended view. You can have different hard restart parameters for voice voice.

However this hard restart alters the envelope of notes so these next commands are offered to control this mechanism.
The hard restart applies to the release stage of a note so enable or disable this setting before the note that needs it.

HRE - Enables the hard restart for this voice.

HRD - Disables the hard restart for this voice.

The following block commands allow the voice hard restart values to be changed while the music is playing.
If these commands are used the default starting hard restart value is remembered the next time any music plays so each voice should use these commands to set default values at the start of each piece of music.
HAD:XX - Sets the hard restart AttackDecay.
HSR:XX - Sets the hard restart SustainRelease.
HWV:XX - Sets the hard restart waveform.
HTI:XX - Sets the hard restart frame time, must be greater than 0.


- FLUENT NOTES: slur command

SLE - This block command enables slurring (or ties) of the next notes, the note will note release until SLD is disabled. Envelope commands have no effect on the sound until the SLD command is used. Hard restart is also disabled for the voice by this command.

SLR - Releases the slur (or ties) of the next note. Hard restart is also enabled for the voice by this command.



- SMOOTH SINE VIBRATO: more accurate and more demanding

Small vibrato uses a sine wave and a lot of maths to make sure the vibrato is using smooth accurate fractional semitones.
Small vibrato can be mixed with glides, arpeggios, extended arpeggios and even the large vibrato included in envelopes. This can generate very complex note sounds.

VIB:XY,VV - Semitone small vibrato. This vibrato will continue for all notes on the channel until it is stopped with VBS.
X - Vibrato shift going up. 1 is the larger vibrato of 2 semitones. 5 is smaller at a fraction of a semitone.
Y - Vibrato shift going down. 1 is the larger vibrato of 2 semitones. 5 is smaller at a fraction of a semitone.
X or Y must not be 0 and will report an error.
VV - Vibrato frequency. Larger numbers produce faster vibratos.

VBD:ZZ - ZZ delays the start of the small vibrato for ZZ frames.

VBS - Stops the small vibrato



- FIXED VIBRATO: nice trade-off for extra rastertime

Fixed vibrato uses less code space and less raster time than small vibrato. However the vibrato pitch bending is not as smooth as the accurate small vibrato sine wave.
Fixed vibrato can be mixed with glides, arpeggios, extended arpeggios and even the large vibrato.
Fixed vibrato cannot be mixed in the same note with the small vibrato.

FVB:XX,VV - Semitone fixed vibrato. This vibrato will continue for all notes on the channel until it is stopped with FVS.
X - Vibrato shift. 1 is the larger vibrato. 5 is smaller at a fraction of a semitone. There is no separate up and down size as there is for the small vibrato. 
X must not be 0 and will report an error.
VV - Vibrato frequency. Larger numbers produce faster vibratos.
As an example FVB:3,3 is nearly the same sound as VIB:33,3. Both have the same speed vibrato, both have similar tone range and the small vibrato has a smoother sine wave for the tone.

FVD:ZZ - ZZ delays the start of the fixed vibrato for ZZ frames.

FVS - Stops the fixed vibrato


- TABLE CONTROL: These commands can alter the table being used by a channel.
Where XX is the envelope number to use:
TWV:XX - Set the wave table.
TNT:XX - Set the note table.
TPL:XX - Set the pulse table.
TFL:XX - Set the filter table.
For example:
	SLE
	ENV:01
	DUR:20
	C-3
	TWV:02
	C-2
	C-1
This enables the slur, plays C-3 with envelope 1 as normal, then uses the wave table assigned to envelope 2 for notes C-2 and C-1.



Example block1 (put this in block 01):
--------------------------------------

block commands		lines explained

FLL:10			low-pass filter: $10, it is just a sublte cut of low frequencies
FLH:80			hi-pass filter: $80, sets the EQ to the middle of the spectrum
FLC:F7			filter resonance: $F, the second digit $7 enables the setting on all channels
FLP:10			volume filter: sets lo-pass
ENV:03			sets sound number 03
DUR:10			sets the tempo
FG:08,04		sets a slow autofilter
ARP:15			sets simple arpeggio, it will loop: "root note, root +1 semitone, root +5 semitone" 
C-2			all the notes are in here
C-3		
E-3		
C-3		


Change the 00 ff in track one to read 01 ff and choose from the menu play all. Your block should be playing.
This example is saved as "p.filter test 2"




The envelope editor:
--------------------

The envelope is a hunk of eight bytes for each sound effect. This can control waveform mixing, pitch, gliding and envelope (attack, decay, sustain, release) for each voice.

At the moment it is just hex number editing but it is hoped to make this much better in the next versions, on the next couple of pages the different bytes are explained.

AD/SR	- Controls the SID envelope attack, decay, sustain and release registers. For example the two values 12/fc are attack 1, decay 2, sustain F and decay C.

The wave, note, pulse and filter table offsets have check boxes to enable the table for this instrument. If the tick box is unticked then the currently running table is left unchanged.
An offset of 0 with the tick box checked will stop the table for that voice.
When adding and removing pairs from the tables the table jump offsets and envelope table offsets are automatically updated.

A blank sound, one that mutes the channel, should use a wave table with 08 or 09 (gated 08) so the SID can trigger the next note cleanly.

Each envelope also has a transpose value in semitones. If the value is >= $80 then the transpose will be treated as a negative value. For example FF = -1 semitone, FE = -2 semitones etc.
Each subsequent note in a block will be transposed by this transpose value if the envelope command is used in that block.

The "Force used" check box can mark envelopes to be included in the output file even if they are unused in blocks. This is useful for marking which envelopes must be exported for sound effects.

The "Allow voice effects" check box will allow other voice effects such as arpeggio, glide and smooth vibrato. Unchecked disables the voice effects and saves some raster time.

- Tables
Each table uses pairs of values arranged vertically:
	X1 X2 X3 ...
	Y1 Y2 Y3
The X value is usually the command and the Y value the parameter.
All tables can run on every frame except hard reset frames where the note is silent so running the waveform and pulse tables doesn't make much sense.
These tables can be combined with arpeggio, glide and smooth vibrato effects to make some very complex sounds.


- Wave table
XX
YY
YY is always the frame delay.
XX:
0 = Don't change wave this time, previous waveform is used.
FF = Jump to position and frame delay ignored. YY = pos 0 = stop
Any other value is used as the waveform using the SID format which can be ORed together:
bit7	80	Noise
bit6	40	Pulse
bit5	20	Sawtooth
bit4	10	Triangle
bit3	08	Test
bit2	04	Ring modulation
bit1	02	Sync
bit0	01	Gate

Remember to set the gate bit to get any sounds.
The gate used by SID will be set if the wave table and the gate time from the duration are both set.
If either the wave table or the note duration are clear then the gate sent to SID will also be clear.
You have full control over the test bit which can be useful for resetting the waveform.


- Pulse table
If a pulse waveform is active for a frame (bit 6) then the pulse table will execute for that frame:
XY
ZZ
X = 0 Set pulse Y & ZZ = Pulse
X = 1 Pulse add with unsigned ZZ for Y ticks (ticks zero based)
X = 2 Pulse subtract with unsigned ZZ for Y ticks (ticks zero based)
FF = Jump to position. ZZ = pos 0 = stop


- Note table
XY
ZZ
Y = Except command 0 frame delay (0-15) (ticks zero based)
X = Command
Command:
00 (both nybbles XY) = No note change. Do nothing. Delay time in ZZ
1 = Set absolute note ZZ = note and remembers the previous note (used by command 2). Does this once at the start of the command. Effects can be active.
2 = Restore remembred note
8 = Timed note step add upper nybble Z+1 with time of lower nybble+1 (zero based).
9 = Timed note step sub upper nybble Z-1 with time of lower nybble+1 (zero based).
A = Relative note step ZZ = step signed 8 bit value each frame. For fast glides it is better to use this, the code is quicker and shorter.
B = Set hi frequency to ZZ lo is zeroed. Skips other effects for the duration of this effect.
FF = Jump to position and frame delay ignored. ZZ = pos 0 = stop


- Filter table
XX
YY
XX 00 = Set hi cutoff with YY
XX 7F = Set lo cutoff with YY
XX 01-7E = Alter filter cutoff by signed YY for XX frames
XX 80-FE = Set band pass flags (SIDVolumeFilter = XX & 70 | volume) YY = Resonance and channel mask (SIDFilterControl)
	For example: Using 90 F7 will set the low pass filter (80+10=90) with filter resonance F and enabled on all voices (1+2+4=7)
	For example: Using A0 F4 will set the band pass filter (80+20=A0) with filter resonance F and enabled on voice 3 (4 = voice 3)
	For example: Using C0 E1 will set the band pass filter (80+40=C0) with filter resonance E and enabled on voice 1 (1 = voice 1)
XX FF = Jump

If Set band pass ($80-FE) is followed by 00 which can be followed by an optional 7F then these will be executed in the same frame.
If hi cutoff (00) is followed by lo cutoff (7F) it will be executed in the same frame.


Extended arpeggio editor:
-------------------------

Choosing the menu option "View->Extended view" will show the extended arpeggio editor.
Like the arpeggio command each frame the note is transposed by a value in a rapid cycle.
You can use up to a maximum of eight entries as transpose values.
For example with a new file extended arpeggio 2 is called "Minor seventh".
This has a length of 4 and uses the following semitone transposes 0,3,6,10
This equates to 03 then another 03 (3+3 = 6) then 04 (3+3+4=10) then F6 (-10 in decimal).
The first frame the note is played.
The next frames it is transposed up by 3 then another 3 then another 4 semitones.
Then it is transposed down -10 semitones. This is because 3 + 3 + 4 = 10 and we want the cycle to repeat without altering the pitch of the note.

Try changing the ED to EC, the note will slowly glide downwards. The extended arpeggio editor can therefore be used to create very strange effects the envelope's large vibrato cannot.



Tune information:
-----------------

Also, in the menu option "View->Extended view" there are info boxes 'Title', 'Author', 'Released' and 'General Information' where production info can be included. Exporting .SID files will include the one-liner boxes.



ReSID-FP parameters:
--------------------

The ini file %USERPROFILE%\Application Data\sidplay2\sidplay2.ini is used by the ReSID-FP library to determine SID playback parameters and filter type.
This file can be altered to reduce the CPU time needed for some PC configurations.



Commmand line options:
----------------------

In addition to the standard Windows MFC command line options the following are recognised:
	/hide
	/exit
	/openFile <filename>
	/exportToSID <filename>
	/exportToSelf <y/n> <filename>
	/exportToC64 <$hex or decimal address> <y/n> <filename>
	/play
	/loadEnvelope <hex position> <filename>
	/saveEnvelope <hex position> <filename>
	/insertEnvelope <hex position>
	/pressEnvelopeForceUsedFX <hex position>
	/deleteTrackPos <hex voice> <hex position> <hex count>

All hex values are zero based.
These options can be concatenated together and are executed in sequence. For example:
	/openFile ..\MeanStreak.msmus /saveEnvelope 1 "c:\temp\t.msenv" /insertEnvelope 1 /loadEnvelope 1 "Sounds\Tom tom1.msenv" /deleteTrackPos 0 3 3 /exportToC64 $400 y c:\temp\t2.prg /exit


Credits:
--------

Martin Piper and Alan Peters - Original C64 code
Martin Piper - PC Editor code.
Alan Peters - Demo music
Special thank you to NecroPolo, Richard Bayliss and Chabee for their testing and demo music.

Portions of this software use source code from RESID-FP http://sidplay-residfp.sourceforge.net/
A special tweaked version of ACME is used in the release. The original source is available from http://www.esw-heim.tu-clausthal.de/~marco/smorbrod/acme/
Source code for this tool and the tweaked ACME source is available from http://www.wellytop.com/C64.html
