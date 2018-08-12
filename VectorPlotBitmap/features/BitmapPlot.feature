Feature: Bitmap plotting tests

  Tests the bitmap plotting routines for expected screen output

  Scenario: Simple line plot draw test
    Given I have a simple overclocked 6502 system
    And I create file "t.a" with
      """
      !source "../stdlib/stdlib.a"

      kSpanPlot_maxHeight = 16	; Limited size execution
      SpanPlot_MaskEnds	= 1
      ;EnableInterlace		= 1
      Bitmap0Memory = $2000

      !source "BitmapPlotMacros.a"
      !source "SpanCalcMacros.a"
      ZPBitmap_VarBase			= $02
      !source "BitmapPlotVars.a"
      ZPSpanCalc_VarBase			= ZPBitmap_endVars
      !source "SpanCalcVars.a"

      * = $4000
      init
      	lda #>Bitmap0Memory
      	ldy #%10000100
      	jsr BitmapPlot_clear

      	lda #>Bitmap_paletteStart
      	sta ZPBitmap_Palette+1

      	jsr SpanCalc_expireCleared
      	jsr SpanCalc_dirtyClear
      	
      	rts

      doPlot
      	lda #5	; Dithering colours
      	ldx #0
      	ldy #kSpanPlot_maxHeight
      	jsr SpanCalc_scanPoly

      	+MWordValueToAddress_A Bitmap0Memory , ZPBitmap_CurrentPos
      	jsr BitmapPlot_drawSpans
      	
      	rts

      !source "BitmapPlot.a"
      !source "SpanCalcMem.a"
      !source "SpanCalc.a"
      
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
    When I execute the procedure at doPlot for no more than 2600 instructions
    When I hex dump memory between SpanCalc_screenNumSegments and SpanCalc_screenNumSegmentsEnd
    When I hex dump memory between SpanCalc_screenSegmentsLen and SpanCalc_screenSegmentsLenEnd
    When I hex dump memory between SpanCalc_screenSegmentsColour and SpanCalc_screenSegmentsColourEnd

    Then I expect to see SpanCalc_screenNumSegments equal 2

    Then I expect to see SpanCalc_screenSegmentsLen equal 62
    Then I expect to see SpanCalc_screenSegmentsColour equal 5

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

