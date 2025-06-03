@smoketest
Feature: Smoke test

  Quickly load the output and check various simple aspects of the code

  # Run with Convert.bat
  @Demo1
  Scenario: Smoke test for video
    Given clear all external devices
    Given a new audio expansion
    And audio mix 85
    Given a new video display with overscan and 16 colours
    Given set the video display to RGB colour 5 6 5
    Given set the video display with 32 palette banks
#	And enable video display bus debug output
    Given video display processes 8 pixels per instruction
    And audio refresh window every 0 instructions
    And audio refresh is independent
    Given video display refresh window every 256 instructions
    Given video display add joystick to port 1
    Given video display saves debug BMP images to leaf filename "tmp/frames/TC-1-"
    Given property "bdd6502.bus24.trace" is set to string "true"
    Given I disable trace
    Given I have a simple overclocked 6502 system
    When I enable uninitialised memory read protection with immediate fail
    * That does fail on BRK
    Given a user port to 24 bit bus is installed
#    Given add a GetBackground layer fetching from layer index '1'
	# Order with priority register
    Given add a Mode7 layer with registers at '0xa000' and addressEx '0x08'
    And the layer has 16 colours
    And the layer has overscan
    Given add a Tiles layer with registers at '0x9e00' and screen addressEx '0x80' and planes addressEx '0x40'
    And the layer has 16 colours
    And the layer has overscan
    Given add a Chars V4.0 layer with registers at '0x9000' and screen addressEx '0x80' and planes addressEx '0x20'
    And the layer has 16 colours
    And the layer has overscan
#    Given add a Sprites layer with registers at '0x9800' and addressEx '0x10'
    Given add a Sprites V9.5 layer with registers at '0x9800' and addressEx '0x10' and running at 16MHz
    And the layer has 16 colours
    And the layer has overscan

    Given show video window
	Given randomly initialise all memory using seed 4321

    # Instead of writing this data via the 6502 CPU, just send it straight to memory
	#  Music
	Given write data from file "tmp/target/exportedMusicSamples.bin" to 24bit bus at '0x0000' and addressEx '0x04'
	# In main.a check IncludeGraphicsData for data upload
    # Palette
    Given write data from file "tmp/PaletteData.bin" to 24bit bus at '0x9c00' and addressEx '0x01'
    # Sprites
    Given write data from file "tmp/sprite_plane0.bin" to 24bit bus at '0x2000' and addressEx '0x10'
    Given write data from file "tmp/sprite_plane1.bin" to 24bit bus at '0x4000' and addressEx '0x10'
    Given write data from file "tmp/sprite_plane2.bin" to 24bit bus at '0x8000' and addressEx '0x10'
    Given write data from file "tmp/sprite_plane3.bin" to 24bit bus at '0x0000' and addressEx '0x10'
    # Tiles
    Given write data from file "tmp/map.bin" to 24bit bus at '0x2000' and addressEx '0x80'
    Given write data from file "tmp/background_plane0.bin" to 24bit bus at '0x2000' and addressEx '0x40'
    Given write data from file "tmp/background_plane1.bin" to 24bit bus at '0x4000' and addressEx '0x40'
    Given write data from file "tmp/background_plane2.bin" to 24bit bus at '0x8000' and addressEx '0x40'
    Given write data from file "tmp/background_plane3.bin" to 24bit bus at '0x0000' and addressEx '0x40'
    # Chars
    Given write data from file "tmp/status_map.bin" to 24bit bus at '0x4000' and addressEx '0x80'
    Given write data from file "tmp/status_map.bin2" to 24bit bus at '0x8000' and addressEx '0x80'
    Given write data from file "tmp/status_plane0.bin" to 24bit bus at '0x2000' and addressEx '0x20'
    Given write data from file "tmp/status_plane1.bin" to 24bit bus at '0x4000' and addressEx '0x20'
    Given write data from file "tmp/status_plane2.bin" to 24bit bus at '0x8000' and addressEx '0x20'
    Given write data from file "tmp/status_plane3.bin" to 24bit bus at '0x0000' and addressEx '0x20'
    # Mode7
    Given write data from file "tmp/clouds_screen.bin" to 24bit bus at '0x2000' and addressEx '0x08'
    Given write data from file "tmp/clouds_tiles.bin" to 24bit bus at '0x4000' and addressEx '0x08'
    Given write data from file "tmp/clouds_tiles.bin2" to 24bit bus at '0x8000' and addressEx '0x08'

    And I load prg "bin/main.prg"
    And I load labels "tmp/main.map"

	When ignore address Video_WaitVBlank_startGuard to Video_WaitVBlank_endGuard for trace

	When assert on exec memory from EndMainMemory to $10000

#    And I enable trace with indent
    When I execute the procedure at start for no more than 1000000 instructions until PC = mainLoop
	Given render a video display frame
#    Then expect image "testdata/TC-1-000000.bmp" to be identical to "tmp/frames/TC-1-000000.bmp"

#    And I enable trace with indent
    Given execute "setupFrame" for eight times

#    Then expect image "testdata/TC-1-000003.bmp" to be identical to "tmp/frames/TC-1-000003.bmp"
#    Then expect image "testdata/TC-1-000004.bmp" to be identical to "tmp/frames/TC-1-000004.bmp"
#    Then expect image "testdata/TC-1-000005.bmp" to be identical to "tmp/frames/TC-1-000005.bmp"
#    Then expect image "testdata/TC-1-000006.bmp" to be identical to "tmp/frames/TC-1-000006.bmp"
#    Then expect image "testdata/TC-1-000007.bmp" to be identical to "tmp/frames/TC-1-000007.bmp"
#    Then expect image "testdata/TC-1-000008.bmp" to be identical to "tmp/frames/TC-1-000008.bmp"


    # This allows the last frame to be observed and window zoomed/resized
#    When rendering the video until window closed

    # This allows code to be executed until the window is closed, with the option of saving debug BMP files
    Given I disable trace
#    Given I enable trace with indent
    Given property "bdd6502.bus24.trace" is set to string "false"
    Given video display does not save debug BMP images
#    Given video display processes 24 pixels per instruction
    Given limit video display to 60 fps
    When I execute the procedure at mainLoop until return




  # Run with Convert2.bat and Mode7LayersEnable defined in main.a
  @Demo2
  Scenario: Smoke test for video with Mode7LayersEnable
    Given clear all external devices
    Given a new audio expansion
    And audio mix 85
    Given a new video display with 16 colours
    Given set the video display to RGB colour 5 6 5
    Given set the video display with 32 palette banks
#	And enable video display bus debug output
    Given video display processes 8 pixels per instruction
    And audio refresh window every 0 instructions
    And audio refresh is independent
    Given video display refresh window every 256 instructions
    Given video display add joystick to port 1
#    Given video display saves debug BMP images to leaf filename "tmp/frames/TC-2-"
    Given property "bdd6502.bus24.trace" is set to string "true"
    Given I disable trace
    Given I have a simple overclocked 6502 system
    When I enable uninitialised memory read protection with immediate fail
    * That does fail on BRK
    Given a user port to 24 bit bus is installed
    Given add a Tiles layer with registers at '0x9e00' and screen addressEx '0x80' and planes addressEx '0x40'
    And the layer has 16 colours
	# For Mode7LayersEnable
    Given add a Mode7 layer with registers at '0xb000' and addressEx '0x08'
    Given add a Mode7 layer with registers at '0xa000' and addressEx '0x08'
    Given add a Mode7 layer with registers at '0xa800' and addressEx '0x08'
    Given add a Sprites layer with registers at '0x9800' and addressEx '0x10'
    And the layer has 16 colours
#    Given add a Chars layer with registers at '0x9000' and addressEx '0x20'
#    And the layer has 16 colours
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
    Given write data from file "tmp/sprite_plane3.bin" to 24bit bus at '0x0000' and addressEx '0x10'
    # Tiles
    Given write data from file "tmp/map.bin" to 24bit bus at '0x2000' and addressEx '0x80'
    Given write data from file "tmp/background_plane0.bin" to 24bit bus at '0x2000' and addressEx '0x40'
    Given write data from file "tmp/background_plane1.bin" to 24bit bus at '0x4000' and addressEx '0x40'
    Given write data from file "tmp/background_plane2.bin" to 24bit bus at '0x8000' and addressEx '0x40'
    Given write data from file "tmp/background_plane3.bin" to 24bit bus at '0x0000' and addressEx '0x40'
    # Mode7
    Given write data from file "tmp/clouds_screen.bin" to 24bit bus at '0x2000' and addressEx '0x08'
    Given write data from file "tmp/clouds_tiles.bin" to 24bit bus at '0x4000' and addressEx '0x08'
    Given write data from file "tmp/clouds_tiles.bin2" to 24bit bus at '0x8000' and addressEx '0x08'


    And I load prg "bin/main.prg"
    And I load labels "tmp/main.map"

#    And I enable trace with indent
    When I execute the procedure at start for no more than 1000000 instructions until PC = mainLoop
	Given render a video display frame

#    And I enable trace with indent
    When I execute the procedure at setupFrame for no more than 1000000 instructions

    # This allows the last frame to be observed and window zoomed/resized
#    When rendering the video until window closed

    # This allows code to be executed until the window is closed, with the option of saving debug BMP files
    Given I disable trace
    Given property "bdd6502.bus24.trace" is set to string "false"
    Given video display does not save debug BMP images
#    Given video display processes 24 pixels per instruction
    Given limit video display to 60 fps
    When I execute the procedure at mainLoop until return





  # Run with Convert3.bat
  @Demo3
  Scenario: Smoke test for chars with sprites (BatBall)
    Given clear all external devices
    Given a new video display with overscan and 16 colours
    Given set the video display to RGB colour 5 6 5
    Given set the video display with 32 palette banks
