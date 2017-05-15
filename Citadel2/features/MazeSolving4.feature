@c64 @java
@MazeSolving
Feature: Checking blocked routes generate expected results
	
  Scenario: A blocked route should generate the correct responses
    Given a maze:
    """
    ########
    # #@  X#
    # #  ###
    # #### #
    #      #
    ########
    """
    And a starting point @ and a target point X
    When the unit plots a route
    Then the cheapest route is RRR
    And the route cost is 3
    And the iterations are 3
    And the frame count is 2
    And the max depth is 3

    Given I set a new starting point 1 , 1
    Given I set a new target point 6 , 1
    When the unit plots a route
    Then there is no route
    And the route cost is -1
    And the iterations are 10
    And the frame count is 10
    And the max depth is 1
