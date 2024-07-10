#include "InstrumentSelector.h"
#include <Arduino.h>

InstrumentSelector::InstrumentSelector() {

  // instrument angle ranges
  bottomDrum1YawMin = -90.0;
  bottomDrum1YawMax = -5.0;
  bottomDrum1PitchMin = -30.0;
  bottomDrum1PitchMax = 35.0;

  bottomDrum2YawMin = 5.0;
  bottomDrum2YawMax = 90.0;
  bottomDrum2PitchMin = -30.0;
  bottomDrum2PitchMax = 35.0;

  symbol1YawMin = -90.0;
  symbol1YawMax = -5;
  symbol1PitchMin = 35.0;
  symbol1PitchMax = 90.0;

  symbol2YawMin = 5;
  symbol2YawMax = 90.0;
  symbol2PitchMin = 35.0;
  symbol2PitchMax = 90.0;
}

void InstrumentSelector::selectInstrument(float *yaw, float *pitch, int *selectedInstrumentID) {



  // Check if the angles fall within the defined ranges for each instrument
  if (*yaw >= bottomDrum1YawMin && *yaw <= bottomDrum1YawMax && *pitch >= bottomDrum1PitchMin && *pitch <= bottomDrum1PitchMax)
  {
    // printInstrumentInfo("Bottom Drum 1", yaw, pitch);
    *selectedInstrumentID = 1;
  }
  else if (*yaw >= bottomDrum2YawMin && *yaw <= bottomDrum2YawMax && *pitch >= bottomDrum2PitchMin && *pitch <= bottomDrum2PitchMax)
  {
    // printInstrumentInfo("Bottom Drum 2", yaw, pitch);
    //  Serial.print(" Yaw: ");
    // Serial.print(*yaw);
    // Serial.print(" Pitch: ");
    // Serial.print(*pitch);
    // Serial.print(" Instrument: ");
    // Serial.println("Bottom Drum 2");
    *selectedInstrumentID = 2;

  }
  else if (*yaw >= symbol1YawMin && *yaw <= symbol1YawMax && *pitch >= symbol1PitchMin && *pitch <= symbol1PitchMax)
  {
//     printInstrumentInfo("Symbol 1", yaw, pitch);
    *selectedInstrumentID = 3;

  }
  else if (*yaw >= symbol2YawMin && *yaw <= symbol2YawMax && *pitch >= symbol2PitchMin && *pitch <= symbol2PitchMax)
  {
//     printInstrumentInfo("Symbol 2", yaw, pitch);
    *selectedInstrumentID = 4;

  }
  else
  {
//     printInstrumentInfo("No instrument", *yaw, *pitch);
    *selectedInstrumentID = 0;

  }

    // for some reason printing here is inefficient, and it will cause the mpu fifo buffer to overflow lol
    //   Serial.println("Bottom Drum 2");

  
  lightUpSelectedInstrument(selectedInstrumentID);

}


// for some reason this function is inefficient
void InstrumentSelector::printInstrumentInfo(int *id, float *yaw, float *pitch) {

  Serial.print(" Yaw: ");
  Serial.print(*yaw);
  Serial.print(" Pitch: ");
  Serial.print(*pitch);
  Serial.print(" Instrument: ");
  switch (*id)
  {
    case 1:
        Serial.println("Bottom Drum 1");
        break;
    case 2:

        Serial.println("Bottom Drum 2");
        break;

    case 3:
        Serial.println("Symbol 1");
        break;

    case 4:

        Serial.println("Symbol 2");
        break;

    default:
        Serial.println("No instrument");
        break;

  }
  
}

void InstrumentSelector::setLEDController(LEDController *ledController) {
  this->ledController = ledController;
}

void InstrumentSelector::lightUpSelectedInstrument(int *selectedInstrumentID) {
  this->ledController->showSelectedInstrument(selectedInstrumentID);
}