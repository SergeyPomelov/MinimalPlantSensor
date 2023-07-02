
#include <Arduino.h>

uint16_t toMv(int reading, float koef);
float mvToMoisture(int mv);
int mvToLux(int mv);
String toDeviceId(const char *label);
