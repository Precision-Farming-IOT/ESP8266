#include <ESP8266WiFi.h>
#include <SPI.h>
#include <Wire.h>
#include <DHT.h>  // Including library for dht
#define DHTPIN D4          //pin where the dht11 is connected
DHT dht(D4, DHT11);
String apiKey = "7QG3VTURR20M3M4G"; // Enter your Write API key from ThingSpeak
const char *ssid = "pesu_iot";     // replace with your wifi ssid and wpa2 key
const char *pass = "Pesuiot@1108";
const char* server = "api.thingspeak.com";
const int motorPin = D5;
const int soilPin = A0;
 
WiFiClient client;

 void setup() {
  Serial.begin(115200); 
  pinMode(A0, INPUT);
  pinMode(motorPin, OUTPUT);
  digitalWrite(motorPin, LOW);
  WiFi.disconnect();
  delay(10);

  dht.begin();
  WiFi.begin(ssid, pass);

    while (WiFi.status() != WL_CONNECTED)
  {
  delay(500);
  }
    Serial.println("");
    Serial.println("WiFi connected");
    delay(4000);
}  
void loop() 
{
  int soil=(100.00-((analogRead(soilPin) / 1024.00) * 100.00));
  float h = dht.readHumidity((D4)-32)*5/9;
  float t = dht.readTemperature(D4)/3.1;
 
if (soil < 45){
    digitalWrite(motorPin, HIGH);
    Serial.print("Soil is dry,Motor is ON!\n");//turn on motor
    delay(15000);
    digitalWrite(motorPin, LOW);
    Serial.print("Soil is wet, Motor is OFF!\n");        
  }
  if (client.connect(server, 80)) // "184.106.153.149" or api.thingspeak.com
  {
    String postStr = apiKey;
      postStr += "&field1=";
      postStr += String(soil);
      postStr += "&field2=";
      postStr += String(t);
      postStr += "&field3=";
      postStr += String(h);
      postStr += "\r\n\r\n\r\n\r\n";
    
    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n");
    client.print(postStr);
    Serial.print("Soil Moisture: "); //Displaying the temperature in serial monitor
    Serial.print(soil); 
    Serial.print("\n");
    Serial.print("Temperature: ");
    Serial.println(t);
    Serial.print("Humidity: ");
    Serial.println(h);
  }
    client.stop();
    delay(15000);
}
      

