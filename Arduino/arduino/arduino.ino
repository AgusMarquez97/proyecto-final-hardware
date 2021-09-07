#include <SoftwareSerial.h>
#include <DHT.h>
#include <ArduinoJson.h>

//PINS
#define DHTPIN 7
#define MQ7PIN 2
#define TRIGPIN 9
#define ECHOPIN 10
#define SR5PIN 8

#define DHTTYPE DHT11

// Variables aux
String str;
const int distancia_maxima = 20;

long duration; 
int distance;
DHT dht(DHTPIN, DHT11);
const int ledPin =  LED_BUILTIN; // Se define a la led integrada en el arduino
int ledState = LOW; // Se define un estado inicial HIGH = prendido, LOW = apagado

bool movementDetected = LOW; // para detectar movimiento


DynamicJsonDocument doc(1024);

SoftwareSerial espSerial(5, 6);

/*
struct postRequestData{
  char fecha[8];
  int area;
  char nombre[8];
};

struct sensorData {
  int id;
  char tipo[16];
  char valor[10];
};

*/

void setup(){ 
  
  pinMode(TRIGPIN,OUTPUT); // Pin trigger sensor movimiento sonido
  pinMode(ECHOPIN ,INPUT); // Pin echo sensor movimiento sonido
  pinMode(MQ7PIN, INPUT); // Pin MQ7 sensor gases
  pinMode(ledPin, OUTPUT); // led integrado en el arduino
  pinMode(SR5PIN, INPUT); // Pin SR501 sensor de movimiento

  dht.begin(); // API DHT para el sensor de humedad y temperatura
  
  Serial.begin(9600); 
  espSerial.begin(115200);

  delay(5000);
}

void loop()
{
  char msj[1024];
  /*
   * SENSOR DHT11 TEMPERATURA Y HUMEDAD
   */
  
  float h = dht.readHumidity();        // humedad
  float t = dht.readTemperature();     // temperatura en Celcius
  
  if (isnan(h) || isnan(t)) 
    Serial.println("Failed to read from DHT sensor!");
  else
  {  
  doc[0]["mac"] = ":7T";
  doc[0]["sensor_type"] = "TEMPERATURE";
  doc[0]["value"] = t;
  doc[0]["unit"] = "°C";

  doc[1]["mac"] = ":7H";
  doc[1]["sensor_type"] = "HUMIDITY";
  doc[1]["value"] = h;
  doc[1]["unit"] = "% HR";  
  }


  /*
   * SENSOR MQ7
   */
   
   // -- 1 version digital
   boolean mq_estado = digitalRead(MQ7PIN); // Se obtiene el valor negado al sensado, arroja 1 si no detecta gas
 
  
  // -- 2 version analogica
  int adc_MQ = analogRead(A0); //Lemos la salida analógica del MQ
  float voltaje = adc_MQ * (5.0 / 1023.0); //Convertimos la lectura en un valor de voltaje

  doc[2]["mac"] = ":A0";
  doc[2]["sensor_type"] = "GAS";
  doc[2]["value"] = adc_MQ;
  doc[2]["unit"] = "PPM CO2";

  /*
   * SENSOR SR501
   */
 
  movementDetected = digitalRead(SR5PIN);  

  doc[3]["mac"] = ":8M";
  doc[3]["sensor_type"] = "MOVEMENT";
  doc[3]["value"] = movementDetected;
  doc[3]["unit"] = "BOOLEAN";
   
  
   /*
   * SENSOR HC MOVIMIENTO
   */
  /* 
  digitalWrite(TRIGPIN, LOW); // digitalWrite modifica el estado de una variable conectada al arduino, en este caso se apaga la salida trigger (sensor no esta funcionado)
  delay(1000); // delay en microsegundos
  digitalWrite(TRIGPIN, HIGH); // Se activa la salida trigger => el sensor empieza a funcionar
  delay(1000);
  digitalWrite(TRIGPIN, LOW); // Se apaga el sensor
  duration = pulseIn(ECHOPIN, HIGH); // Se obtiene el valor retornado por el pin echo
  distance = duration*0.034/2; // Se calcula la distancia en cm 
  delay(1000);

  if(distance < distancia_maxima) // Si esta a menos de la distancia maxima, se activa un evento
  {
    ledState = HIGH;
  }
  else
  {
    ledState = LOW;
  }
  digitalWrite(ledPin, LOW);
  */
  
  serializeJson(doc, msj);

  espSerial.println(msj);

  Serial.println(msj);

  msj[0] = 0;
  
  delay(2500);
  
}
