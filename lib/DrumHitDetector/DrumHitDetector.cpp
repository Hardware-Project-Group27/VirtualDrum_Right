#include "DrumHitDetector.h"

DrumHitDetector::DrumHitDetector(int threshold)
  : accelerationThreshold(threshold), previousValue1(0), previousValue2(0), isDrumHit(false), previousMillis(0) {}

bool DrumHitDetector::detectHit(int currentValue) {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= INTERVAL) {
    previousMillis = currentMillis;
    
    int acceleration = currentValue - previousValue1;
    
    if (acceleration < -accelerationThreshold && !isDrumHit) {
      isDrumHit = true;
      previousValue2 = previousValue1;
      previousValue1 = currentValue;
      return true;
    } else if (acceleration > -accelerationThreshold) {
      isDrumHit = false;
    }
    
    previousValue2 = previousValue1;
    previousValue1 = currentValue;
  }
  return false;
}