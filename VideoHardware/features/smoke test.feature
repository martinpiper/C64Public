@smoketest
Feature: Smoke test

  Quickly load the output and check various simple aspects of the code

  Scenario: Smoke test for video
    Given a new audio expansion
    Given a new video display
    Given video display processes 8 pixels per instruction
    And audio refresh window every 0 instructions
    And audio refresh is independent
    Given video display refresh window every 256 instructions
    Given video display add joystick to port 1
    Given video display saves debug BMP images to leaf filename "tmp/frames/TC-1-"
    Given property "bdd6502.bus24.trace" is set to string "true"
    Given I disable trace
    Given I have a simple overclocked 6502 system
    When I enable unitialised memory read protection with immediate fail
    * That does fail on BRK
    Given a user port to 24 bit bus is installed
    Given add a Tiles layer with registers at '0x9e00' and screen addressEx '0x80' and planes addressEx '0x40'
    Given add a Sprites layer with registers at '0x9800' and addressEx '0x10'
    Given add a Mode7 layer with registers at '0xa000' and addressEx '0x08'
#    Given add a Chars layer with registers at '0x9000' and addressEx '0x20'
    Given show video window

    # Instead of writing this data via the 6502 CPU, just send it straight to memory
	#  Music
	Given write data from file "tmp/target/exportedMusicSamples.bin" to 24bit bus at '0x0000' and addressEx '0x04'
    # Palette
    Given write data from file "tmp/PaletteData.bin" to 24bit bus at '0x9c00' and addressEx '0x01'
    # Sprites
    Given write data from file "tmp/sprite_plane0.bin" to 24bit bus at '0x2000' and addressEx '0x10'
    Given write data from file "tmp/sprite_plane1.bin" to 24bit bus at '0x4000' and addressEx '0x10'
    Given write data from file "tmp/sprite_plane2.bin" to 24bit bus at '0x8000' and addressEx '0x10'
    # Tiles
    Given write data from file "tmp/map.bin" to 24bit bus at '0x2000' and addressEx '0x80'
    Given write data from file "tmp/background_plane0.bin" to 24bit bus at '0x2000' and addressEx '0x40'
    Given write data from file "tmp/background_plane1.bin" to 24bit bus at '0x4000' and addressEx '0x40'
    Given write data from file "tmp/background_plane2.bin" to 24bit bus at '0x8000' and addressEx '0x40'
    # Chars
#    Given write data from file "C:\work\BombJack\05_k08t.bin" to 24bit bus at '0x2000' and addressEx '0x20'
#    Given write data from file "C:\work\BombJack\04_h08t.bin" to 24bit bus at '0x4000' and addressEx '0x20'
#    Given write data from file "C:\work\BombJack\03_e08t.bin" to 24bit bus at '0x8000' and addressEx '0x20'
    # Mode7
    Given write data from file "tmp/clouds_screen.bin" to 24bit bus at '0x2000' and addressEx '0x08'
    Given write data from file "tmp/clouds_tiles.bin" to 24bit bus at '0x4000' and addressEx '0x08'
    Given write data from file "tmp/clouds_tiles.bin2" to 24bit bus at '0x8000' and addressEx '0x08'


    And I load prg "bin/main.prg"
    And I load labels "tmp/main.map"

#    And I enable trace with indent
    When I execute the procedure at start for no more than 1000000 instructions until PC = mainLoop
	Given render a video display frame
#    Then expect image "testdata/TC-1-000000.bmp" to be identical to "tmp/frames/TC-1-000000.bmp"

#    And I enable trace with indent
    When I execute the procedure at setupFrame for no more than 1000000 instructions
    When I execute the procedure at setupFrame for no more than 1000000 instructions
    When I execute the procedure at setupFrame for no more than 1000000 instructions
    When I execute the procedure at setupFrame for no more than 1000000 instructions
    When I execute the procedure at setupFrame for no more than 1000000 instructions
    When I execute the procedure at setupFrame for no more than 1000000 instructions
    When I execute the procedure at setupFrame for no more than 1000000 instructions
    When I execute the procedure at setupFrame for no more than 1000000 instructions

    Then expect image "testdata/TC-1-000001.bmp" to be identical to "tmp/frames/TC-1-000001.bmp"
    Then expect image "testdata/TC-1-000002.bmp" to be identical to "tmp/frames/TC-1-000002.bmp"
    Then expect image "testdata/TC-1-000003.bmp" to be identical to "tmp/frames/TC-1-000003.bmp"
    Then expect image "testdata/TC-1-000004.bmp" to be identical to "tmp/frames/TC-1-000004.bmp"
    Then expect image "testdata/TC-1-000005.bmp" to be identical to "tmp/frames/TC-1-000005.bmp"
    Then expect image "testdata/TC-1-000006.bmp" to be identical to "tmp/frames/TC-1-000006.bmp"
    Then expect image "testdata/TC-1-000007.bmp" to be identical to "tmp/frames/TC-1-000007.bmp"
    Then expect image "testdata/TC-1-000008.bmp" to be identical to "tmp/frames/TC-1-000008.bmp"
    Then expect image "testdata/TC-1-000009.bmp" to be identical to "tmp/frames/TC-1-000009.bmp"


    # This allows the last frame to be observed and window zoomed/resized
#    When rendering the video until window closed

    # This allows code to be executed until the window is closed, with the option of saving debug BMP files
    Given I disable trace
    Given property "bdd6502.bus24.trace" is set to string "false"
    Given video display does not save debug BMP images
#    Given video display processes 24 pixels per instruction
    Given limit video display to 60 fps
    When I execute the procedure at mainLoop until return
