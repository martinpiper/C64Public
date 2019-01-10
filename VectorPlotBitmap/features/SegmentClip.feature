Feature: Segment clip tests

  Test the segment clip code

  Scenario Outline: Simple clip insertion test, one existing segment, new segment completely obscured by first span, no new segment
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
    Then I expect to see ZPSegments_primaryAllocatorAddrLo equal lo(Segments_array+(1*kSegment_length))
    Then I expect to see ZPSegments_primaryAllocatorAddrHi equal hi(Segments_array+(1*kSegment_length))

    Then I expect to see Segments_linesLo equal lo(Segments_array+(0*kSegment_length))
    Then I expect to see Segments_linesHi equal hi(Segments_array+(0*kSegment_length))

    Then I expect to see Segments_array+(0*kSegment_length)+kSegment_offset_nextHi equal 0
    Then I expect to see Segments_array+(0*kSegment_length)+kSegment_offset_left equal <existing left>
    Then I expect to see Segments_array+(0*kSegment_length)+kSegment_offset_right equal <existing right>
    Then I expect to see Segments_array+(0*kSegment_length)+kSegment_offset_colour equal 0

  Examples:
    | existing left | existing right   | new left | new right            |
	| 0             | kSpanPlot_maxLen | 0        | kSpanPlot_maxLen     |
	| 0             | kSpanPlot_maxLen | 3        | kSpanPlot_maxLen-3   |
	| 0             | kSpanPlot_maxLen | 3        | kSpanPlot_maxLen     |
	| 0             | kSpanPlot_maxLen | 0        | kSpanPlot_maxLen-3   |
	| 10            | 30               | 10       | 30                   |
	| 10            | 30               | 15       | 30                   |
	| 10            | 30               | 10       | 25                   |
	| 10            | 30               | 15       | 25                   |




  Scenario Outline: Simple clip insertion test, existing segments, new segment completely obscured by spans, no new segment
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

    When I set register A to 1
    When I set register X to <existing left2>
    When I set register Y to <existing right2>
    When I execute the procedure at SetupTest_LeftRightAddLast for no more than 100 instructions

    When I set register A to 2
    When I set register X to <existing left3>
    When I set register Y to <existing right3>
    When I execute the procedure at SetupTest_LeftRightAddLast for no more than 100 instructions

	# Check no brk encountered
	Then I expect register ST exclude stI

	# Check existing data is correct
    Then I expect to see ZPSegments_primaryAllocatorAddrLo equal lo(Segments_array+(3*kSegment_length))
    Then I expect to see ZPSegments_primaryAllocatorAddrHi equal hi(Segments_array+(3*kSegment_length))

    Then I expect to see Segments_linesLo equal lo(Segments_array+(0*kSegment_length))
    Then I expect to see Segments_linesHi equal hi(Segments_array+(0*kSegment_length))

    Then I expect to see Segments_array+(0*kSegment_length)+kSegment_offset_nextLo equal lo(Segments_array+(1*kSegment_length))
    Then I expect to see Segments_array+(0*kSegment_length)+kSegment_offset_nextHi equal hi(Segments_array+(1*kSegment_length))
    Then I expect to see Segments_array+(0*kSegment_length)+kSegment_offset_left equal <existing left>
    Then I expect to see Segments_array+(0*kSegment_length)+kSegment_offset_right equal <existing right>
    Then I expect to see Segments_array+(0*kSegment_length)+kSegment_offset_colour equal 0

    Then I expect to see Segments_array+(1*kSegment_length)+kSegment_offset_nextLo equal lo(Segments_array+(2*kSegment_length))
    Then I expect to see Segments_array+(1*kSegment_length)+kSegment_offset_nextHi equal hi(Segments_array+(2*kSegment_length))
    Then I expect to see Segments_array+(1*kSegment_length)+kSegment_offset_left equal <existing left2>
    Then I expect to see Segments_array+(1*kSegment_length)+kSegment_offset_right equal <existing right2>
    Then I expect to see Segments_array+(1*kSegment_length)+kSegment_offset_colour equal 1

    Then I expect to see Segments_array+(2*kSegment_length)+kSegment_offset_nextHi equal 0
    Then I expect to see Segments_array+(2*kSegment_length)+kSegment_offset_left equal <existing left3>
    Then I expect to see Segments_array+(2*kSegment_length)+kSegment_offset_right equal <existing right3>
    Then I expect to see Segments_array+(2*kSegment_length)+kSegment_offset_colour equal 2

	# Test with new span
    When I set register A to 3
    When I set register X to 0
    When I set register Y to kSpanPlot_maxHeight
    When I execute the procedure at Segments_scanPoly for no more than 150 instructions
    When I hex dump memory between Segments_array and Segments_arrayEnd

	# Check no brk encountered
	Then I expect register ST exclude stI

	# Check expected data is correct
    Then I expect to see ZPSegments_primaryAllocatorAddrLo equal lo(Segments_array+(3*kSegment_length))
    Then I expect to see ZPSegments_primaryAllocatorAddrHi equal hi(Segments_array+(3*kSegment_length))

    Then I expect to see Segments_linesLo equal lo(Segments_array+(0*kSegment_length))
    Then I expect to see Segments_linesHi equal hi(Segments_array+(0*kSegment_length))

    Then I expect to see Segments_array+(0*kSegment_length)+kSegment_offset_nextLo equal lo(Segments_array+(1*kSegment_length))
    Then I expect to see Segments_array+(0*kSegment_length)+kSegment_offset_nextHi equal hi(Segments_array+(1*kSegment_length))
    Then I expect to see Segments_array+(0*kSegment_length)+kSegment_offset_left equal <existing left>
    Then I expect to see Segments_array+(0*kSegment_length)+kSegment_offset_right equal <existing right>
    Then I expect to see Segments_array+(0*kSegment_length)+kSegment_offset_colour equal 0

    Then I expect to see Segments_array+(1*kSegment_length)+kSegment_offset_nextLo equal lo(Segments_array+(2*kSegment_length))
    Then I expect to see Segments_array+(1*kSegment_length)+kSegment_offset_nextHi equal hi(Segments_array+(2*kSegment_length))
    Then I expect to see Segments_array+(1*kSegment_length)+kSegment_offset_left equal <existing left2>
    Then I expect to see Segments_array+(1*kSegment_length)+kSegment_offset_right equal <existing right2>
    Then I expect to see Segments_array+(1*kSegment_length)+kSegment_offset_colour equal 1

    Then I expect to see Segments_array+(2*kSegment_length)+kSegment_offset_nextHi equal 0
    Then I expect to see Segments_array+(2*kSegment_length)+kSegment_offset_left equal <existing left3>
    Then I expect to see Segments_array+(2*kSegment_length)+kSegment_offset_right equal <existing right3>
    Then I expect to see Segments_array+(2*kSegment_length)+kSegment_offset_colour equal 2


  Examples:
    | existing left | existing right   | existing left2 | existing right2 | existing left3 | existing right3  | new left | new right            |
	| 0             | 30               | 30             | 50              | 50             | kSpanPlot_maxLen | 0        | kSpanPlot_maxLen     |
	| 0             | 30               | 30             | 50              | 50             | kSpanPlot_maxLen | 3        | kSpanPlot_maxLen-3   |
	| 0             | 30               | 30             | 50              | 50             | kSpanPlot_maxLen | 3        | kSpanPlot_maxLen     |
	| 0             | 30               | 30             | 50              | 50             | kSpanPlot_maxLen | 0        | kSpanPlot_maxLen-3   |
	| 10            | 20               | 20             | 25              | 25             | 30               | 10       | 30                   |
	| 10            | 20               | 20             | 25              | 25             | 30               | 15       | 30                   |
	| 10            | 20               | 20             | 25              | 25             | 30               | 10       | 25                   |
	| 10            | 20               | 20             | 25              | 25             | 30               | 15       | 25                   |
	| 10            | 20               | 20             | 25              | 25             | 30               | 15       | 25                   |
	| 10            | 20               | 20             | 25              | 25             | 30               | 20       | 30                   |



  Scenario Outline: Simple clip insertion test, existing segments all the same colour, new segment also of the same colour causes all segments to be merged, no new segment
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

    When I set register A to 0
    When I set register X to <existing left2>
    When I set register Y to <existing right2>
    When I execute the procedure at SetupTest_LeftRightAddLast for no more than 100 instructions

    When I set register A to 0
    When I set register X to <existing left3>
    When I set register Y to <existing right3>
    When I execute the procedure at SetupTest_LeftRightAddLast for no more than 100 instructions

	# Check no brk encountered
	Then I expect register ST exclude stI

	# Check existing data is correct
    Then I expect to see ZPSegments_primaryAllocatorAddrLo equal lo(Segments_array+(3*kSegment_length))
    Then I expect to see ZPSegments_primaryAllocatorAddrHi equal hi(Segments_array+(3*kSegment_length))

    Then I expect to see Segments_linesLo equal lo(Segments_array+(0*kSegment_length))
    Then I expect to see Segments_linesHi equal hi(Segments_array+(0*kSegment_length))

    Then I expect to see Segments_array+(0*kSegment_length)+kSegment_offset_nextLo equal lo(Segments_array+(1*kSegment_length))
    Then I expect to see Segments_array+(0*kSegment_length)+kSegment_offset_nextHi equal hi(Segments_array+(1*kSegment_length))
    Then I expect to see Segments_array+(0*kSegment_length)+kSegment_offset_left equal <existing left>
    Then I expect to see Segments_array+(0*kSegment_length)+kSegment_offset_right equal <existing right>
    Then I expect to see Segments_array+(0*kSegment_length)+kSegment_offset_colour equal 0

    Then I expect to see Segments_array+(1*kSegment_length)+kSegment_offset_nextLo equal lo(Segments_array+(2*kSegment_length))
    Then I expect to see Segments_array+(1*kSegment_length)+kSegment_offset_nextHi equal hi(Segments_array+(2*kSegment_length))
    Then I expect to see Segments_array+(1*kSegment_length)+kSegment_offset_left equal <existing left2>
    Then I expect to see Segments_array+(1*kSegment_length)+kSegment_offset_right equal <existing right2>
    Then I expect to see Segments_array+(1*kSegment_length)+kSegment_offset_colour equal 0

    Then I expect to see Segments_array+(2*kSegment_length)+kSegment_offset_nextHi equal 0
    Then I expect to see Segments_array+(2*kSegment_length)+kSegment_offset_left equal <existing left3>
    Then I expect to see Segments_array+(2*kSegment_length)+kSegment_offset_right equal <existing right3>
    Then I expect to see Segments_array+(2*kSegment_length)+kSegment_offset_colour equal 0

	# Test with new span
    When I set register A to 0
    When I set register X to 0
    When I set register Y to kSpanPlot_maxHeight
    When I execute the procedure at Segments_scanPoly for no more than 1050 instructions
    When I hex dump memory between Segments_array and Segments_arrayEnd

	# Check no brk encountered
	Then I expect register ST exclude stI

	# Check expected data is correct
    Then I expect to see ZPSegments_primaryAllocatorAddrLo equal lo(Segments_array+(3*kSegment_length))
    Then I expect to see ZPSegments_primaryAllocatorAddrHi equal hi(Segments_array+(3*kSegment_length))

    Then I expect to see Segments_linesLo equal lo(Segments_array+(0*kSegment_length))
    Then I expect to see Segments_linesHi equal hi(Segments_array+(0*kSegment_length))

    Then I expect to see Segments_array+(0*kSegment_length)+kSegment_offset_nextHi equal 0
    Then I expect to see Segments_array+(0*kSegment_length)+kSegment_offset_left equal <new left>
    Then I expect to see Segments_array+(0*kSegment_length)+kSegment_offset_right equal <new right>
    Then I expect to see Segments_array+(0*kSegment_length)+kSegment_offset_colour equal 0

  Examples:
    | existing left | existing right   | existing left2 | existing right2 | existing left3 | existing right3  | new left | new right            |
	| 0             | 30               | 40             | 50              | 60             | kSpanPlot_maxLen | 0        | kSpanPlot_maxLen     |
	| 10            | 30               | 40             | 50              | 60             | 130              | 0        | kSpanPlot_maxLen     |




  Scenario Outline: Segment extension test, one existing segment, new segment has the same colour and extends one or both ends of the first segment, no new segment
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
    When I set register A to 0
    When I set register X to 0
    When I set register Y to kSpanPlot_maxHeight
    When I execute the procedure at Segments_scanPoly for no more than 100 instructions
    When I hex dump memory between Segments_array and Segments_arrayEnd

	# Check no brk encountered
	Then I expect register ST exclude stI

	# Check expected data is correct
    Then I expect to see ZPSegments_primaryAllocatorAddrLo equal lo(Segments_array+(1*kSegment_length))
    Then I expect to see ZPSegments_primaryAllocatorAddrHi equal hi(Segments_array+(1*kSegment_length))

    Then I expect to see Segments_linesLo equal lo(Segments_array+(0*kSegment_length))
    Then I expect to see Segments_linesHi equal hi(Segments_array+(0*kSegment_length))

    Then I expect to see Segments_array+(0*kSegment_length)+kSegment_offset_nextHi equal 0
    Then I expect to see Segments_array+(0*kSegment_length)+kSegment_offset_left equal <expected left>
    Then I expect to see Segments_array+(0*kSegment_length)+kSegment_offset_right equal <expected right>
    Then I expect to see Segments_array+(0*kSegment_length)+kSegment_offset_colour equal 0

  Examples:
    | existing left | existing right | new left | new right | expected left | expected right |
	| 50            | 130            | 40       | 60        | 40            | 130            |
	| 50            | 130            | 60       | 140       | 50            | 140            |
	| 50            | 130            | 40       | 140       | 40            | 140            |


  @ignore
  Scenario Outline: Segment extension test, existing segments, new segment has different colour and it clipped, new segment.
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

    When I set register A to 1
    When I set register X to <existing left2>
    When I set register Y to <existing right2>
    When I execute the procedure at SetupTest_LeftRightAddLast for no more than 100 instructions

    When I set register A to 2
    When I set register X to <existing left3>
    When I set register Y to <existing right3>
    When I execute the procedure at SetupTest_LeftRightAddLast for no more than 100 instructions

	# Check no brk encountered
	Then I expect register ST exclude stI

	# Check existing data is correct
    Then I expect to see ZPSegments_primaryAllocatorAddrLo equal lo(Segments_array+(3*kSegment_length))
    Then I expect to see ZPSegments_primaryAllocatorAddrHi equal hi(Segments_array+(3*kSegment_length))

    Then I expect to see Segments_linesLo equal lo(Segments_array+(0*kSegment_length))
    Then I expect to see Segments_linesHi equal hi(Segments_array+(0*kSegment_length))

    Then I expect to see Segments_array+(0*kSegment_length)+kSegment_offset_nextLo equal lo(Segments_array+(1*kSegment_length))
    Then I expect to see Segments_array+(0*kSegment_length)+kSegment_offset_nextHi equal hi(Segments_array+(1*kSegment_length))
    Then I expect to see Segments_array+(0*kSegment_length)+kSegment_offset_left equal <existing left>
    Then I expect to see Segments_array+(0*kSegment_length)+kSegment_offset_right equal <existing right>
    Then I expect to see Segments_array+(0*kSegment_length)+kSegment_offset_colour equal 0

    Then I expect to see Segments_array+(1*kSegment_length)+kSegment_offset_nextLo equal lo(Segments_array+(2*kSegment_length))
    Then I expect to see Segments_array+(1*kSegment_length)+kSegment_offset_nextHi equal hi(Segments_array+(2*kSegment_length))
    Then I expect to see Segments_array+(1*kSegment_length)+kSegment_offset_left equal <existing left2>
    Then I expect to see Segments_array+(1*kSegment_length)+kSegment_offset_right equal <existing right2>
    Then I expect to see Segments_array+(1*kSegment_length)+kSegment_offset_colour equal 1

    Then I expect to see Segments_array+(2*kSegment_length)+kSegment_offset_nextHi equal 0
    Then I expect to see Segments_array+(2*kSegment_length)+kSegment_offset_left equal <existing left3>
    Then I expect to see Segments_array+(2*kSegment_length)+kSegment_offset_right equal <existing right3>
    Then I expect to see Segments_array+(2*kSegment_length)+kSegment_offset_colour equal 2

	# Test with new span
    When I set register A to 3
    When I set register X to 0
    When I set register Y to kSpanPlot_maxHeight
    When I execute the procedure at Segments_scanPoly for no more than 100 instructions
    When I hex dump memory between Segments_array and Segments_arrayEnd

	# Check no brk encountered
	Then I expect register ST exclude stI

	# Check expected data is correct
    Then I expect to see ZPSegments_primaryAllocatorAddrLo equal lo(Segments_array+(4*kSegment_length))
    Then I expect to see ZPSegments_primaryAllocatorAddrHi equal hi(Segments_array+(4*kSegment_length))

    Then I expect to see Segments_linesLo equal lo(Segments_array+(3*kSegment_length))
    Then I expect to see Segments_linesHi equal hi(Segments_array+(3*kSegment_length))

    Then I expect to see Segments_array+(3*kSegment_length)+kSegment_offset_nextLo equal lo(Segments_array+(0*kSegment_length))
    Then I expect to see Segments_array+(3*kSegment_length)+kSegment_offset_nextHi equal hi(Segments_array+(0*kSegment_length))
    Then I expect to see Segments_array+(3*kSegment_length)+kSegment_offset_left equal <expected left1>
    Then I expect to see Segments_array+(3*kSegment_length)+kSegment_offset_right equal <expected right1>
    Then I expect to see Segments_array+(3*kSegment_length)+kSegment_offset_colour equal 3

    Then I expect to see Segments_array+(0*kSegment_length)+kSegment_offset_nextLo equal lo(Segments_array+(1*kSegment_length))
    Then I expect to see Segments_array+(0*kSegment_length)+kSegment_offset_nextHi equal hi(Segments_array+(1*kSegment_length))
    Then I expect to see Segments_array+(0*kSegment_length)+kSegment_offset_left equal <existing left>
    Then I expect to see Segments_array+(0*kSegment_length)+kSegment_offset_right equal <existing right>
    Then I expect to see Segments_array+(0*kSegment_length)+kSegment_offset_colour equal 0

    Then I expect to see Segments_array+(1*kSegment_length)+kSegment_offset_nextLo equal lo(Segments_array+(2*kSegment_length))
    Then I expect to see Segments_array+(1*kSegment_length)+kSegment_offset_nextHi equal hi(Segments_array+(2*kSegment_length))
    Then I expect to see Segments_array+(1*kSegment_length)+kSegment_offset_left equal <existing left2>
    Then I expect to see Segments_array+(1*kSegment_length)+kSegment_offset_right equal <existing right2>
    Then I expect to see Segments_array+(1*kSegment_length)+kSegment_offset_colour equal 1

    Then I expect to see Segments_array+(2*kSegment_length)+kSegment_offset_nextHi equal 0
    Then I expect to see Segments_array+(2*kSegment_length)+kSegment_offset_left equal <existing left3>
    Then I expect to see Segments_array+(2*kSegment_length)+kSegment_offset_right equal <existing right3>
    Then I expect to see Segments_array+(2*kSegment_length)+kSegment_offset_colour equal 2


  Examples:
    | existing left | existing right   | existing left2 | existing right2 | existing left3 | existing right3  | new left | new right            | expected left1 | expected right1 |
	| 10            | 20               | 20             | 25              | 30             | 40               | 0        | 15                   | 0              | 10              |





  @ignore
  Scenario Outline: Segment extension test, one existing segment, new segment has the same colour and extends one or both ends of some segments, no new segment
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

    When I set register A to 1
    When I set register X to <existing left2>
    When I set register Y to <existing right2>
    When I execute the procedure at SetupTest_LeftRightAddLast for no more than 100 instructions

    When I set register A to 2
    When I set register X to <existing left3>
    When I set register Y to <existing right3>
    When I execute the procedure at SetupTest_LeftRightAddLast for no more than 100 instructions

	# Check no brk encountered
	Then I expect register ST exclude stI

	# Check existing data is correct
    Then I expect to see ZPSegments_primaryAllocatorAddrLo equal lo(Segments_array+(3*kSegment_length))
    Then I expect to see ZPSegments_primaryAllocatorAddrHi equal hi(Segments_array+(3*kSegment_length))

    Then I expect to see Segments_linesLo equal lo(Segments_array+(0*kSegment_length))
    Then I expect to see Segments_linesHi equal hi(Segments_array+(0*kSegment_length))

    Then I expect to see Segments_array+(0*kSegment_length)+kSegment_offset_nextLo equal lo(Segments_array+(1*kSegment_length))
    Then I expect to see Segments_array+(0*kSegment_length)+kSegment_offset_nextHi equal hi(Segments_array+(1*kSegment_length))
    Then I expect to see Segments_array+(0*kSegment_length)+kSegment_offset_left equal <existing left>
    Then I expect to see Segments_array+(0*kSegment_length)+kSegment_offset_right equal <existing right>
    Then I expect to see Segments_array+(0*kSegment_length)+kSegment_offset_colour equal 0

    Then I expect to see Segments_array+(1*kSegment_length)+kSegment_offset_nextLo equal lo(Segments_array+(2*kSegment_length))
    Then I expect to see Segments_array+(1*kSegment_length)+kSegment_offset_nextHi equal hi(Segments_array+(2*kSegment_length))
    Then I expect to see Segments_array+(1*kSegment_length)+kSegment_offset_left equal <existing left2>
    Then I expect to see Segments_array+(1*kSegment_length)+kSegment_offset_right equal <existing right2>
    Then I expect to see Segments_array+(1*kSegment_length)+kSegment_offset_colour equal 1

    Then I expect to see Segments_array+(2*kSegment_length)+kSegment_offset_nextHi equal 0
    Then I expect to see Segments_array+(2*kSegment_length)+kSegment_offset_left equal <existing left3>
    Then I expect to see Segments_array+(2*kSegment_length)+kSegment_offset_right equal <existing right3>
    Then I expect to see Segments_array+(2*kSegment_length)+kSegment_offset_colour equal 2

	# Test with new span
    When I set register A to 3
    When I set register X to 0
    When I set register Y to kSpanPlot_maxHeight
    When I execute the procedure at Segments_scanPoly for no more than 100 instructions
    When I hex dump memory between Segments_array and Segments_arrayEnd

	# Check no brk encountered
	Then I expect register ST exclude stI

	# Check expected data is correct
    Then I expect to see ZPSegments_primaryAllocatorAddrLo equal lo(Segments_array+(3*kSegment_length))
    Then I expect to see ZPSegments_primaryAllocatorAddrHi equal hi(Segments_array+(3*kSegment_length))

    Then I expect to see Segments_linesLo equal lo(Segments_array+(0*kSegment_length))
    Then I expect to see Segments_linesHi equal hi(Segments_array+(0*kSegment_length))

    Then I expect to see Segments_array+(0*kSegment_length)+kSegment_offset_nextLo equal lo(Segments_array+(1*kSegment_length))
    Then I expect to see Segments_array+(0*kSegment_length)+kSegment_offset_nextHi equal hi(Segments_array+(1*kSegment_length))
    Then I expect to see Segments_array+(0*kSegment_length)+kSegment_offset_left equal <existing left>
    Then I expect to see Segments_array+(0*kSegment_length)+kSegment_offset_right equal <existing right>
    Then I expect to see Segments_array+(0*kSegment_length)+kSegment_offset_colour equal 0

    Then I expect to see Segments_array+(1*kSegment_length)+kSegment_offset_nextLo equal lo(Segments_array+(2*kSegment_length))
    Then I expect to see Segments_array+(1*kSegment_length)+kSegment_offset_nextHi equal hi(Segments_array+(2*kSegment_length))
    Then I expect to see Segments_array+(1*kSegment_length)+kSegment_offset_left equal <existing left2>
    Then I expect to see Segments_array+(1*kSegment_length)+kSegment_offset_right equal <existing right2>
    Then I expect to see Segments_array+(1*kSegment_length)+kSegment_offset_colour equal 1

    Then I expect to see Segments_array+(2*kSegment_length)+kSegment_offset_nextHi equal 0
    Then I expect to see Segments_array+(2*kSegment_length)+kSegment_offset_left equal <existing left3>
    Then I expect to see Segments_array+(2*kSegment_length)+kSegment_offset_right equal <existing right3>
    Then I expect to see Segments_array+(2*kSegment_length)+kSegment_offset_colour equal 2


  Examples:
    | existing left | existing right   | existing left2 | existing right2 | existing left3 | existing right3  | new left | new right            |
	| 0             | 30               | 30             | 50              | 50             | kSpanPlot_maxLen | 0        | kSpanPlot_maxLen     |
