#include <SoftwareSerial.h>
#include <MQ2.h>
#include <Wire.h> 

MQ2 mq2(A0);

//I2C pins declaration
int smokeA0 = A0;
int Buzzer = 8; 
int sensorThres = 400;
SoftwareSerial Serial1(10,11);//RX,TX
float lpg,co,smoke;

void setup(){
  Serial.begin(9600);
  Serial1.begin(9600);
  pinMode(Buzzer, OUTPUT);
  pinMode(smokeA0, INPUT);
  mq2.begin();
}
void loop(){
 int analogSensor = analogRead(smokeA0);
  Serial.print("Pin A0: ");
  Serial.println(analogSensor);
//  Serial1.println(analogSensor);
//  float* values= mq2.read(true); //set it false if you don't want to print the values in the Serial
  lpg=mq2.readLPG();
  co=mq2.readCO();
  smoke=mq2.readSmoke();
  Serial.println(String(lpg)+","+String(co)+","+String(smoke)+".");
  Serial1.println(String(lpg)+","+String(co)+","+String(smoke)+"");
  //Serial1.println("Hello");
  // Checks if it has reached the threshold value
  if (analogSensor > sensorThres){
    digitalWrite(Buzzer, HIGH);
  } else {
    digitalWrite(Buzzer, LOW);
  }
  delay(1000);
}
