#include <Arduino.h>
#include "Network.h"
#include "Display.h"

Network network;
Display display;

void setup() {
  delay(100);
  Serial.begin(115200);
  
  network.setup();
  display.setup();
}

void loop() {
  unsigned long milliseconds = millis();
  network.update(&milliseconds);

  if (network.hasResponse()) {
    display.setValue(network.getResponse());
  }
  display.update(&milliseconds);
}
