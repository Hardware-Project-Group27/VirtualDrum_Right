#include <Arduino.h>
#include "BatteryL.h"
// #include "WebSocketCon.h"


class WSMsgRecievedHandler {
   private:
    BatteryL* batteryL; 
    // WebSocketCon* webSocketCon;
    int thisGlove;
    
  public:
    WSMsgRecievedHandler(int thisGlove);
    void handleMessage(const String& message);
    void setBatteryL(BatteryL* batteryL);
    // void setWebSocketCon(WebSocketCon* webSocketCon);

 
};
