TapeTool.exe
A tool to provide a command line interface that enables C64 format TAP files to be read and created with kernal and turbo data.
It provides very low level tape read/write capability with higher level functions to write files.
Using "TapeTool.exe -h" will display more detailed command line help.

A quick start guide:

Any line starting with ">" is intended to be run from the DOS command line.

TapeTool will most often be used with ACME to assemble the MartyLoad tape turbo files and write the data to a TAP file.
The ExampleBuild.bat file shows how to run the examples given below from a batch file.

First make sure the "_config.a" file only has the following lines enabled:
	OpenBorders = 1 
	MainLoad_EnableScreen = 1 
	EnableBlockChecksum = 1 
	EnableBlockCompression = 1 
	TapeTurboSpeed = $80 
	NovaishLoad = 1 
This config file tells the tape turbo code how to build. It enables the border opening code in the Nova load style loader.
It also enables extended error checksums for file data. It sets the tape turbo speed to be $80 and also defines the different
turbo tape byte codes used. The turbo byte codes don't really need to be changed but they can be configured if required.
The block compression is also enabled.

Now assemble the code using:
>	acme.exe --lib  ../ --lib ../../  -v3 --msvc Loaders.a
This creates three files.
"Loaders.bin" the assembled code.
"Loaders.map" the map file of symbols used.
"Loaders.lbl" VICE monitor format labels, ready for being loaded by VICE if needed.

Next start writing the auto-boot tape header. This contains a simple turbo tape loader that loads data faster than normal.
This is a special format of the kernal file structure that contains data in the tape buffer and only saves one copy of the
file rather than two copies of the file. This enables the auto-boot code to load and run quicker than normal.
>	TapeTool.exe wn "test.tap" m "Loaders.map" ocb1 Loaders.bin c
Explanation:
(wn "test.tap") This creates a new file "test.tap" for writing.
(m "Loaders.map") Loads the symbols from "Loaders.map".
(ocb1 Loaders.bin) Writes the auto-boot header using data from "Loaders.bin" using the default values from the map file.
(c) Closes the file and sets the correct data length.

Next write the real turbo loader using turbo tape format that will load the rest of the example files. Because this data
is loaded from the small auto-boot code it uses a special format of turbo data that doesn't contain much header or
error correction information.
This real turbo loader is larger than the small auto-boot code and includes routines to scroll a message and play music.
It also loads information with a turbo tape format that includes extra error correction checks and can prompt for the tape
to be rewound if an error is detected.
>	TapeTool.exe w "test.tap" a m "Loaders.map" otl$c0 otft "Loaders.bin" .RealCodeStart .RealCodeEnd c
Explanation:
(w "test.tap" a) Open the file "test.tap" for writing. Appends data to the file.
(m "Loaders.map") Loads the symbols from "Loaders.map".
(otl$c0) Writes $c0 bytes of blank data in turbo format, a blank leader like this allows time for the tape motor to start.
(otft "Loaders.bin" .RealCodeStart .RealCodeEnd) From the file "Loaders.bin" writes bytes starting from file
offset .RealCodeStart and ending at .RealCodeEnd. These values are read from the symbol map file.
(c) Closes the file and sets the correct data length.

All tape data that follows is loaded by the real turbo loader. It is written with the block checksum method.
The bottom border sprite data needs to be written.
>	TapeTool.exe w "test.tap" a m "Loaders.map" otl$c0 otfbr "Loaders.bin" 0 .SpriteDataStart .SpriteDataEnd $200 c
Explanation:
(w "test.tap" a m "Loaders.map") Open the tap file for writing, appends data, loads the symbol map file.
(otl$c0) Another short leader for the tape motor to start.
(otfbr "Loaders.bin" 0 .SpriteDataStart .SpriteDataEnd $200) Writes turbo data with checksum blocks using data from the
file "Loaders.bin". It saves using a filename byte of 0. The sata offset starts from .SpriteDataStart and ends at
.SpriteDataEnd. The load address for the data is $200
(c) Closes the file and sets the correct data length.

