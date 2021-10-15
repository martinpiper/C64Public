// MinGW build:
// gcc main.cpp C64Tape.cpp -llibstdc++ -o TapeTool.exe

// Testing parameters:
// r "FileABCDEF Data01234567.tap" bc100000
// r "test.tap" bc100000
// ft 0x100 0x200 1 0xaa r "FileABCDEF Data01234567.tap" bc100000 r "Only turbo.tap" bt10  r "Kernal then turbo.tap" bc100000 p $a8d0 bt20 bt20
// Don't forget the number of chars in a kernal header from the start of the filename is 187, so 0 to 186 *inclusive*. This means there are 171 ($ab) bytes after the name.

// Writes the data 0,1,2,3,4,5,6 at the start of the screen with filename contained in the file "Loaders.bin"
// wn "test.tap" ocl$6a10 ocn $89 $88 $87 $86 $85 $84 $83 $82 $81 ocnr $03 $00 $04 $07 $04 @"Loaders.bin" 0 187 ! oce ocl$4f ocn $9 $8 $7 $6 $5 $4 $3 $2 $1 ocnr $03 $00 $04 $07 $04 @"Loaders.bin" 0 187 ! oce ocl$1500 ocn $89 $88 $87 $86 $85 $84 $83 $82 $81 ocnr $00 $01 $02 $03 $04 $05 $06 ! oce ocl$4f ocn $9 $8 $7 $6 $5 $4 $3 $2 $1 ocnr $00 $01 $02 $03 $04 $05 $06 ! oce ocl$1500 c

// Writes the autoboot tape loader from the file "Loaders.bin"
// wn "test.tap" ocl$6a10 ocn $89 $88 $87 $86 $85 $84 $83 $82 $81 ocnr $03 $a7 $02 $16 $03 @"Loaders.bin" 0 16 @"Loaders.bin" $16f $21a ! oce ocl$4f ocn $09 $08 $07 $06 $05 $04 $03 $02 $01 ocnr $03 $a7 $02 $16 $03 @"Loaders.bin" 0 16 @"Loaders.bin" $16f $21a ! oce ocl$1500 ocn $89 $88 $87 $86 $85 $84 $83 $82 $81 ocnr @"Loaders.bin" $100 $16f ! oce ocl$4f c
// Appends the real second turbo loader with the short header
// w "test.tap" a otl$c0 otn $aa $69 otnr @"Loaders.bin" $1000 $173a ! ote c
// Appends sprite data using the manual full checksum header option
// w "test.tap" a otl$c0 otnr $aa $f0 $96 $00 $00 $02 $01 $3f $02 ! otnr @"Loaders.bin" $173b $1779 ! ote c

