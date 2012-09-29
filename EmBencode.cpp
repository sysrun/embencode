// Embedded bencode support, header definitions.
// 2012-09-29 <jc@wippler.nl> http://opensource.org/licenses/mit-license.php

#include "EmBencode.h"

uint8_t EmBdecode::process (char ch) {
  *next++ = ch;
  switch (state) {
    case EMB_ANY:
      if (ch < '0' || ch > '9') {
        if (ch == 'i')
          state = EMB_INT;
        else if (ch == 'd' || ch == 'l')
          ++level;
        else if (ch == 'e') {
          --level;
          break; // end of dict or list
        }
        return 0;
      }
      state = EMB_LEN;
      count = 0;
      // fall through
    case EMB_LEN:
    case EMB_INT:
      if ('0' <= ch && ch <= '9')
        count = 10 * count + (ch - '0');
      else if (ch == '-')
        ;
      else if (state == EMB_LEN) {
        if (count == 0)
          break; // empty string
        state = EMB_STR;
      } else
        break; // end of int
      return 0;
    case EMB_STR:
      if (--count == 0)
        break; // end of string
      return 0;
  }
  // end of an item reached
  if (level > 0) {
    state = EMB_ANY;
    return 0;
  }
  return reset(); // not in dict or list, so data is complete
}
