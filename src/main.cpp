#include <Arduino.h>
#include <Battery.h>
#include <WebServer.h>
#include <DNSServer.h>
#include <Preferences.h>
#include "WiFiManager.h"
#include "AccessPoint.h"
#include "WebsocketCon.h"
#include "BatteryL.h"
#include "WSMsgRecievedHandler.h"
#include "MPU6050Handler.h"
#include "InstrumentSelector.h"
#include "LEDController.h"
#include "FlexSensor.h"
#include "DrumHitDetector.h"
#include "OTAManager.h"

#define GLOVE_NO 1 // 0 for left, 1 for right glove

const char *defaultAPSSID = "DRUM_GLOVE";
const char *defaultAPPassword = "";

String ssid;
String password;
String serverIP;
int port;

bool isConnectedToServer = false; // this will be set to true when the glove is connected to the server

#define INTERRUPT_PIN 19
#define RESET_ANGLES_BTN 12

// --- for Drum Select LED
#define LEFT_SYMBOL_LED 17
#define LEFT_DRUM_LED 16
#define RIGHT_DRUM_LED 18
#define RIGHT_SYMBOL_LED 5
// ---

WebServer server(80);
DNSServer dnsServer;
Preferences preferences;

WebSocketCon ws = WebSocketCon();
WiFiManager wifiManager = WiFiManager();
AccessPoint accessPoint = AccessPoint();
BatteryL batteryL = BatteryL(GLOVE_NO, ACTIVATION_PIN);
WSMsgRecievedHandler wsMsgRecievedHandler = WSMsgRecievedHandler(GLOVE_NO);
MPU6050Handler mpu6050Handler = MPU6050Handler(INTERRUPT_PIN, RESET_ANGLES_BTN);
InstrumentSelector instrumentSelector = InstrumentSelector();
LEDController ledController = LEDController(LEFT_DRUM_LED, RIGHT_DRUM_LED, LEFT_SYMBOL_LED, RIGHT_SYMBOL_LED);
OTAManager otaManager;

FlexSensor flexSensor(34);
DrumHitDetector drumHitDetector(50); // Flex bend acceleration threshold is the parameter

float yaw = 0;
float pitch = 0;

int selectedInstrumentID = 0;

unsigned long lastMillis = 0;
unsigned long drumHitSendingMillis = 0;
const int DRUM_HIT_SENDING_INTERVAL = 100;

unsigned long wsDisconnectedTime = 0;
unsigned long timeAfterSetup = 0;

void setup()
{
  Serial.begin(115200);

  wifiManager.loadCredentials();
  if (GLOVE_NO == 0)
  {
    defaultAPSSID = "DRUM_GLOVE_LEFT";
  }
  else
  {
    defaultAPSSID = "DRUM_GLOVE_RIGHT";
  }

  if (ssid == "" || password == "")
  {
    accessPoint.start();
  }
  else
  {
    if (wifiManager.connectToWiFi())
    {
      Serial.println("\nConnected to WiFi");
      Serial.println("IP Address: " + WiFi.localIP().toString());

      ws.setup(GLOVE_NO); // the global flag will be set to true by this if connect
      // we dont start the server connection here, the onevent needs to detect the connection
      // so we check in the main loop
    }
    else
    {
      Serial.println("\nFailed to connect to WiFi. Starting AP mode.");
      accessPoint.start();
      otaManager.begin(&server, &ledController);

    }
  }

  batteryL.BatteryInit(&ws);
  wsMsgRecievedHandler.setBatteryL(&batteryL);

  ws.setWSMsgRecievedHandler(&wsMsgRecievedHandler);
  
  mpu6050Handler.setLEDController(&ledController);
  mpu6050Handler.setup();

  instrumentSelector.setLEDController(&ledController);

  wsDisconnectedTime = millis();
  timeAfterSetup = millis();
}

void loop()
{
  // put your main code here, to run repeatedly:
  if (!accessPoint.isStarted())
  {
    ws.loop();
  }

  // if 4 seconds passed after setup
  if ((millis() - timeAfterSetup > 4000) && !isConnectedToServer && !accessPoint.isStarted())
  {
    // if 4 seconds after last disconnection
    if (millis() - wsDisconnectedTime > 4000)
    {
      Serial.println("Failed to connect to server. Starting AP mode.");
      accessPoint.start();
      otaManager.begin(&server, &ledController);

    }
  }

  batteryL.measureBatteryLevel();

  if (isConnectedToServer) {
    if (mpu6050Handler.isMPU6050Working())
    {
      mpu6050Handler.loop();
      mpu6050Handler.getYawPitch(&yaw, &pitch);
      instrumentSelector.selectInstrument(&yaw, &pitch, &selectedInstrumentID);

      // instrumentSelector.printInstrumentInfo(&selectedInstrumentID, &yaw, &pitch);
    }
    else
    {
      ledController.showMPU6050Error();
    }
  }

  // flex drum hit detect code
  int flexVal = flexSensor.read();
  // Serial.println(flexVal);

  int avgFlexVal = flexSensor.readAverage(10); // 10 reads

  if (drumHitDetector.detectHit(avgFlexVal))
  {
    switch (selectedInstrumentID)
    {
    case 0:
      // nothing selected dont play
      break;
    case 1:
      ws.sendMsg("play:drum1:10000");
      break;

    case 2:
      ws.sendMsg("play:drum2:10000");

      break;

    case 3:
      ws.sendMsg("play:drum3:10000");

      break;

    case 4:
      ws.sendMsg("play:drum4:10000");

      break;

    default:
      break;
    }
  }

  // for wifi disconnection
  if (!wifiManager.isConnectedToWifi() && !accessPoint.isStarted())
  {
    unsigned long startTime = millis();
    while (!wifiManager.connectToWiFi() && millis() - startTime < 5000)
    {
      delay(500);
      Serial.print(".");
    }

    if (wifiManager.isConnectedToWifi())
    {
      Serial.println("\nConnected to WiFi");
      Serial.println("IP Address: " + WiFi.localIP().toString());
    }
    else
    {
      Serial.println("\nFailed to connect to WiFi. Starting AP mode.");
      accessPoint.start();
    }
  }

  if (accessPoint.isStarted())
  {
    ledController.showAPEnabled();

    dnsServer.processNextRequest();
    // Serial.println("AP mode");
    server.handleClient();
    otaManager.handle();  // Handle OTA updates

  }
}
