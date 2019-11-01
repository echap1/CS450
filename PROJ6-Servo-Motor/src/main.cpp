#include "Arduino.h"
#include "../lib/Servo/src/Servo.h"

#define X_PIN A0
#define Y_PIN A1

Servo servo;

int pos = 0;

void setup() {
    pinMode(X_PIN, INPUT);
    pinMode(Y_PIN, INPUT);

    servo.attach(3);
}

void loop() {
    servo.write(analogRead(X_PIN) / 8);
}