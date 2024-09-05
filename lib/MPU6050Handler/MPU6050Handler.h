#ifndef MPU6050HANDLER_H
#define MPU6050HANDLER_H

#include "LEDController.h"

#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"

#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
#include "Wire.h"
#endif


class MPU6050Handler {
public:
    MPU6050Handler(int interruptPin, int resetAnglesBtnPin);
    void setup();
    void loop();
    void getYawPitch(float* yaw, float* pitch); // only yaw and pitch are used for now
    void resetAngles();
    bool isMPU6050Working();
    void setLEDController(LEDController* LEDController);

private:
    int interruptPin;
    int resetAnglesBtnPin;

    LEDController* ledController;
    MPU6050 mpu;
    bool dmpReady;
    uint8_t mpuIntStatus;
    uint8_t devStatus;
    uint16_t packetSize;
    uint16_t fifoCount;
    uint8_t fifoBuffer[64];

    Quaternion q;                   // [w, x, y, z]         quaternion container
    VectorFloat gravity;            // [x, y, z]            gravity vector

    float ypr[3];
    float zeroYaw, zeroPitch, zeroRoll;

    static volatile bool mpuInterrupt;
    static void dmpDataReady();
    void mpuFunctionalityLoop();
    void resetAnglesBtnLoop();

    bool resetAnglesBtnState;
    bool resetAnglesBtnLastState;
    unsigned long lastButtonPressTime;
    unsigned long currentTime;
    const unsigned long debounceDelay;

};

#endif
