#include "Arduino.h"

#define DATA_PIN 2
#define CLOCK_PIN 3
#define LATCH_PIN 4

void setLEDs(int val);

void setup() {
    pinMode(DATA_PIN, OUTPUT);
    pinMode(CLOCK_PIN, OUTPUT);
    pinMode(LATCH_PIN, OUTPUT);

    setLEDs(0);

    Serial.begin(9600);
}

void loop() {
    if (Serial.available()) {
        setLEDs(Serial.read());
        delay(1000);
        setLEDs(0);
        delay(200);
    }
}

void setLEDs(int val) {
    digitalWrite(LATCH_PIN, LOW);
    shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, val);
    digitalWrite(LATCH_PIN, HIGH);
}