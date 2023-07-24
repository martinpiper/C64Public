@ignore @c64
Feature: Animation tests

  Execute this with: java -jar ..\..\BDD6502\target\BDD6502-1.0.3-SNAPSHOT-jar-with-dependencies.jar
  Or with CPU trace: java -Dbdd6502.trace=true -jar ..\..\BDD6502\target\BDD6502-1.0.3-SNAPSHOT-jar-with-dependencies.jar

  Performs animation routine tests using known data.
  Creates animations at known positions with screen characters at known points for collision tests.
  Moves animations at known velocities and in combination with screen pixel scroll values.
  Tests for animations hitting the screen collision points and animations being deleted.

  MPi: TODO: Implement tests for frames


  Background:
    Given I have a simple overclocked 6502 system
    And I run the command line: ..\acme.exe --lib ../Scroller/ --labeldump test.lbl asm\AnimationsStub.a
    And I load prg "AnimationsStub.prg"
    And I load labels "test.lbl"
    
#    Given enable remote debugging
#    And wait for debugger command

    # Add some characters to collide with
    # Top
    Given I write memory at SCREENRAM + (6*40) + 9 with $ff
    Given I write memory at SCREENRAM + (6*40) + 10 with $ff

    # Bottom
    Given I write memory at SCREENRAM + (9*40) + 9 with $ff
    Given I write memory at SCREENRAM + (9*40) + 10 with $ff

    # Left
    Given I write memory at SCREENRAM + (6*40) + 8 with $ff
    Given I write memory at SCREENRAM + (7*40) + 8 with $ff
    Given I write memory at SCREENRAM + (8*40) + 8 with $ff

    When I reset the cycle count

