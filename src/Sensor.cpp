#include <Sensor.h>
#include <Data.h>
#include <Constants.h>
#include <Common.h>

int soilMoisture;

void readSensors()
{
  digitalWrite(SENSOR_PWR, HIGH);
  digitalWrite(LED_BUILTIN, LOW);
  Serial.println("Sensor power on.");
  delay(1000);

  soilMv = toMv(analogRead(SOIL_SENSOR_PIN), SOIL_DIVIDER_K);

  Serial.println("Soil Voltage: " + String(soilMv) + "mv");
  Serial.println("Sensor power off.");
  
  digitalWrite(SENSOR_PWR, LOW);
  digitalWrite(LED_BUILTIN, HIGH);
}
