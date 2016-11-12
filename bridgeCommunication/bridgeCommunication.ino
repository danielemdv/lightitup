#include <Process.h>
#include <Bridge.h>

void setup() {
  // put your setup code here, to run once:
  Bridge.begin();
  Serial.begin(9600);
  
  while(!Serial) //Wait until the Serial connection opens.
  
  runCurlConnection();
  
  delay(200);
}

void loop() {
  Serial.println("LOOP");
  runCurlConnection();
  
  
  
  delay(200);

}

void runCurlConnection(){
  Serial.println("Entrando CurlConnection");
  Process p;
  
  //Run the full shell command.
  p.runShellCommand("curl -H \"Content-Type: application/json\" -X POST -d '{\"light\":534, \"motion\":1}' 192.168.1.3:8081/clientupdate"); //Testing just with begin.
    
    
  
  
  // A process output can be read with the stream methods
  while (p.available() > 0) {
    char c = p.read();
    Serial.print(c);
  }
  // Ensure the last bit of data is sent.
  Serial.flush();
  
}

