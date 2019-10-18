
#include <MQ2.h>
#include <Wire.h> 


//I2C pins declaration
int smokeA0 = A0;
int Buzzer = 8; 
int sensorThres = 400;


void setup(){
  Serial.begin(9600);
  pinMode(Buzzer, OUTPUT);
  pinMode(smokeA0, INPUT);
}
void loop(){
 int analogSensor = analogRead(smokeA0);
  Serial.print("Pin A0: ");
  Serial.println(analogSensor);
  // Checks if it has reached the threshold value
  if (analogSensor > sensorThres){
    digitalWrite(Buzzer, HIGH);
  } else {
    digitalWrite(Buzzer, LOW);
  }
  delay(500);
}
