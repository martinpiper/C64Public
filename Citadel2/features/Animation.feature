@c64 @java
Feature: Animation tests

	Execute this with: java -jar ..\..\BDD6502\target\BDD6502-1.0.3-SNAPSHOT-jar-with-dependencies.jar
	Or with CPU trace: java -Dbdd6502.trace=true -jar ..\..\BDD6502\target\BDD6502-1.0.3-SNAPSHOT-jar-with-dependencies.jar

	Performs animation routine tests using known data

MPi: TODO: Implement tests for frames and movement

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

	*=$400
	; Test specific code goes here
	AllocateEnemyTest
		rts
;	ldy #AnimationType_Enemy1
;	jmp AnimationFindFreeSlot
	"""
	And I run the command line: ..\acme.exe -o test.prg --lib ../Scroller/ --labeldump test.lbl -f cbm test.a
	And I run the command line: ..\bin\LZMPi.exe -c64 test.prg testcomp.prg $400
	And I load prg "test.prg"
	And I load labels "test.lbl"
	# The first instruction at $400 is LDA #
#	Then I expect to see $400 equal $a9
#	When I execute the procedure at ScrollerDemoInitOneTimeData for no more than 50000 instructions
#	When I execute the procedure at InitialiseGameWithoutWait for no more than 20000 instructions
#	When I execute the procedure at AnimationInit for no more than 300 instructions
#	Then I expect to see AnimationType equal $ff
#	When I execute the procedure at AllocateEnemyTest for no more than 50 instructions
#	Then I expect to see AnimationType equal AnimationType_Enemy1