Next write the music.
>	TapeTool.exe w "test.tap" a m "Loaders.map" otl$c0 otfbr "R.MUS8000.PRG" 1 c
Explanation:
(w "test.tap" a m "Loaders.map") As before, opens the tap file, appends data, loads the symbol map file.
(otl$c0) Another short leader.
(otfbr "R.MUS8000.PRG" 1) Writes turbo data with checksum blocks using data from the "R.MUS8000.PRG" file.
This time the filename byte is 1. The start/end offset and load address is optional and not included. In this case the
information is calculated from the PRG format file.
(c) As before, closes the file and sets the correct data length.

Next write the multiplexor demo.
>	TapeTool.exe w "test.tap" a m "Loaders.map" otl$c0 otfbr "TestMultiplexor.prg" 2 otl$c0 c
(w "test.tap" a m "Loaders.map") As before, opens the tap file, appends data, loads the symbol map file.
(otl$c0) Another short leader.
(otfbr "TestMultiplexor.prg" 2) As before writes PRG format data, but this time it uses the "TestMultiplexor.prg" file.
(otl$c0 c) Because this is the last file on the tape it is a good idea to include a short amount of leadout. The file is
also closed and the correct file length set with the 'c' parameter.


So in summary the following lines are executed:
acme.exe --lib  ../ --lib ../../  -v3 --msvc Loaders.a
TapeTool.exe wn "test.tap" m "Loaders.map" ocb1 Loaders.bin c
TapeTool.exe w "test.tap" a m "Loaders.map" otl$c0 otft "Loaders.bin" .RealCodeStart .RealCodeEnd c
TapeTool.exe w "test.tap" a m "Loaders.map" otl$c0 otfbr "Loaders.bin" 0 .SpriteDataStart .SpriteDataEnd $200 c
TapeTool.exe w "test.tap" a m "Loaders.map" otl$c0 otfbr "R.MUS8000.PRG" 1 c
TapeTool.exe w "test.tap" a m "Loaders.map" otl$c0 otfbr "TestMultiplexor.prg" 2 otl$c0 c


TapeTape tool can also write multiple input files at the same time by including them on the same command line. So the
above lines can become:
>	TapeTool.exe wn "test.tap" m "Loaders.map" ocb1 Loaders.bin otl$c0 otft "Loaders.bin" .RealCodeStart .RealCodeEnd otl$c0 otfbr "Loaders.bin" 0 .SpriteDataStart .SpriteDataEnd $200 otl$c0 otfbr "R.MUS8000.PRG" 1 otl$c0 otfbr "TestMultiplexor.prg" 2 otl$c0 c
The output TAP files would be identical regardless of the method used to write them since the same data is being written.


It is also a good idea to include a leader because any processing of the loaded file can take some time, which can be
longer than the gap between files if there was no leader. It also helps the tape routines sync when going from the kernal
to the turbo data.


*The "_config.a" file can have one of the following lines enabled:
	SmallLoader = 1
	MartyLoad = 1
	NovaishLoad = 1
These enable the different auto-boot loaders:
The "SmallLoader" doesn't enable the screen until the real loader is started.
The "MartyLoad" displays some animated bars like the old Cyber loader.
The "NovaishLoad" displays a screen that looks like the old Nova loader.

If none of the auto-boot loader options are chosen then a different auto-boot loader is used. This loader doesn't use a tiny
header, instead it uses a header with some extra filename and load address information.
So instead of the real loader being written with "otft" it needs to be written with "otf" instead.
For example use: otf "Loaders.bin" 255 .RealCodeStart .RealCodeEnd .MainSecondLoaderStart
Instead of: otft "Loaders.bin" .RealCodeStart .RealCodeEnd


