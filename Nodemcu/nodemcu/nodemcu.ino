#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include<SoftwareSerial.h>

const char* ssid = "AGUSCASA";
const char* password = "ardp3456.";
const char* serverName = "http://192.168.1.6:8080";

unsigned long lastTime = 0;
unsigned long timerDelay = 5000;

SoftwareSerial mySerial(D1, D2); 

void setup()
{
  Serial.begin(115200);
  mySerial.begin(9600);

  setUpWifi();
}

void loop()
{
  String msg = "{\"ssss\":\"123\"}";
  //mySerial.readStringUntil('\n');
  //Serial.println(msg);
  sendPostRequests(msg);
  delay(5000);
}

void setUpWifi() 
{
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
 
  Serial.println("Timer set to 5 seconds (timerDelay variable), it will take 5 seconds before publishing the first reading.");
}

void sendPostRequests(String msg) 
{
  // Envio un Post cada 5 segundos
  
  if ((millis() - lastTime) > timerDelay) {
    //Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED){
      WiFiClient client;
      HTTPClient http;
      
      http.begin(client, serverName);
      
      // seguridad ... ?
      
      http.addHeader("Content-Type", "application/json");
      int httpResponseCode = http.POST("{\"fecha\":\"fecha del nodemcu\",\"areas\":[{\"id\":1,\"nombre\":\"area almacenaje\",\"sensores\":[{\"id\":1,\"tipo\":\"movimiento\",\"valor\":\"10\"},{\"id\":2,\"tipo\":\"humedad y temperatura\",\"valor\":\"20\"},{\"id\":3,\"tipo\":\"monoxido de carbono\",\"valor\":\"20\"}]}]}");

      //int httpResponseCode = http.POST(msg);
      
     
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
        
      http.end();
    }
    else {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }
}
