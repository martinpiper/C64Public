rem copy /Y OriginalData\armalytemusic.tas t.a
rem copy /Y OriginalData\Citadel.tas t.a
copy /Y %1 t.a

..\ExternalTools\Gnu\bin\sed.exe -i "s/ADC/adc/g" t.a
..\ExternalTools\Gnu\bin\sed.exe -i "s/AND/and/g" t.a
..\ExternalTools\Gnu\bin\sed.exe -i "s/ASL/asl/g" t.a
..\ExternalTools\Gnu\bin\sed.exe -i "s/BCC/bcc/g" t.a
..\ExternalTools\Gnu\bin\sed.exe -i "s/BCS/bcs/g" t.a
..\ExternalTools\Gnu\bin\sed.exe -i "s/BEQ/beq/g" t.a
..\ExternalTools\Gnu\bin\sed.exe -i "s/BIT/bit/g" t.a
..\ExternalTools\Gnu\bin\sed.exe -i "s/BMI/bmi/g" t.a
..\ExternalTools\Gnu\bin\sed.exe -i "s/BNE/bne/g" t.a
..\ExternalTools\Gnu\bin\sed.exe -i "s/BPL/bpl/g" t.a
..\ExternalTools\Gnu\bin\sed.exe -i "s/BRK/brk/g" t.a
..\ExternalTools\Gnu\bin\sed.exe -i "s/BVC/bvc/g" t.a
..\ExternalTools\Gnu\bin\sed.exe -i "s/BVS/bvs/g" t.a
..\ExternalTools\Gnu\bin\sed.exe -i "s/CLC/clc/g" t.a
..\ExternalTools\Gnu\bin\sed.exe -i "s/CLD/cld/g" t.a
..\ExternalTools\Gnu\bin\sed.exe -i "s/CLI/cli/g" t.a
..\ExternalTools\Gnu\bin\sed.exe -i "s/CLV/clv/g" t.a
..\ExternalTools\Gnu\bin\sed.exe -i "s/CMP/cmp/g" t.a
..\ExternalTools\Gnu\bin\sed.exe -i "s/CPX/cpx/g" t.a
..\ExternalTools\Gnu\bin\sed.exe -i "s/CPY/cpy/g" t.a
..\ExternalTools\Gnu\bin\sed.exe -i "s/DEC/dec/g" t.a
..\ExternalTools\Gnu\bin\sed.exe -i "s/DEX/dex/g" t.a
..\ExternalTools\Gnu\bin\sed.exe -i "s/DEY/dey/g" t.a
..\ExternalTools\Gnu\bin\sed.exe -i "s/EOR/eor/g" t.a
..\ExternalTools\Gnu\bin\sed.exe -i "s/INC/inc/g" t.a
..\ExternalTools\Gnu\bin\sed.exe -i "s/INX/inx/g" t.a
..\ExternalTools\Gnu\bin\sed.exe -i "s/INY/iny/g" t.a
..\ExternalTools\Gnu\bin\sed.exe -i "s/JMP/jmp/g" t.a
..\ExternalTools\Gnu\bin\sed.exe -i "s/JSR/jsr/g" t.a
..\ExternalTools\Gnu\bin\sed.exe -i "s/LDA/lda/g" t.a
..\ExternalTools\Gnu\bin\sed.exe -i "s/STA/sta/g" t.a
..\ExternalTools\Gnu\bin\sed.exe -i "s/LDX/ldx/g" t.a
..\ExternalTools\Gnu\bin\sed.exe -i "s/LSR/lsr/g" t.a
..\ExternalTools\Gnu\bin\sed.exe -i "s/NOP/nop/g" t.a
..\ExternalTools\Gnu\bin\sed.exe -i "s/ORA/ora/g" t.a
..\ExternalTools\Gnu\bin\sed.exe -i "s/PHA/pha/g" t.a
..\ExternalTools\Gnu\bin\sed.exe -i "s/PHP/php/g" t.a
..\ExternalTools\Gnu\bin\sed.exe -i "s/PLA/pla/g" t.a
..\ExternalTools\Gnu\bin\sed.exe -i "s/PLP/plp/g" t.a
..\ExternalTools\Gnu\bin\sed.exe -i "s/ROL/rol/g" t.a
..\ExternalTools\Gnu\bin\sed.exe -i "s/ROR/ror/g" t.a
..\ExternalTools\Gnu\bin\sed.exe -i "s/RTI/rti/g" t.a
..\ExternalTools\Gnu\bin\sed.exe -i "s/RTS/rts/g" t.a
..\ExternalTools\Gnu\bin\sed.exe -i "s/SBC/sbc/g" t.a
..\ExternalTools\Gnu\bin\sed.exe -i "s/SEC/sec/g" t.a
..\ExternalTools\Gnu\bin\sed.exe -i "s/SED/sed/g" t.a
..\ExternalTools\Gnu\bin\sed.exe -i "s/SEI/sei/g" t.a
..\ExternalTools\Gnu\bin\sed.exe -i "s/STX/stx/g" t.a
..\ExternalTools\Gnu\bin\sed.exe -i "s/LDY/ldy/g" t.a
..\ExternalTools\Gnu\bin\sed.exe -i "s/STY/sty/g" t.a
..\ExternalTools\Gnu\bin\sed.exe -i "s/TAX/tax/g" t.a
..\ExternalTools\Gnu\bin\sed.exe -i "s/TAY/tay/g" t.a
..\ExternalTools\Gnu\bin\sed.exe -i "s/TSX/tsx/g" t.a
..\ExternalTools\Gnu\bin\sed.exe -i "s/TXA/txa/g" t.a
..\ExternalTools\Gnu\bin\sed.exe -i "s/TXS/txs/g" t.a
..\ExternalTools\Gnu\bin\sed.exe -i "s/TYA/tya/g" t.a

..\ExternalTools\Gnu\bin\sed.exe -i "s/\.BYTE/!by/g" t.a
..\ExternalTools\Gnu\bin\sed.exe -i "s/\.TEXT/!scr/g" t.a
..\ExternalTools\Gnu\bin\sed.exe -i "s/\.Y/\.y/g" t.a
..\ExternalTools\Gnu\bin\sed.exe -i "s/\.X/\.x/g" t.a

del sed* /q

..\acme.exe -v4 --setpc $4000 -f cbm -o t.prg t.a
