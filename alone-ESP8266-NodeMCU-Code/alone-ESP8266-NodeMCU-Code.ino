#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ThingSpeak.h>
#include <MQ2.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

MQ2 mq2(A0);

const char* ssid = "Skynet"; //Your Network SSID
const char* password = "24081999"; //Your Network Password

const char* IFTTT_URL="maker.ifttt.com";
const char* IFTTT_KEY="c_QxEnZGw7ieOc28-jWDIR";
const char* IFTTT_EVENT="MQ2sensor";

int smokeA0 = A0;
int Buzzer = 2;//D4 
String Slpg,Sco,Ssmoke;
float lpg,co,smoke;
int Dlpg=0,Dco=0,Dsmoke=0;

WiFiClient client;
unsigned long myChannelNumber = 858265; //Your Channel Number (Without Brackets)
const char * myWriteAPIKey = "23NJGFJ3EP1MPUWB"; //Your Write API Key

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

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
  mq2.begin();
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(2000);
  display.clearDisplay();
  display.display();
  
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
      if(Dlpg==0){
        Dlpg=1;
        Serial.println("lpg");
        String url="https://maker.ifttt.com/trigger/lpgDetect/with/key/c_QxEnZGw7ieOc28-jWDIR";
        client.print(String("POST ") + url + " HTTP/1.1\r\n" +
             "Host: " + IFTTT_URL + "\r\n" + 
             "Connection: close\r\n\r\n");
      }
    }else{
      Dlpg=0;
    }
    if(co>400 || co<0){
      if(Dco==0){
        Dco=1;
        Serial.println("co2");
        String url="https://maker.ifttt.com/trigger/co2Detect/with/key/c_QxEnZGw7ieOc28-jWDIR";
        client.print(String("POST ") + url + " HTTP/1.1\r\n" +
             "Host: " + IFTTT_URL + "\r\n" + 
             "Connection: close\r\n\r\n");
      }
    }else{
      Dco=0;
    }
    if(smoke>400 || smoke<0){
      if(Dsmoke==0){
        Dsmoke=1;
        Serial.println("smoke");
        String url="https://maker.ifttt.com/trigger/smokeDetect/with/key/c_QxEnZGw7ieOc28-jWDIR";
        client.print(String("POST ") + url + " HTTP/1.1\r\n" +
             "Host: " + IFTTT_URL + "\r\n" + 
             "Connection: close\r\n\r\n");
      }
    }else{
      Dsmoke=0;
    }
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 10);
    display.println("LPG  : "+String(lpg)+"ppm");
    display.println("CO2  : "+String(co)+"ppm");
    display.println("SMOKE: "+String(smoke)+"ppm");
    display.display(); 
    ThingSpeak.setField(1, lpg);
    ThingSpeak.setField(2, co);
    ThingSpeak.setField(3, smoke);
    ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
    delay(1000);
}
