@c64 @java
Feature: Testing the FIFO with Integer objects


  Scenario: FIFO test 1
    Given I have a new FIFO with maximum 16 elements
    Then I expect to see the FIFO is empty
    Then I expect to see the FIFO size = 0

    When I push the value 7 to the FIFO
    Then I expect to see the FIFO size = 1
    And I expect to see the FIFO is not empty

    When I pop the value it equals 7
    Then I expect to see the FIFO size = 0
    And I expect to see the FIFO is empty


  Scenario: FIFO test 2
    Given I have a new FIFO with maximum 16 elements
    Then I expect to see the FIFO is empty
    Then I expect to see the FIFO size = 0

    When I push the value 7 to the FIFO
    Then I expect to see the FIFO size = 1
    And I expect to see the FIFO is not empty

    When I push the value 13 to the FIFO
    Then I expect to see the FIFO size = 2
    And I expect to see the FIFO is not empty

    When I push the value 27 to the FIFO
    Then I expect to see the FIFO size = 3
    And I expect to see the FIFO is not empty

    When I pop the value it equals 7
    Then I expect to see the FIFO size = 2
    And I expect to see the FIFO is not empty

    When I pop the value it equals 13
    Then I expect to see the FIFO size = 1
    And I expect to see the FIFO is not empty

    When I pop the value it equals 27
    Then I expect to see the FIFO size = 0
    And I expect to see the FIFO is empty


  Scenario: FIFO test 3
    Given I have a new FIFO with maximum 16 elements
    Then I expect to see the FIFO is empty
    Then I expect to see the FIFO size = 0

    When I push the value 8 to the FIFO
    Then I expect to see the FIFO size = 1
    And I expect to see the FIFO is not empty

    When I push the value 14 to the FIFO
    Then I expect to see the FIFO size = 2
    And I expect to see the FIFO is not empty

    When I push the value 28 to the FIFO
    Then I expect to see the FIFO size = 3
    And I expect to see the FIFO is not empty

    When I pop the value it equals 8
    Then I expect to see the FIFO size = 2
    And I expect to see the FIFO is not empty

    When I push the value 11 to the FIFO
    Then I expect to see the FIFO size = 3
    And I expect to see the FIFO is not empty

    When I pop the value it equals 14
    Then I expect to see the FIFO size = 2
    And I expect to see the FIFO is not empty

    When I pop the value it equals 28
    Then I expect to see the FIFO size = 1
    And I expect to see the FIFO is not empty

    When I pop the value it equals 11
    Then I expect to see the FIFO size = 0
    And I expect to see the FIFO is empty
