rem @echo off

java -DZbdd6502.trace=true -DZcom.replicanet.cukesplus.server.featureEditor -DZcom.replicanet.ACEServer.debug.requests= -jar ..\..\..\BDD6502\target\BDD6502-1.0.9-SNAPSHOT-jar-with-dependencies.jar --monochrome --plugin pretty --plugin html:target/cucumber --plugin json:target/report1.json --glue macros --glue TestGlue features %1 %2
IF ERRORLEVEL 1 goto error

rem Enable this line to stop any main code build
rem exit /B 0

goto end
:error
rem type t.txt
exit /B -1
:end

