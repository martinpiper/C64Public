@c64 @java
Feature: Simple 6502 code test

  Simple 6502 code test that runs a simple procedure (sei, inc $d020, rts, jmp $401 ) and tests expected memory.

  This ensures the BDD6502 system is included and initialised properly.

Scenario: Simple code test
  Given I have a simple 6502 system
  And I start writing memory at $400
  And I write the following hex bytes
      | 78 ee 20 d0 60 4c 01 04 |
      | 00 01 02 03 04 05 06 07 |
  And I start writing memory at $c100
  And I write the following bytes
    | 49152 + 12 |
    | low(49152 + 12) |
    | hi(49152 + 12) |
    | 256 - 3 |
  And I write memory at $c000 with 12
  And I setup a 16 byte stack slide
  When I execute the procedure at $400 for no more than 3 instructions
  Then I expect to see $d020 equal 1
  And I expect to see $409 equal 1
  And I expect to see $40f equal 7
  And I expect to see 49152 equal $c
  And I expect to see $c100 equal 12
  And I expect to see $c101 equal 12
  And I expect to see $c102 equal 192
  And I expect to see $c103 equal 253
