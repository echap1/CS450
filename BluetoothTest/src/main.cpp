#include "Arduino.h"

int waitForRead();

// Left Stick
unsigned char x1 = 128;
unsigned char y1 = 128;

// Right Stick
unsigned char x2 = 128;
unsigned char y2 = 128;

// DPad
unsigned char x3 = 128;
unsigned char y3 = 128;

// Triggers
unsigned char l_trig = 128;
unsigned char r_trig = 128;

//Buttons
unsigned char buttons = 0;

#define BUTTON_A      0b00000001
#define BUTTON_B      0b00000010
#define BUTTON_X      0b00000100
#define BUTTON_Y      0b00001000
#define BUTTON_LB     0b00010000
#define BUTTON_RB     0b00100000
#define BUTTON_START  0b01000000
#define BUTTON_SELECT 0b10000000

#define setButtonUp(KEY) buttons |= KEY
#define setButtonDown(KEY) buttons &= ~KEY

#define isButtonDown(KEY) buttons & KEY

void setup() {
    Serial.begin(38400);

    Serial.println("Connected!");

    pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
    if (Serial.available()) {
        if (Serial.read() == 'C') {
            x1 = waitForRead();
            y1 = waitForRead();
            x2 = waitForRead();
            y2 = waitForRead();
            x3 = waitForRead();
            y3 = waitForRead();
            l_trig = waitForRead();
            r_trig = waitForRead();
            buttons = waitForRead();

            if (isButtonDown(BUTTON_A)) digitalWrite(LED_BUILTIN, HIGH);
            else digitalWrite(LED_BUILTIN, LOW);
        }
    }
}

int waitForRead() {
    while (!Serial.available());
    return Serial.read();
}