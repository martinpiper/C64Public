Feature: Animation tests

	Execute this with: java -jar ..\..\BDD6502\target\BDD6502-1.0.9-SNAPSHOT-jar-with-dependencies.jar
	Or with CPU trace: java -Dbdd6502.trace=true -jar ..\..\BDD6502\target\BDD6502-1.0.9-SNAPSHOT-jar-with-dependencies.jar

	Performs animation routine tests using known data
    Data from ANIMTSTS.P00
    Enemy at $340c
    Map pos: 00 00
    Movement address: $3fcb
    X Pos (*2): $24
    Y Pos: $60
    Animation type: 0
    Cycle animation frames $42 to $53 every frame. Movement:
    Speed 3
      p  d  r  u  l  r  d  r  l  p  l  p
     10 54 a1 58 82 11 54 91 58 a0 c2 70 ff
    
    MPi: TODO: Implement tests for the above movement
    
    
    MPi: TODO: Document the rest
     Dir hold
     pdpdrup
     f0 f0 f0 f0 40 14 f0 90 34 18 21 38 f0 e0 ff
     Cycle
     drurdlul
     10 84 81 88 81 84 82 88 82 ff
     Dir
     prprprprplrdudu
     f0 20 31 f0 40 41 60 61 b0 41 50 f2 22 f1 21 54 48 44 58 ff
     Dir hold
        4r 1p 4l 1p 4d 4u
     10 41 10 42 10 44 48 ff

Scenario: Enemy animation allocation test
	Given I have a simple overclocked 6502 system
	And I create file "test.a" with
	"""
	!sal
	!cpu 6510
	!ct pet
	!initmem %10101010

	!source "../stdlib/stdlib.a"
	!source "ScrollerVars.a"

	Multiplex_VarBase	= scrollerDemo_endVars
	Multiplex_spritepointer	= $cbf8
	Multiplex_items	= 32

	!source "../SpriteMultiplexor/SpriteMultiplexorVars.a"

	Multiplex_EnableEarlyOut = 1
	Multiplex_OverflowRasterCheck1 = 1
	Multiplex_OverflowRasterCheck2 = 1
	Multiplex_BunchingCheck = 1
	Multiplex_LogCollisions = 1
	Multiplexor_UpdateCollisionDetailsSoftEnable = 1
	Multiplex_LeanAndMean = 1

	;Scroller_IsHorizontal = 1

	Scroller_PlaySoundEffectsDuringGame = 1
	;Scroller_LinkedEnemyTest = 1
	CollisionHotSpotTweakY = 8
	CollisionHotSpotTweakX = 4
	;TryOtherPlayerSafePositionForRespawn = 1
	;Scroller_NoMusic = 1
	Scroller_FullScreen = 1
	IRQOpenBorders = 1
	ScorePlayer1XPos = 44
	ScorePlayer2XPos = 228
	Scroller_SkipEmptySpriteExplodingEnemies = 1
	ClearGameScreensChar = 0
	ScrollerTestingNoWait = 1

	!source "ScrollEntryCommon.a"
	TitleScreen_Enable_ScrollerDemoWait_Hooks !by 0

	; Test specific code goes here
	AllocateEnemyTest
	ldy #AnimationType_Enemy1
	jmp AnimationFindFreeSlot
	!macro SEUCKFileData .size, .offset {
	!set .offset = $1a + 2 - $900 + .offset
	!bin "../SEUCK/ANIMTSTS.P00",.size,.offset
	}

	!source "ScrollerData.a"
	"""
	And I run the command line: ..\acme.exe -o test.prg --labeldump test.lbl -f cbm test.a
	And I run the command line: ..\bin\LZMPi.exe -c64 test.prg testcomp.prg $400
	And I load prg "test.prg"
	And I load labels "test.lbl"
	# The first instruction at $400 is LDA #
	Then I expect to see $400 equal $a9
	When I execute the procedure at ScrollerDemoInitOneTimeData for no more than 50000 instructions
	When I execute the procedure at InitialiseGameWithoutWait for no more than 20000 instructions
	When I execute the procedure at AnimationInit for no more than 300 instructions
	Then I expect to see AnimationType equal $ff
	When I execute the procedure at AllocateEnemyTest for no more than 50 instructions
	Then I expect to see AnimationType equal AnimationType_Enemy1



