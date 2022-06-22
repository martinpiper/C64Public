@smoketest
Feature: Smoke test

  Quickly load the output and check various simple aspects of the code

  # Run with Convert.bat
  @Demo1
  Scenario: Smoke test for video
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
    Given add a Chars V4.0 layer with registers at '0x9000' and screen addressEx '0x80' and planes addressEx '0x20'
    And the layer has 16 colours
    And the layer has overscan

    Given show video window

    And I load prg "C:\Work\C64\VideoHardware\Prototype3\just chars.prg"

    When I execute the procedure at 2061 until return
