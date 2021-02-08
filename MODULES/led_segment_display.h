
/*
 * LCD Display Module (Handmade)
 *
 * PINOUT
 *
 * Digits
 *   1 - j  //   2 - k  //  3 - l  //  4 - h
 *
 * Colon
 *   Vcc - i
 *   Gnd => Resistor - m
 *
 * Segments
 *   A  - o
 *   B  - p
 *   C  - f
 *   D  - c
 *   E  - e
 *   F  - n
 *   G  - g
 *   DP - d
 *
*/

#include "globals.h"
#include "Arduino.h"

static byte segmentMasks[] = {
    0b00000011, // 0
    0b10011111, // 1
    0b00100101, // 2
    0b00001101, // 3
    0b10011001, // 4
    0b01001001, // 5
    0b01000001, // 6
    0b00011111, // 7
    0b00000001, // 8
    0b00011001, // 9
    0b11100011,  // L                  // 10
    0b11101111,  // underscore (_)     // 11
    0b11111101,  // middlescore (_)    // 12
    0b01111111,  // overscore (_)      // 13
    0b11101101,  // doublescore (_)    // 14
    0b01101101,  // triplescore (_)    // 15
    0b11010101,  // hat ,-,             // 16
    0b01110001,  // F ,-,               // 17
    0b10010001,  // h ,-,               // 18
    0b11000101,  // o ,-,               // 19
    0b11111111, // BLANK                // 20
};

void display_humidity_on_led() {
  led_is_displaying_time = 0;
  int long x = int(humidity);
  int long digits[10];
  int digits_count = 0;
  int rem;

  // TODO REVERSE THIS
  while (x != 0) {
    rem = x % 10;
    x /= 10;
    digits[0] = digits[1];
    digits[1] = digits[2];
    digits[2] = digits[3];
    digits[3] = rem;
    digits_count++;
  }

  byte tmp[] = { segmentMasks[0], segmentMasks[1], segmentMasks[2], segmentMasks[3], };
  shift_register(tmp);
}

void display_temp_on_led() {
  led_is_displaying_time = 1;
  int long x = (int(temperature) * 9/5) + 32;
  Serial.println(x);

  int long digits[10];
  int digits_count = 0;
  int rem;

  while (x != 0) {
    rem = x % 10;
    x /= 10;
    digits[0] = digits[1];
    digits[1] = digits[2];
    digits[2] = digits[3];
    digits[3] = rem;
    digits_count++;
  }

  byte tmp[] = { segmentMasks[0], segmentMasks[1], segmentMasks[2], segmentMasks[3], };
  shift_register(tmp);
//  switch (digits_count) {
//    case 1:
//      shift_register({ segmentMasks[20], segmentMasks[20], segmentMasks[digits[3]], segmentMasks[17]});
//      break;
//    case 2:
//      shift_register({ segmentMasks[20], segmentMasks[digits[3]], segmentMasks[digits[2]], segmentMasks[17]});
//      break;
//    case 3:
//      shift_register({ segmentMasks[digits[3]], segmentMasks[digits[2]], segmentMasks[digits[1]], segmentMasks[17]});
//      break;
//    default:
//      shift_register({ segmentMasks[20], segmentMasks[digits[3]], segmentMasks[20], segmentMasks[20]});
//      break;
//  }
}

void display_time_on_led() {
  led_is_displaying_time = 1;
  int digits[4];
  int hour = time_right_now.Hour();
  int minute = time_right_now.Minute();
  hourMinuteToDigits(hour, minute, digits);
  for (int i = 1; i < 5; i++) {
    //segmentMasks[digits[i - 1]]
  }

  byte tmp[] = { segmentMasks[4], segmentMasks[5], segmentMasks[6], segmentMasks[7], };
  shift_register(tmp);
};

void display_moisture_on_led() {
//  led_is_displaying_time = 0;
//  sr_reset();
//
//  int long _moisture = moisture;
//  int long digits[10];
//  int digits_count = 0;
//  int rem;
//
//  while (_moisture != 0) {
//    rem = _moisture % 10;
//    _moisture /= 10;
//    digits[0] = digits[1];
//    digits[1] = digits[2];
//    digits[2] = digits[3];
//    digits[3] = rem;
//    digits_count++;
//  }
//
//  switch (digits_count) {
//    case 3:
//      flash_digit(segmentMasks[digits[3]], 1);
//      flash_digit(segmentMasks[digits[2]], 2);
//      flash_digit(segmentMasks[digits[1]], 3);
//    break;
//    case 2:
//      flash_digit(segmentMasks[digits[3]], 2);
//      flash_digit(segmentMasks[digits[2]], 3);
//    break;
//    case 1:
//      flash_digit(segmentMasks[digits[3]], 3);
//    break;
//    default:
//    break;
//  }
//
//  flash_digit(segmentMasks[16], 4);

  byte tmp[] = { segmentMasks[5], segmentMasks[1], segmentMasks[2], segmentMasks[0], };
  shift_register(tmp);
};

void display_lumen_on_led() {
//  led_is_displaying_time = 0;
//  sr_reset();
//
//  int long _lumen = lumen;
//  int long digits[10];
//  int digits_count = 0;
//  int rem;
//
//  while (_lumen != 0) {
//    rem = _lumen % 10;
//    _lumen /= 10;
//    digits[0] = digits[1];
//    digits[1] = digits[2];
//    digits[2] = digits[3];
//    digits[3] = rem;
//    digits_count++;
//  }
//
//  // Third digit will either be the hundredths place, OR symbol multiplier
//  switch (digits_count) {
//    case 5:
//      flash_digit(segmentMasks[digits[3]], 1);
//      flash_digit(segmentMasks[digits[2]], 2);
//      flash_digit(segmentMasks[15], 3);
//    break;
//    case 4:
//      flash_digit(segmentMasks[digits[3]], 1);
//      flash_digit(segmentMasks[digits[2]], 2);
//      flash_digit(segmentMasks[14], 3);
//    break;
//    case 3:
//      flash_digit(segmentMasks[digits[3]], 1);
//      flash_digit(segmentMasks[digits[2]], 2);
//      flash_digit(segmentMasks[digits[1]], 3);
//    break;
//    case 2:
//      flash_digit(segmentMasks[digits[3]], 2);
//      flash_digit(segmentMasks[digits[2]], 3);
//    break;
//    case 1:
//      flash_digit(segmentMasks[digits[3]], 3);
//    break;
//    default:
//    break;
//  }
//
//  // Add L. Unit
//  byte lumen_char = segmentMasks[10];
//  lumen_char &= ~1;
//  flash_digit(lumen_char, 4);

  byte tmp[] = { segmentMasks[3], segmentMasks[2], segmentMasks[2], segmentMasks[3], };
  shift_register(tmp);
};