Scenario: Enemy animation looping frames
	When I execute the procedure at AnimationInit for no more than 300 instructions
	Then I expect to see AnimationType equal $ff

#  Given I enable trace with indent

	When I execute the procedure at AnimationTriggerEnemies for no more than 200 instructions
	Then I expect to see AnimationType equal AnimationType_Enemy1

	# This first frame is slightly more expensive because some extra initialisation happens
	When I execute the procedure at AnimationUpdateFrameMovement for no more than 250 instructions
	Then I expect to see AnimationType equal AnimationType_Enemy1
	Then I expect to see AnimationSpriteFrame equal $42	

	# Slightly less expensive for the subsequent frames
	When I execute the procedure at AnimationUpdateFrameMovement for no more than 220 instructions
	Then I expect to see AnimationType equal AnimationType_Enemy1
	Then I expect to see AnimationSpriteFrame equal $43

	When I execute the procedure at AnimationUpdateFrameMovement for no more than 220 instructions
	Then I expect to see AnimationType equal AnimationType_Enemy1
	Then I expect to see AnimationSpriteFrame equal $44

	When I execute the procedure at AnimationUpdateFrameMovement for no more than 220 instructions
	Then I expect to see AnimationType equal AnimationType_Enemy1
	Then I expect to see AnimationSpriteFrame equal $45

	# Direction change happens here, slightly more time
	When I execute the procedure at AnimationUpdateFrameMovement for no more than 250 instructions
	Then I expect to see AnimationType equal AnimationType_Enemy1
	Then I expect to see AnimationSpriteFrame equal $46

	When I execute the procedure at AnimationUpdateFrameMovement for no more than 220 instructions
	Then I expect to see AnimationType equal AnimationType_Enemy1
	Then I expect to see AnimationSpriteFrame equal $47

	When I execute the procedure at AnimationUpdateFrameMovement for no more than 220 instructions
	Then I expect to see AnimationType equal AnimationType_Enemy1
	Then I expect to see AnimationSpriteFrame equal $48

	When I execute the procedure at AnimationUpdateFrameMovement for no more than 220 instructions
	Then I expect to see AnimationType equal AnimationType_Enemy1
	Then I expect to see AnimationSpriteFrame equal $49

	When I execute the procedure at AnimationUpdateFrameMovement for no more than 220 instructions
	Then I expect to see AnimationType equal AnimationType_Enemy1
	Then I expect to see AnimationSpriteFrame equal $4a

	When I execute the procedure at AnimationUpdateFrameMovement for no more than 220 instructions
	Then I expect to see AnimationType equal AnimationType_Enemy1
	Then I expect to see AnimationSpriteFrame equal $4b

	When I execute the procedure at AnimationUpdateFrameMovement for no more than 220 instructions
	Then I expect to see AnimationType equal AnimationType_Enemy1
	Then I expect to see AnimationSpriteFrame equal $4c

	When I execute the procedure at AnimationUpdateFrameMovement for no more than 220 instructions
	Then I expect to see AnimationType equal AnimationType_Enemy1
	Then I expect to see AnimationSpriteFrame equal $4d

	When I execute the procedure at AnimationUpdateFrameMovement for no more than 220 instructions
	Then I expect to see AnimationType equal AnimationType_Enemy1
	Then I expect to see AnimationSpriteFrame equal $4e

	When I execute the procedure at AnimationUpdateFrameMovement for no more than 220 instructions
	Then I expect to see AnimationType equal AnimationType_Enemy1
	Then I expect to see AnimationSpriteFrame equal $4f

	When I execute the procedure at AnimationUpdateFrameMovement for no more than 220 instructions
	Then I expect to see AnimationType equal AnimationType_Enemy1
	Then I expect to see AnimationSpriteFrame equal $50

	When I execute the procedure at AnimationUpdateFrameMovement for no more than 220 instructions
	Then I expect to see AnimationType equal AnimationType_Enemy1
	Then I expect to see AnimationSpriteFrame equal $51

	When I execute the procedure at AnimationUpdateFrameMovement for no more than 220 instructions
	Then I expect to see AnimationType equal AnimationType_Enemy1
	Then I expect to see AnimationSpriteFrame equal $52

	# Animation loop, slightly more time
	When I execute the procedure at AnimationUpdateFrameMovement for no more than 230 instructions
	Then I expect to see AnimationType equal AnimationType_Enemy1
	Then I expect to see AnimationSpriteFrame equal $53

	When I execute the procedure at AnimationUpdateFrameMovement for no more than 220 instructions
	Then I expect to see AnimationType equal AnimationType_Enemy1
	Then I expect to see AnimationSpriteFrame equal $42

	When I execute the procedure at AnimationUpdateFrameMovement for no more than 220 instructions
	Then I expect to see AnimationType equal AnimationType_Enemy1
	Then I expect to see AnimationSpriteFrame equal $43





