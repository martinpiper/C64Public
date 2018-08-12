rem @echo off

start /B java -Dbdd6502.trace=true -Dcom.replicanet.cukesplus.server.featureEditor -DZcom.replicanet.ACEServer.debug.requests= --add-opens java.base/java.util=ALL-UNNAMED --add-opens java.base/java.lang.reflect=ALL-UNNAMED --add-opens java.base/java.text=ALL-UNNAMED --add-opens java.desktop/java.awt.font=ALL-UNNAMED -jar ..\..\BDD6502\target\BDD6502-1.0.9-SNAPSHOT-jar-with-dependencies.jar --monochrome --plugin pretty --plugin html:target/cucumber --plugin json:target/report1.json --glue TestGlue features
timeout /T 2
start http://127.0.0.1:8001/ace-builds-master/demo/autocompletion.html
