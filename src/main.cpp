#include <Arduino.h>
#include "Display.h"

Display display;

void setup() {
  delay(100);
  Serial.begin(19200);

  display.setup();
  display.value = 2;
}

void loop() {
  unsigned long milliseconds = millis();
  display.update(&milliseconds);


}