#	And enable video display bus debug output
    Given video display processes 24 pixels per instruction
    Given video display refresh window every 32 instructions
    Given video display add joystick to port 1
    Given video display add CIA1 timers with raster offset 0 , 0
#    Given video display saves debug BMP images to leaf filename "tmp/frames/TC-3-"
#    Given property "bdd6502.bus24.trace" is set to string "true"
#	Given property "bdd6502.apu.trace" is set to string "true"
    Given I disable trace
    Given I have a simple overclocked 6502 system
    When I enable uninitialised memory read protection with immediate fail
    * That does fail on BRK
    Given a user port to 24 bit bus is installed
#    And enable user port bus debug output
    And enable APU mode
    And APU clock divider 1
    And APU memory clock divider 2
#    Given add a StaticColour layer for palette index '0x7f'
    Given add a GetBackground layer fetching from layer index '1'
    And the layer has 16 colours
    And the layer has overscan
    Given add a Tiles layer with registers at '0x9e00' and screen addressEx '0x80' and planes addressEx '0x40'
    And the layer has 16 colours
    And the layer has overscan
    Given add a Chars V4.0 layer with registers at '0x9000' and screen addressEx '0x80' and planes addressEx '0x20'
    And the layer has 16 colours
    And the layer has overscan
    Given add a Sprites layer with registers at '0x9800' and addressEx '0x10'
    And the layer has 16 colours
    And the layer has overscan
    Given show video window
    Given randomly initialise all memory using seed 4321

    # Instead of writing this data via the 6502 CPU, just send it straight to memory
    # Palette
#    Given write data from file "tmp/BatBallPaletteData.bin" to 24bit bus at '0x9c00' and addressEx '0x01'
    # Sprites
#    Given write data from file "tmp/BatBallsprite_plane0.bin" to 24bit bus at '0x2000' and addressEx '0x10'
#    Given write data from file "tmp/BatBallsprite_plane1.bin" to 24bit bus at '0x4000' and addressEx '0x10'
#    Given write data from file "tmp/BatBallsprite_plane2.bin" to 24bit bus at '0x8000' and addressEx '0x10'
#    Given write data from file "tmp/BatBallsprite_plane3.bin" to 24bit bus at '0x0000' and addressEx '0x10'
    # Chars
#    Given write data from file "tmp/BatBallmap.bin" to 24bit bus at '0x9000' and addressEx '0x01'
#    Given write data from file "tmp/BatBallchars_plane0.bin" to 24bit bus at '0x2000' and addressEx '0x20'
#    Given write data from file "tmp/BatBallchars_plane1.bin" to 24bit bus at '0x4000' and addressEx '0x20'
#    Given write data from file "tmp/BatBallchars_plane2.bin" to 24bit bus at '0x8000' and addressEx '0x20'
#    Given write data from file "tmp/BatBallchars_plane3.bin" to 24bit bus at '0x0000' and addressEx '0x20'


    And I load prg "bin/main.prg"
    And I load labels "tmp/main.map"

    When enable remote debugging
#    And wait for debugger connection
#    And wait for debugger command

#    And I enable trace with indent
    When I execute the procedure at start for no more than 1000000 instructions until PC = mainLoop
	Given render a video display frame

#    And I enable trace with indent

    # This allows the last frame to be observed and window zoomed/resized
#    When rendering the video until window closed

    # This allows code to be executed until the window is closed, with the option of saving debug BMP files
    Given I disable trace
    Given property "bdd6502.bus24.trace" is set to string "false"
    Given video display does not save debug BMP images
    Given video display processes 24 pixels per instruction
    Given limit video display to 60 fps
    When I execute the procedure at mainLoop until return



# Run with Convert4.bat
  @Demo4
  Scenario: Smoke test for Turrican demo
    Given clear all external devices
#    Given a new video display with 16 colours
    Given a new video display with overscan and 16 colours
    Given set the video display to RGB colour 5 6 5
    Given set the video display with 32 palette banks
#    And enable video display bus debug output
    Given video display processes 24 pixels per instruction
    Given video display refresh window every 32 instructions
    Given video display add joystick to port 1
    Given video display add CIA1 timers with raster offset 0 , 0
#	Given video display saves debug BMP images to leaf filename "tmp/frames/TC-4-"
    Given property "bdd6502.bus24.trace" is set to string "true"
    Given I disable trace
#	And I enable trace with indent
    Given I have a simple overclocked 6502 system
    When I enable uninitialised memory read protection with immediate fail
    * That does fail on BRK
    Given a user port to 24 bit bus is installed
#    Given add a StaticColour layer for palette index '0x7f'
#    Given add a GetBackground layer fetching from layer index '1'
    Given add a Mode7 layer with registers at '0xa000' and addressEx '0x08'
    And the layer has 16 colours
    And the layer has overscan
    Given add a Tiles layer with registers at '0x9e00' and screen addressEx '0x80' and planes addressEx '0x40'
    And the layer has 16 colours
    And the layer has overscan
    Given add a Chars V4.0 layer with registers at '0x9000' and screen addressEx '0x80' and planes addressEx '0x20'
    And the layer has 16 colours
    And the layer has overscan
    Given add a Sprites layer with registers at '0x9800' and addressEx '0x10'
    And the layer has 16 colours
    And the layer has overscan
    Given show video window

    # Instead of writing this data via the 6502 CPU, just send it straight to memory
    # Palette
    Given write data from file "tmp/TurricanPaletteData.bin" to 24bit bus at '0x9c00' and addressEx '0x01'
    # Sprites
    Given write data from file "tmp/TurricanSprites_plane0.bin" to 24bit bus at '0x2000' and addressEx '0x10'
    Given write data from file "tmp/TurricanSprites_plane1.bin" to 24bit bus at '0x4000' and addressEx '0x10'
    Given write data from file "tmp/TurricanSprites_plane2.bin" to 24bit bus at '0x8000' and addressEx '0x10'
    Given write data from file "tmp/TurricanSprites_plane3.bin" to 24bit bus at '0x0000' and addressEx '0x10'
    # Tiles
    Given write data from file "tmp/TurricanTiles_map.bin" to 24bit bus at '0x2000' and addressEx '0x80'
    Given write data from file "tmp/TurricanTiles_plane0.bin" to 24bit bus at '0x2000' and addressEx '0x40'
    Given write data from file "tmp/TurricanTiles_plane1.bin" to 24bit bus at '0x4000' and addressEx '0x40'
    Given write data from file "tmp/TurricanTiles_plane2.bin" to 24bit bus at '0x8000' and addressEx '0x40'
    Given write data from file "tmp/TurricanTiles_plane3.bin" to 24bit bus at '0x0000' and addressEx '0x40'
    # Chars
    Given write data from file "tmp/TurricanStatus_map.bin" to 24bit bus at '0x4000' and addressEx '0x80'
    Given write data from file "tmp/TurricanStatus_map.bin2" to 24bit bus at '0x8000' and addressEx '0x80'
    Given write data from file "tmp/TurricanStatus_plane0.bin" to 24bit bus at '0x2000' and addressEx '0x20'
    Given write data from file "tmp/TurricanStatus_plane1.bin" to 24bit bus at '0x4000' and addressEx '0x20'
    Given write data from file "tmp/TurricanStatus_plane2.bin" to 24bit bus at '0x8000' and addressEx '0x20'
    Given write data from file "tmp/TurricanStatus_plane3.bin" to 24bit bus at '0x0000' and addressEx '0x20'
    # Mode7
    Given write data from file "tmp/TurricanClouds_screen.bin" to 24bit bus at '0x2000' and addressEx '0x08'
    Given write data from file "tmp/TurricanClouds_tiles.bin" to 24bit bus at '0x4000' and addressEx '0x08'
    Given write data from file "tmp/TurricanClouds_tiles.bin2" to 24bit bus at '0x8000' and addressEx '0x08'


    And I load prg "bin/main.prg"
    And I load labels "tmp/main.map"

#    And I enable trace with indent
    When I execute the procedure at start for no more than 1000000 instructions until PC = mainLoop
	Given render a video display frame

#    And I enable trace with indent

    # This allows the last frame to be observed and window zoomed/resized
#    When rendering the video until window closed

    # This allows code to be executed until the window is closed, with the option of saving debug BMP files
    Given I disable trace
    Given property "bdd6502.bus24.trace" is set to string "false"
    Given video display does not save debug BMP images
    Given video display processes 24 pixels per instruction
    Given limit video display to 60 fps
    When I execute the procedure at mainLoop until return




# Run with Convert5.bat
  @Demo5
  Scenario: Smoke test for Road demo
    Given clear all external devices
#    Given a new video display with 16 colours
    Given a new video display with overscan and 16 colours
    Given set the video display to RGB colour 5 6 5
    Given set the video display with 32 palette banks
#	And enable video display bus debug output
    Given video display processes 24 pixels per instruction
    Given video display refresh window every 32 instructions
    Given video display add joystick to port 1
    Given video display add CIA1 timers with raster offset 0 , 0
#    Given video display saves debug BMP images to leaf filename "tmp/frames/TC-5-"
    Given property "bdd6502.bus24.trace" is set to string "true"
    Given I disable trace
    Given I have a simple overclocked 6502 system
    When I enable uninitialised memory read protection with immediate fail
    * That does fail on BRK
    Given a user port to 24 bit bus is installed
