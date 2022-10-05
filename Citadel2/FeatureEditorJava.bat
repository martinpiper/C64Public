start http://127.0.0.1:8001/ace-builds-master/demo/autocompletion.html

java -Dcom.replicanet.cukesplus.server.featureEditor -Dcom.replicanet.ACEServer.debug.requests= -DZbdd6502.trace=true -jar target\PathFinding-1.0-SNAPSHOT-jar-with-dependencies.jar --tags ~@ignore --tags @java --tags ~@tooLarge --monochrome --plugin pretty --plugin html:target/cucumber --plugin json:target/report1.json --glue TestGlue --glue MazeGlue features
