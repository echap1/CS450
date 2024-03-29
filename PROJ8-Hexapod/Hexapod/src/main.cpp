const char *Version = "#RV2r1a";

#include "Arduino.h"
#include <Wire.h>
#include "../lib/Adafruit_PWMServoDriver/Adafruit_PWMServoDriver.h"
#include "controller.h"
#include <SoftwareSerial.h>

void setup();
void loop();

void beep(int f, int t);
void beep(int f);
void setLeg(int legmask, int hip_pos, int knee_pos, int adj);
void setLeg(int legmask, int hip_pos, int knee_pos, int adj, int raw);
void setLeg(int legmask, int hip_pos, int knee_pos, int adj, int raw, int leanangle);
void setHipRaw(int leg, int pos);
void setHip(int leg, int pos);
void setHip(int leg, int pos, int adj);
void setKnee(int leg, int pos);
void turn(int ccw, int hipforward, int hipbackward, int kneeup, int kneedown, long timeperiod);
void turn(int ccw, int hipforward, int hipbackward, int kneeup, int kneedown, long timeperiod, int leanangle);
void stand();
void tiptoes();
void gait_sidestep(int left, long timeperiod);
void gait_tripod(int reverse, int hipforward, int hipbackward, int kneeup, int kneedown, long timeperiod);
void gait_tripod(int reverse, int hipforward, int hipbackward, int kneeup, int kneedown, long timeperiod, int leanangle);
void gait_tripod_scamper(int reverse, int turn);
void gait_ripple(int reverse, int hipforward, int hipbackward, int kneeup, int kneedown, long timeperiod, int leanangle);
void random_gait(int timingfactor);
void foldup();
void attach_all_servos();
void detach_all_servos();
void resetServoDriver();
void setServo(int servonum, int position);
void transactServos();
void commitServos();
void checkForCrashingHips();
void gait_command(int gaittype, int reverse, int hipforward, int hipbackward, int kneeup, int kneedown, int leanangle, int timeperiod);

int FreqMult = 1;   // PWM frequency multiplier, use 1 for analog servos and up to about 3 for digital.
// The recommended setting for digital is 2 (probably safe for all digital servos)
// A shunt between Nano D5 and D6 will set this to "1" in setup, this allows you
// to select digital servo mode (2) or analog servo mode (1) using a shunt
// or short jumper wire.

#define SERVO_IIC_ADDR  (0x40)    // default servo driver IIC address
Adafruit_PWMServoDriver servoDriver = Adafruit_PWMServoDriver(SERVO_IIC_ADDR);

#define BeeperPin 4           // digital 4 used for beeper
#define ServoTypePin 5        // 5 is used to signal digital vs. analog servo mode
#define ServoTypeGroundPin 6  // 6 provides a ground to pull 5 low if digital servos are in use
#define BF_ERROR  100         // deep beep for error situations
#define BD_MED    50          // medium long beep duration

// Depending on your servo make, the pulse width min and max may vary, you
// want these to be as small/large as possible without hitting the hard stop
// for max range. You'll have to tweak them as necessary to match the servos you
// have!  If you hear buzzing or jittering, you went too far.
// These values are good for MG90S clone small metal gear servos and Genuine Tower Pro MG90S

#define PWMFREQUENCY (60*FreqMult)

#define SERVOMIN  (190*FreqMult) // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  (540*FreqMult) // this is the 'maximum' pulse length count (out of 4096)

// Basic functions that move legs take a bit pattern
// indicating which legs to move. The legs are numbered
// clockwise starting with the right front leg being
// number zero, going around
// to the left legs, and finishing with the left front leg
// being number 5

#define NUM_LEGS 6

// Bit patterns for different combinations of legs
// bottom six bits. LSB is leg number 0

#define ALL_LEGS      0b111111
#define LEFT_LEGS     0b111000
#define RIGHT_LEGS    0b000111
#define TRIPOD1_LEGS  0b010101
#define TRIPOD2_LEGS  0b101010
#define FRONT_LEGS    0b100001
#define MIDDLE_LEGS   0b010010
#define BACK_LEGS     0b001100
#define NO_LEGS       0b000000

// individual leg bitmasks
#define LEG0 0b000001
#define LEG1 0b000010
#define LEG2 0b000100
#define LEG3 0b001000
#define LEG4 0b010000
#define LEG5 0b100000

#define LEG0BIT  0b000001
#define LEG1BIT  0b000010
#define LEG2BIT  0b000100
#define LEG3BIT  0b001000
#define LEG4BIT  0b010000
#define LEG5BIT  0b100000

#define ISFRONTLEG(LEG) (LEG==0||LEG==5)
#define ISMIDLEG(LEG)   (LEG==1||LEG==4)
#define ISBACKLEG(LEG)  (LEG==2||LEG==3)
#define ISLEFTLEG(LEG)  (LEG==0||LEG==1||LEG==2)
#define ISRIGHTLEG(LEG) (LEG==3||LEG==4||LEG==5)

// default positions for knee and hip. Note that hip position is
// automatically reversed for the left side by the setHip function
// These are in degrees

#define KNEE_UP_MAX 180
#define KNEE_UP    150
#define KNEE_RELAX  120
#define KNEE_NEUTRAL 90
#define KNEE_CROUCH 110
#define KNEE_HALF_CROUCH 80
#define KNEE_STAND 30
#define KNEE_DOWN  30
#define KNEE_TIPTOES 5
#define KNEE_FOLD 170

#define KNEE_SCAMPER (KNEE_NEUTRAL-20)

#define KNEE_TRIPOD_UP (KNEE_NEUTRAL-40)
#define KNEE_TRIPOD_ADJ 30

