#include <Servo.h>
const int echoPin = 11; // attach pin D2 Arduino to pin Echo of HC-SR04
const int trigPin = 10; //attach pin D3 Arduino to pin Trig of HC-SR04
const int LED = 9;
const int buzz = 8;
Servo myservo;

unsigned long sensorTrigger;   
unsigned long outputOnAt;
unsigned long turnOnDelay = 2000;
unsigned long turnOffDelay = 3000;
bool outputReady = false;
bool outputState = false;
long distance;

void setup() {
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPin, INPUT); // Sets the echoPin as an INPUT
  pinMode(buzz, OUTPUT);
  pinMode(LED, OUTPUT);
  myservo.attach(4);
  myservo.write(15);
  Serial.begin(9600); // // Serial Communication is starting with 9600 of baudrate speed
}
void loop() {
  unsigned long currentTime = millis();
  displayDistance();
  distance = readUltrasonicDistance(trigPin, echoPin);
  if(distance <= 55){
    Serial.println("obstacle found");
  } else{
    Serial.println("no obstacle found");
  }
  if(distance <= 55 && !outputReady){ 
    sensorTrigger = currentTime;
    Serial.println("ACTIVATE");
    outputReady = true;
  }else if (distance > 55){
    outputReady = false;
  }
  if(outputReady){ 
    if(currentTime - sensorTrigger >= turnOnDelay){
      //wait for time to pass
      digitalWrite(LED, HIGH);
      digitalWrite(buzz, HIGH);
      myservo.write(180);
      Serial.println("ONNNNNNNNNNNNNNNNNNNNNNNNN");
      outputState = true;
      outputOnAt = currentTime;
      outputReady = false;
    }
  }

  if(outputState){
    if(currentTime - outputOnAt >= turnOffDelay){
      outputState = false;
      Serial.println("                         ");
      myservo.write(20);
      digitalWrite(LED, LOW);
      digitalWrite(buzz, LOW);
    }
  }
  
}

long readUltrasonicDistance(int triggerPin, int echoPin){
  pinMode(triggerPin, OUTPUT);  // Clear the trigger
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  // Sets the trigger pin to HIGH state for 10 microseconds
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  pinMode(echoPin, INPUT);
  // Reads the echo pin, and returns the sound wave travel time in microseconds
  return 0.01723 * pulseIn(echoPin, HIGH);
}

void displayDistance(){
  long detectDistance = readUltrasonicDistance(trigPin, echoPin);
  Serial.print("Distance: ");
  Serial.println(detectDistance);
}
