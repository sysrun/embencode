// Demo sketch to receive some bencoded data structures from the serial port.
// 2012-09-29 <jc@wippler.nl> http://opensource.org/licenses/mit-license.php

// Sample data:
//  5:abcde3:123i12345eli987eli654eei321eei999999999ed3:onei11e3:twoi22ee4:bye!

#include "EmBencode.h"

char embuf [200];
EmBdecode decode (embuf, sizeof embuf);

void setup () {
  Serial.begin(57600);
  Serial.println("\n[serialReceive]");
}

void loop () {
  if (Serial.available() > 0) {
    uint8_t bytes = decode.process(Serial.read());
    if (bytes > 0) {
      embuf[bytes] = 0;
      Serial.println(embuf);
    }
  }
}