#define HIPSWING 25      // how far to swing hips on gaits like tripod or quadruped
#define HIPSMALLSWING 10  // when in fine adjust mode how far to move hips
#define HIPSWING_RIPPLE 20
#define HIP_FORWARD_MAX 175
#define HIP_FORWARD (HIP_NEUTRAL+HIPSWING)
#define HIP_FORWARD_SMALL (HIP_NEUTRAL+HIPSMALLSWING)
#define HIP_NEUTRAL 90
#define HIP_BACKWARD (HIP_NEUTRAL-HIPSWING)
#define HIP_BACKWARD_SMALL (HIP_NEUTRAL-HIPSMALLSWING)
#define HIP_BACKWARD_MAX 0
#define HIP_FORWARD_RIPPLE (HIP_NEUTRAL+HIPSWING_RIPPLE)
#define HIP_BACKWARD_RIPPLE (HIP_NEUTRAL-HIPSWING_RIPPLE)
#define HIP_FOLD 150

#define NOMOVE (-1)   // fake value meaning this aspect of the leg (knee or hip) shouldn't move

#define LEFT_START 3  // first leg that is on the left side
#define RIGHT_START 0 // first leg that is on the right side
#define KNEE_OFFSET 6 // add this to a leg number to get the knee servo number

short ServoPos[2 * NUM_LEGS]; // the last commanded position of each servo
long ServoTime[2 * NUM_LEGS]; // the time that each servo was last commanded to a new position

void beep(int f, int t) {
    if (f > 0 && t > 0) {
        tone(BeeperPin, f, t);
    } else {
        noTone(BeeperPin);
    }
}

void beep(int f) {  // if no second param is given we'll default to 250 milliseconds for the beep
    beep(f, 250);
}

// This function sets the positions of both the knee and hip in
// a single command.  For hip, the left side is reversed so
// forward direction is consistent.

// This function takes a bitmask to specify legs to move, note that
// the basic setHip and setKnee functions take leg numbers, not masks

// if a position is -1 then that means don't change that item

void setLeg(int legmask, int hip_pos, int knee_pos, int adj) {
    setLeg(legmask, hip_pos, knee_pos, adj, 0, 0);  // use the non-raw version with leanangle=0
}

// version with leanangle = 0
void setLeg(int legmask, int hip_pos, int knee_pos, int adj, int raw) {
    setLeg(legmask, hip_pos, knee_pos, adj, raw, 0);
}

void setLeg(int legmask, int hip_pos, int knee_pos, int adj, int raw, int leanangle) {
    for (int i = 0; i < NUM_LEGS; i++) {
        if (legmask & 0b1) {  // if the lowest bit is ON
            if (hip_pos != NOMOVE) {
                if (!raw) {
                    setHip(i, hip_pos, adj);
                } else {
                    setHipRaw(i, hip_pos);
                }
            }
            if (knee_pos != NOMOVE) {
                int pos = knee_pos;
                if (leanangle != 0) {
                    switch (i) {
                        case 0:
                        case 6:
                        case 5:
                        case 11:
                            if (leanangle < 0) pos -= leanangle;
                            break;
                        case 1:
                        case 7:
                        case 4:
                        case 10:
                            pos += abs(leanangle / 2);
                            break;
                        case 2:
                        case 8:
                        case 3:
                        case 9:
                            if (leanangle > 0) pos += leanangle;
                            break;
                    }
                    //Serial.print("Lean:"); Serial.print(leanangle); Serial.print("pos="); Serial.println(pos);
                }

                setKnee(i, pos);
            }
        }
        legmask = (legmask >> 1);  // shift down one bit position
    }
}

// this version of setHip does no processing at all (for example
// to distinguish left from right sides)
void setHipRaw(int leg, int pos) {
    setServo(leg, pos);
}

// this version of setHip adjusts for left and right legs so
// that 0 degrees moves "forward" i.e. toward legs 5-0 which is
// nominally the front of the robot

void setHip(int leg, int pos) {
    // reverse the left side for consistent forward motion
    if (leg >= LEFT_START) {
        pos = 180 - pos;
    }
    setHipRaw(leg, pos);
}

// this version of setHip adjusts not only for left and right,
// but also shifts the front legs a little back and the back legs
// forward to make a better balance for certain gaits like tripod or quadruped

void setHip(int leg, int pos, int adj) {
    if (ISFRONTLEG(leg)) {
        pos -= adj;
    } else if (ISBACKLEG(leg)) {
        pos += adj;
    }
    // reverse the left side for consistent forward motion
    if (leg >= LEFT_START) {
        pos = 180 - pos;
    }

    setHipRaw(leg, pos);
}

void setKnee(int leg, int pos) {
    // find the knee associated with leg if this is not already a knee
    if (leg < KNEE_OFFSET) {
        leg += KNEE_OFFSET;
    }
    setServo(leg, pos);
}

void turn(int ccw, int hipforward, int hipbackward, int kneeup, int kneedown, long timeperiod) {
    turn(ccw, hipforward, hipbackward, kneeup, kneedown, timeperiod, 0);
}

void turn(int ccw, int hipforward, int hipbackward, int kneeup, int kneedown, long timeperiod, int leanangle) {
    // use tripod groups to turn in place
    if (ccw) {
        int tmp = hipforward;
        hipforward = hipbackward;
        hipbackward = tmp;
    }

#define NUM_TURN_PHASES 6
#define FBSHIFT_TURN    40   // shift front legs back, back legs forward, this much

    long t = millis() % timeperiod;
    long phase = (NUM_TURN_PHASES * t) / timeperiod;

    //Serial.print("PHASE: ");
    //Serial.println(phase);

    switch (phase) {
        case 0:
            // in this phase, center-left and noncenter-right legs raise up at
            // the knee
            setLeg(TRIPOD1_LEGS, NOMOVE, kneeup, 0);
            break;

        case 1:
            // in this phase, the center-left and noncenter-right legs move clockwise
            // at the hips, while the rest of the legs move CCW at the hip
            setLeg(TRIPOD1_LEGS, hipforward, NOMOVE, FBSHIFT_TURN, 1);
            setLeg(TRIPOD2_LEGS, hipbackward, NOMOVE, FBSHIFT_TURN, 1);
            break;

        case 2:
            // now put the first set of legs back down on the ground
            setLeg(TRIPOD1_LEGS, NOMOVE, kneedown, 0);
            break;

        case 3:
            // lift up the other set of legs at the knee
            setLeg(TRIPOD2_LEGS, NOMOVE, kneeup, 0);
            break;

        case 4:
            // similar to phase 1, move raised legs CW and lowered legs CCW
            setLeg(TRIPOD1_LEGS, hipbackward, NOMOVE, FBSHIFT_TURN, 1);
            setLeg(TRIPOD2_LEGS, hipforward, NOMOVE, FBSHIFT_TURN, 1);
            break;

        case 5:
            // put the second set of legs down, and the cycle repeats
            setLeg(TRIPOD2_LEGS, NOMOVE, kneedown, 0);
            break;
    }

}

