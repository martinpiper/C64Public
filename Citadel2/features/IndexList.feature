@c64
Feature: Test the index list code

  This assembles the IndexList.a code and checks the expected results after executing it

  Background:
    Given I have a simple overclocked 6502 system
    Given I fill memory with 0

    And I run the command line: ..\acme.exe -o test.prg --labeldump test.lbl -f cbm --setpc $1000 asm\IndexListTest.a
    And I load prg "test.prg"
    And I load labels "test.lbl"


  Scenario: Simple index list init and iterate test

    When I execute the procedure at IndexListTest_Init for no more than 70 instructions

    Then I expect to see List1Head equal 0
    Then I expect to see List1Next+0 equal 1
    Then I expect to see List1Next+1 equal 2
    Then I expect to see List1Next+2 equal 3
    Then I expect to see List1Next+3 equal 4
    Then I expect to see List1Next+4 equal 5
    Then I expect to see List1Next+5 equal 6
    Then I expect to see List1Next+6 equal 255

    When I execute the procedure at List1_Start for no more than 10 instructions
    Then I expect register x equal 0
    And I expect register ST exclude stN


    When I execute the procedure at List1_Get for no more than 2 instructions
    Then I expect register a equal 10
    Then I expect register x equal 0

    When I execute the procedure at List1_Next for no more than 10 instructions
    Then I expect register x equal 1
    And I expect register ST exclude stN


    When I execute the procedure at List1_Get for no more than 2 instructions
    Then I expect register a equal 11
    Then I expect register x equal 1

    When I execute the procedure at List1_Next for no more than 10 instructions
    Then I expect register x equal 2
    And I expect register ST exclude stN


    When I execute the procedure at List1_Get for no more than 2 instructions
    Then I expect register a equal 12
    Then I expect register x equal 2

    When I execute the procedure at List1_Next for no more than 10 instructions
    Then I expect register x equal 3
    And I expect register ST exclude stN


    When I execute the procedure at List1_Get for no more than 2 instructions
    Then I expect register a equal 13
    Then I expect register x equal 3

    When I execute the procedure at List1_Next for no more than 10 instructions
    Then I expect register x equal 4
    And I expect register ST exclude stN


    When I execute the procedure at List1_Get for no more than 2 instructions
    Then I expect register a equal 14
    Then I expect register x equal 4

    When I execute the procedure at List1_Next for no more than 10 instructions
    Then I expect register x equal 5
    And I expect register ST exclude stN


    When I execute the procedure at List1_Get for no more than 2 instructions
    Then I expect register a equal 15
    Then I expect register x equal 5

    When I execute the procedure at List1_Next for no more than 10 instructions
    Then I expect register x equal 6
    And I expect register ST exclude stN


    When I execute the procedure at List1_Get for no more than 2 instructions
    Then I expect register a equal 16
    Then I expect register x equal 6

    When I execute the procedure at List1_Next for no more than 10 instructions
    Then I expect register x equal 255
    And I expect register ST contain stN

    Then I expect to see List1Head equal 0
    Then I expect to see List1Next+0 equal 1
    Then I expect to see List1Next+1 equal 2
    Then I expect to see List1Next+2 equal 3
    Then I expect to see List1Next+3 equal 4
    Then I expect to see List1Next+4 equal 5
    Then I expect to see List1Next+5 equal 6
    Then I expect to see List1Next+6 equal 255


  Scenario: Simple index list pop from head test

    When I execute the procedure at IndexListTest_Init for no more than 70 instructions

    When I execute the procedure at List1_Start for no more than 10 instructions
    Then I expect register x equal 0
    And I expect register ST exclude stN
    Then I expect to see List1HeadIterator+kIndexList_Iterator_current equal 0
    Then I expect to see List1HeadIterator+kIndexList_Iterator_previous equal 255

    When I execute the procedure at List1_Get for no more than 2 instructions
    Then I expect register a equal 10
    Then I expect register x equal 0

    When I execute the procedure at List1_Pop for no more than 10 instructions
    Then I expect register x equal 1
    And I expect register ST exclude stN

    Then I expect to see List1Head equal 1
    Then I expect to see List1Next+1 equal 2



  Scenario: Simple index list pop from middle test

    When I execute the procedure at IndexListTest_Init for no more than 70 instructions

    When I execute the procedure at List1_Start for no more than 10 instructions
    Then I expect register x equal 0
    And I expect register ST exclude stN

    When I execute the procedure at List1_Next for no more than 10 instructions
    Then I expect register x equal 1
    And I expect register ST exclude stN

    When I execute the procedure at List1_Get for no more than 2 instructions
    Then I expect register a equal 11
    Then I expect register x equal 1

    Then I expect to see List1HeadIterator+kIndexList_Iterator_current equal 1
    Then I expect to see List1HeadIterator+kIndexList_Iterator_previous equal 0


    When I execute the procedure at List1_Pop for no more than 10 instructions
    Then I expect register x equal 2
    And I expect register ST exclude stN

    Then I expect to see List1Head equal 0
    Then I expect to see List1Next+0 equal 2
