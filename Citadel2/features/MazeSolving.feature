@c64 @java
@MazeSolving
Feature: Solving simple mazes

  Describing behaviour for the maze solving algorithms used in the game.

  Each empty tile has a cost of 1 to travel or wait on it.

  Scenario: An invalid maze that tests parsing of data values
    Given a maze:
    """
	0#@X0159
	"""
    And a starting point @ and a target point X


  Scenario: An invalid maze that tests parsing of data values with blocking
    Given a maze where numbers signify a block at that time index:
    """
	#@X0159
	"""
    And a starting point @ and a target point X


  Scenario: A really simple maze
    Given a maze:
    """
    ########
	#@    X#
	########
	"""
    And a starting point @ and a target point X
    When the unit plots a route
    Then the cheapest route is RRRRR
    And the iterations are 4
    And the frame count is 4
    And the max depth is 1


  Scenario: A simple maze
    Given a maze:
    """
	########
	#@####X#
	#      #
	########
	"""
    And a starting point @ and a target point X
    When the unit plots a route
    Then the cheapest route is DRRRRRU
    And the iterations are 6
    And the frame count is 6
    And the max depth is 1


  Scenario: A simple maze 2
    Given a maze:
    """
	########
	#@####X#
	# #### #
	#      #
	########
	"""
    And a starting point @ and a target point X
    When the unit plots a route
    Then the cheapest route is DDRRRRRUU
    And the iterations are 8
    And the frame count is 8
    And the max depth is 1


  Scenario: A simple maze 3
    Given a maze:
    """
	########
	#@#  #X#
	# # ## #
	#      #
	########
	"""
    And a starting point @ and a target point X
    When the unit plots a route
    Then the cheapest route is DDRRRRRUU
    And the iterations are 11
    And the frame count is 8
    And the max depth is 2


  Scenario: A simple maze 4
    Given a maze:
    """
	########
	#@#  #X#
	# # ## #
	#   #  #
	########
	"""
    And a starting point @ and a target point X
    When the unit plots a route
    Then there is no route


  @tooLarge
  Scenario: A simple maze 5
    Given a maze:
    """
	########
	#@#  #X#
	# # ## #
	#   #  #
	########
	"""
    And a starting point @ and a target point X
    When the unit plots a route
    Then there is no route
    And the closest route is DDRRUUR
    And the iterations are 8
    And the frame count is 8
    And the max depth is 1
