/*
The aim of this lil script is to geta  lil server running on the arduino Yun and
getting an external agent to control its light turning on or off.

We'll also see if I manage to get the lil server to report its status to the big server

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


int sensorPin = A0; //analog pin to get light level readings.
int ledPin = 12; //Led to simulate lamp
int sensorValue = 0; // variable to store light reading
String ledState; //prolly gonna throw it to hell

void setup() {
  // put your setup code here, to run once:
  String ledState = "";
  pinMode(ledPin, OUTPUT);
  
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
  // put your main code here, to run repeatedly:
  
 checkLightLevel(); //Check light, turn LED state accordingly, print value
 
 serveClient(); // Serve client depending on his request.
 
 delay(200);
  
}

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
  sensorValue = analogRead(sensorPin);
  
  // Send feedback to client
  client.print("lightStatus=");
  client.println(sensorValue);
}


void checkLightLevel(){
   sensorValue = analogRead(sensorPin);
  
  //Check if LED should be on
  if(sensorValue < 400)
  {
    digitalWrite(ledPin, HIGH);
    ledState = "LED On!";
  }
  else
  {
    digitalWrite(ledPin, LOW);
    ledState = "LED OFF!";
  }
  
  Serial.println(sensorValue);
}
