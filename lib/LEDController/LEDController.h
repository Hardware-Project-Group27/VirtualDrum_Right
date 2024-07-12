#ifndef LEDCONTROLLER_H
#define LEDCONTROLLER_H

#include <Arduino.h>

class LEDController {
public:
    LEDController(int leftDrumPin, int rightDrumPin, int leftSymbolPin, int rightSymbolPin);
    void showSelectedInstrument(int *instrumentID);
    void showMPU6050Error();
    void showAPEnabled();
    void lightUpAll();
    void turnOffAll();
    void showUpdateUploading();
    void showUpdateSuccess();
    void showUpdateFailed();

private:
    int leftDrumPin;
    int rightDrumPin;
    int leftSymbolPin;
    int rightSymbolPin;
};

#endif
