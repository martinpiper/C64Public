Feature: Span calculation tests for clearing

  Test the span calculation code for clearing the buffer

  Scenario: Test the dirty clear check
    Given I have a simple overclocked 6502 system
    And I create file "t.a" with
      """
      kSpanPlot_maxHeight = 4
      !source "SpanCalcTest1.a"

      Poly_linesLeft
      Poly_linesLeftEnd

      Poly_linesRight
      Poly_linesRightEnd
      """
    And I run the command line: ..\acme.exe -o t.prg --labeldump t.lbl -f cbm t.a
    And I load prg "t.prg"
    And I load labels "t.lbl"

    Given I write memory at SpanCalc_screenNumSegments with 0

    Given I write memory at SpanCalc_screenNumSegments+1 with 1
    Given I write memory at SpanCalc_screenSegmentsLen+1 with kSpanPlot_maxLen
    Given I write memory at SpanCalc_screenSegmentsColour+1 with 0

    Given I write memory at SpanCalc_screenNumSegments+2 with 1
    Given I write memory at SpanCalc_screenSegmentsLen+2 with kSpanPlot_maxLen
    Given I write memory at SpanCalc_screenSegmentsColour+2 with 4

    Given I write memory at SpanCalc_screenNumSegments+3 with 2
    Given I write memory at SpanCalc_screenSegmentsLen+3 with 100
    Given I write memory at SpanCalc_screenSegmentsColour+3 with 4

    When I execute the procedure at SpanCalc_dirtyClear for no more than 100 instructions

    When I hex dump memory between SpanCalc_screenNumSegments and SpanCalc_screenNumSegmentsEnd
    When I hex dump memory between SpanCalc_screenSegmentsLen and SpanCalc_screenSegmentsLenEnd
    When I hex dump memory between SpanCalc_screenSegmentsColour and SpanCalc_screenSegmentsColourEnd

    Then I expect to see SpanCalc_screenNumSegments equal 0

    Then I expect to see SpanCalc_screenNumSegments+1 equal 1
    Then I expect to see SpanCalc_screenSegmentsLen+1 equal kSpanPlot_maxLen
    Then I expect to see SpanCalc_screenSegmentsColour+1 equal 0

    Then I expect to see SpanCalc_screenNumSegments+2 equal 1
    Then I expect to see SpanCalc_screenSegmentsLen+2 equal kSpanPlot_maxLen
    Then I expect to see SpanCalc_screenSegmentsColour+2 equal 0

    Then I expect to see SpanCalc_screenNumSegments+3 equal 1
    Then I expect to see SpanCalc_screenSegmentsLen+3 equal kSpanPlot_maxLen
    Then I expect to see SpanCalc_screenSegmentsColour+3 equal 0




  Scenario: Test the dirty expire check
    Given I have a simple overclocked 6502 system
    And I create file "t.a" with
      """
      kSpanPlot_maxHeight = 4
      !source "SpanCalcTest1.a"

      Poly_linesLeft
      Poly_linesLeftEnd

      Poly_linesRight
      Poly_linesRightEnd
      """
    And I run the command line: ..\acme.exe -o t.prg --labeldump t.lbl -f cbm t.a
    And I load prg "t.prg"
    And I load labels "t.lbl"

    Given I write memory at SpanCalc_screenNumSegments with 0

    Given I write memory at SpanCalc_screenNumSegments+1 with 1
    Given I write memory at SpanCalc_screenSegmentsLen+1 with kSpanPlot_maxLen
    Given I write memory at SpanCalc_screenSegmentsColour+1 with 0

    Given I write memory at SpanCalc_screenNumSegments+2 with 1
    Given I write memory at SpanCalc_screenSegmentsLen+2 with kSpanPlot_maxLen
    Given I write memory at SpanCalc_screenSegmentsColour+2 with 4

    Given I write memory at SpanCalc_screenNumSegments+3 with 2
    Given I write memory at SpanCalc_screenSegmentsLen+3 with 100
    Given I write memory at SpanCalc_screenSegmentsColour+3 with 4

    When I execute the procedure at SpanCalc_expireCleared for no more than 100 instructions

    When I hex dump memory between SpanCalc_screenNumSegments and SpanCalc_screenNumSegmentsEnd
    When I hex dump memory between SpanCalc_screenSegmentsLen and SpanCalc_screenSegmentsLenEnd
    When I hex dump memory between SpanCalc_screenSegmentsColour and SpanCalc_screenSegmentsColourEnd

    Then I expect to see SpanCalc_screenNumSegments equal 0

    Then I expect to see SpanCalc_screenNumSegments+1 equal 0

    Then I expect to see SpanCalc_screenNumSegments+2 equal 1
    Then I expect to see SpanCalc_screenSegmentsLen+2 equal kSpanPlot_maxLen
    Then I expect to see SpanCalc_screenSegmentsColour+2 equal 4

    Then I expect to see SpanCalc_screenNumSegments+3 equal 2
    Then I expect to see SpanCalc_screenSegmentsLen+3 equal 100
    Then I expect to see SpanCalc_screenSegmentsColour+3 equal 4
