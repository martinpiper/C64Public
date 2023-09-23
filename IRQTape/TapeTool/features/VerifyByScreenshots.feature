Feature: Verify the tape turbo code and TapeTool can produce working TAP files

  This tests the full range of TapeTool functionality, including generating auto booting kernal files, turbo loader, with title screen, compression, and running known working code, to test the final expected screen display.

  @TC-1
  Scenario: [TC-1] Check an end-to-end scenario
    Given I run the command line: cmd /c del /q Loaders.bin
    And I load labels "Loaders.map"
    Given I run the command line: cmd /c ..\..\acme.exe --lib  ../ --lib ../../  -v4 --msvc _config.a  Loaders.a
    Then property "test.BDD6502.lastProcessOutput" must contain string "Saving"
    Given I run the command line: cmd /c ..\..\bin\TapeTool.exe n 1 wn "test.tap" m "Loaders.map" ocb1 Loaders.bin otl$c0 otft "Loaders.bin" .RealCodeStart .RealCodeEnd otl$c0 otfbr "Loaders.bin" 0 .SpriteDataStart .SpriteDataEnd $200 otl$c0 otfbr "..\R.MUSC000.prg" 1 otl$c0 otfbr "..\..\BlankProjectComp\Tusari.scr" 2 $2 $3eb .BitmapLogoScreen otl$c0 otfbr "..\..\BlankProjectComp\Tusari.col" 3 otl$c0 otfbr "..\..\BlankProjectComp\Tusari.bmp" 4 $2 $1f43 .BitmapLogo otl$c0 otfbr "..\TestMultiplexor.prg" 5 otl$c0 c
    Then property "test.BDD6502.lastProcessOutput" must contain string "Flushing"
    
    Given I run the command line: cmd /c start C:\Downloads\WinVICE-3.1-x86-r34062\WinVICE-3.1-x86-r34062\x64sc.exe -autostartprgmode 1 +autostart-delay-random +VICIIdsize +VICIIdscan -VICIIborders 2 -VICIIfilter 0 +sound -warp -remotemonitor -dszerogapdelay 0 -dsspeedtuning 0 -dstapewobble 0
    # Vice is automatically quit at the end of the test
    Given connect to remote monitor at TCP "127.0.0.1" port "6510"
#    When remote monitor continue without waiting
#    When disconnect remote monitor
    
    # Main loader screen
    When send remote monitor command "break" "rasterTestDone1"
    Then property "test.BDD6502.lastMonitorReply" must contain string "Stop on exec"

    When send remote monitor command without parsing "autostart "%WCD%\test.tap""
    Then property "test.BDD6502.lastMonitorReply" must contain string "autostarting"
    When remote monitor wait for hit
    When remote monitor wait for 4 hits
    When send remote monitor command without parsing "scrsh "%WCD%\target\TC-1-1.png" 2"


    When send remote monitor command without parsing "io d000"
    Then property "test.BDD6502.lastMonitorReply" must contain string "Mode: Standard Text"

    When remote monitor wait for 50 hits
    When send remote monitor command without parsing "sc"
    Then property "test.BDD6502.lastMonitorReply" must contain string "this"


    # Wait for play music in the multiplexor demo
    When send remote monitor command without parsing "del"
    When send remote monitor command without parsing "break 3f03"

    When remote monitor wait for 30 hits

    When send remote monitor command without parsing "scrsh "%WCD%\target\TC-1-2.png" 2"

    Then expect image "target/TC-1-1.png" to be identical to "testdata/TC-1-1.png"
    Then expect image "target/TC-1-2.png" to be identical to "testdata/TC-1-2.png"
