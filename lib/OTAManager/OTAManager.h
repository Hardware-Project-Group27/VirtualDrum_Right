#ifndef OTAMANAGER_H
#define OTAMANAGER_H

#include <ArduinoOTA.h>
#include <WebServer.h>
#include <Update.h>
#include "LEDController.h"

class OTAManager {
public:
    OTAManager();
    void begin(WebServer* server, LEDController* ledController);
    void handle();

private:
    void setupOTA();
    void setupWebUpdate(WebServer* server, LEDController* ledController);
};

#endif // OTAMANAGER_H