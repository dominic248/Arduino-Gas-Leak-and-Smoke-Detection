#include <ESP8266WiFi.h>;
#include <WiFiClient.h>;
#include <ThingSpeak.h>;
#include <ArduinoJson.h>
#include <MQ2.h>
#include <Wire.h> 

MQ2 mq2(A0);

const char* ssid = "Skynet"; //Your Network SSID
const char* password = "24081999"; //Your Network Password


const char* IFTTT_URL="maker.ifttt.com";
const char* IFTTT_KEY="c_QxEnZGw7ieOc28-jWDIR";
const char* IFTTT_EVENT="MQ2sensor";

int smokeA0 = A0;
int Buzzer = 2;//D4 

float lpg,co,smoke;
int detect;
int D=0;
String sensors[3]={"","",""};

WiFiClient client;
unsigned long myChannelNumber = 858265; //Your Channel Number (Without Brackets)
const char * myWriteAPIKey = "23NJGFJ3EP1MPUWB"; //Your Write API Key

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  pinMode(Buzzer, OUTPUT);
  pinMode(smokeA0, INPUT);
  // Connect to WiFi network
  Serial.print("Connecting to ");
  Serial.print(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected...");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  ThingSpeak.begin(client);
}

void loop() { // run over and over
  if(!client.connect(IFTTT_URL,80)){
    Serial.println("Connection Failed!");
    return;
  }
  lpg=mq2.readLPG();
  co=mq2.readCO();
  smoke=mq2.readSmoke();
  Serial.println(String(lpg)+","+String(co)+","+String(smoke)+".");
  if(lpg>30 || lpg<0){
    D=1;
    sensors[0]="LPG";
  }else if(co>400 || co<0){
    D=1;
    sensors[1]="CO2";
  }else if(smoke>400 || smoke<0){
    D=1;
    sensors[2]="Smoke";
  }else{
    D=0;
    sensors[0]="";
    sensors[1]="";
    sensors[2]="";
  }
  if(D==1){
    triggerdetect(sensors);
  }else{
    detect=0;
  }
    
  ThingSpeak.setField(1, lpg);
  ThingSpeak.setField(2, co);
  ThingSpeak.setField(3, smoke);
  ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  delay(1000);
}


void triggerdetect(String sensor[]){  
  if(detect==0){
    detect=1;         
    String url="https://maker.ifttt.com/trigger/MQ2sensor/with/key/c_QxEnZGw7ieOc28-jWDIR";
    StaticJsonBuffer<200> jsonBuffer;
    JsonObject& data = jsonBuffer.createObject();
    data["value1"] = sensor[0];
    data["value2"] = sensor[1];
    data["value3"] = sensor[2];
//    Serial.println(distance + "," + posit);
//    data.printTo(Serial);
    client.print("POST ");
    client.print(url);
    client.println(" HTTP/1.1");
    client.print("Host: ");
    client.println(IFTTT_URL);
    client.println("Connection: close\r\nContent-Type: application/json");
    client.print("Content-Length: ");
    client.print(data.measureLength());
    client.print("\r\n");
    client.println();
    data.printTo(client);
  }
}
