#include "globals.h"

void water_plants() {
  digitalWrite(WATER_PUMP_PIN, LOW);
}

void dont_water_plants() {
  digitalWrite(WATER_PUMP_PIN, HIGH);
}

void water_pump_loop() {
  pressing_manual_water_btn = digitalRead(MANUAL_WATER_PIN) == HIGH;
  should_water_plants = pressing_manual_water_btn || (AUTO_WATER_ON && moisture > MOISTURE_LEVEL);

  if (should_water_plants) {
    water_plants();
  } else {
    dont_water_plants();
  }
}
