#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <PubSubClient.h>

#define SERVER_PORT 4080
const int serverPort = 4081;

const char* ssid = "iPhone";
const char* password = "1234aaaa";
/*
const char* ssid = "SFR_B8A8";
const char* password = "omar1234";
*/
String postData = ""; 
long start,endt;
WiFiServer TCPserver(SERVER_PORT);
const char* serverAddress ="172.20.10.5";
int ind1; // , locations
int ind2;
int ind3;
int ind4;
int ind5;
int ind100;
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
WiFiClient TCPclient;
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define NUMFLAKES     10 // Number of snowflakes in the animation example

#define LOGO_HEIGHT   16
#define LOGO_WIDTH    16
/*
IPAddress local_IP(172,20,10,50);
// Set your Gateway IP address
IPAddress gateway(172,20,1, 1);

IPAddress subnet(255, 255, 0, 0);
IPAddress primaryDNS(8, 8, 8, 8);   //optional
IPAddress secondaryDNS(8, 8, 4, 4); //optional
*/
String repeat ="";
void setup()
 {
  Serial.begin(115200);
  Serial.print("ESP32 #2: TCP Server IP address: ");
  Serial.println(WiFi.localIP());
  // 24:DC:C3:C5:23:20
 //connect to wifi

   WiFi.begin(ssid, password);
   while (WiFi.status() != WL_CONNECTED) {
   delay(1000);
   Serial.println("Connecting to WiFi...");
   }
   Serial.println("Connected to WiFi");
//connect to display
 if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever

  }  
  display.clearDisplay();
  display.setTextColor(WHITE);
  /*
 if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
    Serial.println("STA Failed to configure");
  }
  */
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
 if (TCPclient.connect(serverAddress, serverPort)) {
    Serial.println("Connected to TCP server");
  } else {
    Serial.println("Failed to connect to TCP server");
  }
  TCPserver.begin();

 }
