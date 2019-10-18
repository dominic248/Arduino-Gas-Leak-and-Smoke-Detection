#include <Servo.h>

#include <MQ2.h>
#include <Wire.h> 

//I2C pins declaration
int smokeA0 = A0;
double lpg, co, smoke;

MQ2 mq2(smokeA0);

void setup(){
  Serial.begin(9600);
  pinMode(smokeA0, INPUT);
  mq2.begin();
}
void loop(){
  int analogSensor = analogRead(smokeA0);
  Serial.print("Pin A0: ");
  Serial.println(analogSensor);
  float* values= mq2.read(true); //set it false if you don't want to print the values in the Serial
  //lpg = values[0];
  lpg = mq2.readLPG();
  //co = values[1];
  co = mq2.readCO();
  //smoke = values[2];
  smoke = mq2.readSmoke();
 
  Serial.println(lpg);
  Serial.println(co);
  Serial.println(smoke);


  delay(500);
}
