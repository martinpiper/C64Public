@smoketest
Feature: Smoke test

  Quickly load the output and check various simple aspects of the code

  # Run with Convert.bat
  @Demo1
  Scenario: Smoke test for video
    Given clear all external devices
    Given a new audio expansion
    Given a new video display with overscan and 16 colours
	And enable video display bus debug output
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
    Given add a Sprites layer with registers at '0x9800' and addressEx '0x10'
    And the layer has 16 colours
    And the layer has overscan

    Given show video window

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
    When I execute the procedure at setupFrame for no more than 1000000 instructions
    When I execute the procedure at setupFrame for no more than 1000000 instructions
    When I execute the procedure at setupFrame for no more than 1000000 instructions
    When I execute the procedure at setupFrame for no more than 1000000 instructions
    When I execute the procedure at setupFrame for no more than 1000000 instructions
    When I execute the procedure at setupFrame for no more than 1000000 instructions
    When I execute the procedure at setupFrame for no more than 1000000 instructions
    When I execute the procedure at setupFrame for no more than 1000000 instructions

#    Then expect image "testdata/TC-1-000001.bmp" to be identical to "tmp/frames/TC-1-000001.bmp"
#    Then expect image "testdata/TC-1-000002.bmp" to be identical to "tmp/frames/TC-1-000002.bmp"
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
    Given a new video display with 16 colours
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




# Run with Convert4_8.bat
  @Demo4_8
  Scenario: Smoke test for Turrican demo with 8 colours
    Given clear all external devices
    Given a new video display
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
#    Given add a StaticColour layer for palette index '0x7f'
    Given add a GetBackground layer fetching from layer index '1'
    Given add a Tiles layer with registers at '0x9e00' and screen addressEx '0x80' and planes addressEx '0x40'
    Given add a Chars layer with registers at '0x9000' and addressEx '0x20'
    Given add a Sprites layer with registers at '0x9800' and addressEx '0x10'
    Given show video window

    # Instead of writing this data via the 6502 CPU, just send it straight to memory
    # Palette
#    Given write data from file "tmp/TurricanPaletteData.bin" to 24bit bus at '0x9c00' and addressEx '0x01'
    # Sprites
#    Given write data from file "tmp/TurricanSprites_plane0.bin" to 24bit bus at '0x2000' and addressEx '0x10'
#    Given write data from file "tmp/TurricanSprites_plane1.bin" to 24bit bus at '0x4000' and addressEx '0x10'
#    Given write data from file "tmp/TurricanSprites_plane2.bin" to 24bit bus at '0x8000' and addressEx '0x10'
    # Tiles
#    Given write data from file "tmp/TurricanTiles_map.bin" to 24bit bus at '0x2000' and addressEx '0x80'
#    Given write data from file "tmp/TurricanTiles_plane0.bin" to 24bit bus at '0x2000' and addressEx '0x40'
#    Given write data from file "tmp/TurricanTiles_plane1.bin" to 24bit bus at '0x4000' and addressEx '0x40'
#    Given write data from file "tmp/TurricanTiles_plane2.bin" to 24bit bus at '0x8000' and addressEx '0x40'
    # Chars
#    Given write data from file "tmp/TurricanStatus_map.bin" to 24bit bus at '0x9000' and addressEx '0x01'
#    Given write data from file "tmp/TurricanStatus_plane0.bin" to 24bit bus at '0x2000' and addressEx '0x20'
#    Given write data from file "tmp/TurricanStatus_plane1.bin" to 24bit bus at '0x4000' and addressEx '0x20'
#    Given write data from file "tmp/TurricanStatus_plane2.bin" to 24bit bus at '0x8000' and addressEx '0x20'


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
    Given a new video display with 16 colours
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
#    Given add a StaticColour layer for palette index '0x7f'
    Given add a GetBackground layer fetching from layer index '1'
#    Given add a Mode7 layer with registers at '0xa000' and addressEx '0x08'
    Given add a Tiles layer with registers at '0x9e00' and screen addressEx '0x80' and planes addressEx '0x40'
    And the layer has 16 colours
    Given add a Chars V4.0 layer with registers at '0x9000' and screen addressEx '0x80' and planes addressEx '0x20'
    And the layer has 16 colours
    Given add a Sprites layer with registers at '0x9800' and addressEx '0x10'
    And the layer has 16 colours
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




# Run with Convert6.bat
  @Demo6
  Scenario: Smoke test for Shadow of the Beast demo
    Given clear all external devices
    Given a new audio expansion
    Given a new video display with overscan and 16 colours
#	And enable video display bus debug output
    Given video display processes 24 pixels per instruction
    Given video display refresh window every 32 instructions
    And audio refresh window every 0 instructions
    And audio refresh is independent
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
#    Given add a StaticColour layer for palette index '0x7f'
    Given add a GetBackground layer fetching from layer index '1'
    And the layer has 16 colours
    And the layer has overscan
