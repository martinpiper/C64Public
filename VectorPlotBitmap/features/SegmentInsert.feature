Feature: Segment insertion tests

  Test the segment insertion code

  Scenario Outline: Simple insertion tests into empty line
    Given I have a simple overclocked 6502 system
    And I create file "t.a" with
      """
      kSpanPlot_maxHeight = 1
      !source "SegmentsTest1.a"

      Poly_linesLeft
        !by <left>
      Poly_linesLeftEnd

      Poly_linesRight
        !by <right>
      Poly_linesRightEnd
      """
    And I run the command line: ..\acme.exe -o t.prg --labeldump t.lbl -f cbm t.a
    And I load prg "t.prg"
    And I load labels "t.lbl"

    When I execute the procedure at Segments_initStorage for no more than 100 instructions

    When I set register A to 1
    When I set register X to 0
    When I set register Y to kSpanPlot_maxHeight
    
    When I execute the procedure at Segments_scanPoly for no more than 100 instructions

    Then I expect to see ZPSegments_primaryAllocatorAddrLo equal lo(Segments_array+(1*kSegment_length))
    Then I expect to see ZPSegments_primaryAllocatorAddrHi equal hi(Segments_array+(1*kSegment_length))
    
    Then I expect to see Segments_linesLo equal lo(Segments_array)
    Then I expect to see Segments_linesHi equal hi(Segments_array)
    
    Then I expect to see Segments_array+kSegment_offset_nextHi equal 0
    Then I expect to see Segments_array+kSegment_offset_left equal <left>
    Then I expect to see Segments_array+kSegment_offset_right equal <right>
    Then I expect to see Segments_array+kSegment_offset_colour equal 1
    
    When I hex dump memory between Segments_array and Segments_arrayEnd
  Examples:
    | left               | right              |
	| 0                  | kSpanPlot_maxLen   |
	| kSpanPlot_maxLen   | kSpanPlot_maxLen/2 |
	| kSpanPlot_maxLen/3 | kSpanPlot_maxLen/2 |
	| kSpanPlot_maxLen/3 | kSpanPlot_maxLen   |



  Scenario Outline: Simple clip addition test, one existing segment, new segment with different colour on the end
    Given I have a simple overclocked 6502 system
    And I create file "t.a" with
      """
      kSpanPlot_maxHeight = 1
      !source "SegmentsTest1.a"

      Poly_linesLeft
        !by <new left>
      Poly_linesLeftEnd

      Poly_linesRight
        !by <new right>
      Poly_linesRightEnd
      
      """
    And I run the command line: ..\acme.exe -o t.prg --labeldump t.lbl -f cbm t.a
    And I load prg "t.prg"
    And I load labels "t.lbl"

    When I execute the procedure at Segments_initStorage for no more than 100 instructions

	# Setup the existing span to test with
    When I set register A to 0
    When I set register X to <existing left>
    When I set register Y to <existing right>
    When I execute the procedure at SetupTest_LeftRightHead for no more than 100 instructions

	# Check no brk encountered
	Then I expect register ST exclude stI

	# Check existing data is correct
    Then I expect to see ZPSegments_primaryAllocatorAddrLo equal lo(Segments_array+(1*kSegment_length))
    Then I expect to see ZPSegments_primaryAllocatorAddrHi equal hi(Segments_array+(1*kSegment_length))

    Then I expect to see Segments_linesLo equal lo(Segments_array+(0*kSegment_length))
    Then I expect to see Segments_linesHi equal hi(Segments_array+(0*kSegment_length))

    Then I expect to see Segments_array+(0*kSegment_length)+kSegment_offset_nextHi equal 0
    Then I expect to see Segments_array+(0*kSegment_length)+kSegment_offset_left equal <existing left>
    Then I expect to see Segments_array+(0*kSegment_length)+kSegment_offset_right equal <existing right>
    Then I expect to see Segments_array+(0*kSegment_length)+kSegment_offset_colour equal 0

	# Test with new span
    When I set register A to 1
    When I set register X to 0
    When I set register Y to kSpanPlot_maxHeight
    When I execute the procedure at Segments_scanPoly for no more than 100 instructions
    When I hex dump memory between Segments_array and Segments_arrayEnd

	# Check no brk encountered
	Then I expect register ST exclude stI

	# Check expected data is correct
    Then I expect to see ZPSegments_primaryAllocatorAddrLo equal lo(Segments_array+(2*kSegment_length))
    Then I expect to see ZPSegments_primaryAllocatorAddrHi equal hi(Segments_array+(2*kSegment_length))

    Then I expect to see Segments_linesLo equal lo(Segments_array+(0*kSegment_length))
    Then I expect to see Segments_linesHi equal hi(Segments_array+(0*kSegment_length))

    Then I expect to see Segments_array+(0*kSegment_length)+kSegment_offset_nextLo equal lo(Segments_array+(1*kSegment_length))
    Then I expect to see Segments_array+(0*kSegment_length)+kSegment_offset_nextHi equal hi(Segments_array+(1*kSegment_length))
    Then I expect to see Segments_array+(0*kSegment_length)+kSegment_offset_left equal <existing left>
    Then I expect to see Segments_array+(0*kSegment_length)+kSegment_offset_right equal <existing right>
    Then I expect to see Segments_array+(0*kSegment_length)+kSegment_offset_colour equal 0

    Then I expect to see Segments_array+(1*kSegment_length)+kSegment_offset_nextHi equal 0
    Then I expect to see Segments_array+(1*kSegment_length)+kSegment_offset_left equal <expected second left>
    Then I expect to see Segments_array+(1*kSegment_length)+kSegment_offset_right equal <expected second right>
    Then I expect to see Segments_array+(1*kSegment_length)+kSegment_offset_colour equal 1

  Examples:
    | existing left | existing right   | new left | new right | expected second left | expected second right |
	| 10            | 30               | 30       | 40        | 30                   | 40                    |
	| 10            | 30               | 40       | 50        | 40                   | 50                    |
	| 10            | 30               | 20       | 50        | 30                   | 50                    |



  Scenario Outline: Simple clip addition test, one existing segment, new segment with different colour on the start
    Given I have a simple overclocked 6502 system
    And I create file "t.a" with
      """
      kSpanPlot_maxHeight = 1
      !source "SegmentsTest1.a"

      Poly_linesLeft
        !by <new left>
      Poly_linesLeftEnd

      Poly_linesRight
        !by <new right>
      Poly_linesRightEnd
      
      """
    And I run the command line: ..\acme.exe -o t.prg --labeldump t.lbl -f cbm t.a
    And I load prg "t.prg"
    And I load labels "t.lbl"

    When I execute the procedure at Segments_initStorage for no more than 100 instructions

	# Setup the existing span to test with
    When I set register A to 0
    When I set register X to <existing left>
    When I set register Y to <existing right>
    When I execute the procedure at SetupTest_LeftRightHead for no more than 100 instructions

	# Check no brk encountered
	Then I expect register ST exclude stI

	# Check existing data is correct
    Then I expect to see ZPSegments_primaryAllocatorAddrLo equal lo(Segments_array+(1*kSegment_length))
    Then I expect to see ZPSegments_primaryAllocatorAddrHi equal hi(Segments_array+(1*kSegment_length))

    Then I expect to see Segments_linesLo equal lo(Segments_array+(0*kSegment_length))
    Then I expect to see Segments_linesHi equal hi(Segments_array+(0*kSegment_length))

    Then I expect to see Segments_array+(0*kSegment_length)+kSegment_offset_nextHi equal 0
    Then I expect to see Segments_array+(0*kSegment_length)+kSegment_offset_left equal <existing left>
    Then I expect to see Segments_array+(0*kSegment_length)+kSegment_offset_right equal <existing right>
    Then I expect to see Segments_array+(0*kSegment_length)+kSegment_offset_colour equal 0

	# Test with new span
    When I set register A to 1
    When I set register X to 0
    When I set register Y to kSpanPlot_maxHeight
    When I execute the procedure at Segments_scanPoly for no more than 100 instructions
    When I hex dump memory between Segments_array and Segments_arrayEnd

	# Check no brk encountered
	Then I expect register ST exclude stI

	# Check expected data is correct
    Then I expect to see ZPSegments_primaryAllocatorAddrLo equal lo(Segments_array+(2*kSegment_length))
    Then I expect to see ZPSegments_primaryAllocatorAddrHi equal hi(Segments_array+(2*kSegment_length))

    Then I expect to see Segments_linesLo equal lo(Segments_array+(1*kSegment_length))
    Then I expect to see Segments_linesHi equal hi(Segments_array+(1*kSegment_length))

    Then I expect to see Segments_array+(1*kSegment_length)+kSegment_offset_nextLo equal lo(Segments_array+(0*kSegment_length))
    Then I expect to see Segments_array+(1*kSegment_length)+kSegment_offset_nextHi equal hi(Segments_array+(0*kSegment_length))
    Then I expect to see Segments_array+(1*kSegment_length)+kSegment_offset_left equal <expected first left>
    Then I expect to see Segments_array+(1*kSegment_length)+kSegment_offset_right equal <expected first right>
    Then I expect to see Segments_array+(1*kSegment_length)+kSegment_offset_colour equal 1

    Then I expect to see Segments_array+(0*kSegment_length)+kSegment_offset_nextHi equal 0
    Then I expect to see Segments_array+(0*kSegment_length)+kSegment_offset_left equal <existing left>
    Then I expect to see Segments_array+(0*kSegment_length)+kSegment_offset_right equal <existing right>
    Then I expect to see Segments_array+(0*kSegment_length)+kSegment_offset_colour equal 0

  Examples:
    | existing left | existing right   | new left | new right | expected first left  | expected first right |
	| 100           | 130              | 30       | 40        | 30                   | 40                   |
