#include "Arduino.h"
#include "../lib/Stepper/src/Stepper.h"

#define X_PIN A0
#define Y_PIN A1

#define STEPS_PER_REV 2048

Stepper stepper = Stepper(STEPS_PER_REV, 8, 10, 9, 11);

void setup() {
    pinMode(X_PIN, INPUT);
    pinMode(Y_PIN, INPUT);

    stepper.setSpeed(10);
    Serial.begin(9600);
}
void loop() {
    long joystick_x = analogRead(X_PIN) - 512;

    if (abs(joystick_x) > 100) stepper.step(joystick_x < 0 ? -10 : 10);
}