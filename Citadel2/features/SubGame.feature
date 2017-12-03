@c64
Feature: Test the sub-game code

  This assembles the SubGame.a code and checks the expected results after executing it

  Scenario: Simple sub-game test
    Given I have a simple overclocked 6502 system
    And I run the command line: ..\acme.exe --lib ../Scroller/ --labeldump test.lbl asm\SubGameStub.a
    And I load prg "SubGameStub.prg"
    And I load labels "test.lbl"

    When I execute the procedure at SubGameEnter for no more than 7000 instructions
    When I execute the procedure at SubGameUpdate for no more than 1200 instructions
    When I execute the procedure at SubGameUpdate for no more than 1200 instructions
#    Then I expect register a equal $ea
