#pragma once

// Garden Config
int long LIGHTS_ON_TIME = 480; //480 (08:00) time when lights turn off (minutes)
int long LIGHTS_OFF_TIME = 1200; //1200 (20:00) time when lights turn off (minutes)

#define WATER_TIME_LIMIT 6
#define WATER_TIME_DELAY 10000

// PINS
#define LUMEN_PIN A3
#define MOISTURE_PIN A6
#define MANUAL_WATER_PIN 16

#define PLANT_LIGHTS_PIN 5

#define WATER_PUMP_PIN 15

#define SD_CLK_PIN 4

//
#define USE_SERIAL true
#define SD_LOG_FILENAME "firstday.txt"

// TIMER
/*
 * If COMPARE_MATCH_REGISTER is set to 1s
 * then loop will run its checks 1s * TIMER_TRIGGER_COUNT = 2s
*/
// 15650 = approximately 1s
#define COMPARE_MATCH_REGISTER 7825 // (16*10^6) / (1*1024) - 1 (must be <65536) - timer0
#define TIMER_TRIGGER_COUNT 4  // 7825 + 10 = ~5s

// OLED
#define I2C_ADDRESS 0x3C // 0X3C+SA0 - 0x3C or 0x3D
#define RST_PIN -1 // Define proper RST_PIN if required.

// Variables
bool lights_on;
boolean run_loop_checks = 1;

String error_msg = "";
bool has_error = false;
int lumen;
int moisture;
float temperature = 0;
float humidity = 0;
int watering_time = 0;
int watering_delay = 0;
bool AUTO_WATER_ON = false;
int MOISTURE_LEVEL = 600;
int LUMEN_LEVEL = 500;
bool pressing_manual_water_btn = false;
bool should_water_plants = false;
