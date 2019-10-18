#include <ESP8266WiFi.h>;
#include <WiFiClient.h>;
#include <ThingSpeak.h>;

const char* ssid = "Skynet"; //Your Network SSID
const char* password = "24081999"; //Your Network Password

//
const char* IFTTT_URL="maker.ifttt.com";
const char* IFTTT_KEY="c_QxEnZGw7ieOc28-jWDIR";
const char* IFTTT_EVENT="MQ2sensor";
//

int detect=0;

WiFiClient client;
unsigned long myChannelNumber = 858265; //Your Channel Number (Without Brackets)
const char * myWriteAPIKey = "23NJGFJ3EP1MPUWB"; //Your Write API Key

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
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
  
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
}

void loop() { // run over and over
  if(!client.connect(IFTTT_URL,80)){
    Serial.println("Connection Failed!");
    return;
  }
  if (Serial.available()>0) {
    String content=Serial.readStringUntil('\n');
    int digit=content.toInt();
    //int digit=Serial.parseInt();
    Serial.println(content);
    if(digit>400){
      if(detect==0){
        detect=1;
        String url="https://maker.ifttt.com/trigger/MQ2sensor/with/key/c_QxEnZGw7ieOc28-jWDIR";
        client.print(String("POST ") + url + " HTTP/1.1\r\n" +
             "Host: " + IFTTT_URL + "\r\n" + 
             "Connection: close\r\n\r\n");
      }
    }else{
      detect=0;
    }
    //ThingSpeak.writeField(myChannelNumber, 1,content, myWriteAPIKey); 
  }
}
