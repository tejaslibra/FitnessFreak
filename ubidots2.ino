#include <LBattery.h>
#include <LGPRS.h>
#include <LGPRSClient.h>
#include "DHT.h"

#define URL    "things.ubidots.com"
#define TOKEN "26cNTAAstEOgNi7HXymobeV5ZlPy17"  // replace with your Ubidots token generated in your profile tab
#define VARID1 "56d186eb762542559314a0a2"    //Temperature variable id           
#define VARID2 "56d1871976254256c161ee1c"    //Humidity variable id
#define VARID3 "56d1878376254259f50d65e1"    //Battery Level variable id 


// Reading temperature or humidity takes about 250 milliseconds!
// Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)  
#define DHTPIN 2     // Digital pin 2 where DHT 22 is connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)
DHT dht(DHTPIN, DHTTYPE);

char buff[256];  //Info about battery level and charging

//Temperature and Humidity sensor readings from DHT 22
float t = 0.0;
float h = 0.0;
 
//Location where this logger is placed!! 
String loclat="19.21833";
//Serial.print("1");
//delay(10);

String loclng="72.978088";
//Serial.print("2");
//delay(10);
String Location;

//Initializing gprs
LGPRSClient client;  //GPRS Client

void setup()
{
  Serial.begin(9600);
  Serial.println("Temperature Humidity Logger by Er.Amol");
  
  dht.begin();

// Creating a string containing latitude and longitude information of the Logger's location
  Location="{\"lat\":";
  Location=Location+loclat;
  Location += " ,\"lng\":";
  Location=Location+loclng+ "}";
  
  Serial.println("Attach to GPRS network by APN setting");
  //Note add Access point name, username and password according to your sim card manufacturer/service provide
   while (!LGPRS.attachGPRS())
  {
    Serial.println("Trying to attach...");
    delay(500);
    Serial.print("3");
delay(10);
   }

  LGPRSClient globalclient ;
  client = globalclient;
  Serial.println("GPRS attached !!.");
  delay(2000);
Serial.print("4");
delay(10);
}

void loop()
{
  Serial.println("Regreat");
  if(dht.readHT(&t, &h)) //Reading Temperature and humidity from DHT 22
 {
  delay(2000);
  dht.readHT(&t, &h);
  Serial.println("------------------------------");
  Serial.print("Temperature: ");
  Serial.println(t);

  Serial.print("Humidity: ");
  Serial.println(h);  

  sprintf(buff,"Battery level = %d", LBattery.level() );
  Serial.println(buff);
 
 String payload = "[{\"variable\":\"" VARID1 "\",\"value\":"+ String(t)+",\"context\":"+Location+"},{\"variable\":\"" VARID2 "\",\"value\":" + String(h) + "},{\"variable\":\"" VARID3 "\",\"value\":" + String(LBattery.level()) + "}]";
 
 String le = String(payload.length());
  
  // if you get a connection, report back via serial:
  Serial.print("Connect to ");
  Serial.println(URL);
  if (client.connect(URL, 80))
  {
    // Build HTTP POST request
    Serial.println("connected");    
    client.print(F("POST /api/v1.6/collections/values/?token="));
    client.print(TOKEN);
    client.println(F(" HTTP/1.1"));
    client.println(F("Content-Type: application/json"));
    client.print(F("Content-Length: "));
    client.println(le);
    client.print(F("Host: "));
    client.println(URL);
    client.println(); 
    client.println(payload);  
  }
  else
  {
    // if you didn't get a connection to the server:
    Serial.println("connection failed");
  }
  delay(100);
  // if there are incoming bytes available
  // from the server, read them and print them:
  if (client.available())
  {
    char c = client.read();   
    Serial.print(c);
  }

  // if the server's disconnected, stop the client:
  if (!client.available() && !client.connected())
  {
    Serial.println();
    Serial.println("disconnecting.");
    client.stop();
  }

  //Delay of about 1 minute or 60 seconds
   for (int minutes = 0; minutes < 7; minutes++) 
   {
     delay(10000);  //10 seconds delay or 10 thousand milli-seconds
   }
   
  }
}

