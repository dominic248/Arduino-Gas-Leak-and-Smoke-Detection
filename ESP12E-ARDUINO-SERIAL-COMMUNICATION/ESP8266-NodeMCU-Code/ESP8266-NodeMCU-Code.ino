#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ThingSpeak.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

const char* ssid = "Skynet"; //Your Network SSID
const char* password = "24081999"; //Your Network Password

//
const char* IFTTT_URL="maker.ifttt.com";
//

String Slpg,Sco,Ssmoke;
int lpg,co,smoke;
int Dlpg=0,Dco=0,Dsmoke=0;
const int motor2 =  16;
int Buzzer = 2; 

WiFiClient client;
const int httpPort = 443;
String host = "script.google.com"; 


unsigned long myChannelNumber = 858265; //Your Channel Number (Without Brackets)
const char * myWriteAPIKey = "23NJGFJ3EP1MPUWB"; //Your Write API Key

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);

  pinMode( motor2 , OUTPUT);
  pinMode(Buzzer, OUTPUT);
  digitalWrite( motor2 , HIGH);
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
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(2000);
  display.clearDisplay();
  display.display();
  
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
    Serial.println(content);
    String Slpg = getValue(content, ',', 0);
    String Sco = getValue(content, ',', 1);
    String Ssmoke = getValue(content, ',', 2);
    int lpg=Slpg.toInt();
    int co=Sco.toInt();
    int smoke=Ssmoke.toInt();
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 10);
    display.println("LPG  : "+String(lpg)+"ppm");
    display.println("CO2  : "+String(co)+"ppm");
    display.println("SMOKE: "+String(smoke)+"ppm");
    display.display(); 
    
    if(lpg>450 || lpg<0 || smoke>2300 || smoke<0){
      if(Dlpg==0){
        Dlpg=1;
        digitalWrite(Buzzer, HIGH);
        digitalWrite( motor2 , LOW);
        
        String url="https://maker.ifttt.com/trigger/lpgDetect/with/key/c_QxEnZGw7ieOc28-jWDIR";
        client.print(String("POST ") + url + " HTTP/1.1\r\n" +
             "Host: " + IFTTT_URL + "\r\n" + 
             "Connection: close\r\n\r\n");
      }
    }else{
      Dlpg=0;
      digitalWrite(Buzzer, LOW);
      digitalWrite( motor2 , HIGH);
      
    }
    
    ThingSpeak.setField(1, lpg);
    ThingSpeak.setField(2, co);
    ThingSpeak.setField(3, smoke);
    ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
    WiFiClientSecure client;
client.setInsecure();
  if (!client.connect(host, httpPort)) 
  {
    Serial.println("connection failed");
  }
  //https://script.google.com/macros/s/AKfycbzT0FpcLk44l2-3uZO-1rgUIjMh9FXzzdZ1lWCDtm73_dM6sHs/exec?Value1=1&Value2=Hello
  String url = "/macros/s/AKfycbx1llAT6tpv76tQhKqZnHas2b3dMVzo16PlMlwcXvNieqzgbI4/exec?Value1="+Slpg+"&Value2="+Sco+"&Value3="+Ssmoke;
  Serial.print("Requesting URL: ");
  Serial.println(url);
  
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
            "Host: " + host + "\r\n" + 
            "Connection: close\r\n\r\n");

  String section="header";
  while(client.available())
  {
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
  Serial.println();
  Serial.println("closing connection");

  }
}
String getValue(String data, char separator, int index){
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