The "_config.a" file can also be used to configure the tape turbo speed. By default it uses: TapeTurboSpeed = $80
However this number can be increased to make the loader slower, but more reliable. Or it can be decreased to make it
faster, but more unreliable. The default value of $80 is good for most tape decks.
TapeTurboSpeed is the time in cycles taken for half of one whole pulse.
The zero pulse is TapeTurboSpeed*2
The one pulse is TapeTurboSpeed*2*2
This results in an average of (TapeTurboSpeed*2 + TapeTurboSpeed*2*2)/2 = TapeTurboSpeed*3 for each pulse.
This means an average of $180 (384) cycles for each bit.
Since a PAL C64 operates at 985248 Hz
This gives 985248 / 384 = 2565 bits per second.



If the configuration value "EnableBlockChecksum = 1" is not enabled then all tape data is loaded without checksum
blocks. This means all TapeTool "otfbr" parameters must be changed to be "otf". Generally though checksum blocks should be
enabled since they greatly improve loading reliability.


Including a label called JMPStartAddress will initialise the machine to a BASIC friendly state then jumps to the address.
Including a label called JMPQuickStartAddress does nothing to tidy the machine to a known state before jumping to the address.
If none of the JMP labels are included then the loader will warm start BASIC and execute any program that is loaded with "RUN".


*Interleaving data on the tape
When using the turbo block checksum method it is possible to interleave more than one data file on the tape by using the 's' parameter.
For example: otl$c0 otfb "TestMultiplexor.prg" 2 s otfb "R.MUS8000.PRG" 100 s otfb "HelloWorldBasic.prg" 101 otl$c0 c
This will write the blocks for "TestMultiplexor.prg" and interleave the blocks from "R.MUS8000.PRG" and "HelloWorldBasic.prg".
Each file must have a unique filename so the loader can skip blocks and only load the required file.
Note each file does not need a new leader because they share the leader before the interleaved blocks.
This feature is especially useful for multiloaders where a game can load either level file without winding the tape to a different position.
The interleaved files do not have to be the same length.


*Using the option "otfbr" uses RLE compression for each block, if there is a significant saving for the block.
EnableBlockCompression = 1 will need to be enabled (in _config.a) to handle this data block type


*Using TapeTool to reverse engineer tape files.
TapeTool can also read kernal or turbo format data. This allows existing TAP files to be read and help debug them.
For example:
Execute: TapeTool.exe r "FileABCDEF Data01234567.tap" bc1000
Reads the example file "FileABCDEF Data01234567.tap" and displays upto 1000 kernal format bytes.
You should see something like:
	New data at offset $6a2c short pulse at $31
	 $89 $88 $87 $86 $85 $84 $83 $82 $81 $03 $00 $04 $07 $04 $41 $42 $43 $44 $45 $46 $2e $54 $41 $50 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $
	20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20
	 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $
	20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20
	 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $
	20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $48 EOD

	New data at offset $7a45 short pulse at $31
	 $09 $08 $07 $06 $05 $04 $03 $02 $01 $03 $00 $04 $07 $04 $41 $42 $43 $44 $45 $46 $2e $54 $41 $50 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $
	20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20
	 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $
	20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20
	 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $
	20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $20 $48 EOD

	New data at offset $9f61 short pulse at $31
	 $89 $88 $87 $86 $85 $84 $83 $82 $81 $00 $01 $02 $03 $04 $05 $06 $07 EOD

	New data at offset $a106 short pulse at $30
	 $09 $08 $07 $06 $05 $04 $03 $02 $01 $00 $01 $02 $03 $04 $05 $06 $07 EOD
	End of file reached by data.

The first block of data at offset $6a2c is the first tape header and tape buffer area bytes.
The second block of data at offset $7a45 is the duplicate tape header and tape buffer containing the same bytes as before but with a slightly different header.
The tape header contains the load address $400 and ending address $407, the file name and blank tape header bytes.
The third block of data at offset $9f61 is another header followed by the byte 0 to 7 which are loaded to the start of screen memory at $400
