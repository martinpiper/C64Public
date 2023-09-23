Feature: Checks files for expected compression performance
  
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
    | item | options | decomp  | filename                        | bytes |
    | 1    | -c      | -d      | ..\Compression\TestData\1.bin   | 3176  |
    | 2    | -ce     | -de     | ..\Compression\TestData\1.bin   | 3322  |
    | 3    | -cr     |         | ..\Compression\TestData\1.bin   | 7997  |
    | 4    | -cu     | -du     | ..\Compression\TestData\1.bin   | 3143  |
    | 5    | -c      | -d      | ..\Compression\TestData\2.bin   | 2410  |
    | 6    | -ce     | -de     | ..\Compression\TestData\2.bin   | 2572  |
    | 7    | -cr     |         | ..\Compression\TestData\2.bin   | 4313  |
    | 8    | -cu     | -du     | ..\Compression\TestData\2.bin   | 2372  |
    | 9    | -c      | -d      | ..\Compression\TestData\3.bin   | 1927  |
    | 10   | -ce     | -de     | ..\Compression\TestData\3.bin   | 2051  |
    | 11   | -cr     |         | ..\Compression\TestData\3.bin   | 3494  |
    | 12   | -cu     | -du     | ..\Compression\TestData\3.bin   | 1912  |
    | 13   | -c      | -d      | ..\Compression\TestData\4.bin   | 3562  |
    | 14   | -ce     | -de     | ..\Compression\TestData\4.bin   | 3767  |
    | 15   | -cr     |         | ..\Compression\TestData\4.bin   | 6450  |
    | 16   | -cu     | -du     | ..\Compression\TestData\4.bin   | 3524  |
    | 17   | -c      | -d      | ..\Compression\TestData\5.bin   | 20315 |
    | 18   | -ce     | -de     | ..\Compression\TestData\5.bin   | 20512 |
    | 19   | -cr     |         | ..\Compression\TestData\5.bin   | 27881 |
    | 20   | -cu     | -du     | ..\Compression\TestData\5.bin   | 19036 |
    | 21   | -c      | -d      | ..\Compression\TestData\6.bin   | 8709  |
    | 22   | -ce     | -de     | ..\Compression\TestData\6.bin   | 8785  |
    | 23   | -cr     |         | ..\Compression\TestData\6.bin   | 12467 |
    | 24   | -cu     | -du     | ..\Compression\TestData\6.bin   | 8147  |
    | 25   | -c      | -d      | ..\Compression\TestData\7.bin   | 9230  |
    | 26   | -ce     | -de     | ..\Compression\TestData\7.bin   | 9360  |
    | 27   | -cr     |         | ..\Compression\TestData\7.bin   | 17478 |
    | 28   | -cu     | -du     | ..\Compression\TestData\7.bin   | 8809  |
    | 29   | -c      | -d      | ..\Compression\TestData\8.bin   | 3044  |
    | 30   | -ce     | -de     | ..\Compression\TestData\8.bin   | 3095  |
    | 31   | -cr     |         | ..\Compression\TestData\8.bin   | 5441  |
    | 32   | -cu     | -du     | ..\Compression\TestData\8.bin   | 2912  |
    | 33   | -c      | -d      | ..\Compression\TestData\9.bin   | 5552  |
    | 34   | -ce     | -de     | ..\Compression\TestData\9.bin   | 5618  |
    | 35   | -cr     |         | ..\Compression\TestData\9.bin   | 7961  |
    | 36   | -cu     | -du     | ..\Compression\TestData\9.bin   | 5207  |
    | 37   | -c      | -d      | ..\Compression\TestData\10.bin  | 37027 |
    | 38   | -ce     | -de     | ..\Compression\TestData\10.bin  | 37493 |
    | 39   | -cr     |         | ..\Compression\TestData\10.bin  | 55615 |
    | 40   | -cu     | -du     | ..\Compression\TestData\10.bin  | 34864 |
    | 41   | -c      | -d      | ..\Compression\TestData\11.bin  | 38    |
    | 42   | -ce     | -de     | ..\Compression\TestData\11.bin  | 39    |
    | 43   | -cr     |         | ..\Compression\TestData\11.bin  | 782   |
    | 44   | -cu     | -du     | ..\Compression\TestData\11.bin  | 36    |

