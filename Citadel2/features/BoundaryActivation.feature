@c64 @java
@ignore
Feature: Checking boundary activation for the enemy database code

  Most of these tests setup the enemy DB positions and then move the screen position to check onscreen activation.
  The enemy onscreen position when the screen scrolls is also checked.
  Lastly, deactivation behaviour of enemies that are onscreen back into the enemy DB is checked.

  Scenario:

    Given I setup the enemy DB with the following positions:
    | type  | x     | y     |
    | 12    | 0     | 0     |
    | 31    | 128   | 128   |

    When I run the enemy DB activation check

    Then I expect to see the following onscreen animations
    | DB index  | x                                 | y                               |
    | 0         | 0 + VIC2SpriteXBorderLeft38 -7    | 0 + VIC2SpriteYBorderTop24 -7   |
    | 1         | 128 + VIC2SpriteXBorderLeft38 - 7 | 128 + VIC2SpriteYBorderTop24 -7 |