Scenario: Enemy directional fire animation with speed zero bullets
	When I execute the procedure at AnimationInit for no more than 300 instructions
	Then I expect to see AnimationType equal $ff

	When I execute the procedure at LevelInit for no more than 300 instructions

	# Skip forward six levels
	And I write memory at LevelStaticCountSeconds with 0
	And I execute the procedure at LevelFrameLogic for no more than 9000 instructions
	And I write memory at LevelStaticCountSeconds with 0
	And I execute the procedure at LevelFrameLogic for no more than 9000 instructions
	And I write memory at LevelStaticCountSeconds with 0
	And I execute the procedure at LevelFrameLogic for no more than 9000 instructions
	And I write memory at LevelStaticCountSeconds with 0
	And I execute the procedure at LevelFrameLogic for no more than 9000 instructions
	And I write memory at LevelStaticCountSeconds with 0
	And I execute the procedure at LevelFrameLogic for no more than 9000 instructions
	And I write memory at LevelStaticCountSeconds with 0
	And I execute the procedure at LevelFrameLogic for no more than 9000 instructions

	
	When I execute the procedure at AnimationTriggerEnemies for no more than 200 instructions
	Then I expect to see AnimationType equal AnimationType_Enemy1 + 1
	And I expect to see AnimationDirectionIn equal 0

	And I hex dump memory between AnimationFireRateCount and AnimationFireRateCount + Multiplex_items
	Then I expect to see AnimationFireRateCount equal 50

	# This first frame is slightly more expensive because some extra initialisation happens
	When I execute the procedure at AnimationUpdateEnemyFire for no more than 150 instructions
	When I execute the procedure at AnimationUpdateFrameMovement for no more than 250 instructions
	Then I expect to see AnimationType equal 23
	And I expect to see AnimationSpriteFrame equal 70
	And I expect to see AnimationFireRateCount equal 49
	And I expect to see AnimationDirectionIn equal 0


	When I execute the procedure at AnimationUpdateEnemyFire for no more than 150 instructions
	When I execute the procedure at AnimationUpdateFrameMovement for no more than 200 instructions
	When I execute the procedure at AnimationUpdateEnemyFire for no more than 150 instructions
	When I execute the procedure at AnimationUpdateFrameMovement for no more than 200 instructions
	When I execute the procedure at AnimationUpdateEnemyFire for no more than 150 instructions
	When I execute the procedure at AnimationUpdateFrameMovement for no more than 200 instructions
	When I execute the procedure at AnimationUpdateEnemyFire for no more than 150 instructions
	When I execute the procedure at AnimationUpdateFrameMovement for no more than 200 instructions
	When I execute the procedure at AnimationUpdateEnemyFire for no more than 150 instructions
	When I execute the procedure at AnimationUpdateFrameMovement for no more than 200 instructions
	When I execute the procedure at AnimationUpdateEnemyFire for no more than 150 instructions
	When I execute the procedure at AnimationUpdateFrameMovement for no more than 200 instructions
	When I execute the procedure at AnimationUpdateEnemyFire for no more than 150 instructions
	When I execute the procedure at AnimationUpdateFrameMovement for no more than 200 instructions
	When I execute the procedure at AnimationUpdateEnemyFire for no more than 150 instructions
	When I execute the procedure at AnimationUpdateFrameMovement for no more than 200 instructions
	When I execute the procedure at AnimationUpdateEnemyFire for no more than 150 instructions
	When I execute the procedure at AnimationUpdateFrameMovement for no more than 200 instructions

	# Frame 10
	When I execute the procedure at AnimationUpdateEnemyFire for no more than 150 instructions
	# Movement update
	When I execute the procedure at AnimationUpdateFrameMovement for no more than 240 instructions
	When I execute the procedure at AnimationUpdateEnemyFire for no more than 150 instructions
	When I execute the procedure at AnimationUpdateFrameMovement for no more than 200 instructions
	When I execute the procedure at AnimationUpdateEnemyFire for no more than 150 instructions
	When I execute the procedure at AnimationUpdateFrameMovement for no more than 200 instructions
	When I execute the procedure at AnimationUpdateEnemyFire for no more than 150 instructions
	When I execute the procedure at AnimationUpdateFrameMovement for no more than 200 instructions
	When I execute the procedure at AnimationUpdateEnemyFire for no more than 150 instructions
	When I execute the procedure at AnimationUpdateFrameMovement for no more than 200 instructions
	When I execute the procedure at AnimationUpdateEnemyFire for no more than 150 instructions
	When I execute the procedure at AnimationUpdateFrameMovement for no more than 200 instructions
	When I execute the procedure at AnimationUpdateEnemyFire for no more than 150 instructions
	When I execute the procedure at AnimationUpdateFrameMovement for no more than 200 instructions
	When I execute the procedure at AnimationUpdateEnemyFire for no more than 150 instructions
	When I execute the procedure at AnimationUpdateFrameMovement for no more than 200 instructions
	When I execute the procedure at AnimationUpdateEnemyFire for no more than 150 instructions
	When I execute the procedure at AnimationUpdateFrameMovement for no more than 200 instructions
	When I execute the procedure at AnimationUpdateEnemyFire for no more than 150 instructions
	When I execute the procedure at AnimationUpdateFrameMovement for no more than 200 instructions

	# Frame 20
	When I execute the procedure at AnimationUpdateEnemyFire for no more than 150 instructions
	# Movement update
	When I execute the procedure at AnimationUpdateFrameMovement for no more than 240 instructions
	When I execute the procedure at AnimationUpdateEnemyFire for no more than 150 instructions
	When I execute the procedure at AnimationUpdateFrameMovement for no more than 200 instructions
	When I execute the procedure at AnimationUpdateEnemyFire for no more than 150 instructions
	When I execute the procedure at AnimationUpdateFrameMovement for no more than 200 instructions
	When I execute the procedure at AnimationUpdateEnemyFire for no more than 150 instructions
	When I execute the procedure at AnimationUpdateFrameMovement for no more than 200 instructions
	When I execute the procedure at AnimationUpdateEnemyFire for no more than 150 instructions
	When I execute the procedure at AnimationUpdateFrameMovement for no more than 200 instructions
	When I execute the procedure at AnimationUpdateEnemyFire for no more than 150 instructions
	When I execute the procedure at AnimationUpdateFrameMovement for no more than 200 instructions
	When I execute the procedure at AnimationUpdateEnemyFire for no more than 150 instructions
	When I execute the procedure at AnimationUpdateFrameMovement for no more than 200 instructions
	When I execute the procedure at AnimationUpdateEnemyFire for no more than 150 instructions
	When I execute the procedure at AnimationUpdateFrameMovement for no more than 200 instructions
	When I execute the procedure at AnimationUpdateEnemyFire for no more than 150 instructions
	When I execute the procedure at AnimationUpdateFrameMovement for no more than 200 instructions
	When I execute the procedure at AnimationUpdateEnemyFire for no more than 150 instructions
	When I execute the procedure at AnimationUpdateFrameMovement for no more than 200 instructions

	# Frame 30
	When I execute the procedure at AnimationUpdateEnemyFire for no more than 150 instructions
	# Movement update
	When I execute the procedure at AnimationUpdateFrameMovement for no more than 240 instructions
	When I execute the procedure at AnimationUpdateEnemyFire for no more than 150 instructions
	When I execute the procedure at AnimationUpdateFrameMovement for no more than 200 instructions
	When I execute the procedure at AnimationUpdateEnemyFire for no more than 150 instructions
	When I execute the procedure at AnimationUpdateFrameMovement for no more than 200 instructions
	When I execute the procedure at AnimationUpdateEnemyFire for no more than 150 instructions
	When I execute the procedure at AnimationUpdateFrameMovement for no more than 200 instructions
	When I execute the procedure at AnimationUpdateEnemyFire for no more than 150 instructions
	When I execute the procedure at AnimationUpdateFrameMovement for no more than 200 instructions
	When I execute the procedure at AnimationUpdateEnemyFire for no more than 150 instructions
	When I execute the procedure at AnimationUpdateFrameMovement for no more than 200 instructions
	When I execute the procedure at AnimationUpdateEnemyFire for no more than 150 instructions
	When I execute the procedure at AnimationUpdateFrameMovement for no more than 200 instructions
	When I execute the procedure at AnimationUpdateEnemyFire for no more than 150 instructions
	When I execute the procedure at AnimationUpdateFrameMovement for no more than 200 instructions
	When I execute the procedure at AnimationUpdateEnemyFire for no more than 150 instructions
	When I execute the procedure at AnimationUpdateFrameMovement for no more than 200 instructions
	When I execute the procedure at AnimationUpdateEnemyFire for no more than 150 instructions
	When I execute the procedure at AnimationUpdateFrameMovement for no more than 200 instructions

	# Frame 40
	When I execute the procedure at AnimationUpdateEnemyFire for no more than 150 instructions
	# Movement update
	When I execute the procedure at AnimationUpdateFrameMovement for no more than 240 instructions
	When I execute the procedure at AnimationUpdateEnemyFire for no more than 150 instructions
	When I execute the procedure at AnimationUpdateFrameMovement for no more than 200 instructions
	When I execute the procedure at AnimationUpdateEnemyFire for no more than 150 instructions
	When I execute the procedure at AnimationUpdateFrameMovement for no more than 200 instructions
	When I execute the procedure at AnimationUpdateEnemyFire for no more than 150 instructions
	When I execute the procedure at AnimationUpdateFrameMovement for no more than 200 instructions
	When I execute the procedure at AnimationUpdateEnemyFire for no more than 150 instructions
	When I execute the procedure at AnimationUpdateFrameMovement for no more than 200 instructions
	When I execute the procedure at AnimationUpdateEnemyFire for no more than 150 instructions
	When I execute the procedure at AnimationUpdateFrameMovement for no more than 200 instructions
	When I execute the procedure at AnimationUpdateEnemyFire for no more than 150 instructions
	When I execute the procedure at AnimationUpdateFrameMovement for no more than 200 instructions
	When I execute the procedure at AnimationUpdateEnemyFire for no more than 150 instructions
	When I execute the procedure at AnimationUpdateFrameMovement for no more than 200 instructions
	When I execute the procedure at AnimationUpdateEnemyFire for no more than 150 instructions
	When I execute the procedure at AnimationUpdateFrameMovement for no more than 200 instructions
	When I execute the procedure at AnimationUpdateEnemyFire for no more than 150 instructions
	When I execute the procedure at AnimationUpdateFrameMovement for no more than 200 instructions

	# Frame 50
	And I hex dump memory between AnimationFireRateCount and AnimationFireRateCount + Multiplex_items
	# Bullet fired
	When I execute the procedure at AnimationUpdateEnemyFire for no more than 400 instructions
	And I hex dump memory between AnimationType and AnimationType + Multiplex_items
	And I hex dump memory between AnimationDirectionIn and AnimationDirectionIn + Multiplex_items
	Then I expect to see AnimationType + 1 equal AnimationType_Enemy1Bullet + 1
	And I expect to see AnimationDirectionIn + 1 equal 0

	When I execute the procedure at AnimationUpdateFrameMovement for no more than 320 instructions
	Then I expect to see AnimationType + 1 equal AnimationType_Enemy1Bullet + 1
	And I expect to see AnimationDirectionIn equal 0
	And I expect to see AnimationDirectionIn + 1 equal 0
