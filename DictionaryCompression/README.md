Dictionary Compression (LZMPiED)
================================

Reference: https://csdb.dk/forums/?roomid=11&topicid=155662

A variant of literal/copy compression (LZMPiE) that uses an extra common dictionary  (hence LZMPiED) calculated from multiple input files, to reduce overall compressed data size.

The build for this tool is also a good demonstration on how to unit test 6502 code along with java code.

Command line parameters:

    <dictionary size in bytes (decimal)> <output file name> <start offset in files (skip start bytes) (decimal)> [input files]


Example usage:

    java -jar target\DictionaryCompression-1.0-SNAPSHOT-jar-with-dependencies.jar 1024 target/compressed 2 TestData/Mule.prg TestData/MeanStreak.prg TestData/Lobster.prg TestData/Dinosaur_Disco.prg "TestData/Turrican remixed.prg" TestData/Nintendo.prg TestData/WizballGuitar.prg

Which equals 11,745 bytes

Compare this with the result of using LZMPi to compress each file individually:

    ..\bin\LZMPi.exe -c TestData/Mule.prg target/compressed_Mule.prg.cmp2
    ..\bin\LZMPi.exe -c TestData/MeanStreak.prg target/compressed_MeanStreak.prg.cmp2
    ..\bin\LZMPi.exe -c TestData/Lobster.prg target/compressed_Lobster.prg.cmp2
    ..\bin\LZMPi.exe -c TestData/Dinosaur_Disco.prg target/compressed_Dinosaur_Disco.prg.cmp2
    ..\bin\LZMPi.exe -c "TestData/Turrican remixed.prg" "target/compressed_Turrican remixed.prg.cmp2"
    ..\bin\LZMPi.exe -c TestData/Nintendo.prg target/compressed_Nintendo.prg.cmp2
    ..\bin\LZMPi.exe -c TestData/WizballGuitar.prg target/compressed_WizballGuitar.prg.cmp2
    dir target\*.cmp2

Which equals 13,088 bytes

The dictionary compression is 1,343 bytes smaller.



Demo
====

To assemble the demo, first run the command line about in "Example usage" to produce the compressed output files.

6502 asm source to decompression is in: asm\DictionaryDecompression.a

Demo source (ACME format) is in: asm\Demo.a

It can be assembled using: BuildIt.bat

Assembled demo prg file is in: target\Demo.prg

The dictionary is at $c000 so if you're using a memory viewer it shows when it is read. The compressed data is much lower in memory at around $a50. The decompression address is $8000
