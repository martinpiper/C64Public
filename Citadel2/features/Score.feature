@c64
Feature: Test the Score code

  This assembles the Rand.a code and checks the expected results after executing it

  Background:
    Given I have a simple overclocked 6502 system
    Given I fill memory with 0

    And I run the command line: ..\acme.exe -o test.prg --labeldump test.lbl -f cbm --setpc $1000 asm\Score.a
    And I load prg "test.prg"
    And I load labels "test.lbl"

    When I set register a to lo($400)
    When I set register x to hi($400)
    When I execute the procedure at ScoreSetAddress for no more than 3 instructions


  Scenario: Simple Score test

    Then I set register y to 5
    When I execute the procedure at ScoreInit for no more than 17 instructions

    Then I expect to see $3ff equal 0
    Then I expect to see $400 equal Score_ZeroCharacter
    Then I expect to see $401 equal Score_ZeroCharacter
    Then I expect to see $402 equal Score_ZeroCharacter
    Then I expect to see $403 equal Score_ZeroCharacter
    Then I expect to see $404 equal Score_ZeroCharacter
    Then I expect to see $405 equal 0


  Scenario: Simple Score add test

    Given I start writing memory at $400
    Given I write the following bytes
      | Score_ZeroCharacter+3 | Score_ZeroCharacter+4 | Score_ZeroCharacter+5 | Score_ZeroCharacter+6 | Score_ZeroCharacter+7 | Score_ZeroCharacter+8 | Score_ZeroCharacter+9 |

    Given I start writing memory at $500
    Given I write the following hex bytes
      | 05 04  06 04 03 01 |

    When I set register a to lo($500)
    When I set register x to hi($500)
    When I execute the procedure at ScoreAdd for no more than 103 instructions

    Then I hex dump memory between $400 and $407
    Then I expect to see $3ff equal 0
    Then I expect to see $400 equal Score_ZeroCharacter+3
    Then I expect to see $401 equal Score_ZeroCharacter+4
    Then I expect to see $402 equal Score_ZeroCharacter+7
    Then I expect to see $403 equal Score_ZeroCharacter+0
    Then I expect to see $404 equal Score_ZeroCharacter+2
    Then I expect to see $405 equal Score_ZeroCharacter+4
    Then I expect to see $406 equal Score_ZeroCharacter+9
    Then I expect to see $407 equal 0

  Scenario: Simple Score add test with ripple

    Given I start writing memory at $400
    Given I write the following bytes
      | Score_ZeroCharacter+9 | Score_ZeroCharacter+9 | Score_ZeroCharacter+9 | Score_ZeroCharacter+9 |

    Given I start writing memory at $500
    Given I write the following hex bytes
      | 03 04  01 02 03 04 |

    When I set register a to lo($500)
    When I set register x to hi($500)
    When I execute the procedure at ScoreAdd for no more than 103 instructions

    Then I hex dump memory between $400 and $404
    Then I expect to see $3ff equal 0
    Then I expect to see $400 equal Score_ZeroCharacter+4
    Then I expect to see $401 equal Score_ZeroCharacter+3
    Then I expect to see $402 equal Score_ZeroCharacter+2
    Then I expect to see $403 equal Score_ZeroCharacter+0
    Then I expect to see $404 equal 0
