Feature: Poly2D scanning tests

  Tests the Poly2D scanning routines

  Scenario: Triangle scan, bottom to top
    Given I have a simple overclocked 6502 system
    And I run the command line: ..\acme.exe -o t.prg --labeldump t.lbl -f cbm Poly2DTest1.a
    And I load prg "t.prg"
    And I load labels "t.lbl"

    # Starting from the bottom, to test the min/max detection code
    When I write memory at Poly2D_vertexBufferX + 0 with $80
    When I write memory at Poly2D_vertexBufferY + 0 with $0a

    When I write memory at Poly2D_vertexBufferX + 1 with $20
    When I write memory at Poly2D_vertexBufferY + 1 with $02

    When I write memory at Poly2D_vertexBufferX + 2 with $a0
    When I write memory at Poly2D_vertexBufferY + 2 with $06

    When I execute the procedure at Poly2D_scanTriangle for no more than 2500 instructions

    When I hex dump memory between Poly_linesLeft and Poly_linesLeftEnd
    Then property "test.BDD6502.lastHexDump" must contain string ignoring whitespace "00 00 2b 37 43 4f 5b 67  73 7f 80 00 00 00 00 00"

    When I hex dump memory between Poly_linesRight and Poly_linesRightEnd
    Then property "test.BDD6502.lastHexDump" must contain string ignoring whitespace "00 00 3f 5f 7f 9f 99 91  89 81 80 00 00 00 00 00"



  Scenario: Triangle scan, top to bottom
    Given I have a simple overclocked 6502 system
    And I run the command line: ..\acme.exe -o t.prg --labeldump t.lbl -f cbm Poly2DTest1.a
    And I load prg "t.prg"
    And I load labels "t.lbl"

    # Test the min/max detection code
    When I write memory at Poly2D_vertexBufferX + 0 with $20
    When I write memory at Poly2D_vertexBufferY + 0 with $02

    When I write memory at Poly2D_vertexBufferX + 1 with $a0
    When I write memory at Poly2D_vertexBufferY + 1 with $06

    When I write memory at Poly2D_vertexBufferX + 2 with $80
    When I write memory at Poly2D_vertexBufferY + 2 with $0a

    When I execute the procedure at Poly2D_scanTriangle for no more than 2500 instructions

    When I hex dump memory between Poly_linesLeft and Poly_linesLeftEnd
    Then property "test.BDD6502.lastHexDump" must contain string ignoring whitespace "00 00 2b 37 43 4f 5b 67  73 7f 80 00 00 00 00 00"

    When I hex dump memory between Poly_linesRight and Poly_linesRightEnd
    Then property "test.BDD6502.lastHexDump" must contain string ignoring whitespace "00 00 3f 5f 7f 9f 99 91  89 81 80 00 00 00 00 00"



  Scenario: Triangle scan, middle first
    Given I have a simple overclocked 6502 system
    And I run the command line: ..\acme.exe -o t.prg --labeldump t.lbl -f cbm Poly2DTest1.a
    And I load prg "t.prg"
    And I load labels "t.lbl"

    # Test the min/max detection code
    When I write memory at Poly2D_vertexBufferX + 0 with $a0
    When I write memory at Poly2D_vertexBufferY + 0 with $06

    When I write memory at Poly2D_vertexBufferX + 1 with $80
    When I write memory at Poly2D_vertexBufferY + 1 with $0a

    When I write memory at Poly2D_vertexBufferX + 2 with $20
    When I write memory at Poly2D_vertexBufferY + 2 with $02

    When I execute the procedure at Poly2D_scanTriangle for no more than 2500 instructions

    When I hex dump memory between Poly_linesLeft and Poly_linesLeftEnd
    Then property "test.BDD6502.lastHexDump" must contain string ignoring whitespace "00 00 2b 37 43 4f 5b 67  73 7f 80 00 00 00 00 00"

    When I hex dump memory between Poly_linesRight and Poly_linesRightEnd
    Then property "test.BDD6502.lastHexDump" must contain string ignoring whitespace "00 00 3f 5f 7f 9f 99 91  89 81 80 00 00 00 00 00"



  Scenario: Triangle scan, flat top
    Given I have a simple overclocked 6502 system
    And I run the command line: ..\acme.exe -o t.prg --labeldump t.lbl -f cbm Poly2DTest1.a
    And I load prg "t.prg"
    And I load labels "t.lbl"

	# Test the min/max detection code
    When I write memory at Poly2D_vertexBufferX + 0 with $00
    When I write memory at Poly2D_vertexBufferY + 0 with $06

    When I write memory at Poly2D_vertexBufferX + 1 with $80
    When I write memory at Poly2D_vertexBufferY + 1 with $00

    When I write memory at Poly2D_vertexBufferX + 2 with $40
    When I write memory at Poly2D_vertexBufferY + 2 with $08

    When I execute the procedure at Poly2D_scanTriangle for no more than 2500 instructions

    When I hex dump memory between Poly_linesLeft and Poly_linesLeftEnd
    Then property "test.BDD6502.lastHexDump" must contain string ignoring whitespace "6b 56 41 2b 16 01 00 3f  40 00 00 00 00 00 00 00"

    When I hex dump memory between Poly_linesRight and Poly_linesRightEnd
    Then property "test.BDD6502.lastHexDump" must contain string ignoring whitespace "79 71 69 61 59 51 49 41  40 00 00 00 00 00 00 00"



  Scenario: Triangle scan, flat bottom, right angle
    Given I have a simple overclocked 6502 system
    And I run the command line: ..\acme.exe -o t.prg --labeldump t.lbl -f cbm Poly2DTest1.a
    And I load prg "t.prg"
    And I load labels "t.lbl"

	# Test the min/max detection code
    When I write memory at Poly2D_vertexBufferX + 0 with $80
    When I write memory at Poly2D_vertexBufferY + 0 with $00

    When I write memory at Poly2D_vertexBufferX + 1 with $80
    When I write memory at Poly2D_vertexBufferY + 1 with $0f

    When I write memory at Poly2D_vertexBufferX + 2 with $7c
    When I write memory at Poly2D_vertexBufferY + 2 with $0f

    When I execute the procedure at Poly2D_scanTriangle for no more than 2500 instructions

    When I hex dump memory between Poly_linesLeft and Poly_linesLeftEnd
