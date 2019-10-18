#include <SoftwareSerial.h>
#include <MQ2.h>
#include <Wire.h> 


//I2C pins declaration
int smokeA0 = A0;
int Buzzer = 8; 
int sensorThres = 400;
SoftwareSerial Serial1(10,11);//RX,TX


void setup(){
  Serial.begin(9600);
  Serial1.begin(9600);
  pinMode(Buzzer, OUTPUT);
  pinMode(smokeA0, INPUT);
}
void loop(){
 int analogSensor = analogRead(smokeA0);
  Serial.print("Pin A0: ");
  Serial.println(analogSensor);
  Serial1.println(analogSensor);
  //Serial1.println("Hello");
  // Checks if it has reached the threshold value
  if (analogSensor > sensorThres){
    digitalWrite(Buzzer, HIGH);
  } else {
    digitalWrite(Buzzer, LOW);
  }
  delay(1000);
}
