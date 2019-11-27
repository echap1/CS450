#include "controller.h"

Controller::Controller(SoftwareSerial *bluetooth) {
    this->bluetooth = bluetooth;
}

void Controller::update() {
    int readSize = bluetooth->available();
    if (readSize == 0) return;

    unsigned char vals[9];
    unsigned char buf[readSize];
    bluetooth->readBytes(buf, readSize);

    int start = -1;

    for (int i = readSize - 1; i >= 0; i--) {
        if (buf[i] == 'C') {
            start = i + 1;
            break;
        }
    }

    if (start == -1) return;

    for (int j = 0; j < 9; j++) {
        if (start + j < readSize) vals[j] = buf[start + j];
        else vals[j] = waitForRead();
    }

    leftX = vals[0];
    leftY = vals[1];
    rightX = vals[2];
    rightY = vals[3];
    dpadX = vals[4];
    dpadY = vals[5];
    trig1 = vals[6];
    trig2 = vals[7];
    buttons = vals[8];
}

int Controller::waitForRead() {
    while (!bluetooth->available());
    return bluetooth->read();
}

bool Controller::isButtonDown(int mask) {
    return buttons & mask;
}
