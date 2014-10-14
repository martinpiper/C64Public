Feature: Animation tests

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
	Given I have a simple 6502 system
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
	Multiplex_IRQServiceRoutineLo = KERNALIRQServiceRoutineLo;
	Multiplex_IRQServiceRoutineHi = KERNALIRQServiceRoutineHi;
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

	!source "ScrollEntryCommon.a"

	; Test specific code goes here
	AllocateEnemyTest
	ldy #AnimationType_Enemy1
	jmp AnimationFindFreeSlot
	!macro SEUCKFileData .offset, .size {
	!bin "../SEUCK/ANIMTSTS.P00",.offset,.size
	}

	!source "ScrollerData.a"
	"""
	And I run the command line: ..\acme.exe -o test.prg --labeldump test.lbl -f cbm test.a
	And I run the command line: ..\bin\LZMPi.exe -c64 test.prg testcomp.prg $400
	And I load prg "test.prg"
	And I load labels "test.lbl"
	# The first instruction at $400 is LDA #
	Then I expect to see $400 contain $a9
	When I execute the procedure at ScrollerDemoInitOneTimeData for no more than 50000 instructions
	When I execute the procedure at InitialiseGameWithoutWait for no more than 20000 instructions
	When I execute the procedure at AnimationInit for no more than 500 instructions
	Then I expect to see AnimationType contain $ff
	When I execute the procedure at AllocateEnemyTest for no more than 50 instructions
	Then I expect to see AnimationType contain AnimationType_Enemy1



Scenario: Enemy animation animation looping
	When I execute the procedure at AnimationInit for no more than 300 instructions
	Then I expect to see AnimationType contain $ff

	When I execute the procedure at AnimationTriggerEnemies for no more than 200 instructions
	Then I expect to see AnimationType contain AnimationType_Enemy1

	# This first frame is slightly more expensive because some extra initialisation happens
	When I execute the procedure at AnimationUpdateFrameMovement for no more than 250 instructions
	Then I expect to see AnimationType contain AnimationType_Enemy1
	Then I expect to see AnimationSpriteFrame contain $42	

	# Slightly less expensive for the subsequent frames
	When I execute the procedure at AnimationUpdateFrameMovement for no more than 220 instructions
	Then I expect to see AnimationType contain AnimationType_Enemy1
	Then I expect to see AnimationSpriteFrame contain $43

	When I execute the procedure at AnimationUpdateFrameMovement for no more than 220 instructions
	Then I expect to see AnimationType contain AnimationType_Enemy1
	Then I expect to see AnimationSpriteFrame contain $44

	When I execute the procedure at AnimationUpdateFrameMovement for no more than 220 instructions
	Then I expect to see AnimationType contain AnimationType_Enemy1
	Then I expect to see AnimationSpriteFrame contain $45

	# Direction change happens here, slightly more time
	When I execute the procedure at AnimationUpdateFrameMovement for no more than 250 instructions
	Then I expect to see AnimationType contain AnimationType_Enemy1
	Then I expect to see AnimationSpriteFrame contain $46

	When I execute the procedure at AnimationUpdateFrameMovement for no more than 220 instructions
	Then I expect to see AnimationType contain AnimationType_Enemy1
	Then I expect to see AnimationSpriteFrame contain $47

	When I execute the procedure at AnimationUpdateFrameMovement for no more than 220 instructions
	Then I expect to see AnimationType contain AnimationType_Enemy1
	Then I expect to see AnimationSpriteFrame contain $48

	When I execute the procedure at AnimationUpdateFrameMovement for no more than 220 instructions
	Then I expect to see AnimationType contain AnimationType_Enemy1
	Then I expect to see AnimationSpriteFrame contain $49

	When I execute the procedure at AnimationUpdateFrameMovement for no more than 220 instructions
	Then I expect to see AnimationType contain AnimationType_Enemy1
	Then I expect to see AnimationSpriteFrame contain $4a

	When I execute the procedure at AnimationUpdateFrameMovement for no more than 220 instructions
	Then I expect to see AnimationType contain AnimationType_Enemy1
	Then I expect to see AnimationSpriteFrame contain $4b

	When I execute the procedure at AnimationUpdateFrameMovement for no more than 220 instructions
	Then I expect to see AnimationType contain AnimationType_Enemy1
	Then I expect to see AnimationSpriteFrame contain $4c

	When I execute the procedure at AnimationUpdateFrameMovement for no more than 220 instructions
	Then I expect to see AnimationType contain AnimationType_Enemy1
	Then I expect to see AnimationSpriteFrame contain $4d

	When I execute the procedure at AnimationUpdateFrameMovement for no more than 220 instructions
	Then I expect to see AnimationType contain AnimationType_Enemy1
	Then I expect to see AnimationSpriteFrame contain $4e

	When I execute the procedure at AnimationUpdateFrameMovement for no more than 220 instructions
	Then I expect to see AnimationType contain AnimationType_Enemy1
	Then I expect to see AnimationSpriteFrame contain $4f

	When I execute the procedure at AnimationUpdateFrameMovement for no more than 220 instructions
	Then I expect to see AnimationType contain AnimationType_Enemy1
	Then I expect to see AnimationSpriteFrame contain $50

	When I execute the procedure at AnimationUpdateFrameMovement for no more than 220 instructions
	Then I expect to see AnimationType contain AnimationType_Enemy1
	Then I expect to see AnimationSpriteFrame contain $51

	When I execute the procedure at AnimationUpdateFrameMovement for no more than 220 instructions
	Then I expect to see AnimationType contain AnimationType_Enemy1
	Then I expect to see AnimationSpriteFrame contain $52

	# Animation loop, slightly more time
	When I execute the procedure at AnimationUpdateFrameMovement for no more than 230 instructions
	Then I expect to see AnimationType contain AnimationType_Enemy1
	Then I expect to see AnimationSpriteFrame contain $53

	When I execute the procedure at AnimationUpdateFrameMovement for no more than 220 instructions
	Then I expect to see AnimationType contain AnimationType_Enemy1
	Then I expect to see AnimationSpriteFrame contain $42

	When I execute the procedure at AnimationUpdateFrameMovement for no more than 220 instructions
	Then I expect to see AnimationType contain AnimationType_Enemy1
	Then I expect to see AnimationSpriteFrame contain $43