void stand() {
    transactServos();
    setLeg(ALL_LEGS, HIP_NEUTRAL, KNEE_STAND, 0);
    commitServos();
}

void tiptoes() {
    setLeg(ALL_LEGS, HIP_NEUTRAL, KNEE_TIPTOES, 0);
}

void gait_sidestep(int left, long timeperiod) {

    // the gait consists of 6 phases and uses tripod definitions

#define NUM_SIDESTEP_PHASES 6
#define FBSHIFTSS    50   // shift front legs back, back legs forward, this much

    long t = millis() % timeperiod;
    long phase = (NUM_SIDESTEP_PHASES * t) / timeperiod;
    int side1 = LEFT_LEGS;
    int side2 = RIGHT_LEGS;

    if (left == 0) {
        side1 = RIGHT_LEGS;
        side2 = LEFT_LEGS;
    }

    //Serial.print("PHASE: ");
    //Serial.println(phase);

    transactServos();

    switch (phase) {
        case 0:
            // Lift up tripod group 1 while group 2 goes to neutral setting
            setLeg(TRIPOD1_LEGS, HIP_NEUTRAL, KNEE_UP, FBSHIFTSS);
            setLeg(TRIPOD2_LEGS, HIP_NEUTRAL, KNEE_NEUTRAL, FBSHIFTSS);
            break;

        case 1:
            // slide over by curling one side under the body while extending the other side
            setLeg(TRIPOD2_LEGS & side1, HIP_NEUTRAL, KNEE_DOWN, FBSHIFTSS);
            setLeg(TRIPOD2_LEGS & side2, HIP_NEUTRAL, KNEE_RELAX, FBSHIFTSS);
            break;

        case 2:
            // now put the first set of legs back down on the ground
            // and at the sametime put the curled legs into neutral position
            setLeg(TRIPOD2_LEGS, HIP_NEUTRAL, KNEE_NEUTRAL, FBSHIFTSS);
            setLeg(TRIPOD1_LEGS, HIP_NEUTRAL, KNEE_NEUTRAL, FBSHIFTSS);
            break;

        case 3:
            // Lift up tripod group 2 while group 2 goes to neutral setting
            setLeg(TRIPOD2_LEGS, HIP_NEUTRAL, KNEE_UP, FBSHIFTSS);
            setLeg(TRIPOD1_LEGS, HIP_NEUTRAL, KNEE_NEUTRAL, FBSHIFTSS);
            break;

        case 4:
            // slide over by curling one side under the body while extending the other side
            setLeg(TRIPOD1_LEGS & side1, HIP_NEUTRAL, KNEE_DOWN, FBSHIFTSS);
            setLeg(TRIPOD1_LEGS & side2, HIP_NEUTRAL, KNEE_RELAX, FBSHIFTSS);
            break;

        case 5:
            // now put all the legs back down on the ground, then the cycle repeats
            setLeg(TRIPOD1_LEGS, HIP_NEUTRAL, KNEE_NEUTRAL, FBSHIFTSS);
            setLeg(TRIPOD2_LEGS, HIP_NEUTRAL, KNEE_NEUTRAL, FBSHIFTSS);
            break;
    }
    commitServos();
}

void gait_tripod(int reverse, int hipforward, int hipbackward,
                 int kneeup, int kneedown, long timeperiod) {

    // this version makes leanangle zero
    gait_tripod(reverse, hipforward, hipbackward,
                kneeup, kneedown, timeperiod, 0);
}

void gait_tripod(int reverse, int hipforward, int hipbackward,
                 int kneeup, int kneedown, long timeperiod, int leanangle) {

    // the gait consists of 6 phases. This code determines what phase
    // we are currently in by using the millis clock modulo the
    // desired time period that all six  phases should consume.
    // Right now each phase is an equal amount of time but this may not be optimal

    if (reverse) {
        int tmp = hipforward;
        hipforward = hipbackward;
        hipbackward = tmp;
    }

#define NUM_TRIPOD_PHASES 6
#define FBSHIFT    15   // shift front legs back, back legs forward, this much

    long t = millis() % timeperiod;
    long phase = (NUM_TRIPOD_PHASES * t) / timeperiod;

    //Serial.print("PHASE: ");
    //Serial.println(phase);

    transactServos(); // defer leg motions until after checking for crashes
    switch (phase) {
        case 0:
            // in this phase, center-left and noncenter-right legs raise up at
            // the knee
            setLeg(TRIPOD1_LEGS, NOMOVE, kneeup, 0, 0, leanangle);
            break;

        case 1:
            // in this phase, the center-left and noncenter-right legs move forward
            // at the hips, while the rest of the legs move backward at the hip
            setLeg(TRIPOD1_LEGS, hipforward, NOMOVE, FBSHIFT);
            setLeg(TRIPOD2_LEGS, hipbackward, NOMOVE, FBSHIFT);
            break;

        case 2:
            // now put the first set of legs back down on the ground
            setLeg(TRIPOD1_LEGS, NOMOVE, kneedown, 0, 0, leanangle);
            break;

        case 3:
            // lift up the other set of legs at the knee
            setLeg(TRIPOD2_LEGS, NOMOVE, kneeup, 0, 0, leanangle);
            break;

        case 4:
            // similar to phase 1, move raised legs forward and lowered legs backward
            setLeg(TRIPOD1_LEGS, hipbackward, NOMOVE, FBSHIFT);
            setLeg(TRIPOD2_LEGS, hipforward, NOMOVE, FBSHIFT);
            break;

        case 5:
            // put the second set of legs down, and the cycle repeats
            setLeg(TRIPOD2_LEGS, NOMOVE, kneedown, 0, 0, leanangle);
            break;
    }
    commitServos(); // implement all leg motions
}

