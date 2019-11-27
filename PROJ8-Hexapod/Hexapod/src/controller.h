#pragma once

#include <SoftwareSerial.h>

#define BUTTON_A      0b00000001
#define BUTTON_B      0b00000010
#define BUTTON_X      0b00000100
#define BUTTON_Y      0b00001000
#define BUTTON_LB     0b00010000
#define BUTTON_RB     0b00100000
#define BUTTON_START  0b01000000
#define BUTTON_SELECT 0b10000000

class Controller {
public:
    explicit Controller(SoftwareSerial *bluetooth);
    void update();

    bool isButtonDown(int mask);

    // Left Stick
    unsigned char leftX = 128;
    unsigned char leftY = 128;

    // Right Stick
    unsigned char rightX = 128;
    unsigned char rightY = 128;

    // DPad
    unsigned char dpadX = 128;
    unsigned char dpadY = 128;

    // Triggers
    unsigned char trig1 = 128;
    unsigned char trig2 = 128;

private:
    int waitForRead();

    SoftwareSerial *bluetooth;
    unsigned char buttons = 0;
};
