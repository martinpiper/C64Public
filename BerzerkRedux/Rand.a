!zn {
; A/Y = Seed, 16 bits
Seed
	sta .seeds
	eor #%11010110
	sta .seeds+1
	eor #%00110110
	sta .seeds+2
	eor #%10100011
	sta .seeds+3
	tya
	eor .seeds
	sta .seeds
	tya
	eor .seeds+1
	sta .seeds+1
	tya
	eor .seeds+2
	sta .seeds+2
	tya
	eor .seeds+3
	sta .seeds+3
	rts
.seeds !by %11010110,%00110110,%10100011,%10101010

Rand
	; Rotate over 32 bits
	lda .seeds
	lsr
	ror .seeds+3
	ror .seeds+2
	ror .seeds+1
	ror .seeds
	; Then incr and decr various seeds at different rates
	inc .seeds
	inc .seeds+1
	inc .seeds+1
	dec .seeds+2
	dec .seeds+3
	dec .seeds+3
	; Finally generate a "random" number from the seeds
	lda .seeds
	adc .seeds+1
	sbc .seeds+2
	eor .seeds+3
	rts
}