#    And enable user port bus debug output
    And enable APU mode
    Given add a GetBackground layer fetching from layer index '1'
    And the layer has 16 colours
    And the layer has overscan
    Given add a Tiles layer with registers at '0x9e00' and screen addressEx '0x80' and planes addressEx '0x40'
    And the layer has 16 colours
    And the layer has overscan
    Given add a Chars V4.0 layer with registers at '0x9000' and screen addressEx '0x80' and planes addressEx '0x20'
    And the layer has 16 colours
    And the layer has overscan
    Given add a Sprites layer with registers at '0x9800' and addressEx '0x10'
    And the layer has 16 colours
    And the layer has overscan
    Given show video window

    # Instead of writing this data via the 6502 CPU, just send it straight to memory
    # Palette
    Given write data from file "tmp/RoadPaletteData.bin" to 24bit bus at '0x9c00' and addressEx '0x01'
    # Tiles
    Given write data from file "tmp/RoadTiles_map.bin" to 24bit bus at '0x2000' and addressEx '0x80'
    Given write data from file "tmp/RoadTiles_plane0.bin" to 24bit bus at '0x2000' and addressEx '0x40'
    Given write data from file "tmp/RoadTiles_plane1.bin" to 24bit bus at '0x4000' and addressEx '0x40'
    Given write data from file "tmp/RoadTiles_plane2.bin" to 24bit bus at '0x8000' and addressEx '0x40'
    Given write data from file "tmp/RoadTiles_plane3.bin" to 24bit bus at '0x0000' and addressEx '0x40'


    And I load prg "bin/main.prg"
    And I load labels "tmp/main.map"

#    And I enable trace with indent
    When I execute the procedure at start for no more than 100000000 instructions until PC = mainLoop
	Given render a video display frame

#    And I enable trace with indent

    # This allows the last frame to be observed and window zoomed/resized
#    When rendering the video until window closed

    # This allows code to be executed until the window is closed, with the option of saving debug BMP files
    Given I disable trace
    Given property "bdd6502.bus24.trace" is set to string "false"
    Given video display does not save debug BMP images
    Given video display processes 24 pixels per instruction
    Given limit video display to 60 fps
    When I execute the procedure at mainLoop until return




# Run with Convert6.bat
  @Demo6
  Scenario: Smoke test for Shadow of the Beast demo
    Given clear all external devices
    Given a new audio expansion
    And audio mix 85
    Given a new video display with overscan and 16 colours
    Given set the video display to RGB colour 5 6 5
    Given set the video display with 32 palette banks
    And enable video display bus debug output
#    And enable debug pixel picking
    Given video display processes 24 pixels per instruction
    Given video display refresh window every 32 instructions
    And audio refresh window every 0 instructions
    And audio refresh is independent
    Given video display add joystick to port 1
    Given video display add CIA1 timers with raster offset 0 , 0
#    Given video display saves debug BMP images to leaf filename "tmp/frames/TC-6-"
    Given property "bdd6502.bus24.trace" is set to string "true"
    Given I disable trace
    Given I have a simple overclocked 6502 system
    When I enable uninitialised memory read protection with immediate fail
    * That does fail on BRK
    Given a user port to 24 bit bus is installed
#    And enable user port bus debug output
    And enable APU mode
    And APU clock divider 1
    And APU memory clock divider 2
    # Layer 3
    Given add a Chars V4.0 layer with registers at '0x9000' and screen addressEx '0x80' and planes addressEx '0x20'
    And the layer has 16 colours
    And the layer has overscan
    # Layer 2
    Given add a Tiles layer with registers at '0x9e00' and screen addressEx '0x80' and planes addressEx '0x40'
    And the layer has 16 colours
    And the layer has overscan
    # Layer 1
    Given add a Sprites2 layer with registers at '0x9200' and addressEx '0x08' and running at 14.31818MHz
    And the layer has 16 colours
    And the layer has overscan
    # Layer 0
#    Given add a Sprites layer with registers at '0x9800' and addressEx '0x10'
    Given add a Sprites V9.5 layer with registers at '0x9800' and addressEx '0x10' and running at 16MHz
    And the layer has 16 colours
    And the layer has overscan
    Given show video window

    # Instead of writing this data via the 6502 CPU, just send it straight to memory
    # Palette
    Given write data from file "tmp/ShadowBeastPaletteData.bin" to 24bit bus at '0x9c00' and addressEx '0x01'
    # Sprites2
    Given write data from file "tmp/Demo6Sprites20.bin" to 24bit bus at '0x2000' and addressEx '0x08'
    Given write data from file "tmp/Demo6Sprites21.bin" to 24bit bus at '0x4000' and addressEx '0x08'
    Given write data from file "tmp/Demo6Sprites22.bin" to 24bit bus at '0x8000' and addressEx '0x08'
    Given write data from file "tmp/Demo6Sprites23.bin" to 24bit bus at '0x0000' and addressEx '0x08'
    # Sprites
    Given write data from file "tmp/ShadowBeastSprites_plane0.bin" to 24bit bus at '0x2000' and addressEx '0x10'
    Given write data from file "tmp/ShadowBeastSprites_plane1.bin" to 24bit bus at '0x4000' and addressEx '0x10'
    Given write data from file "tmp/ShadowBeastSprites_plane2.bin" to 24bit bus at '0x8000' and addressEx '0x10'
    Given write data from file "tmp/ShadowBeastSprites_plane3.bin" to 24bit bus at '0x0000' and addressEx '0x10'
    # Chars
    Given write data from file "tmp/ShadowBeastChars_map.bin" to 24bit bus at '0x4000' and addressEx '0x80'
    Given write data from file "tmp/ShadowBeastChars_map.bin2" to 24bit bus at '0x8000' and addressEx '0x80'
    Given write data from file "tmp/ShadowBeastChars_plane0.bin" to 24bit bus at '0x2000' and addressEx '0x20'
    Given write data from file "tmp/ShadowBeastChars_plane1.bin" to 24bit bus at '0x4000' and addressEx '0x20'
    Given write data from file "tmp/ShadowBeastChars_plane2.bin" to 24bit bus at '0x8000' and addressEx '0x20'
    Given write data from file "tmp/ShadowBeastChars_plane3.bin" to 24bit bus at '0x0000' and addressEx '0x20'
    # Tiles
    Given write data from file "tmp/ShadowBeastTiles_map.bin" to 24bit bus at '0x2000' and addressEx '0x80'
    Given write data from file "tmp/ShadowBeastTiles_plane0.bin" to 24bit bus at '0x2000' and addressEx '0x40'
    Given write data from file "tmp/ShadowBeastTiles_plane1.bin" to 24bit bus at '0x4000' and addressEx '0x40'
    Given write data from file "tmp/ShadowBeastTiles_plane2.bin" to 24bit bus at '0x8000' and addressEx '0x40'
    Given write data from file "tmp/ShadowBeastTiles_plane3.bin" to 24bit bus at '0x0000' and addressEx '0x40'
	#  Music
	Given write data from file "tmp/target/SotBexportedMusicSamples.bin" to 24bit bus at '0x0000' and addressEx '0x04'


    And I load prg "bin/main.prg"
    And I load labels "tmp/main.map"

    When enable remote debugging
#    And wait for debugger connection
#    And wait for debugger command

#    And I enable trace with indent
    When I execute the procedure at start for no more than 1000000 instructions until PC = mainLoop
	Given render a video display frame

#    And I enable trace with indent

    # This allows the last frame to be observed and window zoomed/resized
#    When rendering the video until window closed

    # This allows code to be executed until the window is closed, with the option of saving debug BMP files
    Given I disable trace
    Given property "bdd6502.bus24.trace" is set to string "false"
    Given video display does not save debug BMP images
    Given video display processes 24 pixels per instruction
    Given limit video display to 60 fps
    Given avoid CPU wait during VBlank for address "Video_WaitVBlank_startGuard"
    When I execute the procedure at mainLoop until return



# Run with Convert6.bat
  @Demo6B
  Scenario: Smoke test for Shadow of the Beast running demo
    Given clear all external devices
    Given a new audio expansion
    And audio mix 85
    Given a new video display with overscan and 16 colours
    Given set the video display to RGB colour 5 6 5
    Given set the video display with 32 palette banks
#	And enable video display bus debug output
    Given video display processes 24 pixels per instruction
    Given video display refresh window every 32 instructions
    And audio refresh window every 0 instructions
    And audio refresh is independent
    Given video display add joystick to port 1
    Given video display add CIA1 timers with raster offset 0 , 0
#    Given video display saves debug BMP images to leaf filename "tmp/frames/TC-6-"
    Given property "bdd6502.bus24.trace" is set to string "true"
    Given I disable trace
    Given I have a simple overclocked 6502 system
    When I enable uninitialised memory read protection with immediate fail
    * That does fail on BRK
    Given a user port to 24 bit bus is installed
#    And enable user port bus debug output
    And enable APU mode
    And APU clock divider 1
    And APU memory clock divider 2
    # Layer 3
    Given add a Chars V4.0 layer with registers at '0x9000' and screen addressEx '0x80' and planes addressEx '0x20'
    And the layer has 16 colours
    And the layer has overscan
    # Layer 2
    Given add a Tiles layer with registers at '0x9e00' and screen addressEx '0x80' and planes addressEx '0x40'
    And the layer has 16 colours
    And the layer has overscan
    # Layer 1
    Given add a Sprites2 layer with registers at '0x9200' and addressEx '0x08' and running at 14.31818MHz
    And the layer has 16 colours
    And the layer has overscan
    # Layer 0
