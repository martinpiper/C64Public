Feature: Verify correct rendering

  Verify correct rendering by using Vice emulator and screenshots

  @TC-1
  Scenario: [TC-1] Check 8-way scrolling with default score panel
    Given I run the command line: cmd /c BuildIt.bat ScrollEntry_MultiDirection.a
    And I load labels "Scroller.map"
    Given I run the command line: cmd /c start C:\Downloads\WinVICE-3.1-x86-r34062\WinVICE-3.1-x86-r34062\x64sc.exe -autostartprgmode 1 +autostart-delay-random +VICIIdsize +VICIIdscan -VICIIborders 2 -VICIIfilter 0 +sound -warp -remotemonitor
    # Vice is automatically quit at the end of the test
    Given connect to remote monitor at TCP "127.0.0.1" port "6510"
    When send remote monitor command "break" "afterScrollerDemoWaitScorePanel"
    When send remote monitor command without parsing "attach "%WCD%\scroller.crt" 20"
    When send remote monitor command "reset"
    When remote monitor wait for hit
    When send remote monitor command without parsing "scrsh "%WCD%\target\TC-1-1.png" 2"
#    When send remote monitor command "del"
#    When remote monitor continue without waiting
#    When disconnect remote monitor

    When remote monitor wait for hit
    When send remote monitor command without parsing "scrsh "%WCD%\target\TC-1-2.png" 2"

    When remote monitor wait for 12 hits
    When send remote monitor command without parsing "scrsh "%WCD%\target\TC-1-3.png" 2"

    When remote monitor wait for 12 hits
    When send remote monitor command without parsing "scrsh "%WCD%\target\TC-1-4.png" 2"

    When remote monitor wait for 12 hits
    When send remote monitor command without parsing "scrsh "%WCD%\target\TC-1-5.png" 2"

    When remote monitor wait for 6 hits
    When send remote monitor command without parsing "scrsh "%WCD%\target\TC-1-6.png" 2"

    When remote monitor wait for hit
    When send remote monitor command without parsing "scrsh "%WCD%\target\TC-1-7.png" 2"
    When remote monitor wait for hit
    When send remote monitor command without parsing "scrsh "%WCD%\target\TC-1-8.png" 2"
    When remote monitor wait for hit
    When send remote monitor command without parsing "scrsh "%WCD%\target\TC-1-9.png" 2"
    When remote monitor wait for hit
    When send remote monitor command without parsing "scrsh "%WCD%\target\TC-1-10.png" 2"

    Then expect image "target/TC-1-1.png" to be identical to "testdata/TC-1-1.png"
    Then expect image "target/TC-1-2.png" to be identical to "testdata/TC-1-2.png"
    Then expect image "target/TC-1-3.png" to be identical to "testdata/TC-1-3.png"
    Then expect image "target/TC-1-4.png" to be identical to "testdata/TC-1-4.png"
    Then expect image "target/TC-1-5.png" to be identical to "testdata/TC-1-5.png"
    Then expect image "target/TC-1-6.png" to be identical to "testdata/TC-1-6.png"
    Then expect image "target/TC-1-7.png" to be identical to "testdata/TC-1-7.png"
    Then expect image "target/TC-1-8.png" to be identical to "testdata/TC-1-8.png"
    Then expect image "target/TC-1-9.png" to be identical to "testdata/TC-1-9.png"
    Then expect image "target/TC-1-10.png" to be identical to "testdata/TC-1-10.png"


  @TC-2
  Scenario: [TC-2] Check SEUCK build
    Given I run the command line: cmd /c BuildIt.bat ScrollEntry_SEUCK.a
    And I load labels "Scroller.map"
    Given I run the command line: cmd /c start C:\Downloads\WinVICE-3.1-x86-r34062\WinVICE-3.1-x86-r34062\x64sc.exe -autostartprgmode 1 +autostart-delay-random +VICIIdsize +VICIIdscan -VICIIborders 2 -VICIIfilter 0 +sound -warp -remotemonitor
    # Vice is automatically quit at the end of the test
    Given connect to remote monitor at TCP "127.0.0.1" port "6510"
    When send remote monitor command "break" "titleJoystickTest"
    When send remote monitor command "break" "afterScrollerDemoWaitScorePanel"
    When send remote monitor command without parsing "attach "%WCD%\scroller.crt" 20"
    When send remote monitor command "reset"
    When remote monitor wait for 30 hits
    When send remote monitor command without parsing "scrsh "%WCD%\target\TC-2-1.png" 2"

    # Advance the title screen fire test
    When send remote monitor command "n"
    When send remote monitor command "r a=0"
    When remote monitor wait for hit

#    When send remote monitor command "del"
#    When remote monitor continue without waiting
#    When disconnect remote monitor

    When remote monitor wait for 60 hits
    When send remote monitor command without parsing "scrsh "%WCD%\target\TC-2-2.png" 2"

    When remote monitor wait for 60 hits
    When send remote monitor command without parsing "scrsh "%WCD%\target\TC-2-3.png" 2"

    When remote monitor wait for hit
    When send remote monitor command without parsing "scrsh "%WCD%\target\TC-2-4.png" 2"

    When remote monitor wait for hit
    When send remote monitor command without parsing "scrsh "%WCD%\target\TC-2-5.png" 2"

    When remote monitor wait for hit
    When send remote monitor command without parsing "scrsh "%WCD%\target\TC-2-6.png" 2"

    When remote monitor wait for hit
    When send remote monitor command without parsing "scrsh "%WCD%\target\TC-2-7.png" 2"

    Then expect image "target/TC-2-1.png" to be identical to "testdata/TC-2-1.png"
    Then expect image "target/TC-2-2.png" to be identical to "testdata/TC-2-2.png"
    Then expect image "target/TC-2-3.png" to be identical to "testdata/TC-2-3.png"
    Then expect image "target/TC-2-4.png" to be identical to "testdata/TC-2-4.png"
    Then expect image "target/TC-2-5.png" to be identical to "testdata/TC-2-5.png"
    Then expect image "target/TC-2-6.png" to be identical to "testdata/TC-2-6.png"
