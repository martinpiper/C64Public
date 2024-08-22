Feature: Validates various multiplexor builds

  Using various options for builds, tests that the output binaries are exact.

  Scenario Outline: Check builds for exected output <testNumber>


  When I run the command line: ..\acme.exe -f cbm -o target\TestMultiplexor<testNumber>.prg --lib ..\ --lib ..\..\ -v4 --msvc features\AssemblePre.a TestData\TestBuildOptions<testNumber>.a features\AssemblePost.a
  Then assert that file "target\TestMultiplexor<testNumber>.prg" is binary equal to file "TestData\TestMultiplexor<testNumber>.prg"

  Examples:
    | testNumber |
    | 1          |
    | 2          |
    | 3          |
    | 4          |
    | 5          |
    | 6          |
    | 7          |
    | 8          |
