//Based on http://www.hobbytronics.co.uk/arduino-4digit-7segment

#include "scoreDisplay.h"
#include "Arduino.h"

#define SEG_A 5
#define SEG_B 6
#define SEG_C 7
#define SEG_D 8
#define SEG_E 9
#define SEG_F 10
#define SEG_G 11

#define D1 12
#define D2 13
#define D3 A2
#define D4 A3

#define DISPLAY_BRIGHTNESS  500

#define DIGIT_ON  LOW
#define DIGIT_OFF  HIGH

#define SEGMENT_ON  HIGH
#define SEGMENT_OFF LOW

void displayNumber(int* digitList);
void lightNumber(int numberToDisplay);

int digits[4];

void initScoreboard() {
    pinMode(SEG_A, OUTPUT);
    pinMode(SEG_B, OUTPUT);
    pinMode(SEG_C, OUTPUT);
    pinMode(SEG_D, OUTPUT);
    pinMode(SEG_E, OUTPUT);
    pinMode(SEG_F, OUTPUT);
    pinMode(SEG_G, OUTPUT);

    pinMode(D1, OUTPUT);
    pinMode(D2, OUTPUT);
    pinMode(D3, OUTPUT);
    pinMode(D4, OUTPUT);

    digits[0] = 0;
    digits[1] = 0;
    digits[2] = 0;
    digits[3] = 0;
}

void displayScoreboard(int p1, int p2) {
    if (p1 < 10) {
        digits[0] = p1;
        digits[1] = 10;
    }
    else {
        digits[1] = p1 % 10;
        digits[0] = p1 / 10;
    }

    if (p2 < 10) {
        digits[2] = 10;
        digits[3] = p2;
    }
    else {
        digits[3] = p2 % 10;
        digits[2] = p2 / 10;
    }

    displayNumber(digits);
}

void displayNumber(int* digitList) {
    for(int digit = 4 ; digit > 0 ; digit--) {

        //Turn on a digit for a short amount of time
        switch(digit) {
            case 1:
                digitalWrite(D1, DIGIT_ON);
                break;
            case 2:
                digitalWrite(D2, DIGIT_ON);
                break;
            case 3:
                digitalWrite(D3, DIGIT_ON);
                break;
            case 4:
                digitalWrite(D4, DIGIT_ON);
                break;
            default:
                break;
        }

        lightNumber(digitList[digit - 1]);

        delayMicroseconds(DISPLAY_BRIGHTNESS);

        lightNumber(10);

        digitalWrite(D1, DIGIT_OFF);
        digitalWrite(D2, DIGIT_OFF);
        digitalWrite(D3, DIGIT_OFF);
        digitalWrite(D4, DIGIT_OFF);
    }
}

