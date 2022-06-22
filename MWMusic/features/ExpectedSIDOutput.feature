Feature: Test expected SID output from music the Armalyte player and data

  This checks for expected SID writes for several frames while calling the music player routine.
  Internal data checks are not needed as these can vary considerably, the test considers the music player to be a black box.

  Scenario: Check the first tune expected values
    Given I have a simple overclocked 6502 system
    And I run the command line: ..\acme.exe --setpc $1000 -o t.prg --labeldump t.lbl -f cbm ..\stdlib\stdlib.a MusicPlayer_Defs.a MusicPlayer.a MusicPlayerData.a
    And I load prg "t.prg"
    And I load labels "t.lbl"

    When I reset the cycle count
	When I set register A to 0
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
    When I execute the procedure at MusicPlay for no more than 429 instructions
	Then I expect the cycle count to be no more than 1589 cycles
    When I hex dump memory between $D400 and $d419
	Then I expect to see $d400 equal $bf
	Then I expect to see $d401 equal $49
	Then I expect to see $d402 equal 0
	Then I expect to see $d403 equal 0
	Then I expect to see $d404 equal $81
	Then I expect to see $d405 equal $0a
	Then I expect to see $d406 equal $0a
	Then I expect to see $d407 equal $df

	Then I expect to see $d408 equal $24
	Then I expect to see $d409 equal 0
	Then I expect to see $d40a equal 0
	Then I expect to see $d40b equal $41
	Then I expect to see $d40c equal $0f
	Then I expect to see $d40d equal $6f
	Then I expect to see $d40e equal $03
	Then I expect to see $d40f equal $3e

	Then I expect to see $d410 equal 0
	Then I expect to see $d411 equal $08
	Then I expect to see $d412 equal $81
	Then I expect to see $d413 equal $2d
	Then I expect to see $d414 equal $6d
	Then I expect to see $d415 equal 0
	Then I expect to see $d416 equal 0
	Then I expect to see $d417 equal 0
                            
	Then I expect to see $d418 equal $0f


	# After 30 frames
	When I reset the cycle count
	# TODO: Figure out where the extra cycles come from?
    When I execute the procedure at MusicPlay for no more than 1323 instructions
    When I execute the procedure at MusicPlay for no more than 1277 instructions
    When I execute the procedure at MusicPlay for no more than 1281 instructions
    When I execute the procedure at MusicPlay for no more than 1283 instructions
    When I execute the procedure at MusicPlay for no more than 1278 instructions
    When I execute the procedure at MusicPlay for no more than 1278 instructions
    When I execute the procedure at MusicPlay for no more than 1280 instructions
    When I execute the procedure at MusicPlay for no more than 1285 instructions
    When I execute the procedure at MusicPlay for no more than 1283 instructions
    When I execute the procedure at MusicPlay for no more than 1286 instructions

    When I execute the procedure at MusicPlay for no more than 1275 instructions
    When I execute the procedure at MusicPlay for no more than 1279 instructions
    When I execute the procedure at MusicPlay for no more than 1277 instructions
    When I execute the procedure at MusicPlay for no more than 1281 instructions
    When I execute the procedure at MusicPlay for no more than 1281 instructions
    When I execute the procedure at MusicPlay for no more than 1287 instructions
    When I execute the procedure at MusicPlay for no more than 1341 instructions
    When I execute the procedure at MusicPlay for no more than 1286 instructions
    When I execute the procedure at MusicPlay for no more than 1278 instructions
    When I execute the procedure at MusicPlay for no more than 1276 instructions

    When I execute the procedure at MusicPlay for no more than 1280 instructions
    When I execute the procedure at MusicPlay for no more than 1282 instructions
    When I execute the procedure at MusicPlay for no more than 1279 instructions
    When I execute the procedure at MusicPlay for no more than 1279 instructions
    When I execute the procedure at MusicPlay for no more than 1281 instructions
    When I execute the procedure at MusicPlay for no more than 1286 instructions
    When I execute the procedure at MusicPlay for no more than 1284 instructions
    When I execute the procedure at MusicPlay for no more than 1287 instructions
    When I execute the procedure at MusicPlay for no more than 1274 instructions
    When I execute the procedure at MusicPlay for no more than 1322 instructions
	Then I expect the cycle count to be no more than 129057 cycles
    When I hex dump memory between $D400 and $d419
	Then I expect to see $d400 equal $4b
	Then I expect to see $d401 equal $5a
	Then I expect to see $d402 equal 0
	Then I expect to see $d403 equal 0
	Then I expect to see $d404 equal $80
	Then I expect to see $d405 equal $0a
	Then I expect to see $d406 equal $0a
	Then I expect to see $d407 equal $df

	# TODO: Find out why the SID values changed
	Then I expect to see $d408 equal $24
	Then I expect to see $d409 equal $2a
	Then I expect to see $d40a equal $03
	Then I expect to see $d40b equal $41
	Then I expect to see $d40c equal $0f
	Then I expect to see $d40d equal $6f
	Then I expect to see $d40e equal $4f
	Then I expect to see $d40f equal $02

	Then I expect to see $d410 equal $2a
	Then I expect to see $d411 equal $0b
	Then I expect to see $d412 equal $41
	Then I expect to see $d413 equal $2d
	Then I expect to see $d414 equal $6d
	Then I expect to see $d415 equal 0
	Then I expect to see $d416 equal 0
	Then I expect to see $d417 equal 0
                            
	Then I expect to see $d418 equal $0f
