Feature: Checks the DecompressV reference implementation

  Uses the new LZMPiV compression option
  Assembles the test decompression code
  Runs the decompression
  And validates that memory writes during decompression happen in the expected order and with expected data.
 
  Scenario: Check the V implementation
    Given compressed V test data
    And I have a simple overclocked 6502 system
    And assemble DecompressV test

#   When I enable trace with indent
    And I enable uninitialised memory read protection with immediate fail

    Then expect correct memory writes during decompression with RNXPCompressionDecompressV
    And I expect the cycle count to be no more than 111673036 cycles
