#pragma once
#include <Arduino.h>

const long DISPLAY_DIGIT_DELAY = 1;
const long DISPLAY_UPDATE_RATE = 200;

const byte DIGIT_SELECT_PINS[] = {26, 27, 32, 33};
const byte CLOCK_PIN = 25;
const byte LATCH_PIN = 23;
const byte DATA_PIN = 22;

const byte BCD_COUNT = 4;

const int SEG[] = {
  0xDF, //DP 0
  0xBF, //A  1
  0x7F, //B  2
  0xFB, //C  3
  0xFE, //D  4
  0xFD, //E  5
  0xEF, //F  6
  0xF7  //G  7
};

const int BCD[] = {
  0xFF,
  SEG[1] & SEG[2] & SEG[3] & SEG[4] & SEG[5] & SEG[6],
  SEG[2] & SEG[3],
  SEG[1] & SEG[2] & SEG[7] & SEG[5] & SEG[4],
  SEG[1] & SEG[2] & SEG[3] & SEG[4] & SEG[7],
  SEG[6] & SEG[7] & SEG[2] & SEG[3],
  SEG[1] & SEG[6] & SEG[7] & SEG[3] & SEG[4],
  SEG[1] & SEG[6] & SEG[7] & SEG[4] & SEG[3] & SEG[5],
  SEG[1] & SEG[2] & SEG[3],
  SEG[1] & SEG[2] & SEG[3] & SEG[4] & SEG[5] & SEG[6] & SEG[7],
  SEG[1] & SEG[2] & SEG[3] & SEG[4] & SEG[6] & SEG[7]
};

class Display {
    private:
        int bcdDigits[4];
        byte digitIndex;
        
        unsigned long bcdLastDigitDisplay;
        unsigned long bcdLastDisplay;

    public:
        short value = 0;

        void setup();
        void update(unsigned long *milliseconds);
        void displayDigit();
        void getDigitsForBCD(int value, byte index);
        void blankBCD();
};