#   Then property "test.BDD6502.lastHexDump" must contain string ignoring whitespace "78 6f 67 5e 56 4d 45 3c  34 2b 23 1a 12 09 01 00"

    When I hex dump memory between Poly_linesRight and Poly_linesRightEnd
    Then property "test.BDD6502.lastHexDump" must contain string ignoring whitespace "80 80 80 80 80 80 80 80  80 80 80 80 80 80 80 80"



  Scenario: Quad scan, all angles
    Given I have a simple overclocked 6502 system
    And I run the command line: ..\acme.exe -o t.prg --labeldump t.lbl -f cbm Poly2DTest1.a
    And I load prg "t.prg"
    And I load labels "t.lbl"

    # Test the min/max detection code
    When I write memory at Poly2D_vertexBufferX + 0 with $40
    When I write memory at Poly2D_vertexBufferY + 0 with $00

    When I write memory at Poly2D_vertexBufferX + 1 with $80
    When I write memory at Poly2D_vertexBufferY + 1 with $08

    When I write memory at Poly2D_vertexBufferX + 2 with $40
    When I write memory at Poly2D_vertexBufferY + 2 with $0f

    When I write memory at Poly2D_vertexBufferX + 3 with $00
    When I write memory at Poly2D_vertexBufferY + 3 with $08

    When I execute the procedure at Poly2D_scanQuad for no more than 2500 instructions

    When I hex dump memory between Poly_linesLeft and Poly_linesLeftEnd
    Then property "test.BDD6502.lastHexDump" must contain string ignoring whitespace "39 31 29 21 19 11 09 01  00 12 1b 24 2d 36 3f 40"

    When I hex dump memory between Poly_linesRight and Poly_linesRightEnd
    Then property "test.BDD6502.lastHexDump" must contain string ignoring whitespace "47 4f 57 5f 67 6f 77 7f  77 6e 65 5c 53 4a 41 40"
