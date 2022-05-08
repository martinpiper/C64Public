Feature: Assemble 6502 code test

  This assembles simple code and checks the expected results after executing it

  Scenario: Test decompression
    Given compress files with dictionary size "1024" and target filename "target/compressed" with skip "2":
      | TestData/Mule.prg             |
      | TestData/MeanStreak.prg       |
      | TestData/Lobster.prg          |
      | TestData/Dinosaur_Disco.prg   |
      | TestData/Turrican remixed.prg |
      | TestData/Nintendo.prg         |
      | TestData/WizballGuitar.prg    |

    Given I have a simple overclocked 6502 system
    And I run the command line: ..\acme.exe -v3 --lib ../ -o target\test.prg --labeldump target\test.lbl -f cbm features\TestDecompression.a
    And I load prg "target\test.prg"
    And I load labels "target\test.lbl"
#    Given I enable trace with indent
    And That does fail on BRK
    And I enable uninitialised memory read protection with immediate fail

    When I execute the procedure at init until return
    Given I reset the cycle count
    Given I set register A to 0
    When I execute the procedure at decompress until return
    Then I expect the cycle count to be no more than 411707 cycles

    When I hex dump memory between MusicInit and MusicInit+32
    Then property "test.BDD6502.lastHexDump" must contain string ignoring whitespace "8000: 4c fe 81 4c 71 82 4c ed  86 4d 53 32 32 00 00 00"
    When I hex dump memory between MusicInit+$b00 and MusicInit+$b00+32
    Then property "test.BDD6502.lastHexDump" must contain string ignoring whitespace "8b00: 81 41 ff 81 41 ff 81 41  81 ff 21 ff 17 ff 11 ff"