#	When I execute the procedure at ScrollerDemoInitOneTimeData for no more than 50000 instructions
#	When I execute the procedure at InitialiseGameWithoutWait for no more than 20000 instructions
#	When I execute the procedure at AnimationInit2 for no more than 300 instructions
    When I execute the procedure at AnimationClear for no more than 50 instructions
    Then I expect the cycle count to be no more than 80 cycles

    Then I expect to see AnimationActiveSlot equal 0
    Then I expect to see AnimationActiveSlot+1 equal 0
    Then I expect to see AnimationActiveSlot+2 equal 0
    Then I expect to see AnimationActiveSlot+3 equal 0
    Then I expect to see AnimationYPos equal $ff
    Then I expect to see AnimationYPos+1 equal $ff
    Then I expect to see AnimationYPos+2 equal $ff
    Then I expect to see AnimationYPos+3 equal $ff
    Then I expect to see AnimationFlags equal 0
    Then I expect to see AnimationFlags+1 equal 0
    Then I expect to see AnimationFlags+2 equal 0
    Then I expect to see AnimationFlags+3 equal 0

    When I execute the procedure at AnimationFindFreeSlot for no more than 10 instructions
    Then I expect register X equal 0
    Then I expect register ST exclude stC
    Then I expect to see AnimationFlags equal 0

    # First slot
    When I execute the procedure at AnimationFindFreeSlot for no more than 10 instructions
    Then I expect register X equal 0
    Then I expect register ST exclude stC
    Then I expect to see AnimationFlags equal 0

    When I set register Y to 0
    When I execute the procedure at AnimationInitWithType for no more than 35 instructions
    Then I expect to see AnimationFlags contain AnimationFlags_Cycle

    # Setup some dummy positions for the allocations
    Given I write memory at AnimationYPos with $60
    Given I write memory at AnimationXPosLo with $60
    Given I write memory at AnimationXPosHi with 0


    # Next slot
    When I execute the procedure at AnimationFindFreeSlot for no more than 20 instructions
    Then I expect register X equal 1
    Then I expect register ST exclude stC
    Then I expect to see AnimationFlags contain AnimationFlags_Cycle
    Then I expect to see AnimationFlags+1 equal 0

    When I set register Y to 1
    When I execute the procedure at AnimationInitWithType for no more than 35 instructions
    Then I expect to see AnimationFlags+1 contain AnimationFlags_Cycle

    Given I write memory at AnimationYPos + 1 with $60
    Given I write memory at AnimationXPosLo + 1 with $60
    Given I write memory at AnimationXPosHi + 1 with 0


    # Next slot
    When I execute the procedure at AnimationFindFreeSlot for no more than 30 instructions
    Then I expect register X equal 2
    Then I expect register ST exclude stC
    Then I expect to see AnimationFlags+2 equal 0

    When I set register Y to 0
    When I execute the procedure at AnimationInitWithType for no more than 35 instructions
    Then I expect to see AnimationFlags+2 contain AnimationFlags_Cycle

    Given I write memory at AnimationYPos + 2 with $60
    Given I write memory at AnimationXPosLo + 2 with $60
    Given I write memory at AnimationXPosHi + 2 with 0


    # Next slot
    When I execute the procedure at AnimationFindFreeSlot for no more than 40 instructions
    Then I expect register X equal 3
    Then I expect register ST exclude stC
    Then I expect to see AnimationFlags+3 equal 0

    When I set register Y to 1
    When I execute the procedure at AnimationInitWithType for no more than 35 instructions
    Then I expect to see AnimationFlags+3 contain AnimationFlags_Cycle

    Given I write memory at AnimationYPos + 3 with $60
    Given I write memory at AnimationXPosLo + 3 with $60
    Given I write memory at AnimationXPosHi + 3 with 0

    # Test everything uses the expected flag
    Then I expect to see AnimationFlags contain AnimationFlags_CharCollide
    Then I expect to see AnimationFlags+1 contain AnimationFlags_CharCollide
    Then I expect to see AnimationFlags+2 contain AnimationFlags_CharCollide
    Then I expect to see AnimationFlags+3 contain AnimationFlags_CharCollide


    # Next slot, should fail since there are no more slots
    When I execute the procedure at AnimationFindFreeSlot for no more than 40 instructions
    Then I expect register ST contain stC

    # One frame
    # 275
    When I execute the procedure at UpdateSprites for no more than 330 instructions

    Then I expect to see AnimationXPosLo+0 equal $60
    Then I expect to see AnimationXPosLo+1 equal $60
    Then I expect to see AnimationXPosLo+2 equal $60
    Then I expect to see AnimationXPosLo+3 equal $60
    Then I expect to see AnimationYPos+0 equal $60
    Then I expect to see AnimationYPos+1 equal $60
    Then I expect to see AnimationYPos+2 equal $60
    Then I expect to see AnimationYPos+3 equal $60

    Given I write memory at AnimationSimpleMoveX+0 with -1
    Given I write memory at AnimationSimpleMoveX+1 with 1
    Given I write memory at AnimationSimpleMoveX+2 with 0
    Given I write memory at AnimationSimpleMoveX+3 with 0

    Given I write memory at AnimationSimpleMoveY+0 with 0
    Given I write memory at AnimationSimpleMoveY+1 with 0
    Given I write memory at AnimationSimpleMoveY+2 with -1
    Given I write memory at AnimationSimpleMoveY+3 with 1

    # Should still fail
    When I execute the procedure at AnimationFindFreeSlot for no more than 40 instructions
    Then I expect register ST contain stC


