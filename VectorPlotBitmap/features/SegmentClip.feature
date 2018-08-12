Feature: Segment insertionclip tests

  Test the segment clip code

  Scenario: Simple clip insertion test, one existing segment, left side
    Given I have a simple overclocked 6502 system
    And I create file "t.a" with
      """
      kSpanPlot_maxHeight = 1
      !source "SegmentsTest1.a"

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

    When I execute the procedure at Segments_initStorage for no more than 100 instructions


    When I execute the procedure at SetupTest for no more than 100 instructions

    Then I expect to see ZPSegments_primaryAllocatorAddrLo equal lo(Segments_array+(1*kSegment_length))
    Then I expect to see ZPSegments_primaryAllocatorAddrHi equal hi(Segments_array+(1*kSegment_length))
    Then I expect to see Segments_array+(0*kSegment_length)+kSegment_offset_nextHi equal 0
    Then I expect to see Segments_array+(0*kSegment_length)+kSegment_offset_start equal 0
    Then I expect to see Segments_array+(0*kSegment_length)+kSegment_offset_end equal kSpanPlot_maxLen
    Then I expect to see Segments_array+(0*kSegment_length)+kSegment_offset_colour equal 0


    When I set register A to 1
    When I set register X to 0
    When I set register Y to kSpanPlot_maxHeight
    When I execute the procedure at Segments_scanPoly for no more than 100 instructions
    When I hex dump memory between Segments_array and Segments_arrayEnd

    Then I expect to see ZPSegments_primaryAllocatorAddrLo equal lo(Segments_array+(2*kSegment_length))
    Then I expect to see ZPSegments_primaryAllocatorAddrHi equal hi(Segments_array+(2*kSegment_length))


    Then I expect to see Segments_linesLo equal lo(Segments_array+(1*kSegment_length))
    Then I expect to see Segments_linesHi equal hi(Segments_array+(1*kSegment_length))
    
    Then I expect to see Segments_array+(1*kSegment_length)+kSegment_offset_nextLo equal lo(Segments_array+(0*kSegment_length))
    Then I expect to see Segments_array+(1*kSegment_length)+kSegment_offset_nextHi equal hi(Segments_array+(0*kSegment_length))
    Then I expect to see Segments_array+(1*kSegment_length)+kSegment_offset_start equal 0
    Then I expect to see Segments_array+(1*kSegment_length)+kSegment_offset_end equal kSpanPlot_maxLen/2
    Then I expect to see Segments_array+(1*kSegment_length)+kSegment_offset_colour equal 1

    Then I expect to see Segments_array+(0*kSegment_length)+kSegment_offset_nextHi equal 0
    Then I expect to see Segments_array+(0*kSegment_length)+kSegment_offset_start equal kSpanPlot_maxLen/2
    Then I expect to see Segments_array+(0*kSegment_length)+kSegment_offset_end equal kSpanPlot_maxLen
    Then I expect to see Segments_array+(0*kSegment_length)+kSegment_offset_colour equal 0

