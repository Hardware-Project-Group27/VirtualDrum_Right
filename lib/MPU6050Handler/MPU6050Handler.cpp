#include "MPU6050Handler.h"

#define INTERRUPT_PIN 19
#define RESET_ANGLES_BTN 12

volatile bool MPU6050Handler::mpuInterrupt = false;

MPU6050Handler::MPU6050Handler(int interrupt_Pin, int resetAnglesBtn_Pin)
    : dmpReady(false), mpuIntStatus(0), devStatus(0), packetSize(0), fifoCount(0),
      zeroYaw(0), zeroPitch(0), zeroRoll(0),
      resetAnglesBtnState(false), resetAnglesBtnLastState(false), lastButtonPressTime(0), currentTime(0), debounceDelay(200)

{
  interruptPin = interrupt_Pin;
  resetAnglesBtnPin = resetAnglesBtn_Pin;
}

void MPU6050Handler::setup()
{

  Serial.print(F("MPU setup function works"));

  // join I2C bus (I2Cdev library doesn't do this automatically)
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
  Wire.begin();
  Wire.setClock(400000); // 400kHz I2C clock. Comment this line if having compilation difficulties
#elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
  Fastwire::setup(400, true);
#endif

  // initialize device
  Serial.println(F("Initializing I2C devices..."));
  mpu.initialize();
  pinMode(interruptPin, INPUT);

  // verify connection
  Serial.println(F("Testing device connections..."));
  Serial.println(mpu.testConnection() ? F("MPU6050 connection successful") : F("MPU6050 connection failed"));

  // load and configure the DMP
  Serial.println(F("Initializing DMP..."));
  devStatus = mpu.dmpInitialize();

  mpu.setFullScaleGyroRange(MPU6050_GYRO_FS_2000); // default from the motion apps lib is 2000. supports 250, 500, 1000, 2000
  // note: if GYRO_FS range is changed from default, radian to degree conversion should handled accordingly
  Serial.println(mpu.getFullScaleGyroRange());

  // Calibrate gyro and accel
  mpu.CalibrateGyro();
  mpu.CalibrateAccel();

  // make sure it worked (returns 0 if so)
  if (devStatus == 0)
  {
    // turn on the DMP, now that it's ready
    Serial.println(F("Enabling DMP..."));
    mpu.setDMPEnabled(true);

    // enable Arduino interrupt detection
    Serial.println(F("Enabling interrupt detection (Arduino external interrupt 0)..."));
    attachInterrupt(digitalPinToInterrupt(interruptPin), dmpDataReady, RISING);
    mpuIntStatus = mpu.getIntStatus();

    // set our DMP Ready flag so the main loop() function knows it's okay to use it
    Serial.println(F("DMP ready! Waiting for first interrupt..."));
    dmpReady = true;

    // get expected DMP packet size for later comparison
    packetSize = mpu.dmpGetFIFOPacketSize();

    pinMode(resetAnglesBtnPin, INPUT_PULLUP);
  }
  else
  {
    // ERROR!
    // 1 = initial memory load failed
    // 2 = DMP configuration updates failed
    // (if it's going to break, usually the code will be 1)
    Serial.print(F("DMP Initialization failed (code "));
    Serial.print(devStatus);
    Serial.println(F(")"));
  }
}

void MPU6050Handler::loop()
{

  mpuFunctionalityLoop();
  resetAnglesBtnLoop();
}

void MPU6050Handler::resetAnglesBtnLoop()
{

  static unsigned long lastButtonPressTime = 0;
  unsigned long currentTime = millis();
  const unsigned long debounceDelay = 200; // 200 milliseconds debounce delay

  if ((currentTime - lastButtonPressTime) > debounceDelay)
  {
    resetAnglesBtnState = digitalRead(resetAnglesBtnPin) == HIGH;
    if (resetAnglesBtnState && !resetAnglesBtnLastState)
    {
      resetAngles();
      lastButtonPressTime = currentTime;
    }
  }
  resetAnglesBtnLastState = resetAnglesBtnState;
}

void MPU6050Handler::mpuFunctionalityLoop()
{
  //   Serial.println("mpuFunctionalityLoop");
  // if programming failed, don't try to do anything
  if (!dmpReady)
  {
    Serial.println(F("DMP not ready!"));
    return;
  }

  // wait for MPU interrupt or extra packet(s) available
  if (!mpuInterrupt && fifoCount < packetSize)
  {
    return;
  }
  // reset interrupt flag and get INT_STATUS byte
  mpuInterrupt = false;
  mpuIntStatus = mpu.getIntStatus();

  // get current FIFO count
  fifoCount = mpu.getFIFOCount();
  // Serial.print("fifoCount ");
  // Serial.print(fifoCount);
  // Serial.print(" \n");

  // check for overflow (this should never happen unless our code is too inefficient)
  if ((mpuIntStatus & 0x10) || fifoCount == 1024)
  {
    // reset so we can continue cleanly
    mpu.resetFIFO();
    Serial.println(F("FIFO overflow!"));

    // otherwise, check for DMP data ready interrupt (this should happen frequently)
  }
  else if (mpuIntStatus & 0x02)
  {
    // wait for correct available data length, should be a VERY short wait
    while (fifoCount < packetSize)
      fifoCount = mpu.getFIFOCount();

    // read a packet from FIFO
    mpu.getFIFOBytes(fifoBuffer, packetSize);

    // track FIFO count here in case there is > 1 packet available
    // (this lets us immediately read more without waiting for an interrupt)
    fifoCount -= packetSize;

    // display Euler angles in degrees
    mpu.dmpGetQuaternion(&q, fifoBuffer);
    mpu.dmpGetGravity(&gravity, &q);
    mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);

    // Serial.print("Print ");

    Serial.print("ypr\t");
    Serial.print(ypr[0] * 180 / M_PI);
    Serial.print("\t");
    Serial.print(ypr[1] * 180 / M_PI);
    Serial.print("\t");
    Serial.print(ypr[2] * 180 / M_PI);
    Serial.print("\t");
    Serial.print("\n");
  }
}

bool MPU6050Handler::isMPU6050Working()
{

  if (!mpu.testConnection())
  {
    Serial.println("isMPU6050Working(): MPU6050 connection failed");
    return false;
  }
  return true;
}

void MPU6050Handler::getYawPitch(float *yaw, float *pitch)
{

  *yaw = ypr[0] * 180 / M_PI - zeroYaw;
  *pitch = ypr[1] * 180 * -1 / M_PI - zeroPitch;  // pitch is inverted eg -40 is +90 and vice versa so multiply by 1 to make it right

  // i didnt check roll yet 
}

void MPU6050Handler::resetAngles()
{
  zeroYaw = ypr[0] * 180 / M_PI;
  zeroPitch = ypr[1] * 180 * -1 / M_PI; // // pitch is inverted eg -40 is +90 and vice versa so multiply by 1 to make it right
  zeroRoll = ypr[2] * 180 / M_PI;
}

void MPU6050Handler::dmpDataReady()
{
  mpuInterrupt = true;
}
