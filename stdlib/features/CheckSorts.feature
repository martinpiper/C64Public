Feature: Test sorting library

  This executes unit tests for the sorting library

  Scenario Outline: Sort check

    Given I have a simple overclocked 6502 system
    And I enable trace with indent
    And I run the command line: ..\acme.exe --lib ..\ --lib ..\..\ -o target\CheckSort.prg --labeldump target\CheckSort.lbl -f cbm features\CheckSort<type>.a


    And I load prg "target\CheckSort.prg"
    And I load labels "target\CheckSort.lbl"
    
    When I write memory at SortValueTable+0 with $00
    When I write memory at SortValueTable+1 with $01
    When I write memory at SortValueTable+2 with $02
    When I write memory at SortValueTable+3 with $03
    When I write memory at SortValueTable+4 with $04
    When I write memory at SortValueTable+5 with $05
    # Note these last two values are the same, the sort routine should cope with this and still work
    When I write memory at SortValueTable+6 with $06
    When I write memory at SortValueTable+7 with $06
    
    Given I reset the cycle count
    When I execute the procedure at SortInitGT for no more than 10000 instructions


    When I hex dump memory between SortIndexTable and SortIndexTable+8
    Then property "test.BDD6502.lastHexDump" must contain string ignoring whitespace "1000: 00 01 02 03 04 05 06 07"

    Then I expect the cycle count to be no more than <cyclesInitGT> cycles


    Given I reset the cycle count
    When I execute the procedure at SortSortGT for no more than 10000 instructions

    When I hex dump memory between SortIndexTable and SortIndexTable+8
    Then property "test.BDD6502.lastHexDump" must contain string ignoring whitespace "1000: 00 01 02 03 04 05 06 07"

    Then I expect the cycle count to be no more than <cycles1> cycles

    # Second iteration gives stable results for index and cycle count
    Given I reset the cycle count
    When I execute the procedure at SortSortGT for no more than 10000 instructions

    When I hex dump memory between SortIndexTable and SortIndexTable+8
    Then property "test.BDD6502.lastHexDump" must contain string ignoring whitespace "1000: 00 01 02 03 04 05 06 07"

    Then I expect the cycle count to be no more than <cycles1> cycles



    # Reverse order sort
    Given I reset the cycle count
    When I execute the procedure at SortInitLT for no more than 10000 instructions


    When I hex dump memory between SortIndexTable and SortIndexTable+8
    Then property "test.BDD6502.lastHexDump" must contain string ignoring whitespace "1000: 00 01 02 03 04 05 06 07"

    Then I expect the cycle count to be no more than <cyclesInitLT> cycles

    Given I reset the cycle count
    When I execute the procedure at SortSortLT for no more than 10000 instructions

    When I hex dump memory between SortIndexTable and SortIndexTable+8
    Then property "test.BDD6502.lastHexDump" must contain string ignoring whitespace "1000: 06 07 05 04 03 02 01 00"

    Then I expect the cycle count to be no more than <cycles2> cycles



    # Second iteration reuses the index table so it's quicker
    Given I reset the cycle count
    When I execute the procedure at SortSortLT for no more than 10000 instructions

    When I hex dump memory between SortIndexTable and SortIndexTable+8
    Then property "test.BDD6502.lastHexDump" must contain string ignoring whitespace "1000: 06 07 05 04 03 02 01 00"

    Then I expect the cycle count to be no more than <cycles3> cycles

  Examples:
    | type    | cyclesInitGT  | cyclesInitLT  | cycles1 | cycles2 | cycles3 |
    | Bubble  | 98            | 98            | 198     | 2227    | 186     |
    | Quick   | 382           | 382           | 199     | 1227    | 201     |



