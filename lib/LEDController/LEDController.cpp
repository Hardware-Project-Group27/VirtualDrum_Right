#include "LEDController.h"

LEDController::LEDController(int leftDrumPin, int rightDrumPin, int leftSymbolPin, int rightSymbolPin)
    : leftDrumPin(leftDrumPin), rightDrumPin(rightDrumPin), leftSymbolPin(leftSymbolPin), rightSymbolPin(rightSymbolPin) {
    pinMode(leftDrumPin, OUTPUT);
    pinMode(rightDrumPin, OUTPUT);
    pinMode(leftSymbolPin, OUTPUT);
    pinMode(rightSymbolPin, OUTPUT);

    digitalWrite(leftDrumPin, LOW);
    digitalWrite(rightDrumPin, LOW);
    digitalWrite(leftSymbolPin, LOW);
    digitalWrite(rightSymbolPin, LOW);
}

void LEDController::showSelectedInstrument(int *instrumentID) {
    // Replace with actual LED control logic
    switch (*instrumentID) {
        case 1:
            digitalWrite(leftDrumPin, HIGH);
            digitalWrite(rightDrumPin, LOW);
            digitalWrite(leftSymbolPin, LOW);
            digitalWrite(rightSymbolPin, LOW);
            break;
        case 2:
            digitalWrite(leftDrumPin, LOW);
            digitalWrite(rightDrumPin, HIGH);
            digitalWrite(leftSymbolPin, LOW);
            digitalWrite(rightSymbolPin, LOW);
            break;
        case 3:
            digitalWrite(leftDrumPin, LOW);
            digitalWrite(rightDrumPin, LOW);
            digitalWrite(leftSymbolPin, HIGH);
            digitalWrite(rightSymbolPin, LOW);
            break;
        case 4:
            digitalWrite(leftDrumPin, LOW);
            digitalWrite(rightDrumPin, LOW);
            digitalWrite(leftSymbolPin, LOW);
            digitalWrite(rightSymbolPin, HIGH);
            break;
        default:
            digitalWrite(leftDrumPin, LOW);
            digitalWrite(rightDrumPin, LOW);
            digitalWrite(leftSymbolPin, LOW);
            digitalWrite(rightSymbolPin, LOW);
            break;
    }
}

void LEDController::showMPU6050Error() {
    unsigned long currentMillis = millis();
    static unsigned long previousMillis = 0;
    static int state = LOW;
    if (currentMillis - previousMillis >= 500) {
        previousMillis = currentMillis;
        state = !state;
        digitalWrite(BUILTIN_LED, state);

        digitalWrite(leftDrumPin, state);
        digitalWrite(rightDrumPin, state);
        digitalWrite(leftSymbolPin, LOW);
        digitalWrite(rightSymbolPin, LOW);
    }
    else {
        digitalWrite(BUILTIN_LED, LOW);

        digitalWrite(leftDrumPin, LOW);
        digitalWrite(rightDrumPin, LOW);
        digitalWrite(leftSymbolPin, LOW);
        digitalWrite(rightSymbolPin, LOW);
    }
}


void LEDController::showAPEnabled() {
    // pattern like loading 
    unsigned long currentMillis = millis();
    static unsigned long previousMillis = 0;
    static int state = LOW;
    static int ledIndex = 0;
    int ledPins[] = {rightDrumPin, leftDrumPin, leftSymbolPin, rightSymbolPin};

    const unsigned long interval = 100;  

    digitalWrite(leftDrumPin, LOW);
    digitalWrite(rightDrumPin, LOW);
    digitalWrite(leftSymbolPin, LOW);
    digitalWrite(rightSymbolPin, LOW);


    digitalWrite(ledPins[ledIndex], HIGH);

    if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;
        digitalWrite(ledPins[ledIndex], LOW);

        ledIndex++;
        if (ledIndex >= 4) {
            ledIndex = 0;  // Reset to start
        }

    }
}

void LEDController::lightUpAll() {
    
    static int state = HIGH;
    digitalWrite(leftDrumPin, state);
    digitalWrite(rightDrumPin, state);
    digitalWrite(leftSymbolPin, state);
    digitalWrite(rightSymbolPin, state);

}

void LEDController::turnOffAll() {

    static int state = LOW;
    digitalWrite(leftDrumPin, state);
    digitalWrite(rightDrumPin, state);
    digitalWrite(leftSymbolPin, state);
    digitalWrite(rightSymbolPin, state);
}

