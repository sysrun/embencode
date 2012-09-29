// Demo sketch to send some bencoded data structures out over the serial port.
// 2012-09-29 <jc@wippler.nl> http://opensource.org/licenses/mit-license.php

#include "EmBencode.h"

void EmBencode::PushChar (char ch) {
  Serial.write(ch);
}

static void sendSomeData () {
  EmBencode encoder;
  // send a simple string
  encoder.pushString("abcde");
  // send a number of bytes, could be binary
  encoder.pushBytes("123", 3);
  // send an integer
  encoder.pushNumber(12345);
  // send a list with an int, a nested list, and an int
  encoder.startList();
    encoder.pushNumber(987);
    encoder.startList();
      encoder.pushNumber(654);
    encoder.endList();
    encoder.pushNumber(321);
  encoder.endList();
  // send a large integer
  encoder.pushNumber(999999999);
  // send a dictionary with two entries
  encoder.startDict();
    encoder.pushString("one");
    encoder.pushNumber(11);
    encoder.pushString("two");
    encoder.pushNumber(22);
  encoder.endDict();
  // send one last string
  encoder.pushString("bye!");
}

void setup () {
  Serial.begin(57600);
  Serial.println("\n[serialSend]");
}

void loop () {
  sendSomeData();
  Serial.println();
  delay(3000);
}