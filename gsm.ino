#include <LGSM.h>
#include "DHT.h"
#define DHTPIN 2     // what pin we're connected to

#define DHTTYPE DHT22   // DHT 22  (AM2302)

DHT dht(DHTPIN, DHTTYPE);

char num[20] = "8082488940";           // your number write here
char text[100] = "Lets go for a walk,Its best out there";    // what do you want to send
 
 float t = 0.0;
 float h = 0.0;
//const int pinLight = A0;                // light sensor connect to A0
 
//bool istemp()

 
void setup()
{
    Serial.begin(115200);
 
   // while(!istemp());             // is temp less
   // delay(2000);
    
    Serial.println("DHTxx test!");
{
    dht.begin();
    Serial.println("1");
    delay(10);
}
}
 
 
void loop()
{
  // float t = 0.0;
    //float h = 0.0;
     Serial.println("2");
    delay(10);
    
    
    if(dht.readHT(&t, &h))
    {
        Serial.println("------------------------------");
        Serial.print("temperature = ");
        Serial.println(t);
        
        Serial.print("humidity = ");
        Serial.println(h);
    }
    else  Serial.print("read failed = ");
    
    delay(2000);

    //demo of UV
     int sensorValue;
    long  sum=0;
    
    for(int i=0; i<32; i++)
    {
        sensorValue=analogRead(A0);
        sum=sensorValue+sum;
        delay(1);
    }
    
    sum = sum >> 5;
    
    Serial.print("The voltage value:");
    Serial.print(sum*4980.0/1023.0);
    Serial.println("mV");
    
    delay(20);
    
    Serial.print("\n");
    if(!istemp())                 // box is open
    {
        Serial.println("box had been opened");
 
        while(!LSMS.ready())
        {
            delay(1000);
        }
 
        Serial.println("SIM ready for work!");
        LSMS.beginSMS(num);
        LSMS.print(text);
 
        if(LSMS.endSMS())
        {
            Serial.println("SMS is sent");
        }
        else
        {
            Serial.println("SMS send fail");
        }
 
       //  while(!istemp());             // until put in box
        delay(2000);
    }
 
   // delay(10000);
}

bool istemp()
{
   Serial.println("Inside isTemp()--->");
    return (analogRead(t)<30);   // when temperature less than 30,
}
 
