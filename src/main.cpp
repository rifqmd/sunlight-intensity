#include <DHT.h>
#include <ESP8266WiFi.h>
#include <Wire.h>
#include <BH1750.h>

char ssid[] = "Username";
char pass[] = "Password";
DHT dht(D5, DHT22);

const char* host = "api.thingspeak.com";
const char* apiKey = "API";

WiFiClient client;
BH1750 lightMeter;


void setup() {
  Serial.begin(9600);
  dht.begin();
  WiFi.begin(ssid, pass);
  Wire.begin(D4, D3);
  lightMeter.begin();
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");
  Serial.println(F("BH1750 Test begin"));
}

void loop() {
  delay(15000);
  // delay(100);

  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float lux = lightMeter.readLightLevel();
  // int q = analogRead(A0);
  
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  if (isnan(lux)) {
    Serial.println("Failed to read from BH1750 sensor!");
    return;
  }

  String url = "/update?api_key=";
  url += apiKey;
  url += "&field1=";
  url += String(lux);
  url += "&field2=";
  url += String(t);
  url += "&field3=";
  url += String(h);

  if (client.connect(host, 80)) {
    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "Connection: close\r\n\r\n");
    delay(100);

    Serial.println("Data sent to ThingSpeak");
    Serial.print("Temperature: ");
    Serial.print(t);
    Serial.println("°C");

    Serial.print("Humidity: ");
    Serial.print(h);
    Serial.println("%");

    Serial.print("Lux Meter: ");
    Serial.print(lux);
    Serial.println(" lx");

    client.stop();
  } else {
    Serial.println("Failed to connect to ThingSpeak server");
  }
}