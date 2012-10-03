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
    char ch = Serial.read();
    Serial.write(ch);
    uint8_t bytes = decode.process(ch);
    if (bytes > 0) {
      Serial.print(" => ");
      Serial.print((int) bytes);
      Serial.println(" bytes");
      for (;;) {
        uint8_t token = decode.nextToken();
        if (token == EmBdecode::T_END)
          break;
        switch (token) {
          case EmBdecode::T_STRING:
            Serial.print(" string: ");
            Serial.println(decode.asString());
            break;
          case EmBdecode::T_NUMBER:
            Serial.print(" number: ");
            Serial.println(decode.asNumber());
            break;
          case EmBdecode::T_DICT:
            Serial.println(" > dict");
            break;
          case EmBdecode::T_LIST:
            Serial.println(" > list");
            break;
          case EmBdecode::T_POP:
            Serial.println(" < pop");
            break;
        }
      }
      decode.reset();
    }
  }
}
