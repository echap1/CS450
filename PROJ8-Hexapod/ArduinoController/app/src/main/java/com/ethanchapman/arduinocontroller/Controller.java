package com.ethanchapman.arduinocontroller;

import android.view.InputDevice;
import android.view.KeyEvent;
import android.view.MotionEvent;

public class Controller {
    public static final char BUTTON_A      = 0b00000001;
    public static final char BUTTON_B      = 0b00000010;
    public static final char BUTTON_X      = 0b00000100;
    public static final char BUTTON_Y      = 0b00001000;
    public static final char BUTTON_LB     = 0b00010000;
    public static final char BUTTON_RB     = 0b00100000;
    public static final char BUTTON_START  = 0b01000000;
    public static final char BUTTON_SELECT = 0b10000000;

    public static int buttons = 0;

    public static int[] processJoystickInput(MotionEvent event, int historyPos) {
        InputDevice inputDevice = event.getDevice();

        float x1 = getCenteredAxis(event, inputDevice, MotionEvent.AXIS_X, historyPos);
        float y1 = getCenteredAxis(event, inputDevice, MotionEvent.AXIS_Y, historyPos);

        float x2 = getCenteredAxis(event, inputDevice, MotionEvent.AXIS_Z, historyPos);
        float y2 = getCenteredAxis(event, inputDevice, MotionEvent.AXIS_RZ, historyPos);

        float x3 = getCenteredAxis(event, inputDevice, MotionEvent.AXIS_HAT_X, historyPos);
        float y3 = getCenteredAxis(event, inputDevice, MotionEvent.AXIS_HAT_Y, historyPos);

        float l_trigger = getCenteredAxis(event, inputDevice, MotionEvent.AXIS_LTRIGGER, historyPos);
        float r_trigger = getCenteredAxis(event, inputDevice, MotionEvent.AXIS_RTRIGGER, historyPos);

        return new int[] {toChar(x1), toChar(y1), toChar(x2), toChar(y2), toChar(x3), toChar(y3), toCharTrig(l_trigger), toCharTrig(r_trigger)};
    }

    public static boolean keyDown(int keyCode) {
        int prev = buttons;

        switch (keyCode) {
            case KeyEvent.KEYCODE_BUTTON_A:      buttons |= BUTTON_A; break;
            case KeyEvent.KEYCODE_BUTTON_B:      buttons |= BUTTON_B; break;
            case KeyEvent.KEYCODE_BUTTON_X:      buttons |= BUTTON_X; break;
            case KeyEvent.KEYCODE_BUTTON_Y:      buttons |= BUTTON_Y; break;
            case KeyEvent.KEYCODE_BUTTON_L1:     buttons |= BUTTON_LB; break;
            case KeyEvent.KEYCODE_BUTTON_R1:     buttons |= BUTTON_RB; break;
            case KeyEvent.KEYCODE_BUTTON_START:  buttons |= BUTTON_START; break;
            case KeyEvent.KEYCODE_BUTTON_SELECT: buttons |= BUTTON_SELECT; break;
        }

        return prev != buttons;
    }

    public static boolean keyUp(int keyCode) {
        int prev = buttons;

        switch (keyCode) {
            case KeyEvent.KEYCODE_BUTTON_A:      buttons &= ~BUTTON_A; break;
            case KeyEvent.KEYCODE_BUTTON_B:      buttons &= ~BUTTON_B; break;
            case KeyEvent.KEYCODE_BUTTON_X:      buttons &= ~BUTTON_X; break;
            case KeyEvent.KEYCODE_BUTTON_Y:      buttons &= ~BUTTON_Y; break;
            case KeyEvent.KEYCODE_BUTTON_L1:     buttons &= ~BUTTON_LB; break;
            case KeyEvent.KEYCODE_BUTTON_R1:     buttons &= ~BUTTON_RB; break;
            case KeyEvent.KEYCODE_BUTTON_START:  buttons &= ~BUTTON_START; break;
            case KeyEvent.KEYCODE_BUTTON_SELECT: buttons &= ~BUTTON_SELECT; break;
        }

        return prev != buttons;
    }

    public static char processButtonPresses() {
        return 0;
    }

    public static char toChar(float f) {
        return (char) ((int) (f * 127.99 + 128));
    }

    public static char toCharTrig(float f) {
        return (char) ((int) (f * 255));
    }

    public static float getCenteredAxis(MotionEvent event, InputDevice device, int axis, int historyPos) {
        final InputDevice.MotionRange range = device.getMotionRange(axis, event.getSource());

        if (range != null) {
            final float flat = range.getFlat();
            final float value = historyPos < 0 ? event.getAxisValue(axis): event.getHistoricalAxisValue(axis, historyPos);

            if (Math.abs(value) > flat) return value;
        }
        return 0;
    }

    public static String buttonString() {
        String s = "";

        s += (buttons & BUTTON_A) != 0 ? "A" : " ";
        s += (buttons & BUTTON_B) != 0 ? "B" : " ";
        s += (buttons & BUTTON_X) != 0 ? "X" : " ";
        s += (buttons & BUTTON_Y) != 0 ? "Y" : " ";
        s += (buttons & BUTTON_LB) != 0 ? "L" : " ";
        s += (buttons & BUTTON_RB) != 0 ? "R" : " ";
        s += (buttons & BUTTON_START) != 0 ? " Start" : "      ";
        s += (buttons & BUTTON_SELECT) != 0 ? " Select" : "       ";

        return s;
    }
}