#  @ignore
  Scenario: Enemy animation movement test
    # Make the screen scroll to check the extra pathways in the code
    Given I write memory at scrollerRealMovementXPixels with -1
    Given I write memory at scrollerRealMovementYPixels with -1

    Given I write memory at AnimationSimpleMoveX+0 with -2
    Given I write memory at AnimationSimpleMoveX+1 with 2
    Given I write memory at AnimationSimpleMoveX+2 with 0
    Given I write memory at AnimationSimpleMoveX+3 with 0

    Given I write memory at AnimationSimpleMoveY+0 with 0
    Given I write memory at AnimationSimpleMoveY+1 with 0
    Given I write memory at AnimationSimpleMoveY+2 with -2
    Given I write memory at AnimationSimpleMoveY+3 with 2

    # Check screen scrolling updates the enemies properly

    # Next frame
    # 339
    When I execute the procedure at UpdateSprites for no more than 380 instructions
    Then I expect to see AnimationXPosLo+0 equal $5d
    Then I expect to see AnimationXPosLo+1 equal $61
    Then I expect to see AnimationXPosLo+2 equal $5f
    Then I expect to see AnimationXPosLo+3 equal $5f
    Then I expect to see AnimationYPos+0 equal $5f
    Then I expect to see AnimationYPos+1 equal $5f
    Then I expect to see AnimationYPos+2 equal $5d
    Then I expect to see AnimationYPos+3 equal $61

    # Next frame
    # 339
    When I execute the procedure at UpdateSprites for no more than 380 instructions
    Then I expect to see AnimationXPosLo+0 equal $5a
    Then I expect to see AnimationXPosLo+1 equal $62
    Then I expect to see AnimationXPosLo+2 equal $5e
    Then I expect to see AnimationXPosLo+3 equal $5e
    Then I expect to see AnimationYPos+0 equal $5e
    Then I expect to see AnimationYPos+1 equal $5e
    Then I expect to see AnimationYPos+2 equal $5a
    Then I expect to see AnimationYPos+3 equal $62

    # 339
    When I execute the procedure at UpdateSprites for no more than 380 instructions
    When I hex dump memory between AnimationXPosLo and AnimationXPosLo+4
    When I hex dump memory between AnimationYPos and AnimationYPos+4
    Then I expect to see AnimationYPos+0 equal $5d
    Then I expect to see AnimationYPos+1 equal $5d
    Then I expect to see AnimationYPos+2 equal $57
    Then I expect to see AnimationYPos+3 equal $63

    # 281
    When I execute the procedure at UpdateSprites for no more than 330 instructions
    When I hex dump memory between AnimationXPosLo and AnimationXPosLo+4
    When I hex dump memory between AnimationYPos and AnimationYPos+4
    Then I expect to see AnimationYPos+0 equal $ff
    Then I expect to see AnimationYPos+1 equal $5c
    Then I expect to see AnimationYPos+2 equal $ff
    Then I expect to see AnimationYPos+3 equal $64

    # 234
    When I execute the procedure at UpdateSprites for no more than 300 instructions
    When I hex dump memory between AnimationXPosLo and AnimationXPosLo+4
    When I hex dump memory between AnimationYPos and AnimationYPos+4
    Then I expect to see AnimationYPos+0 equal $ff
    Then I expect to see AnimationYPos+1 equal $5b
    Then I expect to see AnimationYPos+2 equal $ff
    Then I expect to see AnimationYPos+3 equal $65

    # 200
    When I execute the procedure at UpdateSprites for no more than 230 instructions
    When I hex dump memory between AnimationXPosLo and AnimationXPosLo+4
    When I hex dump memory between AnimationYPos and AnimationYPos+4
    Then I expect to see AnimationYPos+0 equal $ff
    Then I expect to see AnimationYPos+1 equal $5a
    Then I expect to see AnimationYPos+2 equal $ff
    Then I expect to see AnimationYPos+3 equal $66

    # 252
    When I execute the procedure at UpdateSprites for no more than 300 instructions
    When I hex dump memory between AnimationXPosLo and AnimationXPosLo+4
    When I hex dump memory between AnimationYPos and AnimationYPos+4
    Then I expect to see AnimationYPos+0 equal $ff
    Then I expect to see AnimationYPos+1 equal $59
    Then I expect to see AnimationYPos+2 equal $ff
    Then I expect to see AnimationYPos+3 equal $67

    # 188
    When I execute the procedure at UpdateSprites for no more than 230 instructions
    When I hex dump memory between AnimationXPosLo and AnimationXPosLo+4
    When I hex dump memory between AnimationYPos and AnimationYPos+4
    Then I expect to see AnimationYPos+0 equal $ff
    Then I expect to see AnimationYPos+1 equal $ff
    Then I expect to see AnimationYPos+2 equal $ff
    Then I expect to see AnimationYPos+3 equal $68

    # 164
    When I execute the procedure at UpdateSprites for no more than 190 instructions
    When I hex dump memory between AnimationXPosLo and AnimationXPosLo+4
    When I hex dump memory between AnimationYPos and AnimationYPos+4
    Then I expect to see AnimationYPos+0 equal $ff
    Then I expect to see AnimationYPos+1 equal $ff
    Then I expect to see AnimationYPos+2 equal $ff
    Then I expect to see AnimationYPos+3 equal $69

    # 135
    When I execute the procedure at UpdateSprites for no more than 160 instructions
    When I hex dump memory between AnimationXPosLo and AnimationXPosLo+4
    When I hex dump memory between AnimationYPos and AnimationYPos+4
    Then I expect to see AnimationXPosLo+0 equal $57
    Then I expect to see AnimationXPosLo+1 equal $67
    Then I expect to see AnimationXPosLo+2 equal $5d
    Then I expect to see AnimationXPosLo+3 equal $57
    Then I expect to see AnimationYPos+0 equal $ff
    Then I expect to see AnimationYPos+1 equal $ff
    Then I expect to see AnimationYPos+2 equal $ff
    Then I expect to see AnimationYPos+3 equal $ff

    Then I expect the cycle count to be no more than 10557 cycles


