int ledPin = 12;

int pirSensorPin = 7;
int pirSensorValue = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(pirSensorPin, INPUT);
  pinMode(ledPin, OUTPUT);
  
  Serial.begin(9600);
}

void loop() {
  pirSensorValue = digitalRead(pirSensorPin);
  digitalWrite(ledPin, pirSensorValue);
  Serial.println(pirSensorValue);
  
  delay(100);

}
