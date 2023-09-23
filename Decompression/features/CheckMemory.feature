Feature: Test compression toolset and code

  This executes original uncompressed code that calculates a checksum by scanning the whole of memory, to test for accurate end state values.
  Then it uses the LZMPi tool to generate various self extracting output files using the original uncompressed code.
  The self extracting files results are then tested and verified as matching the output of the original code.

  Background:
    Given I have a simple overclocked 6502 system
    And I start writing memory at $ff00
    And I write the following hex bytes
      | 00 01 02 03 04 05 06 07 |
    And I start writing memory at $fff8
    And I write the following hex bytes
      | 07 06 05 04 03 02 01 00 |
    And I am using C64 processor port options
    Given add C64 hardware



  @original
  Scenario: Execute assembled output and verify result

    And I create file "target\start.a" with
      """
      !sal
      *=$400
      !source "features\Checks.a"
      """
    And I run the command line: ..\acme.exe -o target\CheckMemory.prg --labeldump target\CheckMemory.lbl -f cbm target\start.a
    And I load prg "target\CheckMemory.prg"
    And I load labels "target\CheckMemory.lbl"
    When I execute the procedure at StartChecks for no more than 4112362 instructions until PC = EndChecks

	# Standard memory validations
    When I hex dump memory between $ff00 and $ff08
    Then property "test.BDD6502.lastHexDump" must contain string "ff00: 00 01 02 03 04 05 06 07"
    When I hex dump memory between $fff8 and $10000
    Then property "test.BDD6502.lastHexDump" must contain string "fff8: 07 06 05 04 03 02 01 00"
    When I hex dump memory between $fee0 and $ff00
    Then property "test.BDD6502.lastHexDump" must contain string "Hello there foo!"
    When I hex dump memory between TheOutput and TheOutputEnd
    Then property "test.BDD6502.lastHexDump" must contain string "2: 43 98 af f1 67 77 7a 62"


  @fromSource
  Scenario Outline: Execute LZMPi self extracting compressed output using <option>
    # Build the self extracting test from source
    Given I run the command line: ..\bin\LZMPi.exe <compress> target\CheckMemory.prg target\CheckMemory.cmp 2
    Given I run the command line: ..\acme.exe -v4 DefineDoCheckMemory.a <source>
    And I load prg "TestDecompression.prg"
    And I load labels "target\CheckMemory.lbl"
    And I load labels "TestDecompression.map"
#    Given I enable trace
    # Check memory move timings, which incorporates the time taken to move the compressed data to the end of memory
    When I execute the procedure at 2061 for no more than <move> instructions until PC = CommonDataCopy_End

    # Validate decompression memory stores are completely expected
    Given load binary file "target\CheckMemory.prg" into temporary memory
    And trim "2" bytes from the start of temporary memory
    Then for memory from "$400" to "$ffff" expect writes at "$400" with temporary memory

    # Validates the time taken to decompress
    When I continue executing the procedure for no more than <instructions> instructions until PC = StartChecks
    
    # Validate machine execute state and final memory
    When I continue executing the procedure for no more than 4112362 instructions until PC = EndChecks

	# Standard memory validations
    When I hex dump memory between $ff00 and $ff08
    Then property "test.BDD6502.lastHexDump" must contain string "ff00: 00 01 02 03 04 05 06 07"
    When I hex dump memory between $fff8 and $10000
    Then property "test.BDD6502.lastHexDump" must contain string "fff8: 07 06 05 04 03 02 01 00"
    When I hex dump memory between $fee0 and $ff00
    Then property "test.BDD6502.lastHexDump" must contain string "Hello there foo!"
    When I hex dump memory between TheOutput and TheOutputEnd
    Then property "test.BDD6502.lastHexDump" must contain string "2: 43 98 af f1 67 77 7a 62"

  Examples:
    | move   | instructions | compress | source                                                                |
    | 97125  | 3234519      | -c       | TestDecompression2.a                                                  |
    | 97125  | 3266959      | -c       | DefineDoBorderEffect.a TestDecompression2.a                           |
    | 97240  | 3230722      | -c       | TestDecompression3.a                                                  |
    | 97240  | 3263162      | -c       | DefineDoBorderEffect.a TestDecompression3.a                           |
    | 98154  | 1502167      | -ce      | DefineDecompress_RNZip.a TestDecompression2.a                         |
    | 98154  | 1536827      | -ce      | DefineDoBorderEffect.a DefineDecompress_RNZip.a TestDecompression2.a  |
    | 98269  | 1502167      | -ce      | DefineDecompress_RNZip.a TestDecompression3.a                         |
    | 98269  | 1536827      | -ce      | DefineDoBorderEffect.a  DefineDecompress_RNZip.a TestDecompression3.a |
    | 147149 | 672755       | -cr      | TestDecompression3RLE.a                                               |
    | 147149 | 679815       | -cr      | DefineDoBorderEffect.a TestDecompression3RLE.a                        |
    | 156405 | 899938       | -cu      | TestDecompression4U.a                                                 |
    | 156405 | 900463       | -cu      | DefineDoBorderEffect.a TestDecompression4U.a                          |




  @selfExtract
  Scenario Outline: Execute LZMPi self extracting compressed output using <option>
    # Test the self extracted tool output
    Given I run the command line: ..\bin\LZMPi.exe -t $ff00 <option> target\CheckMemory.prg target\Compressed<option>.prg $400
    And I load prg "target\Compressed<option>.prg"
    And I load labels "target\CheckMemory.lbl"
    When I execute the procedure at 2061 for no more than <instructions> instructions until PC = StartChecks
    When I continue executing the procedure for no more than 4112362 instructions until PC = EndChecks

    # Standard memory validations
    When I hex dump memory between $ff00 and $ff08
    Then property "test.BDD6502.lastHexDump" must contain string "ff00: 00 01 02 03 04 05 06 07"
    When I hex dump memory between $fff8 and $10000
    Then property "test.BDD6502.lastHexDump" must contain string "fff8: 07 06 05 04 03 02 01 00"
    When I hex dump memory between $fee0 and $ff00
    Then property "test.BDD6502.lastHexDump" must contain string "Hello there foo!"
    When I hex dump memory between TheOutput and TheOutputEnd
    Then property "test.BDD6502.lastHexDump" must contain string "2: 43 98 af f1 67 77 7a 62"

  Examples:
    | instructions | option  |
    | 3331644      | -c64    |
    | 3364084      | -c64b   |
    | 3327962      | -c64m   |
    | 3360402      | -c64mb  |
    | 1600321      | -c64e   |
    | 1634981      | -c64eb  |
    | 1600436      | -c64me  |
    | 1635096      | -c64meb |
    | 819904       | -c64mr  |
    | 826964       | -c64mrb |
    | 1056343      | -c64mu  |
    | 1056868      | -c64mub |


