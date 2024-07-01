#ifndef BATTERYL
#define BATTERYL

#define SENSE_PIN 32
#define ADC_RESOLUTION 12
#define ACTIVATION_PIN 12
#define CHECK_INTERVAL 2000

class BatteryL{

private:
    WebSocketCon wsCon; 
    
    // Battery battery;
    int battery1Level;
    int battery2Level;
    int thisGlove;
    int activationPin;

public:
    BatteryL(int thisGloveBatteryNo ,int activationPin);
    void BatteryInit(WebSocketCon* ws);
    void setBattery1Level(int level);
    void setBattery2Level(int level);
    int getBattery1Level();
    int getBattery2Level();
    void measureBatteryLevel();

};


#endif