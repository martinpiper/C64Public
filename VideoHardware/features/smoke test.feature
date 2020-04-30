Feature: Smoke test

  Quickly load the output and check various simple aspects of the code

  Scenario: Smoke test for video
    Given a new video display
    Given video display saves debug BMP images to leaf filename "tmp/frames/TC-1-"
    Given I have a simple overclocked 6502 system
    Given a user port to 24 bit bus is installed
    Given add a Tiles layer with registers at '0x9e00' and screen addressEx '0x80' and planes addressEx '0x40'
    Given add a Sprites layer with registers at '0x9800' and addressEx '0x10'
    Given add a Mode7 layer with registers at '0xa000' and addressEx '0x08'
#    Given add a Chars layer with registers at '0x9000' and addressEx '0x20'
    Given show video window

    # Instead of writing this data via the 6502 CPU, just send it straight to memory
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

    When I execute the procedure at start for no more than 62980 instructions until PC = mainLoop
    Then expect image "testdata/TC-1-000001.bmp" to be identical to "tmp/frames/TC-1-000001.bmp"

    When I execute the procedure at setupFrame for no more than 62980 instructions
	Given render a video display frame
    Then expect image "testdata/TC-1-000002.bmp" to be identical to "tmp/frames/TC-1-000002.bmp"
    Then expect image "testdata/TC-1-000003.bmp" to be identical to "tmp/frames/TC-1-000003.bmp"
#    Then expect image "testdata/TC-1-000004.bmp" to be identical to "tmp/frames/TC-1-000004.bmp"



#    When rendering the video until window closed
