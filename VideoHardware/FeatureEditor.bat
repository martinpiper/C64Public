start http://127.0.0.1:8001/ace-builds-master/demo/autocompletion.html

rem --add-opens java.base/java.util=ALL-UNNAMED --add-opens java.base/java.lang.reflect=ALL-UNNAMED --add-opens java.base/java.text=ALL-UNNAMED --add-opens java.desktop/java.awt.font=ALL-UNNAMED
java -Dcom.replicanet.cukesplus.server.featureEditor -Dcom.replicanet.ACEServer.debug.requests= -DZbdd6502.trace=true -jar ..\..\BDD6502\target\BDD6502-1.0.9-SNAPSHOT-jar-with-dependencies.jar --monochrome --plugin pretty --plugin html:target/cucumber --plugin json:target/report1.json --glue macros --glue TestGlue features