void lightNumber(int numberToDisplay) {
    switch (numberToDisplay){
        case 0:
            digitalWrite(SEG_A, SEGMENT_ON);
            digitalWrite(SEG_B, SEGMENT_ON);
            digitalWrite(SEG_C, SEGMENT_ON);
            digitalWrite(SEG_D, SEGMENT_ON);
            digitalWrite(SEG_E, SEGMENT_ON);
            digitalWrite(SEG_F, SEGMENT_ON);
            digitalWrite(SEG_G, SEGMENT_OFF);
            break;

        case 1:
            digitalWrite(SEG_A, SEGMENT_OFF);
            digitalWrite(SEG_B, SEGMENT_ON);
            digitalWrite(SEG_C, SEGMENT_ON);
            digitalWrite(SEG_D, SEGMENT_OFF);
            digitalWrite(SEG_E, SEGMENT_OFF);
            digitalWrite(SEG_F, SEGMENT_OFF);
            digitalWrite(SEG_G, SEGMENT_OFF);
            break;

        case 2:
            digitalWrite(SEG_A, SEGMENT_ON);
            digitalWrite(SEG_B, SEGMENT_ON);
            digitalWrite(SEG_C, SEGMENT_OFF);
            digitalWrite(SEG_D, SEGMENT_ON);
            digitalWrite(SEG_E, SEGMENT_ON);
            digitalWrite(SEG_F, SEGMENT_OFF);
            digitalWrite(SEG_G, SEGMENT_ON);
            break;

        case 3:
            digitalWrite(SEG_A, SEGMENT_ON);
            digitalWrite(SEG_B, SEGMENT_ON);
            digitalWrite(SEG_C, SEGMENT_ON);
            digitalWrite(SEG_D, SEGMENT_ON);
            digitalWrite(SEG_E, SEGMENT_OFF);
            digitalWrite(SEG_F, SEGMENT_OFF);
            digitalWrite(SEG_G, SEGMENT_ON);
            break;

        case 4:
            digitalWrite(SEG_A, SEGMENT_OFF);
            digitalWrite(SEG_B, SEGMENT_ON);
            digitalWrite(SEG_C, SEGMENT_ON);
            digitalWrite(SEG_D, SEGMENT_OFF);
            digitalWrite(SEG_E, SEGMENT_OFF);
            digitalWrite(SEG_F, SEGMENT_ON);
            digitalWrite(SEG_G, SEGMENT_ON);
            break;

        case 5:
            digitalWrite(SEG_A, SEGMENT_ON);
            digitalWrite(SEG_B, SEGMENT_OFF);
            digitalWrite(SEG_C, SEGMENT_ON);
            digitalWrite(SEG_D, SEGMENT_ON);
            digitalWrite(SEG_E, SEGMENT_OFF);
            digitalWrite(SEG_F, SEGMENT_ON);
            digitalWrite(SEG_G, SEGMENT_ON);
            break;

        case 6:
            digitalWrite(SEG_A, SEGMENT_ON);
            digitalWrite(SEG_B, SEGMENT_OFF);
            digitalWrite(SEG_C, SEGMENT_ON);
            digitalWrite(SEG_D, SEGMENT_ON);
            digitalWrite(SEG_E, SEGMENT_ON);
            digitalWrite(SEG_F, SEGMENT_ON);
            digitalWrite(SEG_G, SEGMENT_ON);
            break;

        case 7:
            digitalWrite(SEG_A, SEGMENT_ON);
            digitalWrite(SEG_B, SEGMENT_ON);
            digitalWrite(SEG_C, SEGMENT_ON);
            digitalWrite(SEG_D, SEGMENT_OFF);
            digitalWrite(SEG_E, SEGMENT_OFF);
            digitalWrite(SEG_F, SEGMENT_OFF);
            digitalWrite(SEG_G, SEGMENT_OFF);
            break;

        case 8:
            digitalWrite(SEG_A, SEGMENT_ON);
            digitalWrite(SEG_B, SEGMENT_ON);
            digitalWrite(SEG_C, SEGMENT_ON);
            digitalWrite(SEG_D, SEGMENT_ON);
            digitalWrite(SEG_E, SEGMENT_ON);
            digitalWrite(SEG_F, SEGMENT_ON);
            digitalWrite(SEG_G, SEGMENT_ON);
            break;

        case 9:
            digitalWrite(SEG_A, SEGMENT_ON);
            digitalWrite(SEG_B, SEGMENT_ON);
            digitalWrite(SEG_C, SEGMENT_ON);
            digitalWrite(SEG_D, SEGMENT_ON);
            digitalWrite(SEG_E, SEGMENT_OFF);
            digitalWrite(SEG_F, SEGMENT_ON);
            digitalWrite(SEG_G, SEGMENT_ON);
            break;

        default:
            digitalWrite(SEG_A, SEGMENT_OFF);
            digitalWrite(SEG_B, SEGMENT_OFF);
            digitalWrite(SEG_C, SEGMENT_OFF);
            digitalWrite(SEG_D, SEGMENT_OFF);
            digitalWrite(SEG_E, SEGMENT_OFF);
            digitalWrite(SEG_F, SEGMENT_OFF);
            digitalWrite(SEG_G, SEGMENT_OFF);
            break;
    }
}