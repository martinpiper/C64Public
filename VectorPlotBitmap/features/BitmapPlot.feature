Feature: Bitmap plotting tests

  Tests the bitmap plotting routines for expected screen output

  Scenario: Simple line plot draw test
    Given clear all external devices
    Given a new C64 video display
    And show C64 video window
    And force C64 displayed bank to 3
    Given video display processes 24 pixels per instruction
    Given video display refresh window every 32 instructions
    And C64 video display saves debug BMP images to leaf filename "target/frames/TC-1-C64-"

    Given I have a simple overclocked 6502 system
#    Given I am using C64 processor port options
    Given add C64 hardware
    And I create file "t.a" with
      """
      !source "../stdlib/stdlib.a"

      kSpanCalc_backgroundColour = 0
      kSpanPlot_maxHeight = 16	; Limited size execution
;      kSpanPlot_maxHeight = 1	; Very limited size execution
      SpanPlot_MaskEnds	= 1
      ;EnableInterlace		= 1
      Bitmap0Memory = $2000
;      Segments_linesBeenEmptyFor_Max = 2

      !source "BitmapPlotMacros.a"
      ZPBitmap_VarBase			= $02
      !source "BitmapPlotVars.a"
      ZPSegments_VarBase	= ZPBitmap_endVars
      !source "SegmentsVars.a"
      !source "SegmentsMacros.a"
      !source "SpanCalcMacros.a"

      * = $4000
      realC64
        sei
        lda #ProcessorPortAllRAMWithIO
        sta ZPProcessorPort
        jsr VICInit
        jsr init
        jsr doPlot
        jmp *

      VICInit
        +SetDefaultScreen_A
        lda #VIC2ScreenControlVDefault | kVIC2ScreenControlVBits_Bitmap
        sta VIC2ScreenControlV
      	+ClearScreenAt_AX COLOURRAM , VIC2Colour_White
      	+ClearScreenAt_AX SCREENRAM , VIC2Colour_Grey | (VIC2Colour_DarkGrey << 4)
        lda #VIC2ScreenControlHDefault | kVIC2ScreenControlHBits_Multicolour
        sta VIC2ScreenControlH
        +MVIC2MemorySetup_ScreenBitmap_A SCREENRAM , Bitmap0Memory
        sta VIC2MemorySetup
        lda #VIC2Colour_Blue
        sta VIC2BorderColour
        lda #VIC2Colour_Black
        sta VIC2ScreenColour
        rts

      init
      	lda #>Bitmap0Memory
;      	ldy #%10000100
      	ldy #0
      	jsr BitmapPlot_clear

      	lda #>Bitmap_paletteStart
      	sta ZPBitmap_PaletteHi

      	jsr Segments_initStorage
      	
      	rts

      doPlot
      	lda #4	; Dithering colours
      	ldx #0
      	ldy #kSpanPlot_maxHeight
      	jsr Segments_processLines

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
    And I run the command line: ..\acme.exe --pdb t.pdb -o t.prg --labeldump t.lbl -f cbm t.a
    And I load prg "t.prg"
    And I load labels "t.lbl"

    When I execute the procedure at VICInit for no more than 9924720 instructions
    When I execute the procedure at init for no more than 24720 instructions
    When I execute the procedure at doPlot for no more than 3205 instructions

    And render a C64 video display frame
    And render a C64 video display frame

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


    Then expect image "testdata/TC-1-C64-000012.bmp" to be identical to "target/frames/TC-1-C64-000012.bmp"




  Scenario: Poly scans, edge trace and plot test
    Given I have a simple overclocked 6502 system
    And I create file "t.a" with
      """
      !source "../stdlib/stdlib.a"

      kSpanCalc_backgroundColour = 0
      kSpanPlot_maxHeight = 64
      SpanPlot_MaskEnds	= 1
      ;EnableInterlace		= 1
      Bitmap0Memory = $2000


      ZPBitmap_VarBase			= $02
      !source "BitmapPlotVars.a"
      ZPSegments_VarBase	= ZPBitmap_endVars
      !source "SegmentsVars.a"
      ZPPoly2D_VarBase	= ZPSegments_endVars
      !source "Poly2DVars.a"

      !source "BitmapPlotMacros.a"
      !source "SegmentsMacros.a"
      !source "SpanCalcMacros.a"
      !source "Poly2DMacros.a"

      * = $4000
      init
      	lda #>Bitmap0Memory
      	ldy #%10000100
      	jsr BitmapPlot_clear

      	lda #>Bitmap_paletteStart
      	sta ZPBitmap_PaletteHi

      	jsr Segments_initStorage
      	
      	rts

      doTrace1
       	; Setup 2D coords
      	lda #15
      	sta Poly2D_vertexBufferX
      	lda #0
      	sta Poly2D_vertexBufferY
      
      	lda #43
      	sta Poly2D_vertexBufferX+1
      	lda #16
      	sta Poly2D_vertexBufferY+1
      
      	lda #38
      	sta Poly2D_vertexBufferX+2
      	lda #32
      	sta Poly2D_vertexBufferY+2
      
      	jsr Poly2D_scanTriangle
      
      	lda #1
      	jsr Segments_processLines
		rts
      
      doTrace2
      	; Setup 2D coords
      	lda #25
      	sta Poly2D_vertexBufferX
      	lda #8
      	sta Poly2D_vertexBufferY
      
      	lda #63
      	sta Poly2D_vertexBufferX+1
      	lda #20
      	sta Poly2D_vertexBufferY+1
      
      	lda #0
      	sta Poly2D_vertexBufferX+2
      	lda #28
      	sta Poly2D_vertexBufferY+2
      
      	jsr Poly2D_scanTriangle
      
      	lda #2
      	jsr Segments_processLines
      	rts

      doPlot
      	+MWordValueToAddress_A Bitmap0Memory , ZPBitmap_CurrentPos
      	jsr BitmapPlot_drawSpans
      	
      	rts

      !source "BitmapPlot.a"
      !source "SegmentsMemory.a"
      !source "Segments.a"
      !source "Poly2D.a"
      
      SpanBanks_p0
      SpanBanks_p1
      SpanBanks_p2
      SpanBanks_p3

      !source "Poly2DMem.a"
      !source "SpanTable.a"
      """
    And I run the command line: ..\acme.exe -o t.prg --labeldump t.lbl -f cbm t.a
    And I load prg "t.prg"
    And I load labels "t.lbl"

    When I execute the procedure at init for no more than 25000 instructions

    When I execute the procedure at doTrace1 for no more than 25000 instructions
    When I hex dump memory between ZPSegments_primaryAllocatorAddrLo and ZPSegments_primaryAllocatorAddrHi+1
    When I hex dump memory between Segments_array and Segments_arrayEnd
    When I hex dump memory between Segments_linesLo and Segments_linesLoEnd
    When I hex dump memory between Segments_linesHi and Segments_linesHiEnd

    When I execute the procedure at doTrace2 for no more than 25000 instructions
    When I hex dump memory between ZPSegments_primaryAllocatorAddrLo and ZPSegments_primaryAllocatorAddrHi+1
    When I hex dump memory between Segments_array and Segments_arrayEnd
    When I hex dump memory between Segments_linesLo and Segments_linesLoEnd
    When I hex dump memory between Segments_linesHi and Segments_linesHiEnd

    When I execute the procedure at doPlot for no more than 10000 instructions



#    Then I expect to see ZPSegments_primaryAllocatorAddrLo equal lo(Segments_array+(kSpanPlot_maxHeight*kSegment_length))
#    Then I expect to see ZPSegments_primaryAllocatorAddrHi equal hi(Segments_array+(kSpanPlot_maxHeight*kSegment_length))