int ScamperPhase = 0;
unsigned long NextScamperPhaseTime = 0;

long ScamperTracker = 0;

void gait_tripod_scamper(int reverse, int turn) {

    ScamperTracker += 2;  // for tracking if the user is over-doing it with scamper

    // this is a tripod gait that tries to go as fast as possible by not waiting
    // for knee motions to complete before beginning the next hip motion

    // this was experimentally determined and assumes the battery is maintaining
    // +5v to the servos and they are MG90S or equivalent speed. There is very
    // little room left for slower servo motion. If the battery voltage drops below
    // 6.5V then the BEC may not be able to maintain 5.0V to the servos and they may
    // not complete motions fast enough for this to work.

    int hipforward, hipbackward;

    if (reverse) {
        hipforward = HIP_BACKWARD;
        hipbackward = HIP_FORWARD;
    } else {
        hipforward = HIP_FORWARD;
        hipbackward = HIP_BACKWARD;
    }

#define FBSHIFT    15   // shift front legs back, back legs forward, this much
#define SCAMPERPHASES 6

#define KNEEDELAY 35   //30
#define HIPDELAY 100   //90

    if (millis() >= NextScamperPhaseTime) {
        ScamperPhase++;
        if (ScamperPhase >= SCAMPERPHASES) {
            ScamperPhase = 0;
        }
        switch (ScamperPhase) {
            case 0:
                NextScamperPhaseTime = millis() + KNEEDELAY;
                break;
            case 1:
                NextScamperPhaseTime = millis() + HIPDELAY;
                break;
            case 2:
                NextScamperPhaseTime = millis() + KNEEDELAY;
                break;
            case 3:
                NextScamperPhaseTime = millis() + KNEEDELAY;
                break;
            case 4:
                NextScamperPhaseTime = millis() + HIPDELAY;
                break;
            case 5:
                NextScamperPhaseTime = millis() + KNEEDELAY;
                break;
        }

    }

    //Serial.print("ScamperPhase: "); Serial.println(ScamperPhase);

    transactServos();
    switch (ScamperPhase) {
        case 0:
            // in this phase, center-left and noncenter-right legs raise up at
            // the knee
            setLeg(TRIPOD1_LEGS, NOMOVE, KNEE_SCAMPER, 0);
            setLeg(TRIPOD2_LEGS, NOMOVE, KNEE_DOWN, 0);
            break;

        case 1:
            // in this phase, the center-left and noncenter-right legs move forward
            // at the hips, while the rest of the legs move backward at the hip
            setLeg(TRIPOD1_LEGS, hipforward, NOMOVE, FBSHIFT, turn);
            setLeg(TRIPOD2_LEGS, hipbackward, NOMOVE, FBSHIFT, turn);
            break;

        case 2:
            // now put the first set of legs back down on the ground
            setLeg(TRIPOD1_LEGS, NOMOVE, KNEE_DOWN, 0);
            setLeg(TRIPOD2_LEGS, NOMOVE, KNEE_DOWN, 0);
            break;

        case 3:
            // lift up the other set of legs at the knee
            setLeg(TRIPOD2_LEGS, NOMOVE, KNEE_SCAMPER, 0, turn);
            setLeg(TRIPOD1_LEGS, NOMOVE, KNEE_DOWN, 0, turn);
            break;

        case 4:
            // similar to phase 1, move raised legs forward and lowered legs backward
            setLeg(TRIPOD1_LEGS, hipbackward, NOMOVE, FBSHIFT, turn);
            setLeg(TRIPOD2_LEGS, hipforward, NOMOVE, FBSHIFT, turn);
            break;

        case 5:
            // put the second set of legs down, and the cycle repeats
            setLeg(TRIPOD2_LEGS, NOMOVE, KNEE_DOWN, 0);
            setLeg(TRIPOD1_LEGS, NOMOVE, KNEE_DOWN, 0);
            break;
    }
    commitServos();
}

void random_gait(int timingfactor) {

/*#define GATETIME 3500  // number of milliseconds for each demo

    if (millis() > nextGaitTime) {
        curGait++;
        if (curGait >= G_NUMGATES) {
            curGait = 0;
        }
        nextGaitTime = millis() + GATETIME;

        // when switching demo modes, briefly go into a standing position so
        // we're starting at the same position every time.
        setLeg(ALL_LEGS, HIP_NEUTRAL, KNEE_STAND, 0);
        delay(600);
    }

    switch (curGait) {
        case G_STAND:
            stand();
            break;
        case G_TURN:
            turn(1, HIP_FORWARD, HIP_BACKWARD, KNEE_NEUTRAL, KNEE_DOWN, TRIPOD_CYCLE_TIME); // 700
            break;
        case G_TRIPOD:
            gait_tripod(1, HIP_FORWARD, HIP_BACKWARD, KNEE_NEUTRAL, KNEE_DOWN, TRIPOD_CYCLE_TIME); // 900
            break;
        case G_SCAMPER:
            gait_tripod_scamper((nextGaitTime - (millis()) < GATETIME / 2), 0);  // reverse direction halfway through
            break;
        case G_DANCE:
            stand();
            for (int i = 0; i < NUM_LEGS; i++)
                setHipRaw(i, 145);
            delay(350);
            for (int i = 0; i < NUM_LEGS; i++)
                setHipRaw(i, 35);
            delay(350);
            break;
        case G_BOOGIE:
            boogie_woogie(NO_LEGS, SUBMODE_1, 2);
            break;
        case G_FIGHT:
            fight_mode('w', SUBMODE_1, FIGHT_CYCLE_TIME);
            break;

        case G_TEETER:
            wave('r');
            break;

        case G_BALLET:
            flutter();
            break;

    }*/

}

