#include <SoftwareSerial.h>
#include <MQ2.h>
#include <Wire.h> 

MQ2 mq2(A1);

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
  lpg=mq2.readLPG();
  co=mq2.readCO();
  smoke=mq2.readSmoke();
  Serial.println(String(lpg)+","+String(co)+","+String(smoke)+".");
  Serial1.println(String(lpg)+","+String(co)+","+String(smoke)+"");
  delay(5000);
}
