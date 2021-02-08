#include "globals.h"
#include "timer.h"
#include "sensors.h"
#include "real_time_clock.h"
#include "oled.h"
#include "humidity_temp.h"
#include "bluetooth.h"
#include "micro_sd.h"
#include "plant_lights.h"
#include "water_pump.h"

void pins() {
  pinMode(LUMEN_PIN, INPUT);
  pinMode(MOISTURE_PIN, INPUT);
  pinMode(MANUAL_WATER_PIN, INPUT);

  pinMode(PLANT_LIGHTS_PIN, OUTPUT);
  pinMode(WATER_PUMP_PIN, OUTPUT);

  digitalWrite(WATER_PUMP_PIN, HIGH);
  digitalWrite(PLANT_LIGHTS_PIN, HIGH);
}

void setup() {
  if (USE_SERIAL) {
    Serial.begin(9600);
    Serial.println(F("Waiting for Serial..."));
    while (! Serial);
    Serial.println(F("Serial Connected!"));
  }
  pins();
  setup_timer1();
  oled_setup();
  show_splash_screen();
  sd_setup();
  rtc_setup();
  bt_setup();
  setup_humidity_temp();
  checks();
}

int display_mode = 0;
int counter = 0;
void checks() {
  has_error = false;
  lumen = get_lumens();
  moisture = get_moisture();
  check_humidity_temp();
  time_right_now = Rtc.GetDateTime();
  int hour = time_right_now.Hour();
  int minute = time_right_now.Minute();
  check_plant_light_timer(hour, minute);
  set_lights();
  bt_loop();
  
  if (counter > 200) {
    log_stats();
    counter = 0;
  } else {
    counter++;
  }
  
  if (display_mode == 10|| display_mode == 20 || display_mode == 30 || display_mode == 40 || display_mode == 50) {
    oled.clear();
  }

  if (display_mode < 10) {
    display_clock();
  } else if (display_mode < 20) {
    display_humidity();
  } else if (display_mode < 30) {
    display_moisture();
  } else if (display_mode < 40) {
    display_lumen();
  } else if (display_mode < 50) {
    display_lights_detail();
  } else {
    display_mode = 0;
  }
  
  display_mode++;
}

void loop() {
  time_right_now = Rtc.GetDateTime();
  water_pump_loop(); // Check if time to water
  if (run_loop_checks) checks();
}