#    Given add a Sprites layer with registers at '0x9800' and addressEx '0x10'
    Given add a Sprites V9.5 layer with registers at '0x9800' and addressEx '0x10' and running at 16MHz
    And the layer has 16 colours
    And the layer has overscan
    Given show video window

    # Instead of writing this data via the 6502 CPU, just send it straight to memory
    # Palette
    Given write data from file "tmp/ShadowBeastPaletteData.bin" to 24bit bus at '0x9c00' and addressEx '0x01'
    # Sprites
    Given write data from file "tmp/ShadowBeastSprites2_plane0.bin" to 24bit bus at '0x2000' and addressEx '0x10'
    Given write data from file "tmp/ShadowBeastSprites2_plane1.bin" to 24bit bus at '0x4000' and addressEx '0x10'
    Given write data from file "tmp/ShadowBeastSprites2_plane2.bin" to 24bit bus at '0x8000' and addressEx '0x10'
    Given write data from file "tmp/ShadowBeastSprites2_plane3.bin" to 24bit bus at '0x0000' and addressEx '0x10'
    # Chars
    Given write data from file "tmp/ShadowBeastChars2_map.bin" to 24bit bus at '0x4000' and addressEx '0x80'
    Given write data from file "tmp/ShadowBeastChars2_map.bin2" to 24bit bus at '0x8000' and addressEx '0x80'
    Given write data from file "tmp/ShadowBeastChars2_plane0.bin" to 24bit bus at '0x2000' and addressEx '0x20'
    Given write data from file "tmp/ShadowBeastChars2_plane1.bin" to 24bit bus at '0x4000' and addressEx '0x20'
    Given write data from file "tmp/ShadowBeastChars2_plane2.bin" to 24bit bus at '0x8000' and addressEx '0x20'
    Given write data from file "tmp/ShadowBeastChars2_plane3.bin" to 24bit bus at '0x0000' and addressEx '0x20'
    # Tiles
    Given write data from file "tmp/ShadowBeastTiles_map.bin" to 24bit bus at '0x2000' and addressEx '0x80'
    Given write data from file "tmp/ShadowBeastTiles_plane0.bin" to 24bit bus at '0x2000' and addressEx '0x40'
    Given write data from file "tmp/ShadowBeastTiles_plane1.bin" to 24bit bus at '0x4000' and addressEx '0x40'
    Given write data from file "tmp/ShadowBeastTiles_plane2.bin" to 24bit bus at '0x8000' and addressEx '0x40'
    Given write data from file "tmp/ShadowBeastTiles_plane3.bin" to 24bit bus at '0x0000' and addressEx '0x40'
	#  Music
	Given write data from file "tmp/target/SotBexportedMusic2Samples.bin" to 24bit bus at '0x0000' and addressEx '0x04'


    And I load prg "bin/main.prg"
    And I load labels "tmp/main.map"

#    And I enable trace with indent
    When I execute the procedure at start for no more than 1000000 instructions until PC = mainLoop
	Given render a video display frame

#    And I enable trace with indent

    # This allows the last frame to be observed and window zoomed/resized
#    When rendering the video until window closed

    # This allows code to be executed until the window is closed, with the option of saving debug BMP files
    Given I disable trace
    Given property "bdd6502.bus24.trace" is set to string "false"
    Given video display does not save debug BMP images
    Given video display processes 24 pixels per instruction
    Given limit video display to 60 fps
    When I execute the procedure at mainLoop until return

  @Demo6C
  Scenario: Smoke test for Shadow of the Beast title and running demo from cartridge
    Given clear all external devices
#    Given a new C64 video display
#    And show C64 video window
#    And force C64 displayed bank to 3
    Given a new audio expansion
    And audio mix 85
    Given a new video display with overscan and 16 colours
    Given set the video display to RGB colour 5 6 5
    Given set the video display with 32 palette banks
#	And enable video display bus debug output
#    And enable debug pixel picking
    Given video display processes 24 pixels per instruction
    Given video display refresh window every 32 instructions
    And audio refresh window every 0 instructions
    And audio refresh is independent
    Given video display add joystick to port 1
    Given video display add CIA1 timers with raster offset 0 , 0
#    Given video display saves debug BMP images to leaf filename "tmp/frames/TC-6C-"
    Given property "bdd6502.bus24.trace" is set to string "true"
    Given I disable trace
    Given I have a simple overclocked 6502 system
    Given I am using C64 processor port options
    Given a ROM from file "C:\VICE\C64\kernal" at $e000
    Given a ROM from file "C:\VICE\C64\basic" at $a000
    Given a CHARGEN ROM from file "C:\VICE\C64\chargen"
    Given add C64 hardware
#    When I enable uninitialised memory read protection with immediate fail
    * That does fail on BRK
    Given a user port to 24 bit bus is installed
#    And enable user port bus debug output
    And enable APU mode
    # Layer 3
    Given add a Chars V4.0 layer with registers at '0x9000' and screen addressEx '0x80' and planes addressEx '0x20'
    And the layer has 16 colours
    And the layer has overscan
    # Layer 2
    Given add a Tiles layer with registers at '0x9e00' and screen addressEx '0x80' and planes addressEx '0x40'
    And the layer has 16 colours
    And the layer has overscan
    # Layer 1
    Given add a Sprites2 layer with registers at '0x9200' and addressEx '0x08' and running at 14.31818MHz
    And the layer has 16 colours
    And the layer has overscan
    # Layer 0
#    Given add a Sprites layer with registers at '0x9800' and addressEx '0x10'
    Given add a Sprites V9.5 layer with registers at '0x9800' and addressEx '0x10' and running at 16MHz
    And the layer has 16 colours
    And the layer has overscan
    Given show video window
	Given randomly initialise all memory using seed 4321

    And I load crt "bin/main.crt"
    And I load labels "tmp/main.map"

    And I enable trace with indent

    When enable remote debugging
#    And wait for debugger connection
#    And wait for debugger command

    Given I disable trace
    Given property "bdd6502.bus24.trace" is set to string "false"
    Given video display does not save debug BMP images
    Given video display processes 24 pixels per instruction
    Given limit video display to 60 fps
    When I execute the indirect procedure at $8000 until return

	Given render a video display frame
    When rendering the video until window closed



  # Run with BuildIt7.bat
  @Demo7
  Scenario: Smoke test for just the video layer
    Given clear all external devices
    Given a new video display with overscan and 16 colours
    Given set the video display to RGB colour 5 6 5
    Given set the video display with 32 palette banks
#	And enable video display bus debug output
    Given video display processes 24 pixels per instruction
    Given video display refresh window every 32 instructions
    Given video display add joystick to port 1
    Given video display add joystick to port 2
    Given video display add CIA1 timers with raster offset 0 , 0
#    Given video display saves debug BMP images to leaf filename "tmp/frames/TC-3-"
#    Given property "bdd6502.bus24.trace" is set to string "true"
#    Given property "bdd6502.apu.trace" is set to string "true"
    Given I disable trace
    Given I have a simple overclocked 6502 system
    When I enable uninitialised memory read protection with immediate fail
    * That does fail on BRK
    Given a user port to 24 bit bus is installed

	# Some layers plugged in
#    Given add a Mode7 layer with registers at '0xa000' and addressEx '0x08'
#    Given add a GetBackground layer fetching from layer index '1'
    Given add a Chars V4.0 layer with registers at '0xa000' and screen addressEx '0x04' and planes addressEx '0x08'
    And the layer has 16 colours
    And the layer has overscan
    Given add a Tiles layer with registers at '0x9e00' and screen addressEx '0x80' and planes addressEx '0x40'
    And the layer has 16 colours
    And the layer has overscan
    Given add a Chars V4.0 layer with registers at '0x9000' and screen addressEx '0x80' and planes addressEx '0x20'
    And the layer has 16 colours
    And the layer has overscan
#    Given add a Sprites layer with registers at '0x9800' and addressEx '0x10'
    Given add a Sprites V9.5 layer with registers at '0x9800' and addressEx '0x10' and running at 16MHz
    And the layer has 16 colours
    And the layer has overscan

    # Sprites
    Given write data from file "tmp/Demo7Sprites_plane0.bin" to 24bit bus at '0x2000' and addressEx '0x10'
    Given write data from file "tmp/Demo7Sprites_plane1.bin" to 24bit bus at '0x4000' and addressEx '0x10'
    Given write data from file "tmp/Demo7Sprites_plane2.bin" to 24bit bus at '0x8000' and addressEx '0x10'
    Given write data from file "tmp/Demo7Sprites_plane3.bin" to 24bit bus at '0x0000' and addressEx '0x10'
    # Debug for making sprite boundaries more visible
#    Given write data from file "tmp/Demo7Chars_map.bin" to 24bit bus at '0x2000' and addressEx '0x10'
    # Chars1
    Given write data from file "tmp/Demo7Chars_map.bin" to 24bit bus at '0x4000' and addressEx '0x80'
    Given write data from file "tmp/Demo7Chars_map.bin2" to 24bit bus at '0x8000' and addressEx '0x80'
    Given write data from file "tmp/Demo7Chars_plane0.bin" to 24bit bus at '0x2000' and addressEx '0x20'
    Given write data from file "tmp/Demo7Chars_plane1.bin" to 24bit bus at '0x4000' and addressEx '0x20'
    Given write data from file "tmp/Demo7Chars_plane2.bin" to 24bit bus at '0x8000' and addressEx '0x20'
    Given write data from file "tmp/Demo7Chars_plane3.bin" to 24bit bus at '0x0000' and addressEx '0x20'
    # Chars2
    Given write data from file "tmp/Demo7Chars2_map.bin" to 24bit bus at '0x4000' and addressEx '0x04'
    Given write data from file "tmp/Demo7Chars2_map.bin2" to 24bit bus at '0x8000' and addressEx '0x04'
    Given write data from file "tmp/Demo7Chars2_plane0.bin" to 24bit bus at '0x2000' and addressEx '0x08'
    Given write data from file "tmp/Demo7Chars2_plane1.bin" to 24bit bus at '0x4000' and addressEx '0x08'
    Given write data from file "tmp/Demo7Chars2_plane2.bin" to 24bit bus at '0x8000' and addressEx '0x08'
    Given write data from file "tmp/Demo7Chars2_plane3.bin" to 24bit bus at '0x0000' and addressEx '0x08'
    # Tiles
    Given write data from file "tmp/Demo7Tiles_map.bin" to 24bit bus at '0x2000' and addressEx '0x80'
    Given write data from file "tmp/Demo7Tiles_plane0.bin" to 24bit bus at '0x2000' and addressEx '0x40'
    Given write data from file "tmp/Demo7Tiles_plane1.bin" to 24bit bus at '0x4000' and addressEx '0x40'
    Given write data from file "tmp/Demo7Tiles_plane2.bin" to 24bit bus at '0x8000' and addressEx '0x40'
    Given write data from file "tmp/Demo7Tiles_plane3.bin" to 24bit bus at '0x0000' and addressEx '0x40'



