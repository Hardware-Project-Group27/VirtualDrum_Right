#ifndef FLEX_SENSOR_H
#define FLEX_SENSOR_H

#include <Arduino.h>

class FlexSensor {
public:
  FlexSensor(int pin);
  int read();
  int readAverage(int reads);

private:
  int _pin;
};

#endif