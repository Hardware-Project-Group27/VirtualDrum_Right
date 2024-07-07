#ifndef DRUM_HIT_DETECTOR_H
#define DRUM_HIT_DETECTOR_H

#include <Arduino.h>

class DrumHitDetector {
public:
  DrumHitDetector(int threshold);
  bool detectHit(int currentValue);

private:
  int accelerationThreshold;
  int previousValue1;
  int previousValue2;
  bool isDrumHit;
  unsigned long previousMillis;
  static const long INTERVAL = 20;
};

#endif