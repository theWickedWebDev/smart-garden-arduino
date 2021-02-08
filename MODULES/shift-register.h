#include "Arduino.h"

#define SR_DATA_PIN 3
#define SR_OE_PIN 10
#define SR_LATCH_PIN 5
#define SR_CLK_PIN 6
#define SR_RESET 7

void setup_shift_register() {
  pinMode(SR_DATA_PIN, OUTPUT);
  pinMode(SR_OE_PIN, OUTPUT);
  pinMode(SR_LATCH_PIN, OUTPUT);
  pinMode(SR_CLK_PIN, OUTPUT);
  pinMode(SR_RESET, OUTPUT);
  digitalWrite(SR_DATA_PIN, LOW);
  digitalWrite(SR_OE_PIN, LOW); // LOW = output enabled
  digitalWrite(SR_LATCH_PIN, LOW);
  digitalWrite(SR_CLK_PIN, LOW);
  digitalWrite(SR_RESET, HIGH);
}

void sr_latch() {
  digitalWrite(SR_LATCH_PIN, HIGH);
  digitalWrite(SR_LATCH_PIN, LOW);
}

void sr_reset() {
  digitalWrite(SR_RESET, LOW);
  delay(1);
  digitalWrite(SR_RESET, HIGH);
  sr_latch();
}

void sr_clock() {
  digitalWrite(SR_CLK_PIN, HIGH);
  digitalWrite(SR_CLK_PIN, LOW);
}

void sr_output_disabled() {
  digitalWrite(SR_OE_PIN, HIGH);
}

void sr_output_enabled() {
  digitalWrite(SR_OE_PIN, LOW);
}

int low_high(char b, int bit) { return b & (1 << bit) ? HIGH : LOW; }

void set_address(byte address1, byte address2) {
//  sr_output_disabled();
  for (int i = 0; i < 8; i++) {
    digitalWrite(SR_DATA_PIN, low_high(address1, i));
    sr_clock();
  }
  sr_latch();
  for (int i = 0; i < 8; i++) {
    digitalWrite(SR_DATA_PIN, low_high(address2, i));
    sr_clock();
  }
  sr_latch();
//  sr_output_enabled();
}

void shift_register(byte chars[]) {
  sr_reset();
  sr_output_disabled();

  char dig[] = {
    0b10000000,
    0b01000000,
    0b00100000,
    0b00010000
  };

  for (int i = 0; i < 4; i++) {
    byte segment = chars[i];
    if (segment >= 0) {
      for (int s = 0; s < 8; s++) {
        digitalWrite(SR_DATA_PIN, low_high(dig[i], s));
        sr_clock();
      }
      sr_latch();
      for (int s = 0; s < 8; s++) {
        digitalWrite(SR_DATA_PIN, low_high(segment, s));
        sr_clock();
      }
      sr_latch();
      sr_output_enabled();
      sr_reset();
    } else {

      for (int s = 0; s < 8; s++) {
        digitalWrite(SR_DATA_PIN, low_high(0b00000000, s));
        sr_clock();
      }
      sr_latch();
      for (int s = 0; s < 8; s++) {
        digitalWrite(SR_DATA_PIN, low_high(0b00000000, s));
        sr_clock();
      }
      sr_latch();
      sr_output_enabled();
      sr_reset();
    }
  }
}