#    And enable user port bus debug output
    And enable APU mode
    Given show video window

    And I load prg "bin/main.prg"
    And I load labels "tmp/main.map"

    When enable remote debugging
#    And wait for debugger connection
#    And wait for debugger command

#    And I enable trace with indent
    When I execute the procedure at start for no more than 1000000 instructions until PC = mainLoop
	Given render a video display frame

#    And I enable trace with indent

    # This allows the last frame to be observed and window zoomed/resized
#    When rendering the video until window closed

    # This allows code to be executed until the window is closed, with the option of saving debug BMP files
    Given I disable trace
    Given property "bdd6502.bus24.trace" is set to string "false"
    Given video display does not save debug BMP images
    Given video display processes 24 pixels per instruction
    Given video display processes 8 pixels per instruction
    Given limit video display to 60 fps
    When I execute the procedure at mainLoop until return



# Run with Convert9.bat
  @Demo9
  Scenario: Smoke test for Demo9
    Given clear all external devices
    Given a new audio expansion
    And audio mix 85
    And the audio expansion uses exact address matching
    Given a new video display with overscan and 16 colours
    Given set the video display to RGB colour 5 6 5
    Given set the video display with 32 palette banks
    And the display uses exact address matching
#    And enable video display bus debug output
#    And enable debug pixel picking
    Given video display processes 24 pixels per instruction
    Given video display refresh window every 32 instructions
    And audio refresh window every 0 instructions
    And audio refresh is independent
    Given video display add joystick to port 1
    Given video display add CIA1 timers with raster offset 0 , 0
#    Given video display saves debug BMP images to leaf filename "tmp/frames/Demo9-"
    Given property "bdd6502.bus24.trace" is set to string "true"
    Given I enable trace
    Given I disable trace
    Given I have a simple overclocked 6502 system
    Given I am using C64 processor port options
    Given a ROM from file "C:\VICE\C64\kernal" at $e000
    Given a ROM from file "C:\VICE\C64\basic" at $a000
    Given add C64 hardware
    When I enable uninitialised memory read protection with immediate fail
    * That does fail on BRK
    Given a user port to 24 bit bus is installed
#    And enable user port bus debug output
    And enable APU mode
	And the APU uses exact address matching
	# Use with kJustForLogo instead of "Chars V4.0 layer"
#    Given add a StaticColour layer for palette index '0x01'
#    Given add a GetBackground layer fetching from layer index '1'
#    Given add a Mode7 layer with registers at '0xa000' and addressEx '0x08'
#    And the layer has 16 colours

    Given add a 2-to-1 merge layer with registers at '0xa200'
    And the layer uses exact address matching
    And the layer has 16 colours
    And the layer has overscan
      Given add a Chars V4.0 layer with registers at '0x9000' and screen addressEx '0x80' and planes addressEx '0x20'
      And the layer uses exact address matching
      And the layer has 16 colours
      And the layer has overscan
      Given add a Chars V4.0 layer with registers at '0xa800' and screen addressEx '0x90' and planes addressEx '0x30'
      And the layer uses exact address matching
      And the layer has 16 colours
      And the layer has overscan
    Given add a 2-to-1 merge layer with registers at '0xa202'
    And the layer uses exact address matching
    And the layer has 16 colours
    And the layer has overscan
      Given add a Tiles layer with registers at '0x9e00' and screen addressEx '0x80' and planes addressEx '0x40'
      And the layer uses exact address matching
      And the layer has 16 colours
      And the layer has overscan
      Given add a StaticColour layer for palette index '0xff'
      And the layer has 16 colours
      And the layer has overscan
    Given add a Sprites2 layer with registers at '0x9200' and addressEx '0x08' and running at 14.31818MHz
    And the layer has 16 colours
    And the layer has overscan
#    Given add a Sprites layer with registers at '0x9800' and addressEx '0x10'
    Given add a Sprites V9.5 layer with registers at '0x9800' and addressEx '0x10' and running at 16MHz
    And the layer uses exact address matching
    And the layer has 16 colours
    And the layer has overscan
    Given show video window
    Given randomly initialise all memory using seed 43211

    # Instead of writing this data via the 6502 CPU, just send it straight to memory
    # Palette
    Given write data from file "tmp/Demo9PaletteData.bin" to 24bit bus at '0x9c00' and addressEx '0x01'
    # Sprites1
    Given write data from file "tmp/Demo9Sprites10.bin" to 24bit bus at '0x2000' and addressEx '0x10'
    Given write data from file "tmp/Demo9Sprites11.bin" to 24bit bus at '0x4000' and addressEx '0x10'
    Given write data from file "tmp/Demo9Sprites12.bin" to 24bit bus at '0x8000' and addressEx '0x10'
    Given write data from file "tmp/Demo9Sprites13.bin" to 24bit bus at '0x0000' and addressEx '0x10'
    # Sprites2
    Given write data from file "tmp/Demo9Sprites20.bin" to 24bit bus at '0x2000' and addressEx '0x08'
    Given write data from file "tmp/Demo9Sprites21.bin" to 24bit bus at '0x4000' and addressEx '0x08'
    Given write data from file "tmp/Demo9Sprites22.bin" to 24bit bus at '0x8000' and addressEx '0x08'
    Given write data from file "tmp/Demo9Sprites23.bin" to 24bit bus at '0x0000' and addressEx '0x08'
    # Chars
    Given write data from file "tmp/Demo9Chars_map.bin" to 24bit bus at '0x4000' and addressEx '0x80'
    Given write data from file "tmp/Demo9Chars_map.bin2" to 24bit bus at '0x8000' and addressEx '0x80'
    Given write data from file "tmp/Demo9Chars_plane0.bin" to 24bit bus at '0x2000' and addressEx '0x20'
    Given write data from file "tmp/Demo9Chars_plane1.bin" to 24bit bus at '0x4000' and addressEx '0x20'
    Given write data from file "tmp/Demo9Chars_plane2.bin" to 24bit bus at '0x8000' and addressEx '0x20'
    Given write data from file "tmp/Demo9Chars_plane3.bin" to 24bit bus at '0x0000' and addressEx '0x20'
    # Chars2
    Given write data from file "tmp/Demo9Chars2_map.bin" to 24bit bus at '0x4000' and addressEx '0x90'
    Given write data from file "tmp/Demo9Chars2_map.bin2" to 24bit bus at '0x8000' and addressEx '0x90'
    Given write data from file "tmp/Demo9Chars2_plane0.bin" to 24bit bus at '0x2000' and addressEx '0x30'
    Given write data from file "tmp/Demo9Chars2_plane1.bin" to 24bit bus at '0x4000' and addressEx '0x30'
    Given write data from file "tmp/Demo9Chars2_plane2.bin" to 24bit bus at '0x8000' and addressEx '0x30'
    Given write data from file "tmp/Demo9Chars2_plane3.bin" to 24bit bus at '0x0000' and addressEx '0x30'
    # Tiles
    Given write data from file "tmp/Demo9Tiles_map.bin" to 24bit bus at '0x2000' and addressEx '0x80'
    Given write data from file "tmp/Demo9Tiles_plane0.bin" to 24bit bus at '0x2000' and addressEx '0x40'
    Given write data from file "tmp/Demo9Tiles_plane1.bin" to 24bit bus at '0x4000' and addressEx '0x40'
    Given write data from file "tmp/Demo9Tiles_plane2.bin" to 24bit bus at '0x8000' and addressEx '0x40'
    Given write data from file "tmp/Demo9Tiles_plane3.bin" to 24bit bus at '0x0000' and addressEx '0x40'
    # Music
    # Can be replaced with "MinimalMusicDemoTest = 1" to have a minimal music demo
    Given write data from file "tmp/target/MusicMW2000Samples.bin1" to 24bit bus at '0x0000' and addressEx '0x04'
    Given write data from file "tmp/target/MusicMW2000Samples.bin2" to 24bit bus at '0x8000' and addressEx '0x04'

    And I load labels "tmp/main.map"
    # Because the prg is very large we load in the RAM underneath the IO etc
    Then I write memory at ZPProcessorPort with ProcessorPortAllRAM
    And I load crt "bin/main.crt"
    And I load prg "bin/main.prg"

    When enable remote debugging
#    And wait for debugger connection
#    And wait for debugger command

#    And I enable trace with indent
#    When I execute the procedure at start for no more than 1000000 instructions until PC = mainLoop
#	Given render a video display frame
#    And I enable trace with indent

    # This allows the last frame to be observed and window zoomed/resized
#    When rendering the video until window closed

    # This allows code to be executed until the window is closed, with the option of saving debug BMP files
    Given I disable trace
    Given property "bdd6502.bus24.trace" is set to string "false"
    Given video display does not save debug BMP images
    # Different because SendAPUAPULineScrolls takes a lot of time
    Given video display processes 8 pixels per instruction
    Given limit video display to 60 fps
    Given avoid CPU wait during VBlank for address "Video_WaitVBlank_startGuard"
    When I execute the procedure at start until return



  # Run with BuildIt10.bat
  @Demo10
  Scenario: Smoke test for just the video layer
    Given clear all external devices
    Given a new video display with overscan and 16 colours
    Given set the video display to RGB colour 5 6 5
    Given set the video display with 32 palette banks
