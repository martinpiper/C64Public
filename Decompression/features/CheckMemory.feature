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


  @original
  Scenario: Execute assembled output and verify result

    And I am using C64 processor port options
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
    Given I run the command line: ..\acme.exe -v3 DefineDoCheckMemory.a <source>
    And I am using C64 processor port options
    And I load prg "TestDecompression.prg"
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
    | instructions | compress | source                                                                |
    | 3344252      | -c       | TestDecompression2.a                                                  |
    | 3378106      | -c       | DefineDoBorderEffect.a TestDecompression2.a                           |
    | 3340426      | -c       | TestDecompression3.a                                                  |
    | 3374280      | -c       | DefineDoBorderEffect.a TestDecompression3.a                           |
    | 1649778      | -ce      | DefineDecompress_RNZip.a TestDecompression2.a                         |
    | 1685193      | -ce      | DefineDoBorderEffect.a DefineDecompress_RNZip.a TestDecompression2.a  |
    | 1649893      | -ce      | DefineDecompress_RNZip.a TestDecompression3.a                         |
    | 1685308      | -ce      | DefineDoBorderEffect.a  DefineDecompress_RNZip.a TestDecompression3.a |
    | 821719       | -cr      | TestDecompression3RLE.a                                               |
    | 828784       | -cr      | DefineDoBorderEffect.a TestDecompression3RLE.a                        |




  @selfExtract
  Scenario Outline: Execute LZMPi self extracting compressed output using <option>
	# Test the self extracted tool output
    Given I run the command line: ..\bin\LZMPi.exe -t $ff00 <option> target\CheckMemory.prg target\Compressed<option>.prg $400
    And I am using C64 processor port options
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
    | instructions | option  | compress |
    | 3344252      | -c64    | -c       |
    | 3378106      | -c64b   | -c       |
    | 3340426      | -c64m   | -c       |
    | 3374280      | -c64mb  | -c       |
    | 1649778      | -c64e   | -ce      |
    | 1685193      | -c64eb  | -ce      |
    | 1649893      | -c64em  | -ce      |
    | 1685308      | -c64emb | -ce      |
    | 821719       | -c64mr  | -cr      |
    | 828784       | -c64mrb | -cr      |
