#include <Constants.h>
#include <Data.h>
#include <Sensor.h>
#include <WiFiFunctions.h>

void setup()
{
  Serial.begin(76800);
  Serial.setTimeout(2000);
  Serial.setDebugOutput(false);
  Serial.println("setup");

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(SENSOR_PWR, OUTPUT);
  pinMode(WAKEUP, WAKEUP_PULLUP);

  digitalWrite(0U, LOW);
  digitalWrite(12U, LOW);
  digitalWrite(13U, LOW);
  digitalWrite(15U, LOW);
  ESP.wdtEnable(60000U);

  digitalWrite(LED_BUILTIN, LOW);

  readSensors();
  if (soilMv > 0.0F && soilMv < 2800.0F)
  {
    sendData();
  }
  hibernate();
}

void loop()
{
  Serial.println("loop");
  delay(100);
}
