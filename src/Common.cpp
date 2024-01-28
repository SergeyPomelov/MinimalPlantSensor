
#include <Arduino.h>
#include <Constants.h>
#include <math.h>

uint16_t toMv(int reading, float koef)
{
  const float dividerK = (DIVIDER_IN_OHM - PROBE_OHM + DIVIDER_G_OHM - PROBE_OHM) / (DIVIDER_G_OHM - PROBE_OHM);
  // Serial.println(reading);
  // Serial.println(reading * (ADC_MAX_MV / 1023.0F) + DIODE_FALL_MV);
  // Serial.println(dividerK);
  return (uint16_t) round((reading * (ADC_MAX_MV / 1023.0F) * dividerK + DIODE_FALL_MV) * koef);
}

float mvToMoisture(int mv)
{
  const double mv_d = (double) mv;
  const double result = -14973.02 + 107.7763 * mv_d - 0.2844009 * mv_d * mv_d + 0.000328417 * mv_d * mv_d * mv_d - 1.404959e-7 * mv_d * mv_d * mv_d * mv_d;
  return (float) max(0.0, min(100.0, round(result)));
}

int mvToLux(int mv)
{
  float RLDR = (PHOTO_PULLDOWN_OHM * (VIN_MV - mv)) / mv;
  return (int)round(500.0F / (RLDR / 1000.0F));
}

String toDeviceId(const char *label)
{
  return "sensor.virtual_" + String(DEVICE_ID) + "_" + label;
}