void boogie_woogie(int legs_flat, int timingfactor) {

    setLeg(ALL_LEGS, NOMOVE, KNEE_UP, 0);
    //setLeg(legs_flat, NOMOVE, KNEE_RELAX, 0, 0);

#define NUM_BOOGIE_PHASES 2

    long t = millis()%(400*timingfactor);
    long phase = (NUM_BOOGIE_PHASES*t)/(400*timingfactor);

    switch (phase) {
        case 0:
            for (int i = 0; i < NUM_LEGS; i++)
                setHipRaw(i, 140);
            break;

        case 1:
            for (int i = 0; i < NUM_LEGS; i++)
                setHipRaw(i, 40);
            break;
    }
}

void foldup() {
    setLeg(ALL_LEGS, NOMOVE, KNEE_FOLD, 0);
    for (int i = 0; i < NUM_LEGS; i++)
        setHipRaw(i, HIP_FOLD);
}

SoftwareSerial *bt = new SoftwareSerial(3, 2);  // Bluetooth pins: TX=3=Yellow wire,  RX=2=Green wire

int ServosDetached = 0;

void attach_all_servos() {
    Serial.print("ATTACH");
    for (int i = 0; i < 2 * NUM_LEGS; i++) {
        setServo(i, ServoPos[i]);
        Serial.print(ServoPos[i]);
        Serial.print(":");
    }
    Serial.println("");
    ServosDetached = 0;
}

void detach_all_servos() {
    //Serial.println("DETACH");
    for (int i = 0; i < 16; i++) {
        servoDriver.setPin(i, 0, false); // stop pulses which will quickly detach the servo
    }
    ServosDetached = 1;
}

void resetServoDriver() {

    servoDriver.begin();
    servoDriver.setPWMFreq(PWMFREQUENCY);  // Analog servos run at ~60 Hz updates
}

Controller *controller;

#define SENSOR A3

void setup() {
    pinMode(SENSOR, INPUT);

    pinMode(BeeperPin, OUTPUT);
    beep(200);

    pinMode(13, OUTPUT);
    digitalWrite(13, HIGH);
    delay(300);
    digitalWrite(13, LOW);
    delay(150);
    digitalWrite(13, HIGH);
    delay(150);
    digitalWrite(13, LOW);

    ///////////////////// end of indicator flashing
    pinMode(A1, OUTPUT);
    pinMode(A2, OUTPUT);
    pinMode(ServoTypeGroundPin, OUTPUT);    // will provide a ground for shunt on D6 to indicate digital servo mode
    digitalWrite(ServoTypeGroundPin, LOW);
    pinMode(ServoTypePin, INPUT_PULLUP);    // if high we default to analog servo mode, if pulled to ground
    // (via a shunt to D6) then we'll double the PWM frequency for digital servos

    digitalWrite(13, LOW);

    // A1 and A2 provide power to the potentiometer
    digitalWrite(A1, HIGH);
    digitalWrite(A2, LOW);

    delay(300); // give hardware a chance to come up and stabalize

    bt->begin(38400);

    bt->println("");
    delay(250);
    bt->println(Version);

    delay(250);

    resetServoDriver();
    delay(250);

    stand();

    delay(300);

    controller = new Controller(bt);

    beep(400); // Signals end of startup sequence

    yield();
}

// setServo is the lowest level function for setting servo positions.
// It handles trims too.

byte deferServoSet = 0;

void setServo(int servonum, int position) {
    if (position != ServoPos[servonum]) {
        ServoTime[servonum] = millis();
    }
    ServoPos[servonum] = position;  // keep data on where the servo was last commanded to go

    int p = map(position, 0, 180, SERVOMIN, SERVOMAX);

    if (!deferServoSet) {
        servoDriver.setPWM(servonum, 0, p);
    }
}

void transactServos() {
    deferServoSet = 1;
}

void commitServos() {
    checkForCrashingHips();
    deferServoSet = 0;
    for (int servo = 0; servo < 2 * NUM_LEGS; servo++) {
        setServo(servo, ServoPos[servo]);
    }
}


void checkForCrashingHips() {

    for (int leg = 0; leg < NUM_LEGS; leg++) {
        if (ServoPos[leg] > 85) {
            continue; // it's not possible to crash into the next leg in line unless the angle is 85 or less
        }
        int nextleg = ((leg + 1) % NUM_LEGS);
        if (ServoPos[nextleg] < 100) {
            continue;   // it's not possible for there to be a crash if the next leg is less than 100 degrees
            // there is a slight assymmetry due to the way the servo shafts are positioned, that's why
            // this number does not match the 85 number above
        }
        int diff = ServoPos[nextleg] - ServoPos[leg];
        // There's a fairly linear relationship
        if (diff <= 85) {
            // if the difference between the two leg positions is less than about 85 then there
            // is not going to be a crash (or maybe just a slight touch that won't really cause issues)
            continue;
        }
        // if we get here then the legs are touching, we will adjust them so that the difference is less than 85
        int adjust = (diff - 85) / 2 + 1;  // each leg will get adjusted half the amount needed to avoid the crash

        // to debug crash detection, make the following line #if 1, else make it #if 0
#if 1
        Serial.print("#CRASH:");
        Serial.print(leg);
        Serial.print("=");
        Serial.print(ServoPos[leg]);
        Serial.print("/");
        Serial.print(nextleg);
        Serial.print("=");
        Serial.print(ServoPos[nextleg]);
        Serial.print(" Diff=");
        Serial.print(diff);
        Serial.print(" ADJ=");
        Serial.println(adjust);
#endif

        setServo(leg, ServoPos[leg] + adjust);
        setServo(nextleg, ServoPos[nextleg] - adjust);

    }
}

