#include <Arduino.h>
#include <Battery.h>
#include "WebsocketCon.h"
#include "BatteryL.h"
#include "WSMsgRecievedHandler.h"



#define GLOVE_NO 1 // 0 for left, 1 for right glove

WebSocketCon ws = WebSocketCon();
BatteryL batteryL = BatteryL(GLOVE_NO,ACTIVATION_PIN);
WSMsgRecievedHandler wsMsgRecievedHandler = WSMsgRecievedHandler(GLOVE_NO);

void setup() {
   Serial.begin(115200);
  
   ws.setup();
   batteryL.BatteryInit(&ws);
   wsMsgRecievedHandler.setBatteryL(&batteryL);
   ws.setWSMsgRecievedHandler(&wsMsgRecievedHandler);
}

void loop() {
  // put your main code here, to run repeatedly:

  batteryL.measureBatteryLevel();
  ws.loop();


}

