@c64
Feature: Test the index list code

  This assembles the IndexList.a code and checks the expected results after executing it

  Background:
    Given I have a simple overclocked 6502 system
    Given I fill memory with 0

    And I run the command line: ..\acme.exe -o test.prg --labeldump test.lbl -f cbm --setpc $1000 asm\IndexListTest.a
    And I load prg "test.prg"
    And I load labels "test.lbl"


  Scenario: Simple index list test

    When I execute the procedure at IndexListTest_Init for no more than 70 instructions

    Then I expect to see List1Head equal 0
    Then I expect to see List1Next+0 equal 1
    Then I expect to see List1Next+1 equal 2
    Then I expect to see List1Next+2 equal 3
    Then I expect to see List1Next+3 equal 4
    Then I expect to see List1Next+4 equal 5
    Then I expect to see List1Next+5 equal 6
    Then I expect to see List1Next+6 equal 255
