/*
The aim of this lil script is to geta  lil client running on the arduino Yun and
getting an external agent to control its light turning on or off.

We'll also see if I manage to get the lil client to report its status to the big server

Note: I'll have to see whaddup with the server instructions getting here as with the commands
I will copy from the examples only instructions coming from the localhost(linino core) will
be accepted.... there MUST be a work around.


MEGA IMPORTANT NOTE:
  AT THE END OF THE FUCKING URL THERE IS A CARRIAGE RETURN ONE MUST ACCOUNT FOR "\r"
  FUCK ME DEAD!
  
  
*/


//e.g. link <arduinoIP>/arduino/<Aqui lo que se leera del url>



#include <Bridge.h>
#include <BridgeClient.h>
#include <BridgeServer.h>
#include <Process.h>

BridgeServer server;


int lightSensorPin = A0; //analog pin to get light level readings.
int ledPin = 13; //Led to simulate lamp
int lightSensorValue = 0; // variable to store light reading

int motionLed = 12; //LED to see motion values.
int pirSensorPin = 7; //Pin for the digital input of the PIR sensor
int pirSensorValue = 0; //Variable to store motion reading.

int serverTimeout = 0; //Counter to track disconnected time from server.


String serverIPAndPort = "192.168.1.3:8081";

void setup() {
  pinMode(ledPin, OUTPUT); //Initialize ledPin
  pinMode(pirSensorPin, INPUT); //Initialize pir sensor pin
  
  // Bridge startup
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  Bridge.begin();
  digitalWrite(13, HIGH);

  // Listen for incoming connection only from localhost
  // (no one from the external network could connect)
  //server.listenOnLocalhost(); // listen to local connections only
  server.noListenOnLocalhost(); // for non local connections
  server.begin();
  
  
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
}

void loop() {
  
  /*
  Cosas que necesitamos hacer:
    Checar sensores
    Mandar info al servidor
      Actualizar timeout de servidor (si llevamos mucho tiempo sin saber nada del servidor hacemos updates locales)
    Ver si tenemos que ejecutar localUpdate
    Delay
  
  
  */
  
 checkLightLevel(); //Check light, turn LED state accordingly, print value
 checkMovement(); //update the value of the motion sensor's output.
 
 if(serverTimeout > 20000) //20 segundos
 {
    localUpdateLight(); //Update the street light's state according to local variables.
 }

 
 sendUpdate(); //Creates a JSON string that sends all the current info of this client to the server.
 
 serveClient(); // LOCAL SERVER serve the incoming client.
 
 delay(200);
  
}




void checkLightLevel(){
   lightSensorValue = analogRead(lightSensorPin);
}

void checkMovement(){
  pirSensorValue = digitalRead(pirSensorPin);
}

//Functtion to update light depending on
void localUpdateLight(){
    //Check if LED should be on
  if(lightSensorValue < 400)
  {
    digitalWrite(ledPin, HIGH);
  }
  else
  {
    digitalWrite(ledPin, LOW);
  }
  
  Serial.println(lightSensorValue);
  
  
  //Only showing movement light for now
  if(pirSensorValue)
  {
     digitalWrite(motionLed, HIGH);
  }
  else
  {
     digitalWrite(motionLed, LOW);
  }
  
  Serial.println(pirSensorValue);
}


//-------------------------------------------SERVER COMMUNICATION CODE------------------------

void sendUpdate(){
  String updateString =  buildJsonString(lightSensorValue, pirSensorValue);
  runCurlConnection(updateString, serverIPAndPort);
  //Serial.println(updateString);
  
  
  
}

String buildJsonString(int light, int motion){
  String one = "{\"light\":";
  String two = ",\"motion\":";
  String three = "}";
  
  String res = one + light + two + motion + three;
  
  return res;
}

//Function to send json data to the specified server.
void runCurlConnection(String json, String serverIP){
  Serial.println("Entrando CurlConnection");
  Process p;
  
  //Run the full shell command.
  //p.runShellCommand("curl -H \"Content-Type: application/json\" -X POST -d '{\"light\":534, \"motion\":1}' 192.168.1.3:8081/clientupdate"); //Testing just with begin.
    
    
  p.runShellCommand("curl -H \"Content-Type: application/json\" -X POST -d '" + json + "' " + serverIp + "/clientupdate");
    
  
  
  // A process output can be read with the stream methods
  while (p.available() > 0) {
    char c = p.read();
    Serial.print(c);
  }
  // Ensure the last bit of data is sent.
  Serial.flush();
  
}


//-------------------------------------------SERVER COMMUNICATION CODE------------------------


//-----------LOCAL SERVER CODE (TO EVENTUALLY DEPRECATE)----------

void serveClient(){
  
  // Get clients coming from server
  BridgeClient client = server.accept();

  // There is a new client?
  if (client) {
    // Process request
    process(client);

    // Close connection and free resources.
    client.stop();
  }
}

//process the client's petition.
void process(BridgeClient client) {
  
  // read the command
  String command = client.readStringUntil('\r');
  

  // is "status" command?
  //We'll want to return the status of our shit: light level.
  if (command == "status") {
    Serial.println("Client requesting status");
    
    statusCommand(client);
  }
}

void statusCommand(BridgeClient client) {
  
  //Get light reading
  lightSensorValue = analogRead(lightSensorPin);
  //Get motion reading
  pirSensorValue = digitalRead(pirSensorPin);
  
  // Send feedback to client
  client.print("lightStatus=");
  client.println(lightSensorValue);
  
  client.print("motionStatus=");
  client.println(pirSensorValue);
}

//----------------- END LOCAL SERVER CODE---------------------------
