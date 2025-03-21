/*
 * ModSecurity, http://www.modsecurity.org/
 * Copyright (c) 2015 - 2025 Trustwave Holdings, Inc. (http://www.trustwave.com/)
 *
 * Modifications made by Stone Rhino, 2025.
 *
 * You may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 */
#include "util.h"

#include "string.h"

namespace SrSecurity::Transformation::ModSecurity {
std::string cmdLime(std::string_view data) {
  std::string result;
  result.resize(data.length());
  char* d = result.data();

  bool space = false;
  for (auto ch : data) {
    switch (ch) {
    /* remove some characters */
    case '"':
    case '\'':
    case '\\':
    case '^':
      break;

    /* replace some characters to space (only one) */
    case ' ':
    case ',':
    case ';':
    case '\t':
    case '\r':
    case '\n':
      if (space == false) {
        *d++ = ' ';
        space = true;
      }
      break;

    /* remove space before / or ( */
    case '/':
    case '(':
      if (space) {
        d--;
      }
      space = false;
      *d++ = ch;
      break;
      
    /* copy normal characters */
    default:
      char b = std::tolower(ch);
      *d++ = b;
      space = false;
      break;
    }
  }

  const auto new_len = d - result.data();
  const auto changed = new_len != data.length();

  if (changed) {
    result.resize(new_len);
  } else {
    result.clear();
  }

  return result;
}

std::string cssDecode(std::string_view data) {
  using namespace modsecurity;
  using namespace modsecurity::utils::string;

  const auto input_len = data.length();
  auto input = reinterpret_cast<const unsigned char*>(data.data());

  std::string result;
  result.resize(input_len);
  auto d = result.data();
  bool changed = false;

  std::string::size_type i = 0;
  while (i < input_len) {
    /* Is the character a backslash? */
    if (input[i] == '\\') {
      /* Is there at least one more byte? */
      if (i + 1 < input_len) {
        i++; /* We are not going to need the backslash. */

        /* Check for 1-6 hex characters following the backslash */
        std::string::size_type j = 0;
        while ((j < 6) && (i + j < input_len) && (VALID_HEX(input[i + j]))) {
          j++;
        }

        if (j > 0) {
          /* We have at least one valid hexadecimal character. */
          int fullcheck = 0;

          /* For now just use the last two bytes. */
          switch (j) {
          /* Number of hex characters */
          case 1:
            *d++ = utils::string::xsingle2c(&input[i]);
            break;

          case 2:
          case 3:
            /* Use the last two from the end. */
            *d++ = utils::string::x2c(&input[i + j - 2]);
            break;

          case 4:
            /* Use the last two from the end, but request
             * a full width check.
             */
            *d = utils::string::x2c(&input[i + j - 2]);
            fullcheck = 1;
            break;

          case 5:
            /* Use the last two from the end, but request
             * a full width check if the number is greater
             * or equal to 0xFFFF.
             */
            *d = utils::string::x2c(&input[i + j - 2]);
            /* Do full check if first byte is 0 */
            if (input[i] == '0') {
              fullcheck = 1;
            } else {
              d++;
            }
            break;

          case 6:
            /* Use the last two from the end, but request
             * a full width check if the number is greater
             * or equal to 0xFFFF.
             */
            *d = utils::string::x2c(&input[i + j - 2]);

            /* Do full check if first/second bytes are 0 */
            if ((input[i] == '0') && (input[i + 1] == '0')) {
              fullcheck = 1;
            } else {
              d++;
            }
            break;
          }

          /* Full width ASCII (0xff01 - 0xff5e) needs 0x20 added */
          if (fullcheck) {
            if ((*d > 0x00) && (*d < 0x5f) &&
                ((input[i + j - 3] == 'f') || (input[i + j - 3] == 'F')) &&
                ((input[i + j - 4] == 'f') || (input[i + j - 4] == 'F'))) {
              (*d) += 0x20;
            }

            d++;
          }

          /* We must ignore a single whitespace after a hex escape */
          if ((i + j < input_len) && isspace(input[i + j])) {
            j++;
          }

          /* Move over. */
          i += j;

          changed = true;
        } else if (input[i] == '\n') {
          /* No hexadecimal digits after backslash */
          /* A newline character following backslash is ignored. */
          i++;
          changed = true;
        } else {
          /* The character after backslash is not a hexadecimal digit,
           * nor a newline. */
          /* Use one character after backslash as is. */
          *d++ = input[i++];
        }
      } else {
        /* No characters after backslash. */
        /* Do not include backslash in output
         *(continuation to nothing) */
        i++;
        changed = true;
      }
    } else {
      /* Character is not a backslash. */
      /* Copy one normal character to output. */
      *d++ = input[i++];
    }
  }

  /* Terminate output string. */
  *d = '\0';

  if (changed) {
    result.resize(d - result.data());
  } else {
    result.clear();
  }

  return result;
}
} // namespace SrSecurity::Transformation::ModSecurity