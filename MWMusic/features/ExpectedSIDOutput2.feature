Feature: Test expected SID output from music the Armalyte player and data 2

  This checks for expected SID writes for several frames while calling the music player routine.
  Internal data checks are not needed as these can vary considerably, the test considers the music player to be a black box.

  Scenario: Check the third tune expected values
    Given I have a simple overclocked 6502 system
    And I run the command line: ..\acme.exe --setpc $2000 -o t.prg --labeldump t.lbl -f cbm ..\stdlib\stdlib.a MusicPlayer_Defs.a MusicPlayer.a MusicPlayerData.a
    And I load prg "t.prg"
    And I load labels "t.lbl"

    When I reset the cycle count
	When I set register A to 2
    When I execute the procedure at MusicInit for no more than 256 instructions
	Then I expect the cycle count to be no more than 864 cycles
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
	Then I expect to see $d407 equal $4b

	Then I expect to see $d408 equal $68
	Then I expect to see $d409 equal 0
	Then I expect to see $d40a equal $0a
	Then I expect to see $d40b equal $41
	Then I expect to see $d40c equal $00
	Then I expect to see $d40d equal $7e
	Then I expect to see $d40e equal $bf
	Then I expect to see $d40f equal $49

	Then I expect to see $d410 equal 0
	Then I expect to see $d411 equal $04
	Then I expect to see $d412 equal $81
	Then I expect to see $d413 equal $09
	Then I expect to see $d414 equal $0f
	Then I expect to see $d415 equal 0
	Then I expect to see $d416 equal 0
	Then I expect to see $d417 equal 0
                            
	Then I expect to see $d418 equal $0f


	# After 30 frames
	When I reset the cycle count
	# TODO: Figure out where the extra cycles come from?
    When I execute the procedure at MusicPlay for no more than 1278 instructions
    When I execute the procedure at MusicPlay for no more than 1268 instructions
    When I execute the procedure at MusicPlay for no more than 1269 instructions
    When I execute the procedure at MusicPlay for no more than 1259 instructions
    When I execute the procedure at MusicPlay for no more than 1269 instructions
    When I execute the procedure at MusicPlay for no more than 345 instructions
    When I execute the procedure at MusicPlay for no more than 1274 instructions
    When I execute the procedure at MusicPlay for no more than 318 instructions
    When I execute the procedure at MusicPlay for no more than 1278 instructions
    When I execute the procedure at MusicPlay for no more than 1259 instructions

    When I execute the procedure at MusicPlay for no more than 1269 instructions
    When I execute the procedure at MusicPlay for no more than 1259 instructions
    When I execute the procedure at MusicPlay for no more than 1269 instructions
    When I execute the procedure at MusicPlay for no more than 1263 instructions
    When I execute the procedure at MusicPlay for no more than 1269 instructions
    When I execute the procedure at MusicPlay for no more than 313 instructions
    When I execute the procedure at MusicPlay for no more than 1261 instructions
    When I execute the procedure at MusicPlay for no more than 1260 instructions
    When I execute the procedure at MusicPlay for no more than 1262 instructions
    When I execute the procedure at MusicPlay for no more than 1253 instructions

    When I execute the procedure at MusicPlay for no more than 1262 instructions
    When I execute the procedure at MusicPlay for no more than 1257 instructions
    When I execute the procedure at MusicPlay for no more than 1262 instructions
    When I execute the procedure at MusicPlay for no more than 1262 instructions
    When I execute the procedure at MusicPlay for no more than 1264 instructions
    When I execute the procedure at MusicPlay for no more than 1253 instructions
    When I execute the procedure at MusicPlay for no more than 1262 instructions
    When I execute the procedure at MusicPlay for no more than 1253 instructions
    When I execute the procedure at MusicPlay for no more than 1262 instructions
    When I execute the procedure at MusicPlay for no more than 1257 instructions
	Then I expect the cycle count to be no more than 127392 cycles
    When I hex dump memory between $D400 and $d419
	Then I expect to see $d400 equal $21
	Then I expect to see $d401 equal $35
	Then I expect to see $d402 equal 0
	Then I expect to see $d403 equal 0
	Then I expect to see $d404 equal $80
	Then I expect to see $d405 equal $0a
	Then I expect to see $d406 equal $0a
	Then I expect to see $d407 equal $4b

	Then I expect to see $d408 equal $68
	Then I expect to see $d409 equal $18
	Then I expect to see $d40a equal $0d
	Then I expect to see $d40b equal $41
	Then I expect to see $d40c equal $00
	Then I expect to see $d40d equal $7e
	Then I expect to see $d40e equal $e7
	Then I expect to see $d40f equal $02

	Then I expect to see $d410 equal $c0
	Then I expect to see $d411 equal $04
	Then I expect to see $d412 equal $41
	Then I expect to see $d413 equal $09
	Then I expect to see $d414 equal $0f
	Then I expect to see $d415 equal 0
	Then I expect to see $d416 equal 0
	Then I expect to see $d417 equal 0
                            
	Then I expect to see $d418 equal $0f
