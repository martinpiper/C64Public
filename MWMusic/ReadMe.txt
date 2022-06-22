Keys:

<enter>						= Restart the current track
<space>						= Pause/unpause the music
1/2/4/8						= Bit twiddle the hi nibbles
0							= Clear to zero
Cursor up/down/left/down	= Moves the editing cursor
,/. or </>					= Incr/decr the highlighted number
-/+ or -/=					= Incr/decr the effect/sequence/track depending on which is highlighted by the cursor
q/a							= Scroll up/down the sequence or tracks
t/T							= Change current track tempo
d/u							= Octave down or up for the current value in the sequence editor
c			                = Copy current column into buffer
r							= Replace current column with buffer
<insert>					= Inserts a zero byte at the current sequence or track edit cursor position
<delete>					= Deletes a byte at the current sequence or track edit cursor position
<Home>						= Return to the top of the current editor column
F1/F3/F5					= Toggle channel 1/2/3 mute
m							= Switch editor mode between the main view and vibrato table editor

b							= Set beat length
B							= Set bar length
								BAR:BEAT:NOTE is displayed in the editor
s							= Save file
l							= Load file
$							= During a filename prompt displays the directory
Runstop key					= During a filename prompt aborts the file operation



Useful Vice command line options:

Turn off true drive (slow) hardware emulation. Enable virtual traps for faster drive access. Mount the D64 in drive 8. Inject the prg into memory and start it.
	+truedrive -virtualdev -8 bin\data.d64 -autostartprgmode 1 -remotemonitor MWMusicEditor.prg
