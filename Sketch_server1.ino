#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino.h>
#include <Adafruit_Fingerprint.h>
#include <HardwareSerial.h>
#include <PubSubClient.h>
const int button []={2,3,4};
int ButtonState[]={0,0,0,0};
int pass[]={0,1,2,2};
int level=0;

const char* mqttServer = "172.20.10.4";
const int mqttPort = 1883;


const int buttonPin0 = 0;
const int buttonPin2 = 2;
const int buttonPin15 = 15;

int buttonState0 = 0;
int buttonState2 = 0;
int buttonState15 = 0;
int pressCount15 = 0;

const byte rxPin1 = 25; //rx2
const byte txPin1 = 26; //tx2

const byte rxPin2 = 15; //rx2
const byte txPin2 = 16; //tx2

HardwareSerial serialPort(2); // use UART2
uint8_t getFingerprintID();
void callback(char* topic, byte* payload, unsigned int length);
void reconnect();
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&serialPort);

HardwareSerial dwin(1);
//#define PIN_SG90 32 
//#define DHTpin 15    
#define MQ2 35
#define lum 32
#define ON_Board_LED 2 

#define relayPin1 27
//#define relayPin2 2  
#define relayPin3 3  
#define relayPin4 4  
#define SERVER_PORT 4081

unsigned char Buffer[9];

#define temperature_add   0x61
#define humidity_add     0x62
#define Lum_add   0x63
#define Gaz_add     0x64
#define Pres_add     0x65
#define humidity_topic "sensor/decode"



const char* ssid = "iPhone";
const char* password = "1234aaaa";
//const char* serverAddress = "192.168.1.89";
const char* serverAddress ="172.20.10.7";
const int serverPort = 4080;
WiFiServer TCPserver(SERVER_PORT);

unsigned char   Temperature[8] = {0x5a, 0xa5, 0x05, 0x82, temperature_add , 0x00, 0x00, 0x00};
unsigned char   Humidity[8] = {0x5a, 0xa5, 0x05, 0x82, humidity_add, 0x00, 0x00, 0x00};
unsigned char   Lum[8] = {0x5a, 0xa5, 0x05, 0x82, Lum_add , 0x00, 0x00, 0x00};
unsigned char   Gaz[8] = {0x5a, 0xa5, 0x05, 0x82, Gaz_add, 0x00, 0x00, 0x00};
unsigned char   Pres[8] = {0x5a, 0xa5, 0x05, 0x82, Pres_add, 0x00, 0x00, 0x00};
Adafruit_BME280 bme;

long start,endt;
String postData = ""; //--> Variables sent for HTTP POST request data.
String sensor="";
WiFiClient TCPclient;
bool okReceived = false;
String receivedMessage="";
PubSubClient client(TCPclient);


int getFingerprintIDez();
void setup()
{
  Serial.begin(115200);
  dwin.begin(115200, SERIAL_8N1, rxPin1, txPin1);
  Serial.println();
  delay(100);
  Serial.print("ESP Board MAC Address:  ");
  Serial.println(WiFi.macAddress());
  pinMode(buttonPin0, INPUT_PULLUP);
  pinMode(buttonPin2, INPUT_PULLUP);
  pinMode(buttonPin15, INPUT_PULLUP);

  if (! bme.begin(0x76))
  {
  Serial.println("Could not find a valid BME280 sensor, check wiring!");
  while(1) delay(10);
}
 finger.begin(57600);
   delay(100);
if (finger.verifyPassword())
    {
        Serial.println("Found fingerprint sensor!");
    }
    else
    {
        Serial.println("Did not find fingerprint sensor :(");
        while (1)
        {
            delay(1);
        }
    }
  pinMode(relayPin1, OUTPUT);             // set relay pin to OUTPUT
//  pinMode(relayPin2, OUTPUT);             // set relay pin to OUTPUT
  //pinMode(relayPin3, OUTPUT);             // set relay pin to OUTPUT
  //pinMode(relayPin4, OUTPUT);             // set relay pin to OUTPUT
  pinMode(MQ2, INPUT);
  pinMode(lum, INPUT);
  digitalWrite(relayPin1, LOW);
 
  //digitalWrite(relayPin3, LOW);
 // digitalWrite(relayPin4, LOW);
  bool lightstatus = LOW;
  bool fanstatus = LOW;
  bool tvstatus = LOW;
  bool acstatus = LOW;

int x=-1;
 
while (x==-1)
{
  Serial.println("Please Login first");
  x=getFingerprintIDez();

    delay(500); 
  
}
Serial.println("Stage 1 unlocked");


  //---------------------------------------- Make WiFi on ESP32 in "STA/Station" mode and start connecting to WiFi Router/Hotspot.
 // WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
  delay(250);
  Serial.println("not connected");
  delay(250);

  }
    Serial.println("connected");
    Serial.print("ESP32 #2: TCP Server IP address: ");
    Serial.println(WiFi.localIP());
   delay(500); 

    if (TCPclient.connect(serverAddress, serverPort)) {
    Serial.println("Connected to TCP server");
  } else {
    Serial.println("Failed to connect to TCP server");
  }
    TCPserver.begin();
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);

  if (!client.connected()) {
    reconnect();
  }