short Dialmode = 0;
short priorDialMode = -1;

void gait_command(int gaittype, int reverse, int hipforward, int hipbackward, int kneeup, int kneedown, int leanangle, int timeperiod) {

    if (ServosDetached) { // wake up any sleeping servos
        attach_all_servos();
    }

    switch (gaittype) {
        case 0:
        default:
            gait_tripod(reverse, hipforward, hipbackward, kneeup, kneedown, timeperiod, leanangle);
            break;
        case 1:
            turn(reverse, hipforward, hipbackward, kneeup, kneedown, timeperiod, leanangle);
            break;
        case 2:
            gait_ripple(reverse, hipforward, hipbackward, kneeup, kneedown, timeperiod, leanangle);
            break;
        case 3:
            gait_sidestep(reverse, timeperiod);
            break;
    }

#if 0
    Serial.print("GAIT: style="); Serial.print(gaittype); Serial.print(" dir="); Serial.print(reverse,DEC); Serial.print(" angles=");Serial.print(hipforward);
       Serial.print("/"); Serial.print(hipbackward); Serial.print("/"); Serial.print(kneeup,DEC); Serial.print("/"); Serial.print(kneedown);
       Serial.print("/"); Serial.println(leanangle);
#endif
}

unsigned long ReportTime = 0;
unsigned long SuppressModesUntil = 0;

int cycleStart, maxVal, curVal, maxTime, t;

