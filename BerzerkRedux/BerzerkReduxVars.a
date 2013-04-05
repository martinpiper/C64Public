; Initialise any constant values here
mainRoutineZP = $2
irqAStore	= mainRoutineZP
irqXStore	= mainRoutineZP+1
irqYStore	= mainRoutineZP+2
nmiAStore	= mainRoutineZP+3
nmiXStore	= mainRoutineZP+4
nmiYStore	= mainRoutineZP+5
mainAStore	= mainRoutineZP+6
mainXStore	= mainRoutineZP+7
mainYStore	= mainRoutineZP+8
tempWork0	= mainRoutineZP+9
tempWork1	= mainRoutineZP+10
tempWork2	= mainRoutineZP+11
tempWork3	= mainRoutineZP+12
tempWork4	= mainRoutineZP+13
tempWork5	= mainRoutineZP+14
tempWork6	= mainRoutineZP+15
tempWork7	= mainRoutineZP+16
tempWork8	= mainRoutineZP+17

; This must always be the next zero page unused address
endMainRoutineZP = mainRoutineZP+18


!macro SampleUpdateSequence .seq , .sampleStart , .sampleEnd {
	lda #<.sampleStart
	sta SampleStartLoTab + .seq
	lda #>.sampleStart
	sta SampleStartHiTab + .seq
	lda #<.sampleEnd
	sta SampleEndLoTab + .seq
	lda #>.sampleEnd
	sta SampleEndHiTab + .seq
}