// Writes the full turbo tape example using assembled code
/*
Debug\TapeTool.exe wn "test.tap" ocl$6a10 ocn $89 $88 $87 $86 $85 $84 $83 $82 $81 ocnr $03 $a7 $02 $16 $03 @"Loaders.bin" 0 16 @"Loaders.bin" $16f $21a ! oce ocl$4f ocn $09 $08 $07 $06 $05 $04 $03 $02 $01 ocnr $03 $a7 $02 $16 $03 @"Loaders.bin" 0 16 @"Loaders.bin" $16f $21a ! oce ocl$1500 ocn $89 $88 $87 $86 $85 $84 $83 $82 $81 ocnr @"Loaders.bin" $100 $16f ! oce ocl$4f c
Debug\TapeTool.exe w "test.tap" a otl$c0 otn $aa $69 otnr @"Loaders.bin" $1000 $173b ! ote c
Debug\TapeTool.exe w "test.tap" a otl$c0 otfb "Loaders.bin" 0 $173b $177a $200 c
Debug\TapeTool.exe w "test.tap" a otl$c0 otfb "..\R.MUS8000.PRG" 1 c
Debug\TapeTool.exe w "test.tap" a otl$c0 otfb "..\TestMultiplexor.prg" 2 c

Debug\TapeTool.exe wn "test.tap" ocl$6a10 ocn $89 $88 $87 $86 $85 $84 $83 $82 $81 ocnr $03 $a7 $02 $16 $03 @"Loaders.bin" 0 16 @"Loaders.bin" $16f $21a ! oce ocl$4f ocn $09 $08 $07 $06 $05 $04 $03 $02 $01 ocnr $03 $a7 $02 $16 $03 @"Loaders.bin" 0 16 @"Loaders.bin" $16f $21a ! oce ocl$1500 ocn $89 $88 $87 $86 $85 $84 $83 $82 $81 ocnr @"Loaders.bin" $100 $16f ! oce ocl$4f otl$c0 otn $aa $69 otnr @"Loaders.bin" $1000 $173b ! ote otl$c0 otfb "Loaders.bin" 0 $173b $177a $200 otl$c0 otfb "..\R.MUS8000.PRG" 1 otl$c0 otfb "..\TestMultiplexor.prg" 2 otl$c0 c

Debug\TapeTool.exe m "Loaders.map" wn "test.tap" ocl$6a10 ocn $89 $88 $87 $86 $85 $84 $83 $82 $81 ocnr $03 l.TapeRelocatedStart h.TapeRelocatedStart $16 $03 @"Loaders.bin" 0 16 @"Loaders.bin" .tapeHeader .tapeHeaderEnd ! oce ocl$4f ocn $09 $08 $07 $06 $05 $04 $03 $02 $01 ocnr $03 $a7 $02 $16 $03 @"Loaders.bin" 0 16 @"Loaders.bin" .tapeHeader .tapeHeaderEnd ! oce ocl$1500 ocn $89 $88 $87 $86 $85 $84 $83 $82 $81 ocnr @"Loaders.bin" .startBlock .TapeTurboEndOfExtendedZeroPageCodeAndIRQ ! oce ocl$4f otl$c0 otn $aa $69 otnr @"Loaders.bin" .RealCodeStart .RealCodeEnd ! ote otl$c0 otfb "Loaders.bin" 0 .SpriteDataStart .SpriteDataEnd $200 otl$c0 otfb "..\R.MUS8000.PRG" 1 otl$c0 otfb "..\TestMultiplexor.prg" 2 otl$c0 c

// Using the assembled output and label file for offsets:
// With TinyHeader = 1
..\..\acme.exe --lib  ../ --lib ../../  -v4 --msvc Loaders.a && Debug\TapeTool.exe m "Loaders.map" wn "test.tap" ocb1 Loaders.bin otl$c0 otft "Loaders.bin" .RealCodeStart .RealCodeEnd otl$c0 otfb "Loaders.bin" 0 .SpriteDataStart .SpriteDataEnd $200 otl$c0 otfb "..\R.MUS8000.PRG" 1 otl$c0 otfb "..\TestMultiplexor.prg" 2 otl$c0 c && copy /y Loaders.lbl test.lbl && test.tap
// No checksum blocks
..\..\acme.exe --lib  ../ --lib ../../  -v4 --msvc Loaders.a && Debug\TapeTool.exe m "Loaders.map" wn "test.tap" ocb1 Loaders.bin otl$c0 otft "Loaders.bin" .RealCodeStart .RealCodeEnd otl$c0 otf "Loaders.bin" 0 .SpriteDataStart .SpriteDataEnd $200 otl$c0 otf "..\R.MUS8000.PRG" 1 otl$c0 otf "..\TestMultiplexor.prg" 2 otl$c0 c && copy /y Loaders.lbl test.lbl && test.tap
// For LoTD (Scroller_LOTD defined in _config.a)
..\..\acme.exe --lib  ../ --lib ../../  -v4 --msvc Loaders.a && Debug\TapeTool.exe m "Loaders.map" wn "test.tap" ocb1 Loaders.bin otl$c0 otft "Loaders.bin" .RealCodeStart .RealCodeEnd otl$c0 otfb "Loaders.bin" 0 .SpriteDataStart .SpriteDataEnd $200 otl$c0 otfb "..\R.MUS8000.PRG" 1 otl$c0 otfb "..\LotDComp.prg" 2 otl$c0 c && copy /y Loaders.lbl test.lbl && test.tap

// Without TinyHeader
..\..\acme.exe --lib  ../ --lib ../../  -v4 --msvc Loaders.a && Debug\TapeTool.exe m "Loaders.map" wn "test.tap" ocb1 Loaders.bin otl$c0 otf "Loaders.bin" 255 .RealCodeStart .RealCodeEnd .MainSecondLoaderStart otl$c0 otfb "Loaders.bin" 0 .SpriteDataStart .SpriteDataEnd $200 otl$c0 otfb "..\R.MUS8000.PRG" 1 otl$c0 otfb "..\TestMultiplexor.prg" 2 otl$c0 c && copy /y Loaders.lbl test.lbl && test.tap
// No checksum blocks
..\..\acme.exe --lib  ../ --lib ../../  -v4 --msvc Loaders.a && Debug\TapeTool.exe m "Loaders.map" wn "test.tap" ocb1 Loaders.bin otl$c0 otf "Loaders.bin" 255 .RealCodeStart .RealCodeEnd .MainSecondLoaderStart otl$c0 otf "Loaders.bin" 0 .SpriteDataStart .SpriteDataEnd $200 otl$c0 otf "..\R.MUS8000.PRG" 1 otl$c0 otf "..\TestMultiplexor.prg" 2 otl$c0 c && copy /y Loaders.lbl test.lbl && test.tap
// For the BASIC run test without TinyHeader
..\..\acme.exe --lib  ../ --lib ../../  -v4 --msvc Loaders.a && Debug\TapeTool.exe m "Loaders.map" wn "test.tap" ocb1 Loaders.bin otl$c0 otf "Loaders.bin" 255 .RealCodeStart .RealCodeEnd .MainSecondLoaderStart otl$c0 otfb "Loaders.bin" 0 .SpriteDataStart .SpriteDataEnd $200 otl$c0 otfb "..\R.MUS8000.PRG" 1 otl$c0 otfb "HelloWorldBasic.prg" 2 otl$c0 c && copy /y Loaders.lbl test.lbl && test.tap

// Writes the full kernal boot loader in one option using the label file offsets
Debug\TapeTool.exe m "Loaders.map" wn "test.tap" ocb1 Loaders.bin c
Debug\TapeTool.exe m "Loaders.map" wn "test.tap" ocb1 Loaders.bin .TapeRelocatedStart .nameStart .nameEnd .tapeHeader .tapeHeaderEnd .startBlock .TapeTurboEndOfExtendedZeroPageCodeAndIRQ c

// Interleaving test
Debug\TapeTool.exe m "Loaders.map" wn "test.tap" ocb1 Loaders.bin otl$c0 otft "Loaders.bin" .RealCodeStart .RealCodeEnd otl$c0 otfb "Loaders.bin" 0 .SpriteDataStart .SpriteDataEnd $200 otl$c0 otfb "..\R.MUS8000.PRG" 1 otl$c0 otfb "..\TestMultiplexor.prg" 2 s otfb "..\R.MUS8000.PRG" 100 s otfb "HelloWorldBasic.prg" 101 otl$c0 c


// Applying delta test
m "Loaders.map" wn "test.tap" ocb1 Loaders.bin otl$c0 otft "Loaders.bin" .RealCodeStart .RealCodeEnd otl$c0 otfb "Loaders.bin" 0 .SpriteDataStart .SpriteDataEnd $200 otl$c0 otfb "..\R.MUS8000.PRG" 1 otl$c0 otfb "..\TestMultiplexor.prg" 2 otl$c0 c d "test.tap" -1


// Using RLE compressed blocks, for the sprite data
m "Loaders.map" wn "test.tap" ocb1 Loaders.bin otl$c0 otft "Loaders.bin" .RealCodeStart .RealCodeEnd otl$c0 otfbr "Loaders.bin" 0 .SpriteDataStart .SpriteDataEnd $200 otl$c0 otfb "..\R.MUS8000.PRG" 1 otl$c0 otfb "..\TestMultiplexor.prg" 2 otl$c0 c
..\..\acme.exe --lib  ../ --lib ../../  -v4 --msvc Loaders.a && Debug\TapeTool.exe m "Loaders.map" wn "test.tap" ocb1 Loaders.bin otl$c0 otft "Loaders.bin" .RealCodeStart .RealCodeEnd otl$c0 otfbr "Loaders.bin" 0 .SpriteDataStart .SpriteDataEnd $200 otl$c0 otfb "..\R.MUS8000.PRG" 1 otl$c0 otfb "..\TestMultiplexor.prg" 2 otl$c0 c && copy /y Loaders.lbl test.lbl && C:\VICE\x64.exe test.tap


// With IgnoreSecondLoader and SmallLoader defined this will ignore the main second loader and just write the multiplexor demo.
// The fastest (reliable) speed in this configuration is TapeTurboSpeed = $30
// This results is about 985248 / 144 = 6842 bits per second, or 855 bytes per second.
..\..\acme.exe --lib  ../ --lib ../../  -v4 --msvc Loaders.a && Debug\TapeTool.exe m "Loaders.map" wn "test.tap" ocb1 Loaders.bin otl$c0 otft "..\TestMultiplexor.prg" $101 $4896 otl$c0 c && copy /y Loaders.lbl test.lbl && test.tap
// Without TinyHeader
..\..\acme.exe --lib  ../ --lib ../../  -v4 --msvc Loaders.a && Debug\TapeTool.exe m "Loaders.map" wn "test.tap" ocb1 Loaders.bin otl$c0 otf "..\TestMultiplexor.prg" $ff $101 $4896 $900 otl$c0 c && copy /y Loaders.lbl test.lbl && test.tap


// With loading screen
wn "test.tap" m "Loaders.map" ocb1 Loaders.bin otl$c0 otft "Loaders.bin" .RealCodeStart .RealCodeEnd otl$c0 otfbr "Loaders.bin" 0 .SpriteDataStart .SpriteDataEnd $200 otl$c0 otfbr "..\R.MUSC000.prg" 1 otl$c0 otfbr "..\..\BlankProjectComp\Tusari.scr" 2 $2 $3eb .BitmapLogoScreen otl$c0 otfbr "..\..\BlankProjectComp\Tusari.col" 3 otl$c0 otfbr "..\..\BlankProjectComp\Tusari.bmp" 4 $2 $1f43 .BitmapLogo otl$c0 otfbr "..\TestMultiplexor.prg" 5 otl$c0 c

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <vector>
#include <string>
#include "C64Tape.h"

int main(int argc, char **argv)
{
	C64Tape tape;

	return tape.HandleParams( argc , argv );
}
