#include <Bridge.h>
#include <BridgeClient.h>
#include <BridgeServer.h>
#include <Process.h>

BridgeServer server;

void setup() {
  // put your setup code here, to run once:
  Bridge.begin();
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  
  Serial.println("Iniciando loop");
  
  Process p;
  p.runShellCommand("printf '67'");
  
  String inString = "";
  
  while(p.available() > 0){
    char c = p.read();
    inString += c;
  }
  
  Serial.flush();
  
  Serial.println("Leido:" + inString);
  //String temp = "Convirtiendo" + inString.toInt();
  Serial.println(inString.toInt());
  
  delay(1000);
  
  

}
