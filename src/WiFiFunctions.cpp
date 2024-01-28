#include <Arduino.h>
#include <Constants.h>
#include <Data.h>
#include <WiFiFunctions.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <Common.h>

ESP8266WiFiMulti WiFiMulti;
byte ledState = B0;

void hibernate()
{
  ESP.deepSleep(SLEEP_SEC * 1000U * 1000U);
  delay(10);
}

void wait()
{
  delay(SLEEP_SEC * 1000U);
}

void WiFiinit()
{
  Serial.print(F("Initialising Wifi "));
  WiFiMulti.addAP(AP_SSID, AP_PASS);
  WiFi.mode(WIFI_STA);
  WiFi.persistent(true);
  WiFi.setAutoConnect(true);
  WiFi.setAutoReconnect(true);
}

boolean WiFiconnect()
{
  ESP.wdtFeed();
  if (WiFi.SSID() != AP_SSID)
  {
    WiFiinit();
  }

  unsigned long startTime = millis();
  unsigned long tries = 1U;
  while (WiFiMulti.run() != WL_CONNECTED)
  {
    delay(200);
    Serial.print('.');
    ESP.wdtFeed();
    unsigned long elapsedTime = millis() - startTime;
    if (elapsedTime > 15000U)
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

  WiFiClient client;
  HTTPClient http;
  http.begin(client, url);
  http.setTimeout(5000U);

  http.addHeader("Host", BACKEND_HOST);
  http.addHeader("Authorization", "Bearer " + String(SECURITY_TOKEN));
  http.addHeader("Accept", "*/*");
  http.addHeader("Content-Type", "application/json");
  http.addHeader("Upgrade-Insecure-Requests", "1");
  http.addHeader("User-Agent", "ESP8266 Plant Sensor " + String(DEVICE_ID));

  if (terminal)
  {
    http.addHeader("Connection", "close");
  }

  Serial.println((char *)url.c_str());
  Serial.println((char *)payload.c_str());

  digitalWrite(LED_BUILTIN, ledState ^= B1);

  httpCode = http.POST(payload);
  if (httpCode > 0)
  {
    Serial.printf("Send code: %d\n", httpCode);
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

void sendData() {
  const float moisturePercent = mvToMoisture(soilMv);
  if (soilMv > 0U && soilMv < 6000.0F && WiFiconnect())
  {
    digitalWrite(LED_BUILTIN, HIGH);
    updateDevice(toDeviceId("soil_mv"), String(soilMv), true);
    digitalWrite(LED_BUILTIN, LOW);
  }
}
