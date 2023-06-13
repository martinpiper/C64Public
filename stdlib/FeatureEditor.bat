rem @echo off

start /B java -DZbdd6502.trace=true -Dcom.replicanet.cukesplus.server.featureEditor -DZcom.replicanet.ACEServer.debug.requests= -jar ..\..\BDD6502\target\BDD6502-1.0.9-SNAPSHOT-jar-with-dependencies.jar --monochrome --plugin pretty --plugin html:target/cucumber --plugin json:target/report1.json --glue macros --glue TestGlue features

start http://127.0.0.1:8001/ace-builds-master/demo/autocompletion.html