void loop() {
 
  String cc= "";
   String sensorData ="";
  
 WiFiClient client = TCPserver.available(); 
  
  if (client)
{

  String sensorData = client.readStringUntil('x');
  repeat= sensorData ;
  
if (sensorData.isEmpty()) 
{
    Serial.println("Data received but corrrupted");
  } 
  else 
  {
   
    Serial.print("ESP32 #2: - Received sensor data: ");
    Serial.println(sensorData);
 ind1 = sensorData.indexOf(',');  //finds location of first ,
     String tt =  sensorData.substring(0, ind1);   //captures first data String
     int t = tt.toInt();
     ind2 = sensorData.indexOf(',', ind1+1 );   //finds location of second ,
     String hh  = sensorData.substring(ind1+1, ind2+1);   //captures second data String
      int h = hh.toInt();
      ind3 = sensorData.indexOf(',', ind2+1 );
       String pp= sensorData.substring(ind2+1, ind3+1);
      int p = pp.toInt();
      ind4 = sensorData.indexOf(',', ind3+1 );
     String LL= sensorData.substring(ind3+1); //captures remain part of data after last ,
     int L= LL.toInt();
      ind5 = sensorData.indexOf(',', ind4+1 );
     String GG= sensorData.substring(ind4+1); //captures remain part of data after last ,
      int G= GG.toInt();
   
   





  
  // clear display
  display.clearDisplay();
  
  // display temperature
  display.setTextSize(1);
  display.setCursor(0,0);
  display.print("Temperature: ");
  display.setTextSize(2);
  display.setCursor(0,10);
  display.print(t);
  display.print(" ");
  display.setTextSize(1);
  display.cp437(true);
  display.write(167);
  display.setTextSize(2);
  display.print("C");
  
  // display humidity
  display.setTextSize(1);
  display.setCursor(0, 35);
  display.print("Humidity: ");
  display.setTextSize(2);
  display.setCursor(0, 45);
  display.print(h);
  display.print(" %"); 
  display.display();
  delay(500);
   display.clearDisplay();
  
  // display Pressure
  display.setTextSize(1);
  display.setCursor(0,0);
  display.print("Pressure: ");
  display.setTextSize(2);
  display.setCursor(0,10);
  display.print(p);
  display.print(" Pa"); 


// display Luninosity
  display.setTextSize(1);
  display.setCursor(0, 35);
  display.print("Luminosity: ");
  display.setTextSize(2);
  display.setCursor(0, 45);
  display.print(L);
  display.print("  lux"); 

  display.display(); 

  delay(500);
  display.clearDisplay();

// display Gaz
  display.setTextSize(1);
  display.setCursor(0,0);
  display.print("Gaz: ");
  display.setTextSize(2);
  display.setCursor(0,10);
  display.print(G);
  display.print(" ppm"); 

 display.display();
delay(500);
 
    

  }
     //Now we got the data, resend it back                  -_-_-_-_-_-_-_-_-
    
if (TCPclient.connect(serverAddress, serverPort))
     {
       Serial.println("Connected to TCP server");
 
      
      Serial.println("Data sent, Now receiving... :");
      Serial.println();
      String SD="33,33,33,33,33x";
      TCPclient.print(SD);
        

      
      
       TCPclient.flush();
    } 
    else {
      Serial.println("Failed to reconnect to TCP server");
    }
}

ind1 = repeat.indexOf(',');  //finds location of first ,
     String tt =  repeat.substring(0, ind1);   //captures first data String
     int t = tt.toInt();
     ind2 = repeat.indexOf(',', ind1+1 );   //finds location of second ,
     String hh  = repeat.substring(ind1+1, ind2+1);   //captures second data String
      int h = hh.toInt();
      ind3 = repeat.indexOf(',', ind2+1 );
       String pp= repeat.substring(ind2+1, ind3+1);
      int p = pp.toInt();
      ind4 = repeat.indexOf(',', ind3+1 );
     String LL= repeat.substring(ind3+1); //captures remain part of data after last ,
     int L= LL.toInt();
      ind5 = repeat.indexOf(',', ind4+1 );
     String GG= repeat.substring(ind4+1); //captures remain part of data after last ,
      int G= GG.toInt();
   

   display.clearDisplay();
  
  // display temperature
  display.setTextSize(1);
  display.setCursor(0,0);
  display.print("Temperature: ");
  display.setTextSize(2);
  display.setCursor(0,10);
  display.print(t);
  display.print(" ");
  display.setTextSize(1);
  display.cp437(true);
  display.write(167);
  display.setTextSize(2);
  display.print("C");
  
  // display humidity
  display.setTextSize(1);
  display.setCursor(0, 35);
  display.print("Humidity: ");
  display.setTextSize(2);
  display.setCursor(0, 45);
  display.print(h);
  display.print(" %"); 
  display.display();
  delay(500);
   display.clearDisplay();
  
  // display Pressure
  display.setTextSize(1);
  display.setCursor(0,0);
  display.print("Pressure: ");
  display.setTextSize(2);
  display.setCursor(0,10);
  display.print(p);
  display.print(" Pa"); 


// display Luninosity
  display.setTextSize(1);
  display.setCursor(0, 35);
  display.print("Luminosity: ");
  display.setTextSize(2);
  display.setCursor(0, 45);
  display.print(L);
  display.print("  lux"); 

  display.display(); 

  delay(500);
  display.clearDisplay();

// display Gaz
  display.setTextSize(1);
  display.setCursor(0,0);
  display.print("Gaz: ");
  display.setTextSize(2);
  display.setCursor(0,10);
  display.print(G);
  display.print(" ppm"); 

 display.display();
delay(500);

if(WiFi.status()== WL_CONNECTED) {
  HTTPClient httpp;  //--> Declare object of class HTTPClient.
  int httpCode;     //--> Variables for HTTP return code.

    String pp="on";
     String v="on";
      String l1="on";
       String l2="on";
      // t=14,h=14,L=40,p=1000, G=15;
    postData = "&Temperature=" + String(t);
    postData += "&Humidity=" + String(h);
    postData += "&Luminosity=" + String(L);   
    postData += "&Gaz=" + String(G); 
    postData += "&Pressure=" + String(p);   
    postData += "&Printer=" + pp;
    postData += "&Ventilator=" +v;    
    postData += "&Led1=" + l1;
    postData += "&Led2=" + l2;
     Serial.println("Data to Post: ");
      Serial.println(postData);
    httpp.begin("http://172.20.10.4/iot/test.php");  //--> Specify request destination
    httpp.addHeader("Content-Type", "application/x-www-form-urlencoded");        //--> Specify content-type header
    httpCode = httpp.POST(postData); //--> Send the request
    httpp.end();  //--> Close connection
   // delay(500);
   Serial.println("Data post to MARIADB");
 
  }

}



