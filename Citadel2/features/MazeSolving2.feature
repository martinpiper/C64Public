@c64 @java
@MazeSolving
Feature: Route avoidance in mazes
	
  Describing behaviour for the route avoidance algorithms used in the game.

  Each empty tile has a cost of 1 to travel or wait on it.

  Cost in tiles could be due to the following reasons
  - Enemy units have been attacked in that area so the AI is "afraid" to travel that way.
  - The floor is harder to travel over, slowing down the unit or damaging the unit.
  - Other units may have plans to route over that tile at some point in the future.

  Scenario: Route avoidance due to tiles being harder to move over 1
    Given a maze where numbers signify increasing cost to travel that tile:
    """
    ########
    #@2222X#
    # #  # #
    # #### #
    #      #
    ########
    """
    And a starting point @ and a target point X
    When the unit plots a route
    Then the cheapest route is RRRRR
    And the route cost is 9
    And the iterations are 17
    And the frame count is 8
    And the max depth is 3


  Scenario: Route avoidance due to tiles being harder to move over 2
    Given a maze where numbers signify increasing cost to travel that tile:
    """
    ########
    #@3333X#
    # #  # #
    # #### #
    #      #
    ########
    """
    And a starting point @ and a target point X
    When the unit plots a route
    Then the cheapest route is DDDRRRRRUUU
    And the route cost is 11
    And the iterations are 22
    And the frame count is 10
    And the max depth is 3


  Scenario: Route avoidance due to tiles being harder to move over 3
    Given a maze where numbers signify increasing cost to travel that tile:
    """
    ########
    #@4444X#
    # #  # #
    # #### #
    #      #
    ########
    """
    And a starting point @ and a target point X
    When the unit plots a route
    Then the cheapest route is DDDRRRRRUUU
    And the route cost is 11
    And the iterations are 22
    And the frame count is 10
    And the max depth is 3


  Scenario: Route avoidance due to planned routes
    Given a maze where numbers signify a block at that time index:
    """
    ########
    #@0123X#
    # #  # #
    # #### #
    #      #
    ########
    """
    And a starting point @ and a target point X
    When the unit plots a route
    Then the cheapest route is RRRRR
    And the route cost is 6
    And the iterations are 10
    And the frame count is 5
    And the max depth is 4
    And the frames seen are:
    """
    S
    ########
    #@0123X#
    # #  # #
    # #### #
    #      #
    ########

    R
    ########
    # @012X#
    # #  # #
    # #### #
    #      #
    ########

    R
    ########
    #  @01X#
    # #  # #
    # #### #
    #      #
    ########

    R
    ########
    #   @0X#
    # #  # #
    # #### #
    #      #
    ########

    R
    ########
    #    @X#
    # #  # #
    # #### #
    #      #
    ########

    R
    ########
    #     @#
    # #  # #
    # #### #
    #      #
    ########
    """
