#include "Display.h"

void Display::setup() {
    for (byte i=0; i < 4; i++) {
        pinMode(DIGIT_SELECT_PINS[i], OUTPUT);
    }
    pinMode(CLOCK_PIN, OUTPUT);
    pinMode(LATCH_PIN, OUTPUT);
    pinMode(DATA_PIN, OUTPUT);

    isDirty = true;
}

void Display::update(unsigned long *milliseconds) {

    if (isDirty && (*milliseconds - this->bcdLastDisplay) > DISPLAY_UPDATE_RATE) {
        this->bcdLastDisplay = *milliseconds;

        this->getDigitsForBCD(this->value);
        isDirty = false;
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

void Display::getDigitsForBCD(String value) {
    this->blankBCD();

    for (int i = BCD_COUNT - 1, strIndex = value.length() - 1; i >= 0 && strIndex >= 0;  i--, strIndex--) {
        boolean hasDP = false;
        char c = value[strIndex];
        if (c == '.') {
            hasDP = true;
            c = value[--strIndex];
        }
        int result = this->convertCharToBCD(c);
        if (hasDP) {
            result = result & SEG[0];
        }
        this->bcdDigits[i] = result;
    }
}

void Display::blankBCD() {
    for (byte i = 0; i < BCD_COUNT; i++) { 
        this->bcdDigits[i] = BCD[0]; 
    }
}

int Display::convertCharToBCD(char data) {
    switch (data) {
        case '0': return BCD[1];
        case '1': return BCD[2];
        case '2': return BCD[3];
        case '3': return BCD[4];
        case '4': return BCD[5];
        case '5': return BCD[6];
        case '6': return BCD[7];
        case '7': return BCD[8];
        case '8': return BCD[9];
        case '9': return BCD[10];
        case 0x00:
        case 0x20: 
        default: return BCD[0];
    }
}

void Display::setValue(String value) {
    this->value = value;
    isDirty = true;
}