Feature: Segment allocation tests

  Test the segment allocator

  Scenario: Simple allocation test
    Given I have a simple overclocked 6502 system
    And I create file "t.a" with
      """
      kSpanPlot_maxHeight = 1
      !source "SegmentsTest1.a"

      Poly_linesLeft
      Poly_linesLeftEnd

      Poly_linesRight
      Poly_linesRightEnd
      """
    And I run the command line: ..\acme.exe -o t.prg --labeldump t.lbl -f cbm t.a
    And I load prg "t.prg"
    And I load labels "t.lbl"

    When I execute the procedure at Segments_initStorage for no more than 100 instructions

    Then I expect to see ZPSegments_primaryAllocatorAddrLo equal lo(Segments_array)
    And I expect to see ZPSegments_primaryAllocatorAddrHi equal hi(Segments_array)

    When I execute the procedure at Segments_allocate for no more than 100 instructions
    Then I expect to see ZPSegments_currentSegmentLo equal lo(Segments_array)
    And I expect to see ZPSegments_currentSegmentHi equal hi(Segments_array)
    Then I expect to see ZPSegments_primaryAllocatorAddrLo equal lo(Segments_array+kSegment_length)
    And I expect to see ZPSegments_primaryAllocatorAddrHi equal hi(Segments_array+kSegment_length)
