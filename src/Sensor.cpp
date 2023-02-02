#include <Sensor.h>
#include <Data.h>
#include <Constants.h>

int soilMoisture;

void readSensors()
{
  digitalWrite(SENSOR_PWR, HIGH);
  digitalWrite(LED_BUILTIN, LOW);
  Serial.println("Sensor power on.");
  delay(1000);

  soilMv = analogRead(SOIL_SENSOR_PIN) * (1000.0F / 1023.0F) * 4.3F;

  Serial.println("Soil Voltage: " + String(soilMv) + "mv");
  Serial.println("Sensor power off.");
  
  digitalWrite(SENSOR_PWR, LOW);
  digitalWrite(LED_BUILTIN, HIGH);
}
