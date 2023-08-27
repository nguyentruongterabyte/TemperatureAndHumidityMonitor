#include <DHT.h>
#include <ESP8266WiFi.h>
#include "mqtt_secrets.h"

String apiKey = "0857N68YWU0VGMBE";

const char* server = "api.thingspeak.com";
#define DHTPIN D1 //pin where the dht11 is connected (D1)

DHT dht(DHTPIN, DHT11);
WiFiClient client;

void setup() {
  Serial.begin(115200);
  delay(10);
  dht.begin();

  Serial.println("Connecting to ");
  Serial.println(SECRET_WIFI_NAME);

  WiFi.begin(SECRET_WIFI_NAME, SECRET_WIFI_PASSWORD);

  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi Connected");
}

void loop() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (client.connect(server, 80)) // 184.106.153.149 or api.thingspeak.com
  {
    String postStr = apiKey;
    postStr += "&field1=";
    postStr += String(t);
    postStr += "&field2=";
    postStr += String(h);
    postStr += "\r\n\r\n";

    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection.close\n");
    client.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
    client.print("Connect-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n");
    client.print(postStr);

    Serial.print("Temperature: ");
    Serial.print(t);
    Serial.print(" degrees Celcius, Humidity: ");
    Serial.print(h);
    Serial.println("%. Send to Thingspeak.");
    
    
  }
  client.stop();
  Serial.println("Waiting...");
  delay(10000);
}
