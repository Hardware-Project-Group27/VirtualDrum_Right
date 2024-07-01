#include <Arduino.h>
#include <Battery.h>
#include "WebsocketCon.h"
#include "BatteryL.h"
#include "WSMsgRecievedHandler.h"
#include "MPU6050Handler.h"
#include "InstrumentSelector.h"
#include "LEDController.h"


#define GLOVE_NO 1 // 0 for left, 1 for right glove

#define INTERRUPT_PIN 19
#define RESET_ANGLES_BTN 12

// --- for Drum Select LED
#define LEFT_SYMBOL_LED 18
#define LEFT_DRUM_LED 5
#define RIGHT_DRUM_LED BUILTIN_LED //17
#define RIGHT_SYMBOL_LED 16
// ---

WebSocketCon ws = WebSocketCon();
BatteryL batteryL = BatteryL(GLOVE_NO,ACTIVATION_PIN);
WSMsgRecievedHandler wsMsgRecievedHandler = WSMsgRecievedHandler(GLOVE_NO);
MPU6050Handler mpu6050Handler = MPU6050Handler(INTERRUPT_PIN, RESET_ANGLES_BTN);
InstrumentSelector instrumentSelector = InstrumentSelector();
LEDController ledController = LEDController(LEFT_DRUM_LED, RIGHT_DRUM_LED, LEFT_SYMBOL_LED, RIGHT_SYMBOL_LED);

float yaw = 0;
float pitch = 0;

int selectedInstrumentID = 0;

void setup() {
  Serial.begin(115200);
  
  ws.setup(GLOVE_NO);
  batteryL.BatteryInit(&ws);
  wsMsgRecievedHandler.setBatteryL(&batteryL);

  ws.setWSMsgRecievedHandler(&wsMsgRecievedHandler);

  mpu6050Handler.setup();
  instrumentSelector.setLEDController(&ledController);
}

void loop() {
  // put your main code here, to run repeatedly:

  batteryL.measureBatteryLevel();
  ws.loop();

  if (mpu6050Handler.isMPU6050Working()) {
    mpu6050Handler.loop();
    mpu6050Handler.getYawPitch(&yaw, &pitch);
    instrumentSelector.selectInstrument(&yaw, &pitch, &selectedInstrumentID);

    // instrumentSelector.printInstrumentInfo(&selectedInstrumentID, &yaw, &pitch);
  } else {
    ledController.showMPU6050Error();
  }


}