void loop() {
    if (ServosDetached) attach_all_servos();

    checkForCrashingHips();

    int p = analogRead(A0);

    if (p < 50) Dialmode = 0;
    else Dialmode = 1;

    if (Dialmode != priorDialMode && priorDialMode != -1) {
        beep(100 + 100 * Dialmode, 60);   // audio feedback that a new mode has been entered
        SuppressModesUntil = millis() + 1000;
    }
    priorDialMode = Dialmode;

    if (millis() < SuppressModesUntil) return;

    if (Dialmode == 0) { // STAND STILL MODE
        digitalWrite(13, LOW);
        stand();

        return;
    }

    controller->update();

    if (controller->isButtonDown(BUTTON_A)) {
        gait_tripod(0, HIP_FORWARD, HIP_BACKWARD, KNEE_NEUTRAL, KNEE_DOWN, 750);
    } else if (controller->isButtonDown(BUTTON_B)) {
        gait_tripod(1, HIP_FORWARD, HIP_BACKWARD, KNEE_NEUTRAL, KNEE_DOWN, 750);
    } else if (controller->isButtonDown(BUTTON_X)) {
        boogie_woogie(NO_LEGS, 2);
    } else if (controller->isButtonDown(BUTTON_Y)) {
        gait_tripod_scamper(0, 100);
    } else if (controller->isButtonDown(BUTTON_LB)) {
        turn(1, HIP_FORWARD, HIP_BACKWARD, KNEE_NEUTRAL, KNEE_DOWN, 750);
    } else if (controller->isButtonDown(BUTTON_RB)) {
        turn(0, HIP_FORWARD, HIP_BACKWARD, KNEE_NEUTRAL, KNEE_DOWN, 750);
    } else if (controller->isButtonDown(BUTTON_START)) {
        while (!controller->isButtonDown(BUTTON_SELECT)) {
            cycleStart = millis();
            maxVal = 0;
            curVal = 0;
            maxTime = 0;
            t = 0;

            while (t < 7000) {
                curVal = analogRead(SENSOR);

                if (curVal > maxVal) {
                    maxVal = curVal;
                    maxTime = t;

                    bt->print("New Min Light: ");
                    bt->println(1024 - maxVal);
                }

                turn(0, HIP_FORWARD, HIP_BACKWARD, KNEE_NEUTRAL, KNEE_DOWN, 750);

                t = millis() - cycleStart;
            }

            bt->print("Time: ");
            bt->println(maxTime);

            cycleStart = millis();
            t = 0;

            maxTime = 7000 - maxTime;
            

            while (t < maxTime) {
                turn(1, HIP_FORWARD, HIP_BACKWARD, KNEE_NEUTRAL, KNEE_DOWN, 750);

                t = millis() - cycleStart;
            }

            cycleStart = millis();
            t = 0;

            while (t < 10000) {
                gait_tripod(0, HIP_FORWARD, HIP_BACKWARD, KNEE_NEUTRAL, KNEE_DOWN, 750);

                t = millis() - cycleStart;
            }

            controller->update();
        }
    } else {
        stand();
    }

    /*else if (Dialmode == DIALMODE_ADJUST) {  // Servo adjust mode, put all servos at 90 degrees

        digitalWrite(13, flash(100));  // Flash LED13 rapidly in adjust mode
        stand_90_degrees();

        if (millis() > ReportTime) {
            ReportTime = millis() + 1000;
            Serial.println("AdjustMode");
        }

    } else if (Dialmode == DIALMODE_TEST) {   // Test each servo one by one
        pinMode(13, flash(500));      // flash LED13 moderately fast in servo test mode

        for (int i = 0; i < 2 * NUM_LEGS + NUM_GRIPSERVOS; i++) {
            p = analogRead(A0);
            if (p > 300 || p < 150) {
                break;
            }
            setServo(i, 140);
            delay(500);
            if (p > 300 || p < 150) {
                break;
            }
            setServo(i, 40);
            delay(500);
            setServo(i, 90);
            delay(100);
            Serial.print("SERVO: ");
            Serial.println(i);
        }

    } else if (Dialmode == DIALMODE_DEMO) {  // demo mode

        digitalWrite(13, flash(2000));  // flash LED13 very slowly in demo mode
        random_gait(timingfactor);
        if (millis() > ReportTime) {
            ReportTime = millis() + 1000;
            Serial.println("Demo Mode");
        }
        return;

    } else { // bluetooth mode (regardless of whether it's with or without the grip arm)

        digitalWrite(13, HIGH);   // LED13 is set to steady on in bluetooth mode
        if (millis() > ReportTime) {
            ReportTime = millis() + 2000;
            Serial.print("RC Mode:");
            Serial.print(ServosDetached);
            Serial.write(lastCmd);
            Serial.write(mode);
            Serial.write(submode);
            Serial.println("");
        }
        int gotnewdata = receiveDataHandler();  // handle any new incoming data first
        //Serial.print(gotnewdata); Serial.print(" ");

        // if its been more than 1 second since we got a valid bluetooth command
        // then for safety just stand still.

        if (millis() > LastValidReceiveTime + 1000) {
            if (millis() > LastValidReceiveTime + 15000) {
                // after 15 full seconds of not receiving a valid command, reset the bluetooth connection
                Serial.println("Loss of Signal: resetting bluetooth");
                // Make a three tone chirp to indicate reset
                beep(200, 40); // loss of connection test
                delay(100);
                beep(400, 40);
                delay(100);
                beep(600, 40);
                bt.begin(38400);
                LastReceiveTime = LastValidReceiveTime = millis();
                lastCmd = -1;  // for safety put it in stop mode
            }
            long losstime = millis() - LastValidReceiveTime;
            Serial.print("LOS ");
            Serial.println(losstime);  // LOS stands for "Loss of Signal"
            return;  // don't repeat commands if we haven't seen valid data in a while
        }

        if (gotnewdata == 0) {
            // we didn't receive any new instructions so repeat the last command unless it was binary
            // or unless we're in fight adjust mode
            if (lastCmd == -1) {
                //Serial.println("REP");
                return;
            }


            // fight submodes 3 and 4 should not be repeated without receiving
            // a packet because otherwise they'll zoom right to the end state instead
            // of giving the user a chance to make fine adjustments to position
            if (mode == MODE_FIGHT && (submode == SUBMODE_3 || submode == SUBMODE_4)) {
                //Serial.print("f");
                return;
            }

            // If the griparm in enabled then fight mode 2 really is grip control mode and
            // this mode is incremental in nature so the user can adjust the grip up/down/open/closed
            // bit by bit
            if (Dialmode == DIALMODE_RC_GRIPARM && mode == MODE_FIGHT && (submode == SUBMODE_2)) {
                return;
            }

        } else {
            LastReceiveTime = millis();

            checkLegStressSituation();

        }
        // Leg set mode should also not be repeated
        if (mode == MODE_LEG) {
            //Serial.print("l");
            return;
        } else if (mode == MODE_GAIT) {
            // repeat the last Gait command (from scratch typically)
            gait_command(LastGgaittype, LastGreverse, LastGhipforward, LastGhipbackward,
                         LastGkneeup, LastGkneedown, LastGleanangle, LastGtimeperiod);
            return;
        }
        //
        // Now we're either repeating the last command, or reading the new bluetooth command
        //

        ScamperTracker -= 1;
        if (ScamperTracker < 0) {
            ScamperTracker = 0;
        } else {
            //Serial.println(ScamperTracker);
        }

        switch (lastCmd) {
            case '?':
                bt.println("#Vorpal Hexapod");
                break;
            case 'W':
                mode = MODE_WALK;
                break;
            case 'F':
                mode = MODE_FIGHT;
                startedStanding = -1;
                break;
            case 'D':
                mode = MODE_DANCE;
                startedStanding = -1;
                break;
            case '1':
            case '2':
            case '3':
            case '4':
                submode = lastCmd;
                break;
            case 'w':  // weapon mode, special depending on mode
                startedStanding = -1;
                switch (mode) {
                    case MODE_FIGHT:
                        fight_mode(lastCmd, submode, 660 * timingfactor);
                        break;
                    case MODE_DANCE:
                        if (submode == SUBMODE_1) {
                            dance_dab(timingfactor);
                        } else if (submode == SUBMODE_2) {
                            dance_ballet(lastCmd);
                        } else if (submode == SUBMODE_3) {
                            wave(lastCmd);
                        } else if (submode == SUBMODE_4) {
                            dance_hands(lastCmd);
                        }
                        break;
                    case MODE_WALK: {
                        beep(400);
                        // stomp in place while beeping horn
                        if (submode == SUBMODE_2) { // high step
                            factor = 2;
                        }
                        int cyc = TRIPOD_CYCLE_TIME * factor;
                        if (submode == SUBMODE_4) {
                            cyc = TRIPOD_CYCLE_TIME / 2;  // faster stomp in scamper mode
                        }
                        gait_tripod(1, 90, 90,
                                    KNEE_TRIPOD_UP + factor * KNEE_TRIPOD_ADJ, KNEE_DOWN,
                                    cyc);
                    }
                        break;
                    default:     // for any other mode implement a "horn"
                        beep(400);
                        break;
                }
                break;

            case 'f':  // forward
                startedStanding = -1;
                switch (mode) {
                    case MODE_WALK:
                        if (submode == SUBMODE_4 && SuppressScamperUntil < millis()) {
                            gait_tripod_scamper(0, 0);
                        } else {
                            if (submode == SUBMODE_2) { // high step
                                factor = 2;
                            }
                            gait_tripod(1, (submode == SUBMODE_3) ? HIP_BACKWARD_SMALL : HIP_BACKWARD,
                                        (submode == SUBMODE_3) ? HIP_FORWARD_SMALL : HIP_FORWARD,
                                        KNEE_TRIPOD_UP + factor * KNEE_TRIPOD_ADJ, KNEE_DOWN,
                                        TRIPOD_CYCLE_TIME * factor);
                        }
                        break;
                    case MODE_DANCE:
                        if (submode == SUBMODE_1) {
                            dance(NO_LEGS, submode, timingfactor);
                        } else if (submode == SUBMODE_2) {
                            dance_ballet(lastCmd);
                        } else if (submode == SUBMODE_3) {
                            wave(lastCmd);
                        } else if (submode == SUBMODE_4) {
                            dance_hands(lastCmd);
                        }
                        break;
                    case MODE_FIGHT:
                        fight_mode(lastCmd, submode, FIGHT_CYCLE_TIME * timingfactor);
                        break;
                }
                break;

            case 'b':  // backward
                startedStanding = -1;
                switch (mode) {
                    case MODE_WALK:
                        if (submode == SUBMODE_4 && SuppressScamperUntil < millis()) {
                            gait_tripod_scamper(1, 0);
                        } else {
                            if (submode == SUBMODE_2) {
                                factor = 2;
                            }
                            gait_tripod(0, (submode == SUBMODE_3) ? HIP_BACKWARD_SMALL : HIP_BACKWARD,
                                        (submode == SUBMODE_3) ? HIP_FORWARD_SMALL : HIP_FORWARD,
                                        KNEE_TRIPOD_UP + factor * KNEE_TRIPOD_ADJ, KNEE_DOWN,
                                        TRIPOD_CYCLE_TIME * factor);
                        }
                        break;
                    case MODE_DANCE:
                        if (submode == SUBMODE_1) {
                            boogie_woogie(NO_LEGS, submode, timingfactor);
                        } else if (submode == SUBMODE_2) {
                            dance_ballet(lastCmd);
                        } else if (submode == SUBMODE_3) {
                            wave(lastCmd);
                        } else if (submode == SUBMODE_4) {
                            dance_hands(lastCmd);
                        }
                        break;
                    case MODE_FIGHT:
                        fight_mode(lastCmd, submode, FIGHT_CYCLE_TIME * timingfactor);
                        break;
                }
                break;

            case 'l': // left
                startedStanding = -1;
                switch (mode) {
                    case MODE_WALK:
                        if (submode == SUBMODE_2) {
                            factor = 2;
                        }
                        if (submode == SUBMODE_4 && SuppressScamperUntil < millis()) {
                            gait_tripod_scamper(1, 1);
                        } else {
                            turn(0, (submode == SUBMODE_3) ? HIP_BACKWARD_SMALL : HIP_BACKWARD,
                                 (submode == SUBMODE_3) ? HIP_FORWARD_SMALL : HIP_FORWARD,
                                 KNEE_TRIPOD_UP + factor * KNEE_TRIPOD_ADJ, KNEE_DOWN, TRIPOD_CYCLE_TIME * factor);
                        }
                        break;
                    case MODE_DANCE:
                        if (submode == SUBMODE_1) {
                            dance(TRIPOD1_LEGS, submode, timingfactor);
                        } else if (submode == SUBMODE_2) {
                            dance_ballet(lastCmd);
                        } else if (submode == SUBMODE_3) {
                            wave(lastCmd);
                        } else if (submode == SUBMODE_4) {
                            dance_hands(lastCmd);
                        }
                        break;
                    case MODE_FIGHT:
                        fight_mode(lastCmd, submode, FIGHT_CYCLE_TIME * timingfactor);
                        break;
                }
                break;

            case 'r':  // right
                startedStanding = -1;
                switch (mode) {
                    case MODE_WALK:
                        if (submode == SUBMODE_2) {
                            factor = 2;
                        }
                        if (submode == SUBMODE_4 && SuppressScamperUntil < millis()) {
                            gait_tripod_scamper(0, 1);
                        } else {
                            turn(1, (submode == SUBMODE_3) ? HIP_BACKWARD_SMALL : HIP_BACKWARD,
                                 (submode == SUBMODE_3) ? HIP_FORWARD_SMALL : HIP_FORWARD,
                                 KNEE_TRIPOD_UP + factor * KNEE_TRIPOD_ADJ,
                                 KNEE_DOWN, TRIPOD_CYCLE_TIME * factor);
                        }
                        break;
                    case MODE_DANCE:
                        if (submode == SUBMODE_1) {
                            dance(TRIPOD2_LEGS, submode, timingfactor);
                        } else if (submode == SUBMODE_2) {
                            dance_ballet(lastCmd);
                        } else if (submode == SUBMODE_3) {
                            wave(lastCmd);
                        } else if (submode == SUBMODE_4) {
                            dance_hands(lastCmd);
                        }
                        break;
                    case MODE_FIGHT:
                        fight_mode(lastCmd, submode, FIGHT_CYCLE_TIME * timingfactor);
                        break;
                }
                break;

            case 's':  // stop and just stand there
                if (startedStanding == -1) {
                    startedStanding = millis();
                }
                if (mode == MODE_FIGHT) {
                    startedStanding = millis();  // reset in fight mode, never sleep the legs
                    fight_mode(lastCmd, submode, 660 * timingfactor);
                } else if (mode == MODE_DANCE && submode == SUBMODE_2) { // ballet
                    tiptoes();
                } else if (mode == MODE_DANCE && submode == SUBMODE_4) {
                    dance_hands(lastCmd);
                } else {
                    if (millis() - startedStanding > BATTERYSAVER) {
                        //Serial.print("DET LC=");Serial.write(lastCmd); Serial.println("");
                        detach_all_servos();
                        return;
                    }
                    stand();
                }


                break;

            case 'a': // adjust mode
                stand_90_degrees();
                break;

            default:
                Serial.print("BAD CHAR:");
                Serial.write(lastCmd);
                Serial.println("");
                beep(100, 20);
        }  // end of switch


    }  // end of main if statement*/
}