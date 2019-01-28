Feature: Segment insertion tests multiple lines

  Test the segment insertion code for multiple lines

  Scenario: Simple insertion tests into multiple lines
    Given I have a simple overclocked 6502 system
    And I create file "t.a" with
      """
      kSpanPlot_maxHeight = 3
      !source "SegmentsTest1.a"

      Poly_linesLeft
        !by 10
        !by 20
        !by 30
      Poly_linesLeftEnd

      Poly_linesRight
        !by 100
        !by 90
        !by 80
      Poly_linesRightEnd
      """
    And I run the command line: ..\acme.exe -o t.prg --labeldump t.lbl -f cbm t.a
    And I load prg "t.prg"
    And I load labels "t.lbl"

    When I execute the procedure at Segments_initStorage for no more than 100 instructions

    When I set register A to 1
    When I set register X to 0
    When I set register Y to kSpanPlot_maxHeight
    
    When I execute the procedure at Segments_processLines for no more than 150 instructions

    When I hex dump memory between Segments_array and Segments_arrayEnd

    Then I expect to see ZPSegments_primaryAllocatorAddrLo equal lo(Segments_array+(3*kSegment_length))
    Then I expect to see ZPSegments_primaryAllocatorAddrHi equal hi(Segments_array+(3*kSegment_length))
    
    Then I expect to see Segments_linesLo+0 equal lo(Segments_array+(0*kSegment_length))
    Then I expect to see Segments_linesHi+0 equal hi(Segments_array+(0*kSegment_length))

    Then I expect to see Segments_linesLo+1 equal lo(Segments_array+(1*kSegment_length))
    Then I expect to see Segments_linesHi+1 equal hi(Segments_array+(1*kSegment_length))

    Then I expect to see Segments_linesLo+2 equal lo(Segments_array+(2*kSegment_length))
    Then I expect to see Segments_linesHi+2 equal hi(Segments_array+(2*kSegment_length))
    
    Then I expect to see Segments_array+(0*kSegment_length)+kSegment_offset_nextHi equal 0
    Then I expect to see Segments_array+(0*kSegment_length)+kSegment_offset_left equal 10
    Then I expect to see Segments_array+(0*kSegment_length)+kSegment_offset_right equal 100
    Then I expect to see Segments_array+(0*kSegment_length)+kSegment_offset_colour equal 1

    Then I expect to see Segments_array+(1*kSegment_length)+kSegment_offset_nextHi equal 0
    Then I expect to see Segments_array+(1*kSegment_length)+kSegment_offset_left equal 20
    Then I expect to see Segments_array+(1*kSegment_length)+kSegment_offset_right equal 90
    Then I expect to see Segments_array+(1*kSegment_length)+kSegment_offset_colour equal 1

    Then I expect to see Segments_array+(2*kSegment_length)+kSegment_offset_nextHi equal 0
    Then I expect to see Segments_array+(2*kSegment_length)+kSegment_offset_left equal 30
    Then I expect to see Segments_array+(2*kSegment_length)+kSegment_offset_right equal 80
    Then I expect to see Segments_array+(2*kSegment_length)+kSegment_offset_colour equal 1
