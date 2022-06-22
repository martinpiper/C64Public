Feature: Test expected SID output from music the Armalyte player and data

  This checks for expected SID writes for several frames while calling the music player routine.
  Internal data checks are not needed as these can vary considerably, the test considers the music player to be a black box.


  Scenario: Check performance metrics for the original Snare.prg
    Given I have a simple overclocked 6502 system
    And I load prg "OriginalData\Snare.prg"

    When I reset the cycle count
	When I set register A to 0
    When I execute the procedure at $27cd for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions
    When I execute the procedure at $282a for no more than 2000 instructions

    When I hex dump memory between $D400 and $d419
	Then I expect the cycle count to be no more than 173262 cycles




  Scenario: Check performance metrics for the converted Snarev2.mus
    Given I have a simple overclocked 6502 system
	And I run the command line: Debug\CompactorTool.exe -i bin\snarev2.mus -o t.a
    And I run the command line: ..\acme.exe --setpc $1000 -o t.prg --labeldump t.lbl -f cbm ..\stdlib\stdlib.a MusicPlayer_Defs.a MusicPlayer.a t.a
    And I load prg "t.prg"
    And I load labels "t.lbl"

    When I reset the cycle count
	When I set register A to 0
    When I execute the procedure at MusicInit for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions
    When I execute the procedure at MusicPlay for no more than 2000 instructions

    When I hex dump memory between $D400 and $d419
#	Then I expect the cycle count to be no more than 173262 cycles

	Then I expect to see $d400 equal $10
	Then I expect to see $d401 equal $27
	Then I expect to see $d402 equal $ad
	Then I expect to see $d403 equal $01
	Then I expect to see $d404 equal $41
	Then I expect to see $d405 equal $00
	Then I expect to see $d406 equal $8a
	Then I expect to see $d407 equal $b3

	Then I expect to see $d408 equal $3d
	Then I expect to see $d409 equal $00
	Then I expect to see $d40a equal $00
	Then I expect to see $d40b equal $80
	Then I expect to see $d40c equal $04
	Then I expect to see $d40d equal $05
	Then I expect to see $d40e equal $bf
	Then I expect to see $d40f equal $07

	Then I expect to see $d410 equal $80
	Then I expect to see $d411 equal $01
	Then I expect to see $d412 equal $41
	Then I expect to see $d413 equal $09
	Then I expect to see $d414 equal $8a
	Then I expect to see $d415 equal $00
	Then I expect to see $d416 equal $00
	Then I expect to see $d417 equal $00

	Then I expect to see $d418 equal $0f
