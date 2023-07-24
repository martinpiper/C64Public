Feature: ActivationObject tests


  Performs ActivationObject routine tests using known data
  

  Scenario: Enemy activation allocation test
    When I enable trace with indent
    When I enable uninitialised memory read protection
    Given I have a simple overclocked 6502 system

    And I run the command line: ..\acme.exe -v4 --msvc --lib asm/ --lib ../ --lib ../Scroller/ -o test.prg --labeldump test.lbl -f cbm ActivationObjectTest.a
    And I load prg "test.prg"
    And I load labels "test.lbl"

    Then I expect to see scrollMapHX equal 2
    Then I expect to see scrollMapLX equal 0
    Then I expect to see scrollMapHY equal 2
    Then I expect to see scrollMapLY equal 0
    Then I expect to see frameScrollX equal 4
    Then I expect to see frameScrollY equal 4
    
    Then I expect to see EnemySpawnedCount equal 0

    When I execute the procedure at ActivationObjectInit for no more than 120 instructions
    Then I expect to see ActivationObjectTrigger_Left equal lo(AnimationEnemyMapPlacementsX)
    Then I expect to see ActivationObjectTrigger_Left+1 equal hi(AnimationEnemyMapPlacementsX)
    Then I expect to see ActivationObjectPosition_Left equal 43
    Then I expect to see ActivationObjectPosition_Left+1 equal 0

    When I write memory at scrollerRealMovementXPixels with -4

    When I execute the procedure at ActivationObject_UpdateScreenEdges for no more than 50 instructions
    Then I expect to see ActivationObjectPosition_Left equal 47
    Then I expect to see ActivationObjectPosition_Left+1 equal 0
    When I execute the procedure at ActivationObject_CheckScreenEdges for no more than 100 instructions
    Then I expect to see EnemySpawnedCount equal 0

    When I execute the procedure at ActivationObject_UpdateScreenEdges for no more than 50 instructions
    Then I expect to see ActivationObjectPosition_Left equal 51
    Then I expect to see ActivationObjectPosition_Left+1 equal 0
    When I execute the procedure at ActivationObject_CheckScreenEdges for no more than 100 instructions
    Then I expect to see EnemySpawnedCount equal 0

    When I execute the procedure at ActivationObject_UpdateScreenEdges for no more than 50 instructions
    Then I expect to see ActivationObjectPosition_Left equal 55
    Then I expect to see ActivationObjectPosition_Left+1 equal 0
    When I execute the procedure at ActivationObject_CheckScreenEdges for no more than 160 instructions
    # Checks the enemy AnimationInit_practice expected edge position was called
    Then I expect to see EnemySpawnedCount equal 1
