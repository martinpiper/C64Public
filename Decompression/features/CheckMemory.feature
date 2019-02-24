Feature: Test compression toolset and code

  This executes original uncompressed code that calculates a checksum by scanning the whole of memory, to test for accurate end state values.
  Then it uses the LZMPi tool to generate various self extracting output files using the original uncompressed code.
  The self extracting files results are then tested and verified as matching the output of the original code.


  @original
  Scenario: Execute assembled output and verify result
    Given I have a simple overclocked 6502 system
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
    When I execute the procedure at StartChecks for no more than 4128747 instructions until PC = EndChecks

    When I hex dump memory between $ffe0 and $10000
	Then property "test.BDD6502.lastHexDump" must contain string "Hello there foo!"

    When I hex dump memory between TheOutput and TheOutputEnd
    Then property "test.BDD6502.lastHexDump" must contain string "2: 96 d0 a5 0c 1c 56 4f 63"



  @selfExtract
  Scenario Outline: Execute LZMPi self extracting compressed output using <option>
    And I run the command line: ..\bin\LZMPi.exe <option> target\CheckMemory.prg target\Compressed<option>.prg $400
    Given I have a simple overclocked 6502 system
    And I am using C64 processor port options
    And I load prg "target\Compressed<option>.prg"
    And I load labels "target\CheckMemory.lbl"
    When I execute the procedure at 2061 for no more than <instructions> instructions until PC = StartChecks
    When I continue executing the procedure for no more than 4128747 instructions until PC = EndChecks

    When I hex dump memory between $ffe0 and $10000
	Then property "test.BDD6502.lastHexDump" must contain string "Hello there foo!"

    When I hex dump memory between TheOutput and TheOutputEnd
    Then property "test.BDD6502.lastHexDump" must contain string "2: 96 d0 a5 0c 1c 56 4f 63"

  Examples:
    | option  | instructions |
    | -c64    | 3344252      |
    | -c64b   | 3378106      |
    | -c64m   | 3340426      |
    | -c64mb  | 3374280      |
    | -c64e   | 1649778      |
    | -c64eb  | 1685193      |
    | -c64em  | 1649893      |
    | -c64emb | 1685308      |
    | -c64mr  | 821719       |
    | -c64mrb | 828784       |
