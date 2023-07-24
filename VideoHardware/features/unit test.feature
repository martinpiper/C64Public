Feature: Unit tests for subroutines
  
  Scenario: Tests non-sorting gap calculation
    Given I have a simple overclocked 6502 system
    And I run the command line: ..\acme.exe -v4 --lib ../ --lib ../../ --lib ../../../ --msvc "features\unit test.a"

    And I load prg "bin/main.prg"
    And I load labels "tmp/main.map"

    Given enable remote debugging
#    And wait for debugger connection
#    And wait for debugger command

    Given I enable trace with indent
    
    Given I start writing memory at GameSpriteRow
    And I write the following hex bytes
      | 02 03 04 00 00 01 02 00   00 01 02 00 00 01 03 01 |


    Given I enable uninitialised memory read protection with immediate fail


    Given I reset the cycle count
    When I execute the procedure at GapFind_Init until return
    Then I expect the cycle count to be no more than 34 cycles

    Given I reset the cycle count
    When I execute the procedure at GapFind_Scan until return
    And I expect to see GameSpriteSplitPos equal 4
    Then I expect the cycle count to be no more than 606 cycles

