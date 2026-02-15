IRQ Disk routines
=================

* IRQ Disk loader (and save) with optional compression.
* Optional two-bit protocol.

The one-bit protocol will work with multiple drives attached. The two-bit protocol uses the ATN line and prefers only one drive to be attached.


BASIC Demo for BuildOptions.bat
===============================

* Use a normal one-bit protocol IRQ loader:
* BuildOptions.bat $c000 0 0 1 15 5 1 1 1 0
* Or use a two-bit protocol IRQ loader:
* BuildOptions.bat $c000 1 0 1 15 5 1 1 1 0
* Load _buildOptions.prg
* Attach test.d64

~~~BASIC
new
10 rem tusaricolcmp tusaribmpcmp
20 rem show a bitmap screen
30 poke52381,0:poke56576,2:poke53270,24:poke53265,59:poke53272,120
40 rem init drive 8
50 poke780,8:sys49152
60 rem load the filename at $807 with length 12
70 poke780,12:poke781,7:poke782,8:sys49158
80 rem load the filename at $814 with length 12
90 poke780,12:poke781,20:poke782,8:sys49158
100 goto 100
run
~~~



File compression
================

* LZMPi with the "-no -yo -cu" options is the compression tool to use. Also note the "2" causes the first two bytes of a prg file need to be skipped.
* For example: ..\bin\LZMPi.exe -no -yo -cu Tusari.col TusariCol.cmp 2
* This compresses the input "Tusari.col" file and writes the data to "TusariCol.cmp"
* The compressed file can then be included in a disk image.
