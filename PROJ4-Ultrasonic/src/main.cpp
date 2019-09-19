#include "Arduino.h"

#define TRIG_PIN A0
#define ECHO_PIN A1
#define BUZZER_PIN 9

#define AVERAGE_AMT 200

double distances[AVERAGE_AMT];
double avg_distance = 1;

long lastMeasure = 0;

void measure();

void setup() {
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
    pinMode(BUZZER_PIN, OUTPUT);
}

void loop() {
    if (millis() - lastMeasure > 500) {
        lastMeasure = millis();
        measure();
    }

    digitalWrite(BUZZER_PIN, LOW);
    if (avg_distance < 20) digitalWrite(BUZZER_PIN, HIGH);
}

void measure() {
    for (int i = 0; i < AVERAGE_AMT; i++) {
        digitalWrite(TRIG_PIN, LOW);
        delayMicroseconds(2);
        digitalWrite(TRIG_PIN, HIGH);
        delayMicroseconds(10);
        digitalWrite(TRIG_PIN, LOW);
        double distance = pulseIn(ECHO_PIN, HIGH) * 0.017;

        distances[i] = distance;
    }

    avg_distance = 0;
    for (double d : distances) avg_distance += d;

    avg_distance /= (double) AVERAGE_AMT;
}