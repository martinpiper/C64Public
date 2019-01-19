Feature: Bitmap plotting tests

  Tests the bitmap plotting routines for expected screen output

  Scenario: Simple line plot draw test
    Given I have a simple overclocked 6502 system
    And I create file "t.a" with
      """
      !source "../stdlib/stdlib.a"

      kSpanCalc_backgroundColour = 0
      kSpanPlot_maxHeight = 16	; Limited size execution
;      kSpanPlot_maxHeight = 1	; Very limited size execution
      SpanPlot_MaskEnds	= 1
      ;EnableInterlace		= 1
      Bitmap0Memory = $2000

      !source "BitmapPlotMacros.a"
      ZPBitmap_VarBase			= $02
      !source "BitmapPlotVars.a"
      ZPSegments_VarBase	= ZPBitmap_endVars
      !source "SegmentsVars.a"
      !source "SegmentsMacros.a"
      !source "SpanCalcMacros.a"

      * = $4000
      init
      	lda #>Bitmap0Memory
      	ldy #%10000100
      	jsr BitmapPlot_clear

      	lda #>Bitmap_paletteStart
      	sta ZPBitmap_PaletteHi

      	jsr Segments_initStorage
      	
      	rts

      doPlot
      	lda #5	; Dithering colours
      	ldx #0
      	ldy #kSpanPlot_maxHeight
      	jsr Segments_scanPoly

      	+MWordValueToAddress_A Bitmap0Memory , ZPBitmap_CurrentPos
      	jsr BitmapPlot_drawSpans
      	
      	rts

      !source "BitmapPlot.a"
      !source "SegmentsMemory.a"
      !source "Segments.a"
      
      SpanBanks_p0
      SpanBanks_p1
      SpanBanks_p2
      SpanBanks_p3

      Poly_linesLeft
      	!for .i , kSpanPlot_maxHeight {
      		!by .i-1
      	}
      Poly_linesLeftEnd
      
      Poly_linesRight
      	!for .i , kSpanPlot_maxHeight {
      		!by 63 - .i
      	}
      Poly_linesRightEnd

      !source "SpanTable.a"
      """
    And I run the command line: ..\acme.exe -o t.prg --labeldump t.lbl -f cbm t.a
    And I load prg "t.prg"
    And I load labels "t.lbl"

    When I execute the procedure at init for no more than 25000 instructions
    When I execute the procedure at doPlot for no more than 3200 instructions

	When I hex dump memory between Segments_array and Segments_arrayEnd

    When I hex dump memory between Segments_linesLo and Segments_linesLoEnd
    When I hex dump memory between Segments_linesHi and Segments_linesHiEnd

    Then I expect to see ZPSegments_primaryAllocatorAddrLo equal lo(Segments_array+(kSpanPlot_maxHeight*kSegment_length))
    Then I expect to see ZPSegments_primaryAllocatorAddrHi equal hi(Segments_array+(kSpanPlot_maxHeight*kSegment_length))

    # Start slope
    When I hex dump memory between Bitmap0Memory+$00 and Bitmap0Memory+$08
    Then property "test.BDD6502.lastHexDump" must contain string ignoring whitespace "99 26 09 02 00 00 00 00"

    When I hex dump memory between Bitmap0Memory+$08 and Bitmap0Memory+$10
    Then property "test.BDD6502.lastHexDump" must contain string ignoring whitespace "99 66 99 66 99 26 09 02"

    When I hex dump memory between Bitmap0Memory+$10 and Bitmap0Memory+$18
    Then property "test.BDD6502.lastHexDump" must contain string ignoring whitespace "99 66 99 66 99 66 99 66"


    # End slope
    When I hex dump memory between Bitmap0Memory+$68 and Bitmap0Memory+$70
    Then property "test.BDD6502.lastHexDump" must contain string ignoring whitespace "99 66 99 66 99 66 99 64"
    When I hex dump memory between Bitmap0Memory+$70 and Bitmap0Memory+$78
    Then property "test.BDD6502.lastHexDump" must contain string ignoring whitespace "99 66 99 64 90 40 00 00"
    When I hex dump memory between Bitmap0Memory+$78 and Bitmap0Memory+$80
    Then property "test.BDD6502.lastHexDump" must contain string ignoring whitespace "90 40 00 00 00 00 00 00"

