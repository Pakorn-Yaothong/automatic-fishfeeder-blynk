#include <Servo.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <WiFiClientSecure.h>  
#include <ArduinoJson.h>       

#define BLYNK_TEMPLATE_ID "TMPL6dur-dYak"
#define BLYNK_TEMPLATE_NAME "lab18"
#define BLYNK_AUTH_TOKEN "VU1PgWmQ2ZSj8LQJjjOayknStUp10cG1"

char auth[] = "VU1PgWmQ2ZSj8LQJjjOayknStUp10cG1";

char ssid[] = "Wifi 5-2";
char pass[] = "0828235387";

const char* lineToken = "398b32a6768c4afc6b328c42172e1b56";
const char* lineApiUrl = "https://api.line.me/v2/bot/message/push";

Servo myservo; 

int pos = 0;    
long FISHFEEDER = 10000; 
long lastFeedTime = 0;
bool manualFeed = false;   

int ledPin = D4; 
int idleLedPin = D5;  

const char* userId = "2006490337"; 

void setup() {
  Serial.begin(9600); 
  Blynk.begin(auth, ssid, pass);
  
  myservo.attach(D1);  
  myservo.write(0);    
  delay(15);
  
  pinMode(ledPin, OUTPUT);  
  pinMode(idleLedPin, OUTPUT);  

  lastFeedTime = millis();  

  digitalWrite(idleLedPin, HIGH);  
  Blynk.virtualWrite(V4, 255);      
}

void loop() {
  Blynk.run();

  long now = millis();
  
  if (now - lastFeedTime >= FISHFEEDER) {
    feedFish();
    lastFeedTime = now;
  }

  if (manualFeed) {
    feedFish();
    manualFeed = false;
  }
}

void feedFish() {
  digitalWrite(idleLedPin, LOW);  
  digitalWrite(ledPin, HIGH);     
  Blynk.virtualWrite(V3, 255);    
  Blynk.virtualWrite(V4, 0);      

  for (pos = 0; pos < 180; pos += 1) {
    myservo.write(pos);
    delay(15);
  }
  for (pos = 180; pos >= 0; pos -= 1) {
    myservo.write(pos);
    delay(15);
  }

  digitalWrite(ledPin, LOW);      
  Blynk.virtualWrite(V3, 0);      
  digitalWrite(idleLedPin, HIGH); 
  Blynk.virtualWrite(V4, 255);    

  sendLineMessage("Fish has been fed!");
}

BLYNK_WRITE(V1) {
  int pinValue = param.asInt();
  if (pinValue == 1) {
    manualFeed = true;
  }
}

BLYNK_WRITE(V2) {
  int feedInterval = param.asInt();
  FISHFEEDER = feedInterval * 3600000L;
}

void sendLineMessage(String message) {
  WiFiClientSecure client;
  client.setInsecure();

  if (!client.connect("api.line.me", 443)) {
    Serial.println("Connection failed");
    return;
  }

  String jsonMessage = "{\"to\":\"" + String(userId) + "\",\"messages\":[{\"type\":\"text\",\"text\":\"" + message + "\"}]}";

  client.println("POST /v2/bot/message/push HTTP/1.1");
  client.println("Host: api.line.me");
  client.println("Authorization: Bearer " + String(lineToken));
  client.println("Content-Type: application/json");
  client.print("Content-Length: ");
  client.println(jsonMessage.length());
  client.println();
  client.println(jsonMessage);

  String line;
  int statusCode = 0;

  while (client.connected() || client.available()) {
    if (client.available()) {
      line = client.readStringUntil('\n');
      Serial.println(line);
      
      if (line.startsWith("HTTP/")) {
        statusCode = line.substring(9, 12).toInt();
      }
    }
  }

  if (statusCode == 200) {
    Serial.println("Message sent successfully: " + message);
  } else {
    Serial.println("Failed to send message. Status code: " + String(statusCode));
  }

  client.stop();
}
