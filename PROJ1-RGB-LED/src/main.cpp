#include "Arduino.h"

#define RED_PIN 9
#define GREEN_PIN 10
#define BLUE_PIN 11

#define X_PIN A0
#define Y_PIN A1

void writeColor(int r, int g, int b);
void updateLEDColor(float dt);
void HSVtoRGB(float& r, float& g, float& b, float& h, float& s, float& v);

void setup() {
    pinMode(RED_PIN, OUTPUT);
    pinMode(GREEN_PIN, OUTPUT);
    pinMode(BLUE_PIN, OUTPUT);

    pinMode(X_PIN, INPUT);
    pinMode(Y_PIN, INPUT);

    Serial.begin(9600);
}

//HSV Values of the LED
float h = 180;
float v = 0.1;
float s = 1;

float last_time = 0;

void loop() {
    // Calculate the time difference between last update
    float dt = millis() - last_time;
    last_time = millis();

    // Update the HSV values
    updateLEDColor(dt);

    // Convert the HSV values of the LED to RGB
    float r, g, b;
    HSVtoRGB(r, g, b, h, s, v);

    // Write the color to the LED
    writeColor((int) (r * 255), (int) (g * 255), (int) (b * 255));
}

void updateLEDColor(float dt) {
    /* Hue */
    float dh = dt * ((analogRead(X_PIN) / 1024.0) - 0.5);  // Calculate the change in hue based on the x pos of the joystick
    if (dh < -0.07 || dh > 0.07) h += dh;  // Add dh to the hue if it is big enough (deadzone)
    if (h < 0) h = 360;  // Make h within [0, inf)
    if (h >= 360) h = 0;  // Make h within [0, 360)

    /* Brightness */
    float dv = dt * ((analogRead(Y_PIN) / 1024.0) - 0.5); // Calculate the change in brightness based on the y pos of the joystick

    if (dv < -0.07 || dv > 0.07) v += dv / -360.0;

    if (v < 0) v = 0;
    if (v > 1) v = 1;

}

void writeColor(int r, int g, int b) {
    analogWrite(RED_PIN, r);
    analogWrite(GREEN_PIN, g);
    analogWrite(BLUE_PIN, b);
}

//From https://gist.github.com/fairlight1337/4935ae72bcbcc1ba5c72
void HSVtoRGB(float& r, float& g, float& b, float& h, float& s, float& v) {
    float fC = v * s; // Chroma
    float fHPrime = fmod(h / 60.0, 6);
    float fX = fC * (1 - fabs(fmod(fHPrime, 2) - 1));
    float fM = v - fC;

    if(0 <= fHPrime && fHPrime < 1) {
        r = fC;
        g = fX;
        b = 0;
    } else if(1 <= fHPrime && fHPrime < 2) {
        r = fX;
        g = fC;
        b = 0;
    } else if(2 <= fHPrime && fHPrime < 3) {
        r = 0;
        g = fC;
        b = fX;
    } else if(3 <= fHPrime && fHPrime < 4) {
        r = 0;
        g = fX;
        b = fC;
    } else if(4 <= fHPrime && fHPrime < 5) {
        r = fX;
        g = 0;
        b = fC;
    } else if(5 <= fHPrime && fHPrime < 6) {
        r = fC;
        g = 0;
        b = fX;
    } else {
        r = 0;
        g = 0;
        b = 0;
    }

    r += fM;
    g += fM;
    b += fM;
}