#include "globals.h"

void turn_plant_lights_on() {
  digitalWrite(PLANT_LIGHTS_PIN, HIGH);
}

void turn_plant_lights_off() {
  digitalWrite(PLANT_LIGHTS_PIN, LOW);
}

void check_plant_light_timer(int hour, int minute) {
  int hour_mins = 0;
  
  if (hour > 0) {
    hour_mins = hour * 60;
  }
  
  bool past_on_time = hour_mins + minute >= LIGHTS_ON_TIME;
  bool before_end_time = hour_mins + minute < LIGHTS_OFF_TIME;
  
  if (before_end_time && past_on_time) {
    lights_on = true;
  } else {
    lights_on = false;
  }
}

void set_lights() {
  if (lights_on == 1) {
    turn_plant_lights_on();
  } else {
    turn_plant_lights_off();
  }
}
