@unittest
Feature: Testing the AnimationEngine


  Scenario: AnimationEngine allocation
    Given I have a simple overclocked 6502 system
    * That does fail on BRK
    When I enable unitialised memory read protection with immediate fail

    And I load prg "bin/main.prg"
    And I load labels "tmp/main.map"

    And I enable trace with indent
    When I execute the procedure at AnimationEngine_Init for no more than 1000 instructions
    
    Then I expect to see AnimationEngine_frameHi+kAnimationEngine_maxSlots-1 equal $00
    And I expect to see AnimationEngine_thisType+kAnimationEngine_maxSlots-1 equal $ff


    When I set register Y to kAnimationType_playerRunLeft
    When I execute the procedure at AnimationEngine_Allocate for no more than 1000 instructions
    Then I expect register ST exclude stC
    And I expect register X equal kAnimationEngine_maxSlots-1

    And I expect to see AnimationEngine_thisType+kAnimationEngine_maxSlots-1 equal kAnimationType_playerRunLeft

    And I expect to see AnimationEngine_frameHi+kAnimationEngine_maxSlots-1 equal hi(EmitSpriteFrame0_0)
    And I expect to see AnimationEngine_frameLo+kAnimationEngine_maxSlots-1 equal lo(EmitSpriteFrame0_0)

    And I expect to see AnimationEngine_framesPositionHi+kAnimationEngine_maxSlots-1 equal hi(AnimationFrames_playerRunLeft+4)
    And I expect to see AnimationEngine_framesPositionLo+kAnimationEngine_maxSlots-1 equal lo(AnimationFrames_playerRunLeft+4)

    And I expect to see AnimationEngine_flips+kAnimationEngine_maxSlots-1 equal $00



  Scenario: AnimationEngine update
    When I execute the procedure at AnimationEngine_Update for no more than 1000 instructions

    And I expect to see AnimationEngine_thisType+kAnimationEngine_maxSlots-1 equal kAnimationType_playerRunLeft

    And I expect to see AnimationEngine_frameHi+kAnimationEngine_maxSlots-1 equal hi(EmitSpriteFrame0_0)
    And I expect to see AnimationEngine_frameLo+kAnimationEngine_maxSlots-1 equal lo(EmitSpriteFrame0_0)

    And I expect to see AnimationEngine_framesPositionHi+kAnimationEngine_maxSlots-1 equal hi(AnimationFrames_playerRunLeft+8)
    And I expect to see AnimationEngine_framesPositionLo+kAnimationEngine_maxSlots-1 equal lo(AnimationFrames_playerRunLeft+8)


  Scenario: AnimationEngine mutate without change
	And I set register X to kAnimationEngine_maxSlots-1
    When I set register Y to kAnimationType_playerRunLeft
    When I execute the procedure at AnimationEngine_MutateToTypeIfDifferent for no more than 4 instructions

    And I expect to see AnimationEngine_thisType+kAnimationEngine_maxSlots-1 equal kAnimationType_playerRunLeft

    And I expect to see AnimationEngine_frameHi+kAnimationEngine_maxSlots-1 equal hi(EmitSpriteFrame0_0)
    And I expect to see AnimationEngine_frameLo+kAnimationEngine_maxSlots-1 equal lo(EmitSpriteFrame0_0)

    And I expect to see AnimationEngine_framesPositionHi+kAnimationEngine_maxSlots-1 equal hi(AnimationFrames_playerRunLeft+8)
    And I expect to see AnimationEngine_framesPositionLo+kAnimationEngine_maxSlots-1 equal lo(AnimationFrames_playerRunLeft+8)


  Scenario: AnimationEngine mutate with change
	And I set register X to kAnimationEngine_maxSlots-1
    When I set register Y to kAnimationType_playerRunUp
    When I execute the procedure at AnimationEngine_MutateToTypeIfDifferent for no more than 4 instructions

    And I expect to see AnimationEngine_thisType+kAnimationEngine_maxSlots-1 equal kAnimationType_playerRunUp

    And I expect to see AnimationEngine_frameHi+kAnimationEngine_maxSlots-1 equal hi(EmitSpriteFrame0_16)
    And I expect to see AnimationEngine_frameLo+kAnimationEngine_maxSlots-1 equal lo(EmitSpriteFrame0_16)

    And I expect to see AnimationEngine_framesPositionHi+kAnimationEngine_maxSlots-1 equal hi(AnimationFrames_playerRunUp+4)
    And I expect to see AnimationEngine_framesPositionLo+kAnimationEngine_maxSlots-1 equal lo(AnimationFrames_playerRunUp+4)
