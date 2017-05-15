@c64 @java
Feature: Heap memory checking

  Describes behaviour for a dynamic memory system.
  Each block is a byte length and a flag to say if it is used or not.
  Blocks cannot be larger than 254 bytes plus 2 bytes of header

  Scenario: Simple heap no allocated blocks
    Given I have 512 bytes of heap memory
    Then there is a free memory block at 2 of 254 bytes
    Then there is a free memory block at 258 of 254 bytes

  Scenario: Simple alocation 2
    Given I have 512 bytes of heap memory
    When I allocate 12 bytes
    Then I get a block at 2
    Then there is a used memory block at 2 of 12 bytes
    Then there is a free memory block at 16 of 240 bytes
    Then there is a free memory block at 258 of 254 bytes

  Scenario: Simple free 2
    Given I have 512 bytes of heap memory
    When I allocate 12 bytes
    When I free the block at 2
    Then there is a free memory block at 2 of 12 bytes
    Then there is a free memory block at 16 of 240 bytes
    Then there is a free memory block at 258 of 254 bytes

    When I run a block sweep
    Then there is a free memory block at 2 of 254 bytes
    Then there is a free memory block at 258 of 254 bytes

  Scenario: Simple alocation 3
    Given I have 512 bytes of heap memory
    When I allocate 12 bytes
    When I allocate 20 bytes
    Then I get a block at 16
    Then there is a used memory block at 2 of 12 bytes
    Then there is a used memory block at 16 of 20 bytes
    Then there is a free memory block at 38 of 218 bytes
    Then there is a free memory block at 258 of 254 bytes

  Scenario: Simple free 3
    Given I have 512 bytes of heap memory
    When I allocate 12 bytes
    When I allocate 20 bytes
    When I free the block at 2
    Then there is a free memory block at 2 of 12 bytes
    Then there is a used memory block at 16 of 20 bytes
    Then there is a free memory block at 38 of 218 bytes
    Then there is a free memory block at 258 of 254 bytes

    When I free the block at 16
    Then there is a free memory block at 2 of 12 bytes
    Then there is a free memory block at 16 of 20 bytes
    Then there is a free memory block at 38 of 218 bytes
    Then there is a free memory block at 258 of 254 bytes

    # First iteration only merges the first two, then moves to the next free block, leaving two free that can be merged next time
    When I run a block sweep
    Then there is a free memory block at 2 of 34 bytes
    Then there is a free memory block at 38 of 218 bytes
    Then there is a free memory block at 258 of 254 bytes

    # Second iteration merges all the free blocks it can
    When I run a block sweep
    Then there is a free memory block at 2 of 254 bytes
    Then there is a free memory block at 258 of 254 bytes