#  @ignore
  Scenario: Enemy animation movement test with scroll y3
    Given I write memory at topScreenScrollY with 3
# Next frame
    When I execute the procedure at UpdateSprites for no more than 359 instructions
    Then I expect to see AnimationXPosLo+0 equal $5f
    Then I expect to see AnimationXPosLo+1 equal $61
    Then I expect to see AnimationXPosLo+2 equal $60
    Then I expect to see AnimationXPosLo+3 equal $60
    Then I expect to see AnimationYPos+0 equal $60
    Then I expect to see AnimationYPos+1 equal $60
    Then I expect to see AnimationYPos+2 equal $5f
    Then I expect to see AnimationYPos+3 equal $61


# Next frame
    When I execute the procedure at UpdateSprites for no more than 359 instructions
    Then I expect to see AnimationXPosLo+0 equal $5e
    Then I expect to see AnimationXPosLo+1 equal $62
    Then I expect to see AnimationXPosLo+2 equal $60
    Then I expect to see AnimationXPosLo+3 equal $60
    Then I expect to see AnimationYPos+0 equal $60
    Then I expect to see AnimationYPos+1 equal $60
    Then I expect to see AnimationYPos+2 equal $5e
    Then I expect to see AnimationYPos+3 equal $62


    When I execute the procedure at UpdateSprites for no more than 359 instructions
    Then I expect to see AnimationYPos+2 equal $5d
    When I execute the procedure at UpdateSprites for no more than 359 instructions
    Then I expect to see AnimationYPos+2 equal $5c

    When I execute the procedure at UpdateSprites for no more than 393 instructions
    Then I expect to see AnimationYPos+2 equal $ff
    When I hex dump memory between AnimationYPos and AnimationYPos+4

    When I execute the procedure at UpdateSprites for no more than 330 instructions
    Then I expect to see AnimationYPos+3 equal $66

    When I execute the procedure at UpdateSprites for no more than 330 instructions
    When I execute the procedure at UpdateSprites for no more than 330 instructions
    When I execute the procedure at UpdateSprites for no more than 330 instructions
    Then I expect to see AnimationXPosLo+0 equal $57
    When I execute the procedure at UpdateSprites for no more than 330 instructions
    Then I expect to see AnimationYPos+0 equal $ff
    When I execute the procedure at UpdateSprites for no more than 330 instructions
    When I execute the procedure at UpdateSprites for no more than 330 instructions
    When I execute the procedure at UpdateSprites for no more than 330 instructions
    Then I expect to see AnimationYPos+3 equal $6d
    When I execute the procedure at UpdateSprites for no more than 330 instructions
    Then I expect to see AnimationYPos+3 equal $ff

    Then I expect the cycle count to be no more than 15049 cycles

