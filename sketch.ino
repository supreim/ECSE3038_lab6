#include <WiFi.h>
#include <Wire.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <WiFiClientSecure.h>
#include "env.h"

void getLightState();
float readTemperature();
void updateTemperature(float currentTemp);

WiFiClientSecure client;




void setup() {
    Serial.begin(115200);

    // Connect to WiFi
    if (IS_WOKWI)
        WiFi.begin(SSID, PASS, CHANNEL);
    else
        WiFi.begin(SSID, PASS);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nConnected to WiFi");
    delay(1000);
     
    client.setInsecure();

    // Set LED pin mode
    pinMode(led_pin, OUTPUT);
}


void loop() {
  getLightState();
  float currentTemp = readTemperature();
  updateTemperature(currentTemp);
}

void getLightState() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = String(ENDPOINT) + "/api/light";
    http.begin(client,url);
    http.addHeader("api-key", API_KEY);

    int httpResponseCode = http.GET();
    if (httpResponseCode > 0) {
      String payload = http.getString();
      Serial.println("GET /api/light response:");
      Serial.println(payload);
      
      // Parse JSON response
      DynamicJsonDocument doc(1024);
      deserializeJson(doc, payload);
      bool lightState = doc["light"];
      
      // Control the LED based on API response
      digitalWrite(led_pin, lightState ? HIGH : LOW);
      Serial.print("LED set to: ");
      Serial.println(lightState ? "ON" : "OFF");
    } else {
      Serial.print("Error on GET: ");
      Serial.println(http.errorToString(httpResponseCode));
    }
    http.end();
  }
}

float readTemperature() {
  int sensorValue = analogRead(temp_sensor);
  float voltage = sensorValue * (3.3 / 4095.0);
  
  float temperature = voltage * 100;
  Serial.print("Temperature: ");
  Serial.println(temperature);
  return temperature;
}

void updateTemperature(float currentTemp) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = String(ENDPOINT) + "/api/temp";
    http.begin(client, url);
    http.addHeader("api-key", API_KEY);
    http.addHeader("Content-Type", "application/json");

    DynamicJsonDocument doc(1024);
    doc["temp"] = currentTemp;
    String requestBody;
    serializeJson(doc, requestBody);

    int httpResponseCode = http.PUT(requestBody);
    if (httpResponseCode > 0) {
      String payload = http.getString();
      Serial.println("PUT /api/temp response:");
      Serial.println(payload);
    } else {
      Serial.print("Error on PUT: ");
      Serial.println(http.errorToString(httpResponseCode));
    }
    http.end();
  }
}
