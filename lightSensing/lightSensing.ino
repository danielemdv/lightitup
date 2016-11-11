
int sensorPin = A0;
int ledPin = 13;
int sensorValue = 0;
String ledState;

void setup() {
  // put your setup code here, to run once:
  String ledState = "";
  pinMode(ledPin, OUTPUT);
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  sensorValue = analogRead(sensorPin);
  
  //Check if LED should be on
  if(sensorValue < 800)
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
  
  delay(1);
  
}