while(okReceived==false)
{
client.loop();
   if (okReceived==true) {
    // Do something when "ok" is received
    Serial.println("Stage 2 unlocked");
    // Reset the flag
    
  }

    
}
okReceived = false;


  waitForUnlockSequence();

void sensor_data();
void Display();

}

void loop()
{
delay(1000);
   WiFiClient client = TCPserver.available(); 
   Serial.print("ESP32 #2: TCP Server IP address: ");
  Serial.println(WiFi.localIP());
 
 ///*
  start=millis();
  endt=start;
 while((endt-start)<=10000)
 {
      Serial.println("Input");

 Display();
 endt=millis();
 }

                                                                          //Recuperation valeurs des capteurs//


  
  float temperature = bme.readTemperature();
  float press=bme.readPressure();
  float humidity = bme.readHumidity();
  int sv=analogRead(MQ2);
  int lv=analogRead(lum);
  int h = humidity;
  int t=temperature;
  int pressure=press/100;   
  







                                                           //Envoie des données à L'IHM
   Temperature[6] = highByte(t);
  Temperature[7] = lowByte(t); 

  Humidity[6] = highByte(h);
  Humidity[7] = lowByte(h);  

  Lum[6] = highByte(lv);
  Lum[7] = lowByte(lv);

  Gaz[6] = highByte(sv);
  Gaz[7] = lowByte(sv); 

  Pres[6] = highByte(pressure);
  Pres[7] = lowByte(pressure);


  sensor_data();

                                                                             //Affichage serial monitor
  Serial.println(t);
  Serial.println(h);
  Serial.println(pressure);
  Serial.println(sv);
  Serial.println(lv);
  
                                                                              //Envoie des données au serveur

  if(WiFi.status()== WL_CONNECTED) {
  HTTPClient http;  //--> Declare object of class HTTPClient.
  int httpCode;     //--> Variables for HTTP return code.

    String p="off";
     String v="off";
      String l1="off";
       String l2="off";
    postData = "&Temperature=" + String(t);
    postData += "&Humidity=" + String(h);
    postData += "&Luminosity=" + String(lv);   
    postData += "&Gaz=" + String(sv); 
    postData += "&Pressure=" + String(pressure);   
    postData += "&Printer=" + p;
    postData += "&Ventilator=" +v;    
    postData += "&Led1=" + l1;
    postData += "&Led2=" + l2;
    
    http.begin("http://172.20.10.4/iot/test_data.php");  //--> Specify request destination
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");        //--> Specify content-type header
    httpCode = http.POST(postData); //--> Send the request
     Serial.println("Data to Post: ");
      Serial.print(postData);
    http.end();  //--> Close connection
   // delay(500);

 
  }
  //delay(100);
  
  

  String sensor="";
      sensor.concat(t);
      sensor.concat(",");
      sensor.concat(h);
       sensor.concat(",");
      sensor.concat(pressure);
       sensor.concat(",");
      sensor.concat(lv);
       sensor.concat(",");
      sensor.concat(sv);
      sensor.concat("x");

 //*/
if (TCPclient.connect(serverAddress, serverPort))
     {
       Serial.println("Connected to TCP server");
 
      
      Serial.println("Data to send :");
      Serial.println(sensor);

      TCPclient.print(sensor);
       TCPclient.flush();
    } 
    else {
      Serial.println("Failed to reconnect to TCP server");
    }

    if (client)
{

  String sensorData = client.readStringUntil('x');
  
  
if (sensorData.isEmpty()) 
{
    Serial.println("Data received but corrrupted");
  } 
  else 
  {
   
    Serial.print("ESP32 #2: - Received sensor data: ");
    Serial.println(sensorData);
  //Now we got the data, resend it back                  -_-_-_-_-_-_-_-_-
    
 
    
    
    
  
    Serial.println("Data Sent: ");
    Serial.println(sensorData);

  }
   
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
  }
*/
void sensor_data()
{
  dwin.write(Temperature, 8);
  dwin.write(Humidity, 8);
  dwin.write(Lum, 8); 
  dwin.write(Gaz, 8);
  dwin.write(Pres, 8);
   
}

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

int getFingerprintIDez()
{
    uint8_t p = finger.getImage();
    if (p != FINGERPRINT_OK)
        return -1;

    p = finger.image2Tz();
    if (p != FINGERPRINT_OK)
        return -1;

    p = finger.fingerFastSearch();
    if (p != FINGERPRINT_OK)
        return -1;

    // found a match!
    Serial.println(finger.fingerID);
    return finger.fingerID;
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    receivedMessage += (char)payload[i];
  }
  Serial.println();
  Serial.println(receivedMessage);

  // Check if the received message is "ok"
if (receivedMessage=="ok") {
    okReceived = true;
  }
receivedMessage="";
}