#  @ignore
  Scenario: Enemy animation movement test with scroll y5
    Given I write memory at topScreenScrollY with 5
# Next frame
    When I execute the procedure at UpdateSprites for no more than 360 instructions
    Then I expect to see AnimationXPosLo+0 equal $5f
    Then I expect to see AnimationXPosLo+1 equal $61
    Then I expect to see AnimationXPosLo+2 equal $60
    Then I expect to see AnimationXPosLo+3 equal $60
    Then I expect to see AnimationYPos+0 equal $60
    Then I expect to see AnimationYPos+1 equal $60
    Then I expect to see AnimationYPos+2 equal $5f
    Then I expect to see AnimationYPos+3 equal $61


# Next frame
    When I execute the procedure at UpdateSprites for no more than 360 instructions
    Then I expect to see AnimationXPosLo+0 equal $5e
    Then I expect to see AnimationXPosLo+1 equal $62
    Then I expect to see AnimationXPosLo+2 equal $60
    Then I expect to see AnimationXPosLo+3 equal $60
    Then I expect to see AnimationYPos+0 equal $60
    Then I expect to see AnimationYPos+1 equal $60
    Then I expect to see AnimationYPos+2 equal $5e
    Then I expect to see AnimationYPos+3 equal $62


    When I execute the procedure at UpdateSprites for no more than 355 instructions
    Then I expect to see AnimationYPos+2 equal $ff
    When I hex dump memory between AnimationYPos and AnimationYPos+4

    When I execute the procedure at UpdateSprites for no more than 300 instructions
    When I execute the procedure at UpdateSprites for no more than 330 instructions
    When I execute the procedure at UpdateSprites for no more than 300 instructions
    When I execute the procedure at UpdateSprites for no more than 330 instructions
    When I execute the procedure at UpdateSprites for no more than 300 instructions
    When I execute the procedure at UpdateSprites for no more than 330 instructions
    Then I expect to see AnimationXPosLo+0 equal $57
    When I execute the procedure at UpdateSprites for no more than 300 instructions
    Then I expect to see AnimationYPos+0 equal $ff
    When I execute the procedure at UpdateSprites for no more than 330 instructions
    When I execute the procedure at UpdateSprites for no more than 300 instructions
    When I execute the procedure at UpdateSprites for no more than 330 instructions
    When I execute the procedure at UpdateSprites for no more than 300 instructions
    When I execute the procedure at UpdateSprites for no more than 330 instructions
    Then I expect to see AnimationYPos+3 equal $6f
    When I execute the procedure at UpdateSprites for no more than 245 instructions
    Then I expect to see AnimationYPos+3 equal $ff
    When I hex dump memory between AnimationYPos and AnimationYPos+4

    Then I expect the cycle count to be no more than 16277 cycles




#  @ignore
  Scenario: Enemy animation movement test with scroll x3
    Given I write memory at topScreenScrollX with 3
# Next frame
    When I execute the procedure at UpdateSprites for no more than 359 instructions
    Then I expect to see AnimationXPosLo+0 equal $5f
    Then I expect to see AnimationXPosLo+1 equal $61
    Then I expect to see AnimationXPosLo+2 equal $60
    Then I expect to see AnimationXPosLo+3 equal $60
    Then I expect to see AnimationYPos+0 equal $60
    Then I expect to see AnimationYPos+1 equal $60
    Then I expect to see AnimationYPos+2 equal $5f
    Then I expect to see AnimationYPos+3 equal $61


