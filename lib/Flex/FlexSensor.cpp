#include "FlexSensor.h"

FlexSensor::FlexSensor(int pin) : _pin(pin) {}

int FlexSensor::read() {
  return analogRead(_pin);
}

int FlexSensor::readAverage(int reads) {
  long sum = 0;
  for (int i = 0; i < reads; i++) {
    sum += analogRead(_pin);
  }
  return sum / reads;
}