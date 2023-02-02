#include <Arduino.h>
#include <Constants.h>
#include <Data.h>
#include <WiFiFunctions.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

HTTPClient http;
WiFiClient wifi;

void WiFiinit()
{
  Serial.print(F("Initialising Wifi "));
  WiFi.mode(WIFI_STA);
  WiFi.config(IP, GATE, MASK);
  WiFi.begin(AP_SSID, AP_PASS);
  WiFi.persistent(true);
  WiFi.setAutoConnect(true);
  WiFi.setAutoReconnect(true);
}

void hibernate()
{
  ESP.deepSleep(SLEEP_SEC * 1000U * 1000U);
  delay(10);
}

void wait()
{
  delay(SLEEP_SEC * 1000U);
}

boolean WiFiconnect()
{
  if (WiFi.status() != WL_CONNECTED && WiFi.SSID() != AP_SSID)
  {
    WiFiinit();
  }

  unsigned long startTime = millis();
  unsigned long tries = 1;
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(200);
    Serial.print('.');
    unsigned long elapsedTime = millis() - startTime;
    if (elapsedTime > 15000)
    {
      WiFiinit();
      startTime = millis();
      tries++;
    }
    if (tries > 3)
    {
      hibernate();
    }
  }
  Serial.print(" Connected! IP address: ");
  Serial.println(WiFi.localIP());
  return true;
}

int updateDevice(const String id, const String value, const boolean terminal)
{
  ESP.wdtFeed();
  int httpCode = 0;
  String url = "http://" + String(BACKEND_HOST) + ":" + BACKEND_PORT + "/api/services/virtual/set";
  String payload = String("{\"entity_id\":\"[id]\",\"value\":[value]}");
  payload.replace("[id]", id);
  payload.replace("[value]", value);

  http.begin(wifi, url);
  http.setTimeout(5000U);

  http.addHeader("Host", BACKEND_HOST);
  http.addHeader("Authorization", "Bearer " + String(SECURITY_TOKEN));
  http.addHeader("Accept", "*/*");
  http.addHeader("Content-Type", "application/json");
  http.addHeader("Upgrade-Insecure-Requests", "1");
  http.addHeader("User-Agent", "ESP8266 Plant Sensor 1 v3");

  if (terminal)
  {
    http.addHeader("Connection", "close");
  }

  Serial.println((char *)url.c_str());
  Serial.println((char *)payload.c_str());

  httpCode = http.POST(payload);
  if (httpCode > 0)
  {
    // Serial.printf("Domoticz send code: %d\n", httpCode);
    if (httpCode != HTTP_CODE_OK)
    {
      String payload = http.getString();
      Serial.println(payload);
    }
  }
  else
  {
    Serial.printf("Send failed, error: %s\n", http.errorToString(httpCode).c_str());
  }

  http.end();
  return httpCode;
}

void sendDataDomoticz() {
  digitalWrite(LED_BUILTIN, LOW);
  const float moisturePercent = (100.0F - ((soilMv * 1.0F) / 20.0F));
  if (moisturePercent > 0.0F && moisturePercent < 100.0F && WiFiconnect())
  {
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.print("Send begin...\n");
    updateDevice("sensor.virtual_plant_2_moisture", String(moisturePercent, 2U), false);
    digitalWrite(LED_BUILTIN, LOW);
  }
}
