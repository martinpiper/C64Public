Creating delta compression files
================================


Example data
------------

Two files:
	ExampleData\f1.bin
	ExampleData\f2.bin

The first file is some screen data. The second file contains updated screen information, the screen could be scrolled, some lines moved around, or new data added, a delta will be calculated for this second file from the first file. The data should be $400 bytes long, for a whole screen worth od data, without any two byte header.


Command line usage
------------------

```
cd C:\Work\C64\Animation\DeltaCompression\
```

Or wherever this directory is located.

```
mkdir c:\temp\
```

If needed.

Use something like:

```
Release\DeltaCompression.exe -nofirstframe -emptyframebuffer ExampleData\f*d.bin 0 1 1 2 1 $400 $400 c:\temp\f*d.cmp
```

This command line explanation:

DeltaCompression : [-savedictionary <max len> <filename>] [-maxframesize <bytes>] [-nofirstframe] [-emptyframebuffer] [-bitmap] <Input file mask in name*05d.ext format> <byte offset for start> <start frame number> <frame step> <end frame number> <number of banks> <first frame size> <other frame size> <output file name mask in name*05d.ext format> [copy size test tweak (254)]


-nofirstframe -emptyframebuffer
: This tells the compression that the frames won't loop, that there isn't any starting data.
: This causes the first delta file to be a roughly compressed screen. In effect, the first "delta" contains all the data to restore the first frame. If the data comes from another source, it was copied or present in RAM, then this first delta can be ignored. The second delta will then apply over the top of the data already present.


ExampleData\f\*d.bin
: This is a wildcard name, the * is replaced with % for a stdC sprintf, so it will translate to "f%d.bin". If "f*05d.bin" was used this would translate to "f%05d.bin" and prefix the number with up to 5 zeros.


0
: 0 is the number of bytes to trim from the start of each file, in this case 0 bytes since they have now two byte header.


1 1 2
: Is the start frame 1, frame step 1, and end frame 2. This reads, in numeric order, files "f1.bin" then "f2.bin".


1
: 1 is the number of banks for the data, is this case the input data is read into 1 bank, meaning the data for each frame replaces the previous data. If 2 was used then each frame of data would be loaded so that two copies, the current and previous frames, were in memory at the same time, a classic double buffered arrangement.


$400 $400
: The size of the data for the first frame, using $400 bytes, and all subsequent frames also use $400 bytes.


c:\temp\f*d.cmp
: A wildcard name for each compressed delta file. File "f2.cmp" would be the delta information for "f2.bin" compared to the previous file "f1.bin".


Assemble the example
--------------------

```
cd C:\Work\C64\Animation\DeltaCompression\

..\..\acme.exe -f cbm --lib ..\ --lib ..\..\ -v -o c:\temp\t.prg ExampleData\SimpleTest.a

Run: c:\temp\t.prg
```

Press fire to decompress the first screen, and then fire again to decompress the second frame using the delta.