#	And enable video display bus debug output
    Given video display processes 24 pixels per instruction
    Given video display refresh window every 32 instructions
    Given video display add joystick to port 1
    Given video display add joystick to port 2
    Given video display add CIA1 timers with raster offset 0 , 0
#    Given video display saves debug BMP images to leaf filename "tmp/frames/TC-3-"
#    Given property "bdd6502.bus24.trace" is set to string "true"
#	Given property "bdd6502.apu.trace" is set to string "true"
    Given I disable trace
    Given I have a simple overclocked 6502 system
    When I enable uninitialised memory read protection with immediate fail
    * That does fail on BRK
    Given a user port to 24 bit bus is installed

    Given add a Tiles layer with registers at '0x9e00' and screen addressEx '0x80' and planes addressEx '0x40'
    And the layer has 16 colours
    And the layer has overscan
    Given add a Chars V4.0 layer with registers at '0x8800' and screen addressEx '0x02' and planes addressEx '0x04'
    And the layer has 16 colours
    And the layer has overscan
    Given add a 2-to-1 merge layer with registers at '0xa200'
    And the layer has 16 colours
    And the layer has overscan
		Given add a Sprites layer with registers at '0xa000' and addressEx '0x10'
		And the layer has 16 colours
		And the layer has overscan
		Given add a Sprites layer with registers at '0x9800' and addressEx '0x10'
		And the layer has 16 colours
		And the layer has overscan
    Given add a Chars V4.0 layer with registers at '0x9000' and screen addressEx '0x80' and planes addressEx '0x20'
    And the layer has 16 colours
    And the layer has overscan

    # Sprites
    Given write data from file "tmp/Demo10Sprites_plane0.bin" to 24bit bus at '0x2000' and addressEx '0x10'
    Given write data from file "tmp/Demo10Sprites_plane1.bin" to 24bit bus at '0x4000' and addressEx '0x10'
    Given write data from file "tmp/Demo10Sprites_plane2.bin" to 24bit bus at '0x8000' and addressEx '0x10'
    Given write data from file "tmp/Demo10Sprites_plane3.bin" to 24bit bus at '0x0000' and addressEx '0x10'
    # Chars1
    Given write data from file "tmp/Demo10Chars2_map.bin" to 24bit bus at '0x4000' and addressEx '0x80'
    Given write data from file "tmp/Demo10Chars2_map.bin2" to 24bit bus at '0x8000' and addressEx '0x80'
    Given write data from file "tmp/Demo10Chars2_plane0.bin" to 24bit bus at '0x2000' and addressEx '0x20'
    Given write data from file "tmp/Demo10Chars2_plane1.bin" to 24bit bus at '0x4000' and addressEx '0x20'
    Given write data from file "tmp/Demo10Chars2_plane2.bin" to 24bit bus at '0x8000' and addressEx '0x20'
    Given write data from file "tmp/Demo10Chars2_plane3.bin" to 24bit bus at '0x0000' and addressEx '0x20'
    # Chars2
    Given write data from file "tmp/Demo10Chars_map.bin" to 24bit bus at '0x4000' and addressEx '0x02'
    Given write data from file "tmp/Demo10Chars_map.bin2" to 24bit bus at '0x8000' and addressEx '0x02'
    Given write data from file "tmp/Demo10Chars_plane0.bin" to 24bit bus at '0x2000' and addressEx '0x04'
    Given write data from file "tmp/Demo10Chars_plane1.bin" to 24bit bus at '0x4000' and addressEx '0x04'
    Given write data from file "tmp/Demo10Chars_plane2.bin" to 24bit bus at '0x8000' and addressEx '0x04'
    Given write data from file "tmp/Demo10Chars_plane3.bin" to 24bit bus at '0x0000' and addressEx '0x04'
    # Tiles
    Given write data from file "tmp/Demo10Tiles_map.bin" to 24bit bus at '0x2000' and addressEx '0x80'
    Given write data from file "tmp/Demo10Tiles_plane0.bin" to 24bit bus at '0x2000' and addressEx '0x40'
    Given write data from file "tmp/Demo10Tiles_plane1.bin" to 24bit bus at '0x4000' and addressEx '0x40'
    Given write data from file "tmp/Demo10Tiles_plane2.bin" to 24bit bus at '0x8000' and addressEx '0x40'
    Given write data from file "tmp/Demo10Tiles_plane3.bin" to 24bit bus at '0x0000' and addressEx '0x40'



#    And enable user port bus debug output
#    And enable APU mode
    Given show video window

    And I load prg "bin/main.prg"
    And I load labels "tmp/main.map"

#    When enable remote debugging
#    And wait for debugger connection
#    And wait for debugger command

#    And I enable trace with indent
    When I execute the procedure at start for no more than 1000000 instructions until PC = mainLoop
	Given render a video display frame

#    And I enable trace with indent

    # This allows the last frame to be observed and window zoomed/resized
#    When rendering the video until window closed

    # This allows code to be executed until the window is closed, with the option of saving debug BMP files
    Given I disable trace
    Given property "bdd6502.bus24.trace" is set to string "false"
    Given video display does not save debug BMP images
    Given video display processes 24 pixels per instruction
    Given limit video display to 60 fps
    When I execute the procedure at mainLoop until return




# Run with Convert11.bat
  @Demo11
  Scenario: Smoke test for Demo11
    Given clear all external devices
	Given a new C64 video display
    And show C64 video window
    And force C64 displayed bank to 3
	Given add C64 display window to C64 keyboard buffer hook
    Given a new audio expansion
    And audio mix 85
    Given a new video display with overscan and 16 colours
    Given set the video display to RGB colour 5 6 5
    Given set the video display with 32 palette banks
#	And enable video display bus debug output
    Given video display processes 24 pixels per instruction
    Given video display refresh window every 32 instructions
    And audio refresh window every 0 instructions
    And audio refresh is independent
    Given video display add joystick to port 1
    Given video display add CIA1 timers with raster offset 0 , 0
#    Given video display saves debug BMP images to leaf filename "tmp/frames/Demo11-"
    Given property "bdd6502.bus24.trace" is set to string "true"
    Given I enable trace
    Given I have a simple overclocked 6502 system
	Given I am using C64 processor port options
    Given a ROM from file "C:\VICE\C64\kernal" at $e000
    Given a ROM from file "C:\VICE\C64\basic" at $a000
	Given a CHARGEN ROM from file "C:\VICE\C64\chargen"
    Given add C64 hardware
    When I enable uninitialised memory read protection with immediate fail
    * That does fail on BRK
    Given a user port to 24 bit bus is installed
#    And enable user port bus debug output
#    And enable APU mode
#    Given add a StaticColour layer for palette index '0x7f'
#    Given add a GetBackground layer fetching from layer index '1'
#    Given add a Mode7 layer with registers at '0xa000' and addressEx '0x08'
#    And the layer has 16 colours
    Given add a Chars V4.0 layer with registers at '0x9000' and screen addressEx '0x80' and planes addressEx '0x20'
    And the layer has 16 colours
    And the layer has overscan
    Given add a Tiles layer with registers at '0x9e00' and screen addressEx '0x80' and planes addressEx '0x40'
    And the layer has 16 colours
    And the layer has overscan
    Given add a Sprites2 layer with registers at '0x9200' and addressEx '0x08' and running at 14.31818MHz
    And the layer has 16 colours
    And the layer has overscan
    Given add a Sprites layer with registers at '0x9800' and addressEx '0x10'
    And the layer has 16 colours
    And the layer has overscan

    # Send any graphics data
    Given write data from file "C:\VICE\C64\chargen" to 24bit bus at '0x2000' and addressEx '0x20'
    Given write data from file "C:\VICE\C64\chargen" to 24bit bus at '0x4000' and addressEx '0x20'
    Given write data from file "C:\VICE\C64\chargen" to 24bit bus at '0x8000' and addressEx '0x20'
    Given write data from file "C:\VICE\C64\chargen" to 24bit bus at '0x0000' and addressEx '0x20'


    Given show video window


    And I load prg "bin/main.prg"
    And I load labels "tmp/main.map"

    When enable remote debugging
#    And wait for debugger connection
#    And wait for debugger command

    # This allows code to be executed until the window is closed, with the option of saving debug BMP files
    Given I disable trace
    Given property "bdd6502.bus24.trace" is set to string "false"
    Given video display does not save debug BMP images
    Given video display processes 24 pixels per instruction
    Given limit video display to 60 fps
    Given add C64 regular IRQ trigger of "100000" cycles
    When I execute the procedure at start for no more than 0 instructions until PC = $E5CF

#    Given property "bdd6502.bus24.trace" is set to string "true"
#    Given I enable trace
    When I hex dump memory between $400 and $800

    # Debug: To just run the C64 kernal without any other update code
    Then I continue executing the procedure until return





# Run with Convert12.bat
  @Demo12
  Scenario: Smoke test for Demo12
    Given clear all external devices
    Given a new audio expansion
    And audio mix 85
    Given a new video display with overscan and 16 colours
    Given set the video display to RGB colour 5 6 5
    Given set the video display with 32 palette banks
#	And enable video display bus debug output
#    And enable debug pixel picking
    # Segments_initListHeaders completes at: 22
