/// @dir blinky
/// Blinky demo sketch, controlled by Bencoded commands.
// 2012-10-03 <jc@wippler.nl> http://opensource.org/licenses/mit-license.php

// Sample data:
//  l4:ratei250eel7:triggeri4eel5:counti10ee
//  l4:ratei100eel7:triggeri10eel5:counti20ee

#include "EmBencode.h"

#define VERSION 100
#define LED 9

char embuf [200];
EmBdecode decoder (embuf, sizeof embuf);
EmBencode encoder;

int rate;       // time between toggling the LED (ms)
int count;      // number of blinks still to go
int trigger;    // after how many blinks to trigger
long total;     // total number of blinks
long lastFlip;  // time when we last flipped the LED (ms)

void EmBencode::PushChar (char ch) {
  Serial.write(ch);
}

static void sendGreeting () {
  encoder.startList();
  encoder.push("blinky");
  encoder.push(VERSION);
  encoder.endList();
}

static void sendErrorMsg (const char* msg) {
  encoder.startList();
  encoder.push(1); // error code
  encoder.push(msg);
  encoder.endList();
}

static void sendDoneMsg () {
  encoder.startList();
  encoder.push("done");
  encoder.endList();
}

void sendTriggerTime () {
  encoder.startList();
  encoder.push("time");
  encoder.push(millis());
  encoder.endList();
}

static void setNumber (int& ivar) {
  if (decoder.nextToken() == EmBdecode::T_NUMBER)
    ivar = decoder.asNumber();
  else
    sendErrorMsg("number expected");
}

void setup () {
  Serial.begin(57600);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, 0);
  lastFlip = millis();
  sendGreeting();
}

void loop () {
  // process incoming serial data
  if (Serial.available() > 0 &&
      decoder.process(Serial.read()) > 0 &&
      decoder.nextToken() == EmBdecode::T_LIST &&
      decoder.nextToken() == EmBdecode::T_STRING) {
    // a complete message has been received
    const char* cmd = decoder.asString();
    if (strcmp(cmd, "rate") == 0)
      setNumber(rate);
    else if (strcmp(cmd, "count") == 0)
      setNumber(count);
    else if (strcmp(cmd, "trigger") == 0)
      setNumber(trigger);
    else
      sendErrorMsg("command expected");
    decoder.reset();
    total = 0; // synchronise on each new setting
  }
  // perform the requested blinky work
  long now = millis();
  if (count > 0 && rate > 0 && now >= lastFlip + rate) {
    // the time has come to flip the LED state
    lastFlip = now;
    bool on = digitalRead(LED);
    digitalWrite(LED, ! on);
    // check whether the LED just went off
    if (!on) {
      // has the time come to send out a trigger report?
      if (trigger > 0 && ++total % trigger == 0)
        sendTriggerTime();
      // have we completed our work?
      if (--count == 0)
        sendDoneMsg();
    }
  }
}