# Next frame
    When I execute the procedure at UpdateSprites for no more than 359 instructions
    Then I expect to see AnimationXPosLo+0 equal $5e
    Then I expect to see AnimationXPosLo+1 equal $62
    Then I expect to see AnimationXPosLo+2 equal $60
    Then I expect to see AnimationXPosLo+3 equal $60
    Then I expect to see AnimationYPos+0 equal $60
    Then I expect to see AnimationYPos+1 equal $60
    Then I expect to see AnimationYPos+2 equal $5e
    Then I expect to see AnimationYPos+3 equal $62


    When I execute the procedure at UpdateSprites for no more than 359 instructions
    Then I expect to see AnimationYPos+2 equal $5d
    When I execute the procedure at UpdateSprites for no more than 359 instructions
    Then I expect to see AnimationYPos+2 equal $5c

    When I execute the procedure at UpdateSprites for no more than 393 instructions

    When I execute the procedure at UpdateSprites for no more than 330 instructions
    Then I expect to see AnimationYPos+3 equal $66

    When I execute the procedure at UpdateSprites for no more than 1319 instructions
    When I execute the procedure at UpdateSprites for no more than 330 instructions
    Then I expect to see AnimationXPosLo+0 equal $59
    When I execute the procedure at UpdateSprites for no more than 330 instructions
    Then I expect to see AnimationYPos+0 equal $ff
    When I hex dump memory between AnimationYPos and AnimationYPos+4
    When I execute the procedure at UpdateSprites for no more than 330 instructions
    Then I expect to see AnimationYPos+0 equal $ff
    When I execute the procedure at UpdateSprites for no more than 330 instructions
    When I execute the procedure at UpdateSprites for no more than 330 instructions
    Then I expect to see AnimationYPos+3 equal $ff
    When I execute the procedure at UpdateSprites for no more than 330 instructions

    Then I expect the cycle count to be no more than 14055 cycles

#  @ignore
  Scenario: Enemy animation movement test with scroll x5
    Given I write memory at topScreenScrollX with 5
# Next frame
    When I execute the procedure at UpdateSprites for no more than 360 instructions
    Then I expect to see AnimationXPosLo+0 equal $5f
    Then I expect to see AnimationXPosLo+1 equal $61
    Then I expect to see AnimationXPosLo+2 equal $60
    Then I expect to see AnimationXPosLo+3 equal $60
    Then I expect to see AnimationYPos+0 equal $60
    Then I expect to see AnimationYPos+1 equal $60
    Then I expect to see AnimationYPos+2 equal $5f
    Then I expect to see AnimationYPos+3 equal $61


# Next frame
    When I execute the procedure at UpdateSprites for no more than 360 instructions
    Then I expect to see AnimationXPosLo+0 equal $5e
    Then I expect to see AnimationXPosLo+1 equal $62
    Then I expect to see AnimationXPosLo+2 equal $60
    Then I expect to see AnimationXPosLo+3 equal $60
    Then I expect to see AnimationYPos+0 equal $60
    Then I expect to see AnimationYPos+1 equal $60
    Then I expect to see AnimationYPos+2 equal $5e
    Then I expect to see AnimationYPos+3 equal $62


    When I execute the procedure at UpdateSprites for no more than 365 instructions
    When I hex dump memory between AnimationYPos and AnimationYPos+4

    When I execute the procedure at UpdateSprites for no more than 380 instructions
    When I execute the procedure at UpdateSprites for no more than 471 instructions
    When I execute the procedure at UpdateSprites for no more than 380 instructions
    When I execute the procedure at UpdateSprites for no more than 330 instructions
    When I execute the procedure at UpdateSprites for no more than 300 instructions
    When I execute the procedure at UpdateSprites for no more than 330 instructions
    Then I expect to see AnimationXPosLo+0 equal $5b
    When I execute the procedure at UpdateSprites for no more than 300 instructions
    Then I expect to see AnimationYPos+0 equal $ff
    When I execute the procedure at UpdateSprites for no more than 330 instructions
    Then I expect to see AnimationYPos+2 equal $ff
    Then I expect to see AnimationYPos+3 equal $ff
    When I hex dump memory between AnimationYPos and AnimationYPos+4

    Then I expect the cycle count to be no more than 12593 cycles
