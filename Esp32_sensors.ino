#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <WiFi.h>
#include <Arduino_JSON.h>
#include <HTTPClient.h>
#include <HardwareSerial.h>
const byte rxPin = 16; //rx2
const byte txPin = 17; //tx2

HardwareSerial dwin(1);
//#define PIN_SG90 32 
//#define DHTpin 15    
#define MQ2 25
#define lum 14
#define ON_Board_LED 2 
int sensorValue = 0;
int lumValue = 0;
#define relayPin1 27
//#define relayPin2 2  
#define relayPin3 3  
#define relayPin4 4  

unsigned char Buffer[9];

#define temperature_add   0x61
#define humidity_add     0x62
#define Lum_add   0x63
#define Gaz_add     0x64
#define Pres_add     0x65

const char* ssid = "REPLACE_WITH_YOUR_SSID";
const char* password = "REPLACE_WITH_YOUR_PASSWORD";

unsigned char   Temperature[8] = {0x5a, 0xa5, 0x05, 0x82, temperature_add , 0x00, 0x00, 0x00};
unsigned char   Humidity[8] = {0x5a, 0xa5, 0x05, 0x82, humidity_add, 0x00, 0x00, 0x00};
unsigned char   Lum[8] = {0x5a, 0xa5, 0x05, 0x82, Lum_add , 0x00, 0x00, 0x00};
unsigned char   Gaz[8] = {0x5a, 0xa5, 0x05, 0x82, Gaz_add, 0x00, 0x00, 0x00};
unsigned char   Pres[8] = {0x5a, 0xa5, 0x05, 0x82, Pres_add, 0x00, 0x00, 0x00};
Adafruit_BME280 bme;

long start,endt;
String postData = ""; //--> Variables sent for HTTP POST request data.

void setup()
{
  Serial.begin(115200);
  dwin.begin(115200, SERIAL_8N1, rxPin, txPin);
  Serial.println();
  
  if (! bme.begin(0x76))
  {
  Serial.println("Could not find a valid BME280 sensor, check wiring!");
  while(1) delay(10);
}
  pinMode(relayPin1, OUTPUT);             // set relay pin to OUTPUT
//  pinMode(relayPin2, OUTPUT);             // set relay pin to OUTPUT
  pinMode(relayPin3, OUTPUT);             // set relay pin to OUTPUT
  pinMode(relayPin4, OUTPUT);             // set relay pin to OUTPUT
  pinMode(MQ2, INPUT);
  pinMode(lum, INPUT);
  digitalWrite(relayPin1, LOW);
  //digitalWrite(relayPin2, LOW);
  digitalWrite(relayPin3, LOW);
  digitalWrite(relayPin4, LOW);
  bool lightstatus = LOW;
  bool fanstatus = LOW;
  bool tvstatus = LOW;
  bool acstatus = LOW;

  //---------------------------------------- Make WiFi on ESP32 in "STA/Station" mode and start connecting to WiFi Router/Hotspot.
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
  Serial.println("Not connected Yet");
  digitalWrite(ON_Board_LED, HIGH);
  delay(250);
  digitalWrite(ON_Board_LED, LOW);
  delay(250);

  }
  
   digitalWrite(ON_Board_LED, HIGH);

}





void sensor_data();
void Display();



void loop()
{  
  start=millis();
   endt=start;
 while((endt-start)<=10000)
 {
  
 Display();
 endt=millis();
 }
  float temperature = bme.readTemperature();
  float press=bme.readPressure();
  float humidity = bme.readHumidity();
  sensorValue = analogRead(MQ2)/10;
  lumValue=analogRead(lum)/4;

  int h = humidity;
  int t=temperature;
  int pressure=press/10;    
  Temperature[6] = highByte(t);
  Temperature[7] = lowByte(t); 
  Humidity[6] = highByte(h);
  Humidity[7] = lowByte(h);  
  Lum[6] = highByte(lumValue);
  Lum[7] = lowByte(lumValue);
  Gaz[6] = highByte(sensorValue);
  Gaz[7] = lowByte(sensorValue); 
  Pres[6] = highByte(pressure);
  Pres[7] = lowByte(pressure);
  sensor_data();
  if(WiFi.status()== WL_CONNECTED) {
  HTTPClient http;  //--> Declare object of class HTTPClient.
  int httpCode;     //--> Variables for HTTP return code.
    http.begin("http://REPLACE_WITH_YOUR_COMPUTER_IP_ADDRESS/REPLACE_WITH_PROJECT_FOLDER_NAME_IN_htdocs_FOLDER/getdata.php");  //--> Specify request destination
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");        //--> Specify content-type header
    httpCode = http.POST(postData); //--> Send the request
       http.end();  //--> Close connection
  }
}

   /*
if (lightstatus)
  {
    digitalWrite(relayPin1, HIGH);
  }
  else
  {
    digitalWrite(relayPin1, LOW);
  }
  if (fanstatus)
  {
    digitalWrite(relayPin2, HIGH);
  }
  else
  {
    digitalWrite(relayPin2, LOW);
  }
  if (tvstatus)
  {
    digitalWrite(relayPin3, HIGH);
  }
  else
  {
    digitalWrite(relayPin3, LOW);
  }
  if (acstatus)
  {
    digitalWrite(relayPin4, HIGH);
  }
  else
  {
    digitalWrite(relayPin4, LOW);
  }*/


void sensor_data()
{
  dwin.write(Temperature, 8);
  dwin.write(Humidity, 8);
  dwin.write(Lum, 8); 
  dwin.write(Gaz, 8);
  dwin.write(Pres, 8);
   
}
/*
void PostData()
{
 
  http.begin("http://192.168.1.70/iot/test_data.php");
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  httpCode = http.POST(postData); //--> Send the request
  
}

 */ 

void Display()
{
  if (dwin.available())
  {   int i=0;
 while(dwin.available()>0)
 
  {
      Buffer[i] = dwin.read();
      delay(2);
     Serial.print(Buffer[i]);
      Serial.print("\t");
      i++;
  }
    Serial.println("");
    if (Buffer[0] == 0X5A && Buffer[3] == 0X83 )
    { 
      switch (Buffer[4])
      {
        case 0x66:   //for light
         Serial.print("Succcessful Communication ");
          if (Buffer[8] == 1)
          {  
            digitalWrite(relayPin1, HIGH);
          }
          else
          {
            digitalWrite(relayPin1, LOW);
          }
          break;
        case 0x67:   //for fan
Serial.print("Succcessful Communication ");
    if (Buffer[8] == 1)
          { 
            digitalWrite(relayPin1, HIGH);
          }
          else
          {
            digitalWrite(relayPin1, LOW);
          }
          break;
        case 0x68:   //for TV
Serial.print("Succcessful Communication ");
if (Buffer[8] == 1)
          {
            digitalWrite(relayPin1, HIGH);
          }
          else
          {
            digitalWrite(relayPin1, LOW);
          }
          break;
          case 0x69:   //for AC
Serial.print("Succcessful Communication ");
    if (Buffer[8] == 1)
          {
            digitalWrite(relayPin1, HIGH);
          }
          else
          {
            digitalWrite(relayPin1, LOW);
          }
          break;
             default: Serial.print("no data");

         
      
      }
    }
    
  }
}