#    Given video display processes 24 pixels per instruction
#    Given video display refresh window every 32 instructions
    # Segments_initListHeaders completes at: f8	(which is faster, like an overclocked 6502)
    Given video display processes 3 pixels per instruction
    Given video display refresh window every 32 instructions
    And audio refresh window every 0 instructions
    And audio refresh is independent
    Given video display add joystick to port 1
    Given video display add CIA1 timers with raster offset 0 , 0
#    Given video display saves debug BMP images to leaf filename "tmp/frames/Demo12-"
    Given property "bdd6502.bus24.trace" is set to string "true"
    Given I enable trace
    Given I disable trace
    Given I have a simple overclocked 6502 system
    When I enable uninitialised memory read protection with immediate fail
    * That does fail on BRK
    Given a user port to 24 bit bus is installed
#    And enable user port bus debug output
    And enable APU mode
    And APU clock divider 1
    And APU memory clock divider 2

	# Use with kJustForLogo instead of "Chars V4.0 layer"
#    Given add a StaticColour layer for palette index '0x01'
#    Given add a GetBackground layer fetching from layer index '1'
#    Given add a Mode7 layer with registers at '0xa000' and addressEx '0x08'
#    And the layer has 16 colours

    # Layer 3
    Given add a Chars V4.0 layer with registers at '0x9000' and screen addressEx '0x80' and planes addressEx '0x20'
    And the layer has 16 colours
    And the layer has overscan
    # Layer 2
    Given add a Tiles layer with registers at '0x9e00' and screen addressEx '0x80' and planes addressEx '0x40'
    And the layer has 16 colours
    And the layer has overscan
    # Layer 1
    Given add a 2-to-1 merge layer with registers at '0xa200'
    And the layer has 16 colours
    And the layer has overscan
      Given add a Sprites2 layer with registers at '0x9200' and addressEx '0x08' and running at 14.31818MHz
      And the layer has 16 colours
      And the layer has overscan
      Given add a Sprites layer with registers at '0x9800' and addressEx '0x10'
      And the layer has 16 colours
      And the layer has overscan
    # Layer 0
    Given add a Vector layer with registers at '0xa100' and addressEx '0x02'
    And the layer has 16 colours
    And the layer has overscan


    Given show video window
	Given randomly initialise all memory using seed 4321


    Given write data from file "tmp/Demo9PaletteData.bin" to 24bit bus at '0x9c00' and addressEx '0x01'


    # Chars
    # oldbridge char screen with rgbfactor 512
    Given write data from file "tmp/Demo9Chars_plane0.bin" to 24bit bus at '0x2000' and addressEx '0x20'
    Given write data from file "tmp/Demo9Chars_plane1.bin" to 24bit bus at '0x4000' and addressEx '0x20'
    Given write data from file "tmp/Demo9Chars_plane2.bin" to 24bit bus at '0x8000' and addressEx '0x20'
    Given write data from file "tmp/Demo9Chars_plane3.bin" to 24bit bus at '0x0000' and addressEx '0x20'
    # Chars screen
    Given write data from file "tmp/Demo9Chars_map.bin" to 24bit bus at '0x4000' and addressEx '0x80'
    Given write data from file "tmp/Demo9Chars_map.bin2" to 24bit bus at '0x8000' and addressEx '0x80'


    And I load prg "bin/main.prg"
    And I load labels "tmp/main.map"

    When enable remote debugging
#    And wait for debugger connection
#    And wait for debugger command

#    And I enable trace with indent
#    When I execute the procedure at start for no more than 1000000 instructions until PC = mainLoop
#	Given render a video display frame
#    And I enable trace with indent

    # This allows the last frame to be observed and window zoomed/resized
#    When rendering the video until window closed

    # This allows code to be executed until the window is closed, with the option of saving debug BMP files
    Given I disable trace
    Given property "bdd6502.bus24.trace" is set to string "false"
    Given video display does not save debug BMP images
#    Given video display processes 24 pixels per instruction
    Given limit video display to 60 fps
    When I execute the procedure at start until return





  @Demo13
  Scenario: Test chars image conversion output
    Given clear all external devices
    Given a new video display with overscan and 16 colours
    Given set the video display to RGB colour 5 6 5
    Given set the video display with 32 palette banks
    And enable video display bus debug output
    Given a new audio expansion
    And audio mix 85
    Given video display processes 8 pixels per instruction
    Given video display refresh window every 32 instructions
    Given video display does not save debug BMP images
    Given video display add joystick to port 1
    Given video display saves debug BMP images to leaf filename "tmp/frames/TC-16-"
    Given property "bdd6502.bus24.trace" is set to string "true"
    Given I have a simple overclocked 6502 system
    And That does fail on BRK
    And I enable uninitialised memory read protection with immediate fail
    Given a user port to 24 bit bus is installed
    And enable user port bus debug output
    And enable APU mode
    And APU clock divider 1
    And APU memory clock divider 2


    # Layer 3
    Given add a Chars V4.0 layer with registers at '0x9000' and screen addressEx '0x80' and planes addressEx '0x20'
    And the layer has 16 colours
    And the layer has overscan
    # Layer 2
    Given add a Tiles layer with registers at '0x9e00' and screen addressEx '0x80' and planes addressEx '0x40'
    And the layer has 16 colours
    And the layer has overscan
    # Layer 1
    Given add a Sprites2 layer with registers at '0x9200' and addressEx '0x08' and running at 14.31818MHz
    And the layer has 16 colours
    And the layer has overscan
    # Layer 0
    Given add a Sprites layer with registers at '0x9800' and addressEx '0x10'
    And the layer has 16 colours
    And the layer has overscan

    Given show video window
    Given randomly initialise all memory using seed 4321
    # Disable display
    Given write data byte '0x00' to 24bit bus at '0x9e00' and addressEx '0x01'

    And I load prg "bin/main.prg"
    And I load labels "tmp/main.map"


#    Then expect image "testdata/TC-16-000000.bmp" to be identical to "tmp/frames/TC-16-000000.bmp"
#    Then expect image "testdata/TC-16-000001.bmp" to be identical to "tmp/frames/TC-16-000001.bmp"
#    Then expect image "testdata/TC-16-000002.bmp" to be identical to "tmp/frames/TC-16-000002.bmp"
#    Then expect image "testdata/TC-16-000003.bmp" to be identical to "tmp/frames/TC-16-000003.bmp"


    When enable remote debugging
#    And wait for debugger connection
#    And wait for debugger command

#    And I enable trace with indent

    # This allows the last frame to be observed and window zoomed/resized
#    When rendering the video until window closed

    # This allows code to be executed until the window is closed, with the option of saving debug BMP files
    Given I disable trace
    Given property "bdd6502.bus24.trace" is set to string "false"
    Given video display does not save debug BMP images
    Given video display processes 24 pixels per instruction
    Given limit video display to 60 fps
    Given avoid CPU wait during VBlank for address "Video_WaitVBlank_startGuard"
    When I execute the procedure at start until return




  @Demo14
  Scenario: Test scaled sprites 4
    Given clear all external devices
#    Given a new C64 video display
#    And show C64 video window
#    And force C64 displayed bank to 3
    Given a new video display with overscan and 16 colours
    Given set the video display to RGB colour 5 6 5
    Given set the video display with 32 palette banks
    And the display uses exact address matching
    Given the display has palette layer expansion
    And enable video display bus debug output
#    Given a new audio expansion with registers at '0x8000' and addressEx '0x40'
    Given a new audio expansion
    And audio mix 85
    Given video display processes 8 pixels per instruction
    Given video display refresh window every 32 instructions
    Given video display does not save debug BMP images
    Given video display add joystick to port 1
    And video display add space as joystick port 2
    Given video display saves debug BMP images to leaf filename "tmp/frames/TC-17-"
    Given property "bdd6502.bus24.trace" is set to string "true"
    Given I have a simple overclocked 6502 system
    And I load prg "bin/main.prg"
    And I load labels "tmp/main.map"
    Given I am using C64 processor port options
    Given a ROM from file "C:\VICE\C64\kernal" at $e000
    Given a ROM from file "C:\VICE\C64\basic" at $a000
    Given a CHARGEN ROM from file "C:\VICE\C64\chargen"
    Given add C64 hardware
    And That does fail on BRK
    And I enable uninitialised memory read protection with immediate fail
#    Given a user port to 24 bit bus is installed
    Given a user port to 32 bit interface running at 4.0MHz and 24 bit bus is installed
    And add to the 32 bit interface a bank of memory at address '0x0' and size '0x100000'
    And add to the 32 bit interface a bank of memory at address '0x100000' and size '0x100000'
    Given load binary file "tmp\Demo14FinalData.bin" into temporary memory
    And trim "0" bytes from the start of temporary memory
    And add temporary memory to the 32 bit interface memory address '0x0'
	# Debug: Simulate a memory checksum failure
#    And trim "200000" bytes from the start of temporary memory
#    And add temporary memory to the 32 bit interface memory address '0x20050'
	# Comment out the above after debugging checksum failure
    And enable user port bus debug output
    And enable APU mode
    And APU clock divider 1
    And APU memory clock divider 2


    # Layer 3
    Given add a Mode7 layer with registers at '0xa000' and addressEx '0x08'
    And the layer has 16 colours
    And the layer has overscan
    And the layer uses exact address matching
    # Layer 2
    Given add a Tiles layer with registers at '0x9e00' and screen addressEx '0x80' and planes addressEx '0x40'
    And the layer has 16 colours
    And the layer has overscan
    And the layer uses exact address matching
    # Layer 1
    Given add a Chars V4.0 layer with registers at '0x9000' and screen addressEx '0x80' and planes addressEx '0x20'
    And the layer has 16 colours
    And the layer has overscan
    And the layer uses exact address matching
    # Layer 0
    # Note: If comparing simulated output with emulated output, some layers need a merge layer
