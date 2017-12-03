@c64
Feature: Test the Rand code

  This assembles the Rand.a code and checks the expected results after executing it

  Scenario: Simple Rand test without seed
    Given I have a simple overclocked 6502 system
    And I run the command line: ..\acme.exe -o test.prg --labeldump test.lbl -f cbm --setpc $400 ..\BerzerkRedux\Rand.a
    And I load prg "test.prg"
    And I load labels "test.lbl"

    When I execute the procedure at Rand for no more than 20 instructions
    Then I expect register a equal $ea
    When I execute the procedure at Rand for no more than 20 instructions
    Then I expect register a equal $47
    When I execute the procedure at Rand for no more than 20 instructions
    Then I expect register a equal $a4
    When I execute the procedure at Rand for no more than 20 instructions
    Then I expect register a equal $db


  Scenario: Simple Rand test with seed
    Given I have a simple overclocked 6502 system
    And I run the command line: ..\acme.exe -o test.prg --labeldump test.lbl -f cbm --setpc $400 ..\BerzerkRedux\Rand.a
    And I load prg "test.prg"
    And I load labels "test.lbl"

    When I set register a to 1
    When I set register y to 2
    When I execute the procedure at Seed for no more than 25 instructions

    When I execute the procedure at Rand for no more than 20 instructions
    Then I expect register a equal $61
    When I execute the procedure at Rand for no more than 20 instructions
    Then I expect register a equal $cf
    When I execute the procedure at Rand for no more than 20 instructions
    Then I expect register a equal $61
    When I execute the procedure at Rand for no more than 20 instructions
    Then I expect register a equal $36

    When I set register a to 1
    When I set register y to 2
    When I execute the procedure at Seed for no more than 25 instructions

    When I execute the procedure at Rand for no more than 20 instructions
    Then I expect register a equal $61
    When I execute the procedure at Rand for no more than 20 instructions
    Then I expect register a equal $cf
    When I execute the procedure at Rand for no more than 20 instructions
    Then I expect register a equal $61
    When I execute the procedure at Rand for no more than 20 instructions
    Then I expect register a equal $36
