#include <DHT.h>
#include <EspMQTTClient.h>
#include "mqtt_secrets.h"

#define DHTPIN D1 //pin where the dht11 is connected (D1)
#define DHTTYPE DHT11

unsigned long delayTime = 10000;

DHT dht(DHTPIN, DHTTYPE);


EspMQTTClient client (
  SECRET_WIFI_NAME,
  SECRET_WIFI_PASSWORD,
  MQTT_BROKER_SERVER_IP,
  SECRET_MQTT_USERNAME,
  SECRET_MQTT_PASSWORD, 
  SECRET_MQTT_CLIENT_ID
);

void onConnectionEstablished() {
  // Subscribe to "channels/""/subscribe" and display received message to Serial
  client.subscribe("channels/2252582/subscribe", [](const String & payload) {
    Serial.println(payload);  
  });  

  float h = dht.readHumidity();
  float t = dht.readTemperature();

  String message = "field1=" + String(t) + 
                   "&field2=" + String(h);

  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" degrees Celcius, Humidity: ");
  Serial.print(h);
  Serial.println("%. Send to Thingspeak.");

  client.publish("channels/2252582/publish", message);

  // Excute delayed instructions
  client.executeDelayed(5 * 1000, [](){});

}

void setup() {
  Serial.begin(115200);
  dht.begin();
  client.enableDebuggingMessages();
  client.enableHTTPWebUpdater();
  client.enableOTA();
  client.enableLastWillMessage("TestClient/lastwill", "I am going offline");

  
}

void loop() {
  client.loop();
  delay(delayTime);
}
