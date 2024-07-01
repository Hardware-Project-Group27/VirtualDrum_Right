
#include <Arduino.h>
#include "WebsocketCon.h"
#include <Battery.h>
#include "BatteryL.h"

int battery1Level;
int battery2Level;
int thisGlove;
int activationPin;
unsigned long lastBatteryCheck = 0;
Battery battery = Battery(3300, 4200, SENSE_PIN, ADC_RESOLUTION);


BatteryL::BatteryL(int thisGloveBatteryNo ,int BatteryActivationPin){
    thisGlove = thisGloveBatteryNo;
    activationPin = BatteryActivationPin;
    battery1Level = 0;
    battery2Level = 0;
    analogReadResolution(ADC_RESOLUTION);
    battery.onDemand(activationPin, HIGH);
    battery.begin(4200, 2, &asigmoidal);
}

void BatteryL::BatteryInit( WebSocketCon *ws) {
    wsCon = *ws;
    
}

int BatteryL::getBattery1Level(){
  return battery1Level;
}
int BatteryL::getBattery2Level(){
  return battery2Level;
}
void BatteryL::setBattery1Level(int level){
  battery1Level = level;
}
void BatteryL::setBattery2Level(int level){
  battery2Level = level;
}   

void BatteryL::measureBatteryLevel(){
    if(millis() - lastBatteryCheck > CHECK_INTERVAL){
      if(thisGlove == 0){
        battery1Level = battery.level();
      }
      else if(thisGlove == 1){
        battery2Level = battery.level();
      }
      lastBatteryCheck = millis();
      Serial.print("Battery Level ");
      Serial.println(battery.level());

      String s = "bat:" + String(thisGlove) + ":" + String(battery.level());
      wsCon.sendMsg(s);
      Serial.print("Battery Voltage ");
      Serial.println(battery.voltage());
    }
}