#    Given add a 2-to-1 merge layer with registers at '0xa200'
#    And the layer has 16 colours
#    And the layer has overscan
    # Layer 0-1
#    Given add a Sprites4 layer with registers at '0x8800' and addressEx '0x05' and running at 16MHz
#    Given add a Sprites4 layer with registers at '0x8800' and addressEx '0x05' and running at 14.31818MHz
#    Given add a Sprites4 layer with registers at '0x8800' and addressEx '0x05' and running at 13.7MHz
    Given add a Sprites4 layer with registers at '0x8800' and addressEx '0x05' and running at 12.096MHz
    And the layer has 16 colours
    And the layer has overscan
    And the layer uses exact address matching
#    # Layer 0-0
#    Given add a Sprites V9.5 layer with registers at '0x9800' and addressEx '0x10' and running at 16MHz
#    And the layer has 16 colours
#    And the layer has overscan
#    And the layer uses exact address matching
    Given show video window
#    Given randomly initialise all memory using seed 4321

    # Sprites4
    # Exact address first
	Given write data byte '0x00' to 24bit bus at '0x8807' and addressEx '0x01'
    Given write data from file "tmp\ScaledSprites4.bin" to 24bit bus at '0x0000' and addressEx '0x05'
	Given write data byte '0x01' to 24bit bus at '0x8807' and addressEx '0x01'
    Given write data from file "tmp\ScaledSprites4.bin1" to 24bit bus at '0x0000' and addressEx '0x05'
	Given write data byte '0x02' to 24bit bus at '0x8807' and addressEx '0x01'
    Given write data from file "tmp\ScaledSprites4.bin2" to 24bit bus at '0x0000' and addressEx '0x05'
	Given write data byte '0x03' to 24bit bus at '0x8807' and addressEx '0x01'
    Given write data from file "tmp\ScaledSprites4.bin3" to 24bit bus at '0x0000' and addressEx '0x05'
	Given write data byte '0x04' to 24bit bus at '0x8807' and addressEx '0x01'
    Given write data from file "tmp\ScaledSprites4.bin4" to 24bit bus at '0x0000' and addressEx '0x05'
	Given write data byte '0x05' to 24bit bus at '0x8807' and addressEx '0x01'
    Given write data from file "tmp\ScaledSprites4.bin5" to 24bit bus at '0x0000' and addressEx '0x05'
	Given write data byte '0x06' to 24bit bus at '0x8807' and addressEx '0x01'
    Given write data from file "tmp\ScaledSprites4.bin6" to 24bit bus at '0x0000' and addressEx '0x05'

	#  Music
    # rem Then non-exact audio hardware using the EBBS
	Given write data from file "tmp/target/exportedSoundEffectsAfterBurnerSamples.bin" to 24bit bus at '0x0000' and addressEx '0x04'

    # Init combiners, for hardware simulation compatibility
    Given write data byte '0x60' to 24bit bus at '0xa200' and addressEx '0x01'
    Given write data byte '0x00' to 24bit bus at '0xa201' and addressEx '0x01'
    Given write data byte '0x20' to 24bit bus at '0xa202' and addressEx '0x01'
    Given write data byte '0x00' to 24bit bus at '0xa203' and addressEx '0x01'

    # Palettes
    Given write data byte '0x00' to 24bit bus at '0x9e0c' and addressEx '0x01'
    Given write data from file "tmp\Demo14ScaledSprites4TitleScreen.pal" to 24bit bus at '0x9c00' and addressEx '0x01'
    Given write data byte '0x01' to 24bit bus at '0x9e0c' and addressEx '0x01'
    Given write data from file "tmp\Demo14ScaledSprites4TitleScreen.pal" to 24bit bus at '0x9c00' and addressEx '0x01'
    Given write data byte '0x02' to 24bit bus at '0x9e0c' and addressEx '0x01'
    Given write data from file "tmp\Demo14ScaledSprites4TitleScreen.pal" to 24bit bus at '0x9c00' and addressEx '0x01'
    Given write data byte '0x03' to 24bit bus at '0x9e0c' and addressEx '0x01'
    Given write data from file "tmp\Demo14ScaledSprites4TitleScreen.pal" to 24bit bus at '0x9c00' and addressEx '0x01'
    Given write data byte '0x04' to 24bit bus at '0x9e0c' and addressEx '0x01'
    Given write data from file "tmp\Demo14ScaledSprites4TitleScreen.pal" to 24bit bus at '0x9c00' and addressEx '0x01'
    Given write data byte '0x0f' to 24bit bus at '0x9e0c' and addressEx '0x01'
    Given write data from file "tmp\Demo14ScaledSprites4TitleScreen.pal" to 24bit bus at '0x9c00' and addressEx '0x01'

    # Sprite palettes: Video_SetAddressVideoPaletteLayersRegister
    Given write data byte '0x10' to 24bit bus at '0x9e0c' and addressEx '0x01'
    Given write data from file "tmp\Demo14ScaledSprites4Game0.pal" to 24bit bus at '0x9c00' and addressEx '0x01'
    Given write data byte '0x11' to 24bit bus at '0x9e0c' and addressEx '0x01'
    Given write data from file "tmp\Demo14ScaledSprites4Game1.pal" to 24bit bus at '0x9c00' and addressEx '0x01'
    Given write data byte '0x12' to 24bit bus at '0x9e0c' and addressEx '0x01'
    Given write data from file "tmp\Demo14ScaledSprites4Game2.pal" to 24bit bus at '0x9c00' and addressEx '0x01'
    Given write data byte '0x13' to 24bit bus at '0x9e0c' and addressEx '0x01'
    Given write data from file "tmp\Demo14ScaledSprites4Game3.pal" to 24bit bus at '0x9c00' and addressEx '0x01'
    Given write data byte '0x14' to 24bit bus at '0x9e0c' and addressEx '0x01'
    Given write data from file "tmp\Demo14ScaledSprites4Game4.pal" to 24bit bus at '0x9c00' and addressEx '0x01'
    Given write data byte '0x1f' to 24bit bus at '0x9e0c' and addressEx '0x01'
    Given write data from file "tmp\Demo14ScaledSprites4TitleScreenSprites.pal" to 24bit bus at '0x9c00' and addressEx '0x01'


    # Chars
    Given write data from file "tmp/Demo14TitleChars_map.bin" to 24bit bus at '0x4000' and addressEx '0x80'
    Given write data from file "tmp/Demo14TitleChars_map.bin2" to 24bit bus at '0x8000' and addressEx '0x80'
    Given write data from file "tmp/Demo14TitleChars_plane0.bin" to 24bit bus at '0x2000' and addressEx '0x20'
    Given write data from file "tmp/Demo14TitleChars_plane1.bin" to 24bit bus at '0x4000' and addressEx '0x20'
    Given write data from file "tmp/Demo14TitleChars_plane2.bin" to 24bit bus at '0x8000' and addressEx '0x20'
    Given write data from file "tmp/Demo14TitleChars_plane3.bin" to 24bit bus at '0x0000' and addressEx '0x20'

    # Tiles
    Given write data from file "tmp/Demo14Runway_map.bin" to 24bit bus at '0x2000' and addressEx '0x80'
    Given write data from file "tmp/Demo14Runway_plane0.bin" to 24bit bus at '0x2000' and addressEx '0x40'
    Given write data from file "tmp/Demo14Runway_plane1.bin" to 24bit bus at '0x4000' and addressEx '0x40'
    Given write data from file "tmp/Demo14Runway_plane2.bin" to 24bit bus at '0x8000' and addressEx '0x40'
    Given write data from file "tmp/Demo14Runway_plane3.bin" to 24bit bus at '0x0000' and addressEx '0x40'

    # Mode7
    Given write data from file "tmp/Demo14Clouds.bin" to 24bit bus at '0x2000' and addressEx '0x08'
    Given write data from file "tmp/Demo14CloudsTiles.bin" to 24bit bus at '0x4000' and addressEx '0x08'
#    Given write data from file "tmp/Demo14CloudsTiles.bin2" to 24bit bus at '0x8000' and addressEx '0x08'

    # Mode7 registers
    Given write data byte '0x01' to 24bit bus at '0xa001' and addressEx '0x01'

    Given write data byte '0xb5' to 24bit bus at '0xa003' and addressEx '0x01'

    Given write data byte '0x01' to 24bit bus at '0xa007' and addressEx '0x01'

    Given write data byte '0x4b' to 24bit bus at '0xa009' and addressEx '0x01'
    Given write data byte '0xff' to 24bit bus at '0xa00a' and addressEx '0x01'
    Given write data byte '0xff' to 24bit bus at '0xa00b' and addressEx '0x01'

    Given write data byte '0x21' to 24bit bus at '0xa00d' and addressEx '0x01'
    Given write data byte '0x61' to 24bit bus at '0xa010' and addressEx '0x01'

    Given write data byte '0x00' to 24bit bus at '0xa014' and addressEx '0x01'
    Given write data byte '0x1f' to 24bit bus at '0xa015' and addressEx '0x01'


    When enable remote debugging
#    And wait for debugger connection
#    And wait for debugger command

    And I enable trace with indent

    # This allows the last frame to be observed and window zoomed/resized
#    When rendering the video until window closed

    # This allows code to be executed until the window is closed, with the option of saving debug BMP files
    Given I disable trace
    Given property "bdd6502.bus24.trace" is set to string "false"
    Given video display does not save debug BMP images
    Given video display processes 24 pixels per instruction
#    Given video display processes 16 pixels per instruction
#    Given video display processes 8 pixels per instruction
    Given limit video display to 60 fps
#    Given avoid CPU wait during VBlank for address "Video_WaitVBlank_startGuard"
    When I execute the procedure at start until return
