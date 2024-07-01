#ifndef INSTRUMENTSELECTOR_H
#define INSTRUMENTSELECTOR_H

#include "LEDController.h"

class InstrumentSelector {
public:
    InstrumentSelector();
    void selectInstrument(float *yawVal, float *pitchVal, int *selectedInstrumentID);
    void setLEDController(LEDController* ledController);
    void printInstrumentInfo(int *id, float *yaw, float *pitch);


private:
    LEDController* ledController;

    float bottomDrum1YawMin;
    float bottomDrum1YawMax;
    float bottomDrum1PitchMin;
    float bottomDrum1PitchMax;

    float bottomDrum2YawMin;
    float bottomDrum2YawMax;
    float bottomDrum2PitchMin;
    float bottomDrum2PitchMax;

    float symbol1YawMin;
    float symbol1YawMax;
    float symbol1PitchMin;
    float symbol1PitchMax;

    float symbol2YawMin;
    float symbol2YawMax;
    float symbol2PitchMin;
    float symbol2PitchMax;

    float yaw;
    float pitch;


    void lightUpSelectedInstrument(int *selectedInstrumentID);
};

#endif
