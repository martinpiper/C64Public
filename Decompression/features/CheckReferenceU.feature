Feature: Checks the DecompressU reference implementation

  Uses the new LZMPiU compression option
  Assembles the test decompression code
  Runs the decompression
  And validates that memory writes during decompression happen in the expected order and with expected data.
 
  Scenario: Check the implementation
    Given compressed test data
    And I have a simple overclocked 6502 system
    And assemble DecompressU test

#    When I enable trace with indent
    And I enable uninitialised memory read protection with immediate fail

    Then expect correct memory writes during decompression