#    Given add a Mode7 layer with registers at '0xa000' and addressEx '0x08'
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
    Given write data from file "tmp/ShadowBeastPaletteData.bin" to 24bit bus at '0x9c00' and addressEx '0x01'
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
    When I execute the procedure at mainLoop until return



# Run with Convert6.bat
  @Demo6B
  Scenario: Smoke test for Shadow of the Beast running demo
    Given clear all external devices
    Given a new audio expansion
    Given a new video display with overscan and 16 colours
#	And enable video display bus debug output
    Given video display processes 24 pixels per instruction
    Given video display refresh window every 32 instructions
    And audio refresh window every 0 instructions
    And audio refresh is independent
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
#    Given add a StaticColour layer for palette index '0x7f'
    Given add a GetBackground layer fetching from layer index '1'
    And the layer has 16 colours
    And the layer has overscan
#    Given add a Mode7 layer with registers at '0xa000' and addressEx '0x08'
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



  # Run with BuildIt7.bat
  @Demo7
  Scenario: Smoke test for just the video layer
    Given clear all external devices
    Given a new video display with overscan and 16 colours
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
    Given add a Sprites layer with registers at '0x9800' and addressEx '0x10'
    And the layer has 16 colours
    And the layer has overscan

    # Sprites
    Given write data from file "tmp/Demo7Sprites_plane0.bin" to 24bit bus at '0x2000' and addressEx '0x10'
    Given write data from file "tmp/Demo7Sprites_plane1.bin" to 24bit bus at '0x4000' and addressEx '0x10'
    Given write data from file "tmp/Demo7Sprites_plane2.bin" to 24bit bus at '0x8000' and addressEx '0x10'
    Given write data from file "tmp/Demo7Sprites_plane3.bin" to 24bit bus at '0x0000' and addressEx '0x10'
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



# Run with Convert9.bat
  @Demo9
  Scenario: Smoke test for Demo9
    Given clear all external devices
    Given a new audio expansion
    Given a new video display with overscan and 16 colours
#	And enable video display bus debug output
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
    When I enable uninitialised memory read protection with immediate fail
    * That does fail on BRK
    Given a user port to 24 bit bus is installed
#    And enable user port bus debug output
#    And enable APU mode
	# Use with kJustForLogo instead of "Chars V4.0 layer"
#    Given add a StaticColour layer for palette index '0x01'
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
    Given show video window

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
    # Tiles
    Given write data from file "tmp/Demo9Tiles_map.bin" to 24bit bus at '0x2000' and addressEx '0x80'
    Given write data from file "tmp/Demo9Tiles_plane0.bin" to 24bit bus at '0x2000' and addressEx '0x40'
    Given write data from file "tmp/Demo9Tiles_plane1.bin" to 24bit bus at '0x4000' and addressEx '0x40'
    Given write data from file "tmp/Demo9Tiles_plane2.bin" to 24bit bus at '0x8000' and addressEx '0x40'
    Given write data from file "tmp/Demo9Tiles_plane3.bin" to 24bit bus at '0x0000' and addressEx '0x40'
	#  Music
	# Can be replaced with "IncludeMusicData = 1" to have a minimal music demo
    Given write data from file "tmp/target/MusicMW2000Samples.bin" to 24bit bus at '0x0000' and addressEx '0x04'

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
    Given video display processes 24 pixels per instruction
    Given limit video display to 60 fps
    When I execute the procedure at start until return



  # Run with BuildIt10.bat
  @Demo10
  Scenario: Smoke test for just the video layer
    Given clear all external devices
    Given a new video display with overscan and 16 colours
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
    Given add a 2-to-1 merge layer
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
    Given a new audio expansion
    Given a new video display with overscan and 16 colours
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
    Given a ROM from file "C:\VICE\C64\kernal" at $e000
    Given a ROM from file "C:\VICE\C64\basic" at $a000
    Given add a C64 VIC
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
    When I execute the procedure at start for no more than 0 instructions until PC = $E5CF

    When I hex dump memory between $400 and $800

#    Given property "bdd6502.bus24.trace" is set to string "true"
#    Given I enable trace

    # Execute a screen draw to fake whatever the real C64 code and its IRQ does at this point
    When I execute the procedure at RenderScreenChunk until return
    Given render a video display frame
    When I execute the procedure at RenderScreenChunk until return
    Given render a video display frame
    When I execute the procedure at RenderScreenChunk until return
    Given render a video display frame
    When I execute the procedure at RenderScreenChunk until return
    Given render a video display frame
    When I execute the procedure at RenderScreenChunk until return

    When I execute the procedure at RenderScreenChunk until return
    Given render a video display frame
    When I execute the procedure at RenderScreenChunk until return
    Given render a video display frame
    When I execute the procedure at RenderScreenChunk until return
    Given render a video display frame
    When I execute the procedure at RenderScreenChunk until return
    Given render a video display frame
    When I execute the procedure at RenderScreenChunk until return

    Given render a video display frame
    When rendering the video until window closed

