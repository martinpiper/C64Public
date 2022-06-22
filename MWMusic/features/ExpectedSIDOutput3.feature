Feature: Test expected SID output from music the Armalyte player and data 3

  This checks for expected SID writes for several frames while calling the music player routine.
  Internal data checks are not needed as these can vary considerably, the test considers the music player to be a black box.

  Specifically checks the two extra $aa bytes don't cause problems just before tune3_2

  Scenario: Check the forth tune expected values
    Given I have a simple overclocked 6502 system
    And I run the command line: ..\acme.exe --setpc $3000 -o t.prg --labeldump t.lbl -f cbm ..\stdlib\stdlib.a MusicPlayer_Defs.a MusicPlayer.a MusicPlayerData.a
    And I load prg "t.prg"
    And I load labels "t.lbl"

    When I reset the cycle count
	When I set register A to 3
    When I execute the procedure at MusicInit for no more than 234 instructions
	Then I expect the cycle count to be no more than 783 cycles
    When I hex dump memory between $D400 and $d419
	Then I expect to see $d400 equal 0
	Then I expect to see $d401 equal 0
	Then I expect to see $d402 equal 0
	Then I expect to see $d403 equal 0
	Then I expect to see $d404 equal 0
	Then I expect to see $d405 equal 0
	Then I expect to see $d406 equal 0
	Then I expect to see $d407 equal 0

	Then I expect to see $d408 equal 0
	Then I expect to see $d409 equal 0
	Then I expect to see $d40a equal 0
	Then I expect to see $d40b equal 0
	Then I expect to see $d40c equal 0
	Then I expect to see $d40d equal 0
	Then I expect to see $d40e equal 0
	Then I expect to see $d40f equal 0

	Then I expect to see $d410 equal 0
	Then I expect to see $d411 equal 0
	Then I expect to see $d412 equal 0
	Then I expect to see $d413 equal 0
	Then I expect to see $d414 equal 0
	Then I expect to see $d415 equal 0
	Then I expect to see $d416 equal 0
	Then I expect to see $d417 equal 0

	Then I expect to see $d418 equal $0f


	# First frame
	When I reset the cycle count
    When I execute the procedure at MusicPlay for no more than 440 instructions
	Then I expect the cycle count to be no more than 1636 cycles
    When I hex dump memory between $D400 and $d419
	Then I expect to see $d400 equal $41
	Then I expect to see $d401 equal $03
	Then I expect to see $d402 equal 0
	Then I expect to see $d403 equal $06
	Then I expect to see $d404 equal $41
	Then I expect to see $d405 equal $cd
	Then I expect to see $d406 equal $0f
	Then I expect to see $d407 equal $e1

	Then I expect to see $d408 equal $04
	Then I expect to see $d409 equal 0
	Then I expect to see $d40a equal $06
	Then I expect to see $d40b equal $41
	Then I expect to see $d40c equal $cd
	Then I expect to see $d40d equal $0f
	Then I expect to see $d40e equal $58
	Then I expect to see $d40f equal $04

	Then I expect to see $d410 equal 0
	Then I expect to see $d411 equal $00
	Then I expect to see $d412 equal $81
	Then I expect to see $d413 equal $0c
	Then I expect to see $d414 equal $0c
	Then I expect to see $d415 equal 0
	Then I expect to see $d416 equal 0
	Then I expect to see $d417 equal 0
                            
	Then I expect to see $d418 equal $0f
