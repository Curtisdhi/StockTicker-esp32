#include "Display.h"

void Display::setup() {
    for (byte i=0; i < 4; i++) {
        pinMode(DIGIT_SELECT_PINS[i], OUTPUT);
    }
    pinMode(CLOCK_PIN, OUTPUT);
    pinMode(LATCH_PIN, OUTPUT);
    pinMode(DATA_PIN, OUTPUT);
}

void Display::update(unsigned long *milliseconds) {

    if ((*milliseconds - this->bcdLastDisplay) > DISPLAY_UPDATE_RATE) {
        this->bcdLastDisplay = *milliseconds;

        value *= 2;
        if (value >= 9999) {
            value = 0;
        } else if (value == 0) { value = 2; }
        this->getDigitsForBCD(this->value, BCD_COUNT - 1);
    }

    if ((*milliseconds - this->bcdLastDigitDisplay) > DISPLAY_DIGIT_DELAY) {
        this->bcdLastDigitDisplay = *milliseconds;
        displayDigit();
    }
}

void Display::displayDigit() {
    digitalWrite(DIGIT_SELECT_PINS[this->digitIndex], LOW);
    digitalWrite(LATCH_PIN, LOW);

    shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, this->bcdDigits[this->digitIndex]);

    this->digitIndex++;
    if (this->digitIndex >= 4) {
        this->digitIndex = 0;
    }
    digitalWrite(LATCH_PIN, HIGH);
    digitalWrite(DIGIT_SELECT_PINS[this->digitIndex], HIGH);
}

void Display::getDigitsForBCD(int value, byte index) {
    if (index == BCD_COUNT - 1) { this->blankBCD(); }
    if (value == 0) {
        this->bcdDigits[index] = BCD[1];
        return;
    }
    if (index > 0 && !(value < 10)) {
        this->getDigitsForBCD(value / 10, index - 1);
    }
    this->bcdDigits[index] = BCD[(value % 10) + 1];

}

void Display::blankBCD() {
    for (byte i = 0; i < BCD_COUNT; i++) { 
        this->bcdDigits[i] = BCD[0]; 
    }
}
