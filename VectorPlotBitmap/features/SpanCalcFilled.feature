Feature: Span calculation tests for filled lines

  Test the span calculation code for filled lines

  Scenario: Simple maximum length insertion test into filled line, same length different colour
    Given I have a simple overclocked 6502 system
    And I create file "t.a" with
      """
      kSpanPlot_maxHeight = 1
      !source "SpanCalcTest1.a"

      Poly_linesLeft
        !by 0
      Poly_linesLeftEnd

      Poly_linesRight
        !by kSpanPlot_maxLen
      Poly_linesRightEnd
      """
    And I run the command line: ..\acme.exe -o t.prg --labeldump t.lbl -f cbm t.a
    And I load prg "t.prg"
    And I load labels "t.lbl"

    Given I write memory at SpanCalc_screenNumSegments with 1
    Given I write memory at SpanCalc_screenSegmentsLen with kSpanPlot_maxLen
    Given I write memory at SpanCalc_screenSegmentsColour with 0

    When I set register A to 4
    When I set register X to 0
    When I set register Y to kSpanPlot_maxHeight


    When I execute the procedure at SpanCalc_scanPoly for no more than 100 instructions
    When I hex dump memory between SpanCalc_screenNumSegments and SpanCalc_screenNumSegmentsEnd
    When I hex dump memory between SpanCalc_screenSegmentsLen and SpanCalc_screenSegmentsLenEnd
    When I hex dump memory between SpanCalc_screenSegmentsColour and SpanCalc_screenSegmentsColourEnd

    Then I expect to see SpanCalc_screenNumSegments equal 1

    Then I expect to see SpanCalc_screenSegmentsLen equal kSpanPlot_maxLen
    Then I expect to see SpanCalc_screenSegmentsColour equal 4




  Scenario: Simple half length insertion test into filled line, on left different colour
    Given I have a simple overclocked 6502 system
    And I create file "t.a" with
      """
      kSpanPlot_maxHeight = 1
      !source "SpanCalcTest1.a"

      Poly_linesLeft
        !by 0
      Poly_linesLeftEnd

      Poly_linesRight
        !by kSpanPlot_maxLen/2
      Poly_linesRightEnd
      """
    And I run the command line: ..\acme.exe -o t.prg --labeldump t.lbl -f cbm t.a
    And I load prg "t.prg"
    And I load labels "t.lbl"

    Given I write memory at SpanCalc_screenNumSegments with 1
    Given I write memory at SpanCalc_screenSegmentsLen with kSpanPlot_maxLen
    Given I write memory at SpanCalc_screenSegmentsColour with 0

    When I set register A to 4
    When I set register X to 0
    When I set register Y to kSpanPlot_maxHeight


    When I execute the procedure at SpanCalc_scanPoly for no more than 100 instructions
    When I hex dump memory between SpanCalc_screenNumSegments and SpanCalc_screenNumSegmentsEnd
    When I hex dump memory between SpanCalc_screenSegmentsLen and SpanCalc_screenSegmentsLenEnd
    When I hex dump memory between SpanCalc_screenSegmentsColour and SpanCalc_screenSegmentsColourEnd

    Then I expect to see SpanCalc_screenNumSegments equal 2

    Then I expect to see SpanCalc_screenSegmentsLen equal kSpanPlot_maxLen/2
    Then I expect to see SpanCalc_screenSegmentsColour equal 4

    Then I expect to see SpanCalc_screenSegmentsLen+kSpanPlot_maxHeight equal kSpanPlot_maxLen/2
    Then I expect to see SpanCalc_screenSegmentsColour+kSpanPlot_maxHeight equal 0




  Scenario: Simple half length insertion test into filled line, on left same colour
    Given I have a simple overclocked 6502 system
    And I create file "t.a" with
      """
      kSpanPlot_maxHeight = 1
      !source "SpanCalcTest1.a"

      Poly_linesLeft
        !by 0
      Poly_linesLeftEnd

      Poly_linesRight
        !by kSpanPlot_maxLen/2
      Poly_linesRightEnd
      """
    And I run the command line: ..\acme.exe -o t.prg --labeldump t.lbl -f cbm t.a
    And I load prg "t.prg"
    And I load labels "t.lbl"

    Given I write memory at SpanCalc_screenNumSegments with 1
    Given I write memory at SpanCalc_screenSegmentsLen with kSpanPlot_maxLen
    Given I write memory at SpanCalc_screenSegmentsColour with 4

    When I set register A to 4
    When I set register X to 0
    When I set register Y to kSpanPlot_maxHeight


    When I execute the procedure at SpanCalc_scanPoly for no more than 100 instructions
    When I hex dump memory between SpanCalc_screenNumSegments and SpanCalc_screenNumSegmentsEnd
    When I hex dump memory between SpanCalc_screenSegmentsLen and SpanCalc_screenSegmentsLenEnd
    When I hex dump memory between SpanCalc_screenSegmentsColour and SpanCalc_screenSegmentsColourEnd

    Then I expect to see SpanCalc_screenNumSegments equal 1

    Then I expect to see SpanCalc_screenSegmentsLen equal kSpanPlot_maxLen
    Then I expect to see SpanCalc_screenSegmentsColour equal 4





  Scenario: Simple half length insertion test into filled line, on right different colour
    Given I have a simple overclocked 6502 system
    And I create file "t.a" with
      """
      kSpanPlot_maxHeight = 1
      !source "SpanCalcTest1.a"

      Poly_linesLeft
        !by kSpanPlot_maxLen/2
      Poly_linesLeftEnd

      Poly_linesRight
        !by kSpanPlot_maxLen
      Poly_linesRightEnd
      """
    And I run the command line: ..\acme.exe -o t.prg --labeldump t.lbl -f cbm t.a
    And I load prg "t.prg"
    And I load labels "t.lbl"

    Given I write memory at SpanCalc_screenNumSegments with 1
    Given I write memory at SpanCalc_screenSegmentsLen with kSpanPlot_maxLen
    Given I write memory at SpanCalc_screenSegmentsColour with 0

    When I set register A to 4
    When I set register X to 0
    When I set register Y to kSpanPlot_maxHeight


    When I execute the procedure at SpanCalc_scanPoly for no more than 100 instructions
    When I hex dump memory between SpanCalc_screenNumSegments and SpanCalc_screenNumSegmentsEnd
    When I hex dump memory between SpanCalc_screenSegmentsLen and SpanCalc_screenSegmentsLenEnd
    When I hex dump memory between SpanCalc_screenSegmentsColour and SpanCalc_screenSegmentsColourEnd

    Then I expect to see SpanCalc_screenNumSegments equal 2

    Then I expect to see SpanCalc_screenSegmentsLen equal kSpanPlot_maxLen/2
    Then I expect to see SpanCalc_screenSegmentsColour equal 0

    Then I expect to see SpanCalc_screenSegmentsLen+kSpanPlot_maxHeight equal kSpanPlot_maxLen/2
    Then I expect to see SpanCalc_screenSegmentsColour+kSpanPlot_maxHeight equal 4




  Scenario: Simple half length insertion test into filled line, on right same colour
    Given I have a simple overclocked 6502 system
    And I create file "t.a" with
      """
      kSpanPlot_maxHeight = 1
      !source "SpanCalcTest1.a"

      Poly_linesLeft
        !by kSpanPlot_maxLen/2
      Poly_linesLeftEnd

      Poly_linesRight
        !by kSpanPlot_maxLen
      Poly_linesRightEnd
      """
    And I run the command line: ..\acme.exe -o t.prg --labeldump t.lbl -f cbm t.a
    And I load prg "t.prg"
    And I load labels "t.lbl"

    Given I write memory at SpanCalc_screenNumSegments with 1
    Given I write memory at SpanCalc_screenSegmentsLen with kSpanPlot_maxLen
    Given I write memory at SpanCalc_screenSegmentsColour with 4

    When I set register A to 4
    When I set register X to 0
    When I set register Y to kSpanPlot_maxHeight


    When I execute the procedure at SpanCalc_scanPoly for no more than 100 instructions
    When I hex dump memory between SpanCalc_screenNumSegments and SpanCalc_screenNumSegmentsEnd
    When I hex dump memory between SpanCalc_screenSegmentsLen and SpanCalc_screenSegmentsLenEnd
    When I hex dump memory between SpanCalc_screenSegmentsColour and SpanCalc_screenSegmentsColourEnd

    Then I expect to see SpanCalc_screenNumSegments equal 1

    Then I expect to see SpanCalc_screenSegmentsLen equal kSpanPlot_maxLen
    Then I expect to see SpanCalc_screenSegmentsColour equal 4




  Scenario: Simple half length insertion test into filled line, in middle different colour
    Given I have a simple overclocked 6502 system
    And I create file "t.a" with
      """
      kSpanPlot_maxHeight = 1
      !source "SpanCalcTest1.a"

      Poly_linesLeft
        !by kSpanPlot_maxLen/4
      Poly_linesLeftEnd

      Poly_linesRight
        !by (3*kSpanPlot_maxLen)/4
      Poly_linesRightEnd
      """
    And I run the command line: ..\acme.exe -o t.prg --labeldump t.lbl -f cbm t.a
    And I load prg "t.prg"
    And I load labels "t.lbl"

    Given I write memory at SpanCalc_screenNumSegments with 1
    Given I write memory at SpanCalc_screenSegmentsLen with kSpanPlot_maxLen
    Given I write memory at SpanCalc_screenSegmentsColour with 0

    When I set register A to 4
    When I set register X to 0
    When I set register Y to kSpanPlot_maxHeight


    When I execute the procedure at SpanCalc_scanPoly for no more than 100 instructions
    When I hex dump memory between SpanCalc_screenNumSegments and SpanCalc_screenNumSegmentsEnd
    When I hex dump memory between SpanCalc_screenSegmentsLen and SpanCalc_screenSegmentsLenEnd
    When I hex dump memory between SpanCalc_screenSegmentsColour and SpanCalc_screenSegmentsColourEnd

    Then I expect to see SpanCalc_screenNumSegments equal 3

    Then I expect to see SpanCalc_screenSegmentsLen equal kSpanPlot_maxLen/4
    Then I expect to see SpanCalc_screenSegmentsColour equal 0

    Then I expect to see SpanCalc_screenSegmentsLen+kSpanPlot_maxHeight equal kSpanPlot_maxLen/2
    Then I expect to see SpanCalc_screenSegmentsColour+kSpanPlot_maxHeight equal 4

    Then I expect to see SpanCalc_screenSegmentsLen+(2*kSpanPlot_maxHeight) equal kSpanPlot_maxLen/4
    Then I expect to see SpanCalc_screenSegmentsColour+(2*kSpanPlot_maxHeight) equal 0




  Scenario: Simple half length insertion test into filled line, in middle same colour
    Given I have a simple overclocked 6502 system
    And I create file "t.a" with
      """
      kSpanPlot_maxHeight = 1
      !source "SpanCalcTest1.a"

      Poly_linesLeft
        !by kSpanPlot_maxLen/4
      Poly_linesLeftEnd

      Poly_linesRight
        !by (3*kSpanPlot_maxLen)/4
      Poly_linesRightEnd
      """
    And I run the command line: ..\acme.exe -o t.prg --labeldump t.lbl -f cbm t.a
    And I load prg "t.prg"
    And I load labels "t.lbl"

    Given I write memory at SpanCalc_screenNumSegments with 1
    Given I write memory at SpanCalc_screenSegmentsLen with kSpanPlot_maxLen
    Given I write memory at SpanCalc_screenSegmentsColour with 4

    When I set register A to 4
    When I set register X to 0
    When I set register Y to kSpanPlot_maxHeight


    When I execute the procedure at SpanCalc_scanPoly for no more than 100 instructions
    When I hex dump memory between SpanCalc_screenNumSegments and SpanCalc_screenNumSegmentsEnd
    When I hex dump memory between SpanCalc_screenSegmentsLen and SpanCalc_screenSegmentsLenEnd
    When I hex dump memory between SpanCalc_screenSegmentsColour and SpanCalc_screenSegmentsColourEnd

    Then I expect to see SpanCalc_screenNumSegments equal 1

    Then I expect to see SpanCalc_screenSegmentsLen equal kSpanPlot_maxLen
    Then I expect to see SpanCalc_screenSegmentsColour equal 4
