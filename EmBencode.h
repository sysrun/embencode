// Embedded bencode support, header definitions.
// 2012-09-29 <jc@wippler.nl> http://opensource.org/licenses/mit-license.php

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

class EmBencode {
public:
  EmBencode () {}
  
  static void pushString (const char* str) {
    pushBytes(str, strlen(str));
  }

  static void pushBytes (const void* ptr, uint8_t len) {
    PushCount(len);
    PushChar(':');
    PushData(ptr, len);
  }

  static void pushNumber (long val) {
    PushChar('i');
    if (val < 0) {
      PushChar('-');
      val = -val;
    }
    PushCount(val);
    PushEnd();
  }

  static void startList () {
    PushChar('l');
  }

  static void endList () {
    PushEnd();
  }

  static void startDict () {
    PushChar('d');
  }

  static void endDict () {
    PushEnd();
  }

protected:
  static void PushCount (uint32_t num) {
    char buf[11];
    PushData(ultoa(num, buf, 10), strlen(buf));
  }

  static void PushEnd () {
    PushChar('e');
  }

  static void PushData (const void* ptr, uint8_t len) {
    for (const char* p = (const char*) ptr; len-- > 0; ++p)
      PushChar(*p);
  }

  static void PushChar (char ch);
};
