#include <SoftwareSerial.h>
#include <DHT.h>

//PINS
#define DHTPIN 7
#define MQ7PIN 2
#define TRIGPIN 9
#define ECHOPIN 10

#define DHTTYPE DHT11

// Variables aux
String str;
const int distancia_maxima = 20;
long duration; 
int distance;
DHT dht(DHTPIN, DHT11);
const int ledPin =  LED_BUILTIN; // Se define a la led integrada en el arduino
int ledState = LOW; // Se define un estado inicial HIGH = prendido, LOW = apagado

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

  dht.begin(); // API DHT para el sensor de humedad y temperatura
  
  Serial.begin(9600); 
}

void loop()
{
  /*
   * SENSOR DHT11 TEMPERATURA Y HUMEDAD
   */
  delay(500);

  float h = dht.readHumidity();        // humedad
  float t = dht.readTemperature();     // temperatura en Celcius
  
  if (isnan(h) || isnan(t)) 
    Serial.println("Failed to read from DHT sensor!");
  else
  {
  Serial.print(" Humidity: ");
  Serial.print(h);
  Serial.print("%  Temperature: ");
  Serial.print(t);
  Serial.println(" °C ");
  }


  /*
   * SENSOR MQ7
   */
   
   // -- 1 version digital
   boolean mq_estado = digitalRead(MQ7PIN); // Se obtiene el valor negado al sensado, arroja 1 si no detecta gas
   if(mq_estado)
      Serial.println("Sin presencia de GAS");
  else 
      Serial.println("GAS detectado");
 
  
  // -- 2 version analogica
  int adc_MQ = analogRead(A0); //Lemos la salida analógica del MQ
  float voltaje = adc_MQ * (5.0 / 1023.0); //Convertimos la lectura en un valor de voltaje

  if(adc_MQ > 100)
      Serial.println("Se detecta un el GAS");

  Serial.print("adc:");
  Serial.print(adc_MQ);
  Serial.print("    voltaje:");
  Serial.println(voltaje);
    
  delay(100); 

  
   /*
   * SENSOR HC MOVIMIENTO
   */
   
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

  /*
   * Comunicacion con nodemcu
   */
  Serial.println("hola desde arduino");
  
}
