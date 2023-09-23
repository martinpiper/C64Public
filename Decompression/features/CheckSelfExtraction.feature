Feature: Test self extraction execution

  Creates test memory in various configurations designed to test match ranges and uses self extracting compression.
  The self extracting files results are then tested and verified as matching the output of the original code.

  Scenario Outline: Executes self extracted output

    And I create file "target\memory.a" with
      """
      !sal
      *=$500
        jmp $500
        
      !for .i , <len1> {
        !by 0,1,2,3,4,5,6,7
      }

      ; Something compressible by RLE as well as the other methods
      !fill <len2> , 10

      ; Something that tries to force a particular match
      !for .i , <len3> {
        !by 0,1,2,3,4,5,6,7
      }
      
      ; Always fill the memory range
      * = $fbff
        !by 0
      """
      
    Given I run the command line: ..\acme.exe -o target\memory.prg -f cbm target\memory.a

    Given setup self extraction test memory using option "-c64"
    Then test self extraction works correctly
    Given setup self extraction test memory using option "-c64b"
    Then test self extraction works correctly
    Given setup self extraction test memory using option "-c64m"
    Then test self extraction works correctly
    Given setup self extraction test memory using option "-c64mb"
    Then test self extraction works correctly

    Given setup self extraction test memory using option "-c64e"
    Then test self extraction works correctly
    Given setup self extraction test memory using option "-c64eb"
    Then test self extraction works correctly
    Given setup self extraction test memory using option "-c64me"
    Then test self extraction works correctly
    Given setup self extraction test memory using option "-c64meb"
    Then test self extraction works correctly

    Given setup self extraction test memory using option "-c64mr"
    Then test self extraction works correctly
    Given setup self extraction test memory using option "-c64mrb"
    Then test self extraction works correctly

    Given setup self extraction test memory using option "-c64mu"
    Then test self extraction works correctly
    Given setup self extraction test memory using option "-c64mub"
    Then test self extraction works correctly

  Examples:
    | len1  | len2  | len3  |
    | $02   | $f600 | $02   |
    | $02   | $7613 | $02   |
    | $01   | $7673 | $01   |
    | $01   | $269c | $03   |
    | $03   | $7378 | $03   |
