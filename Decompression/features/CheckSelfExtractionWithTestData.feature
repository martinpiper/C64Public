Feature: Test self extraction execution with test data

  Uses the test data files to test self extraction with override start and execution address.
  The self extracting files results are then tested and verified as matching the output of the test data with memory boundary checks.

  Scenario Outline: Fully check self extracted output <file>

    Given setup self extraction test memory using file "<file>" and option "-c64mu"

    Then test memory blocks
#    Given I enable trace
    When I execute the procedure at 2061 for no more than 10000000 instructions until PC = $116
    Given I reset the cycle count
    Given load binary file "<file>" into temporary memory
    And trim "2" bytes from the start of temporary memory
    Then for memory from "$400" to "$ffff" expect writes at "$500" with temporary memory
    And I continue executing the procedure until return or until PC = $400
    And test memory blocks
    And I expect the cycle count to be no more than <cycles> cycles


  Examples:
    | file                          | cycles  |
    | ..\Compression\TestData\1.bin | 420115  |
    | ..\Compression\TestData\2.bin | 264355  |
    | ..\Compression\TestData\3.bin | 199139  |
    | ..\Compression\TestData\4.bin | 311702  |
    | ..\Compression\TestData\5.bin | 1930613 |
    | ..\Compression\TestData\6.bin | 1063796 |
    | ..\Compression\TestData\7.bin | 954401  |
    | ..\Compression\TestData\8.bin | 288310  |
    | ..\Compression\TestData\9.bin | 515944  |
