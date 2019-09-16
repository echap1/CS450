#include "Arduino.h"
#include "scoreDisplay.h"

#define DATA_PIN 2
#define CLOCK_PIN 3
#define LATCH_PIN 4

#define POT_PIN_1 A0
#define POT_PIN_2 A1

#define PADDLE_THICKNESS 3
#define PADDLE_RANGE 5

void setMatrixRow(int row, int col_num);
void displayMatrix();
void clearMatrix();
void setPoint(int x, int y);

void moveBall();
void resetBall(bool player1);

unsigned char matrix[8];

double ball_x = 0.0f;
double ball_y = 0.0f;

int ball_sx = 0;
int ball_sy = 0;

double ball_vx = 1.0f;
double ball_vy = 0.45644f;

int p1y = 1;
int p2y = 1;

int p1score = 0;
int p2score = 0;

int millisSinceUpdate = 0;
int updateDelay = 25;

int updatesPerBallUpdate = 1;
int updatesUntilBallUpdate = 0;

bool ai = false;

long lastMillis = 0;

void setup() {
    initScoreboard();

    pinMode(DATA_PIN, OUTPUT);
    pinMode(CLOCK_PIN, OUTPUT);
    pinMode(LATCH_PIN, OUTPUT);

    pinMode(POT_PIN_1, INPUT);
    pinMode(POT_PIN_2, INPUT);

    randomSeed(analogRead(A5));

    ai = analogRead(POT_PIN_1) > 512;
    updatesPerBallUpdate = (int) (((double) analogRead(POT_PIN_2)) / 102.3) + 1;

    resetBall(random(0, 2) == 1);
}

void loop() {
    millisSinceUpdate += millis() - lastMillis;
    lastMillis = millis();

    displayScoreboard(p1score, p2score);

    if (millisSinceUpdate > updateDelay) {
        millisSinceUpdate -= updateDelay;

        clearMatrix();

        p1y = lround(PADDLE_RANGE * (((double) analogRead(POT_PIN_1)) / 1023.0));

        if (ai) {
            p2y = ball_sy - 1;
            if (p2y < 1) p2y = 1;
            if (p2y > 5) p2y = 5;
        }

        else p2y = lround(PADDLE_RANGE * (((double) analogRead(POT_PIN_2)) / 1023.0));

        for(int i = 0; i < PADDLE_THICKNESS; i++) {
            setPoint(0, p1y + i);
            setPoint(7, p2y + i);
        }

        if (updatesUntilBallUpdate == 0) {
            updatesUntilBallUpdate = updatesPerBallUpdate;
            moveBall();
        }

        setPoint(ball_sx, ball_sy);

        updatesUntilBallUpdate--;
    }
    
    displayMatrix();
}

void moveBall() {
    ball_x += ball_vx;
    ball_y += ball_vy;

    if (ball_y < 0.0) {
        ball_y = 0;
        ball_vy = -ball_vy;
        ball_y += ball_vy;
    }

    if (ball_y > 7.0) {
        ball_y = 7;
        ball_vy = -ball_vy;
        ball_y += ball_vy;
    }

    if (ball_x < 1.0) {
        if (ball_sy >= p1y && ball_sy < p1y + PADDLE_THICKNESS) {
            ball_x = 1;
            ball_vx = -ball_vx;
            ball_x += ball_vx;

            if (ball_sy == p1y) ball_vy += 0.1;
            if (ball_sy == p1y + PADDLE_THICKNESS - 1) ball_vy -= 0.1;

            if (ball_vy > 1) ball_vy = 1;
            if (ball_vy < -1) ball_vy = -1;
        }
        else {
            p2score++;
            resetBall(false);
        }
    }

    if (ball_x > 6.0) {
        if (ball_sy >= p2y && ball_sy < p2y + PADDLE_THICKNESS) {
            ball_x = 6;
            ball_vx = -ball_vx;
            ball_x += ball_vx;

            if (ball_sy == p2y) ball_vy += 0.1;
            if (ball_sy == p2y + PADDLE_THICKNESS - 1) ball_vy -= 0.1;

            if (ball_vy > 1) ball_vy = 1;
            if (ball_vy < -1) ball_vy = -1;
        }
        else {
            p1score++;
            resetBall(true);
        }
    }

    ball_sx = lround(ball_x);
    ball_sy = lround(ball_y);
}

void resetBall(bool player1) {
    ball_vx = 1;
    ball_vy = 0.7 * (((double) random(-1000, 1000)) / 1000.0);

    ball_y = 4;
    ball_sy = 4;

    ball_x = 2;
    ball_sx = 2;

    if (player1) {
        ball_vx = -ball_vx;
        ball_x = 5;
        ball_sx = 5;
    }

    updatesUntilBallUpdate = 1000 / updateDelay;
}

void setMatrixRow(int row, int col_num) {
    unsigned char col_val = ~(1UL << col_num);

    digitalWrite(LATCH_PIN, LOW);
    shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, col_val);
    shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, row);
    digitalWrite(LATCH_PIN, HIGH);
}

void displayMatrix() {
    setMatrixRow(matrix[0], 0);
    setMatrixRow(matrix[1], 1);
    setMatrixRow(matrix[2], 2);
    setMatrixRow(matrix[3], 3);
    setMatrixRow(matrix[4], 4);
    setMatrixRow(matrix[5], 5);
    setMatrixRow(matrix[6], 6);
    setMatrixRow(matrix[7], 7);
    setMatrixRow(0, 7);
}

void clearMatrix() {
    matrix[0] = 0;
    matrix[1] = 0;
    matrix[2] = 0;
    matrix[3] = 0;
    matrix[4] = 0;
    matrix[5] = 0;
    matrix[6] = 0;
    matrix[7] = 0;
}

void setPoint(int x, int y) {
    bitSet(matrix[y], x);
}