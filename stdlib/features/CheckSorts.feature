Feature: Test sorting library

  This executes unit tests for the sorting library

  Scenario Outline: Sort check
    Given build sort test "<type>"
    And initialise sort values

    When initialising the greater than sort test
    Then validate the expected ascending sort index table
    Then validate the expected ascending sort values table
    Then I expect the cycle count to be no more than <cyclesInitGT> cycles

    When executing the greater than sort test
    Then validate the expected ascending sort index table
    Then validate the expected ascending sort values table
    Then I expect the cycle count to be no more than <cycles1> cycles

    # Second iteration gives stable results for index and cycle count
    When executing the greater than sort test
    Then validate the expected ascending sort index table
    Then validate the expected ascending sort values table
    Then I expect the cycle count to be no more than <cycles1> cycles


    # Reverse order sort
    When initialising the less than sort test
    Then validate the expected ascending sort index table
    Then validate the expected ascending sort values table
    Then I expect the cycle count to be no more than <cyclesInitLT> cycles

    When executing the less than sort test
    Then validate the expected descending stable sort index table
    Then validate the expected ascending sort values table
    Then I expect the cycle count to be no more than <cycles2> cycles


    # Second iteration reuses the index table so it's quicker
    When executing the less than sort test
    Then validate the expected descending stable sort index table
    Then validate the expected ascending sort values table
    Then I expect the cycle count to be no more than <cycles3> cycles

  Examples:
    | type    | cyclesInitGT  | cyclesInitLT  | cycles1 | cycles2 | cycles3 |
    | Bubble  | 98            | 98            | 198     | 2227    | 186     |
    | Quick   | 382           | 382           | 199     | 1227    | 201     |



