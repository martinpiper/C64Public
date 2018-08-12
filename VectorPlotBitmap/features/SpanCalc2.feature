Feature: Span calculation tests multiple lines

  Test the span calculation code with multiple lines

  Scenario: Multi line polygon insert
    Given I have a simple overclocked 6502 system
    And I create file "t.a" with
      """
      kSpanPlot_maxHeight = 4
      !source "SpanCalcTest1.a"

      Poly_linesLeft
        !by 0
        !by 0
        !by kSpanPlot_smallLen/2
        !by kSpanPlot_smallLen
      Poly_linesLeftEnd

      Poly_linesRight
        !by kSpanPlot_maxLen
        !by kSpanPlot_maxLen/2
        !by kSpanPlot_maxLen-kSpanPlot_smallLen
        !by kSpanPlot_maxLen
      Poly_linesRightEnd
      """
    And I run the command line: ..\acme.exe -o t.prg --labeldump t.lbl -f cbm t.a
    And I load prg "t.prg"
    And I load labels "t.lbl"

    When I set register A to 4
    When I set register X to 0
    When I set register Y to kSpanPlot_maxHeight
    
    When I execute the procedure at SpanCalc_scanPoly for no more than 150 instructions
    When I hex dump memory between SpanCalc_screenNumSegments and SpanCalc_screenNumSegmentsEnd
    When I hex dump memory between SpanCalc_screenSegmentsLen and SpanCalc_screenSegmentsLenEnd
    When I hex dump memory between SpanCalc_screenSegmentsColour and SpanCalc_screenSegmentsColourEnd

    Then I expect to see SpanCalc_screenNumSegments equal 1

    Then I expect to see SpanCalc_screenSegmentsLen equal kSpanPlot_maxLen
    Then I expect to see SpanCalc_screenSegmentsColour equal 4




    Then I expect to see SpanCalc_screenNumSegments+1 equal 2

    Then I expect to see SpanCalc_screenSegmentsLen+1 equal kSpanPlot_maxLen/2
    Then I expect to see SpanCalc_screenSegmentsColour+1 equal 4

    Then I expect to see SpanCalc_screenSegmentsLen+1+kSpanPlot_maxHeight equal kSpanPlot_maxLen/2
    Then I expect to see SpanCalc_screenSegmentsColour+1+kSpanPlot_maxHeight equal 0




    Then I expect to see SpanCalc_screenNumSegments+2 equal 3

    Then I expect to see SpanCalc_screenSegmentsLen+2 equal kSpanPlot_smallLen/2
    Then I expect to see SpanCalc_screenSegmentsColour+2 equal 0

    Then I expect to see SpanCalc_screenSegmentsLen+2+kSpanPlot_maxHeight equal kSpanPlot_maxLen-kSpanPlot_smallLen-(kSpanPlot_smallLen/2)
    Then I expect to see SpanCalc_screenSegmentsColour+2+kSpanPlot_maxHeight equal 4

    Then I expect to see SpanCalc_screenSegmentsLen+2+(2*kSpanPlot_maxHeight) equal kSpanPlot_maxLen/4
    Then I expect to see SpanCalc_screenSegmentsColour+2+(2*kSpanPlot_maxHeight) equal 0





    Then I expect to see SpanCalc_screenNumSegments+3 equal 2

    Then I expect to see SpanCalc_screenSegmentsLen+3 equal kSpanPlot_smallLen
    Then I expect to see SpanCalc_screenSegmentsColour+3 equal 0

    Then I expect to see SpanCalc_screenSegmentsLen+3+kSpanPlot_maxHeight equal kSpanPlot_maxLen-kSpanPlot_smallLen
    Then I expect to see SpanCalc_screenSegmentsColour+3+kSpanPlot_maxHeight equal 4
