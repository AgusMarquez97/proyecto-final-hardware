#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>

const char* ssid = "AGUSCASA";
const char* password = "ardp3456.";
const char* serverName = "http://192.168.1.10/api/v1/metrics";

long contador = 0;

String json;
DeserializationError jsonError;
char * macAddress;

DynamicJsonDocument doc(1024);
DynamicJsonDocument docSendArray(1024);

unsigned long lastTime = 0;

void setup()
{
  Serial.begin(115200);
  while (!Serial) {
  ; // wait for serial port to connect.
  }
  setUpWifi();
  delay(5000);
}

void loop()
{
  while(Serial.available()) {

    String a = Serial.readString();
    
    if(!(a.isEmpty()))
      sendPostRequests(a);
    else
      Serial.println("No se recibe informacion");

      a = "";
    }

    delay(700);
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
}

void sendPostRequests(String _msg) 
{
      String msg = _msg;
      if(msg[0] != '[')
        msg = "[" + _msg;
      
      Serial.print("Se recibe: ");
      Serial.println(msg);
    //Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED){
      WiFiClient client;
      HTTPClient http;
      
      jsonError = deserializeJson(doc, msg);

      if(!jsonError)
        {
            for(int j = 0; j <= 3; j++)
            {
              Serial.print("----------------------------------------------------------------------------------------------------------- CYCLE: ");
              Serial.print(j);
              Serial.println("-------------------------------------------------------------------------------------------------------------------------------------------");

              String aux = doc[j]["mac"];

              if(aux != "")
              {
  
              docSendArray["mac"] = WiFi.macAddress() + aux;
              docSendArray["sensor_type"] = doc[j]["sensor_type"];
              docSendArray["value"] = doc[j]["value"];
              docSendArray["unit"] = doc[j]["unit"];
              
              serializeJson(docSendArray, json);
  
              Serial.println("--------------------------------------------------------- REQUEST STARTS ----------------------------------------------------------------------");
              Serial.print("JSON: ");
              Serial.println(json);
              
              contador++;
              
              Serial.print("CONTADOR DE REQUESTS REALIZADAS: ");
              Serial.println(contador);
                      
              http.begin(client, serverName);
        
              http.addHeader("Content-Type", "application/json");
        
              int httpResponseCode = http.POST(json);
              
              Serial.print("HTTP Response code: ");
              Serial.println(httpResponseCode);
                
              http.end();
  
              json = "";
              Serial.println("--------------------------------------------------------- END OF REQUEST ----------------------------------------------------------------------");
              }
            }
        }
        else
        {
           Serial.print("Error al deserealizar: ");
           Serial.print(msg);
           Serial.print(" error de tipo: ");
           Serial.println(jsonError.c_str());
        }
    }
    else {
      Serial.println("WiFi Disconnected");
    }
}