void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
String clientId="ESP32Client-";
clientId+= String(random(0xffff),HEX);

    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      client.subscribe("sensor/decode");
      client.publish("sensor/decode", "hello someones is trying to access, Answer 'ok' to allow access");
      
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}
void waitForUnlockSequence() {
  Serial.println("Enter the unlock sequence...");
  bool sequenceCorrect = false;
  int button0Count = 0;
  int button2Count = 0;
  int button15Count = 0;

  while (!sequenceCorrect) {
    // Reset counts
    button0Count = 0;
    button2Count = 0;
    button15Count = 0;

    // Wait for button presses
    while (!(button15Count == 2 && button2Count == 1 && button0Count == 2)) {
      // Monitor button presses
      buttonState0 = digitalRead(buttonPin0);
      buttonState2 = digitalRead(buttonPin2);
      buttonState15 = digitalRead(buttonPin15);

      // Print button presses to Serial Monitor
      if (buttonState0 == LOW) {
        while (digitalRead(buttonPin0) == LOW);
        button0Count++;
        Serial.println("Button 0 pressed. Count: " + String(button0Count));
        delay(500);
      }
      if (buttonState2 == LOW) {
        while (digitalRead(buttonPin2) == LOW);
        button2Count++;
        Serial.println("Button 2 pressed. Count: " + String(button2Count));
         delay(500);
      }
      if (buttonState15 == LOW) {
        while (digitalRead(buttonPin15) == LOW);
        button15Count++;
        Serial.println("Button 15 pressed. Count: " + String(button15Count));
         delay(500);
      }

      // Check if the same button is pressed more than twice
      if (button0Count > 2 || button2Count > 2 || button15Count > 2) {
        Serial.println("Incorrect sequence. Please try again...");
        break;
      }
      if (button0Count+button2Count+button15Count > 5) {
        Serial.println("Incorrect sequence. Please try again...");
        break;
      }
    }

    // Check if the sequence is correct
    if (button15Count == 2 && button2Count == 1 && button0Count==2) {
      Serial.println("System Unlocked! Proceeding to the main program...");
      sequenceCorrect = true;
    }
  }
}