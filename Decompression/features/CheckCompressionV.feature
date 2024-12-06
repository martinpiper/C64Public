Feature: Checks files with CompressionV for expected compression performance
  
  Compresses files, checks for the desired expected number of bytes in the output file, then decompresses the compressed data and checks for the data being exactly the same.

  Scenario Outline: Check output compressed files: <item>
    Given I run the command line: ..\bin\LZMPi.exe <options> <filename> target\t<item>.cmp
    Given set property "test.bytes" equal to the file size of "target\t<item>.cmp"
    Then assert that "${test.bytes}" is equal to "<bytes>"

    # Guess who hasn't implemented RLE decompression?
    * if string "<decomp>" is not empty
      Given I run the command line: ..\bin\LZMPi.exe <decomp> target\t<item>.cmp target\t<item>.bin
      Then assert that file "<filename>" is binary equal to file "target\t<item>.bin"
    * endif

  Examples:
    | item | options     | decomp  | filename                        | bytes |
    | 1    | -ol 500 -cv | -dv     | ..\Compression\TestData\1.bin   | 2633  |
    | 2    | -ol 500 -cv | -dv     | ..\Compression\TestData\2.bin   | 2064  |
    | 3    | -ol 500 -cv | -dv     | ..\Compression\TestData\3.bin   | 1730  |
    | 4    | -ol 500 -cv | -dv     | ..\Compression\TestData\4.bin   | 3107  |
    | 5    | -ol 500 -cv | -dv     | ..\Compression\TestData\5.bin   | 17531 |
    | 6    | -ol 500 -cv | -dv     | ..\Compression\TestData\6.bin   | 7678  |
    | 7    | -ol 500 -cv | -dv     | ..\Compression\TestData\7.bin   | 7858  |
    | 8    | -ol 500 -cv | -dv     | ..\Compression\TestData\8.bin   | 2655  |
    | 9    | -ol 500 -cv | -dv     | ..\Compression\TestData\9.bin   | 4916  |
    | 10   | -ol 500 -cv | -dv     | ..\Compression\TestData\10.bin  | 33493 |
    | 11   | -ol 500 -cv | -dv     | ..\Compression\TestData\11.bin  | 30    |
