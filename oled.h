#include "Arduino.h"
#include "globals.h"

#include "SSD1306Ascii.h"
#include "SSD1306AsciiWire.h"
SSD1306AsciiWire oled;

void oled_setup () {
  oled.begin(&Adafruit128x64, I2C_ADDRESS);
  if (USE_SERIAL) {
    Serial.println(F("✔ OLED Adafruit 128x64 I2C"));
  }
}

void show_splash_screen() {
  oled.setFont(Arial_bold_14);
  oled.set2X();
  oled.setCol(30);
  oled.println(F("Smart"));
  oled.setCol(22);
  oled.println(F("Garden "));
  delay(3000);
  oled.clear();
}

bool cleared_on_error = false;

void display_clock() {
  oled.setFont(TimesNewRoman16);
  int hour = time_right_now.Hour();
  int minute = time_right_now.Minute();
  int second = time_right_now.Second();
  int month = time_right_now.Month();
  int day = time_right_now.Day();
  int year = time_right_now.Year();
  oled.set1X();
  oled.setCursor(0,0);
  oled.print(F("     "));
  oled.setCol(3);
  oled.print(month);
  oled.print(F("/"));
  oled.print(day);
  oled.print(F("/"));
  oled.print(year);
  oled.setCol(105);
  oled.print(int((temperature * 9/5) + 32));
  oled.println(F("F   "));
  oled.println();
  oled.set2X();
  oled.setCol(28);
  int is_afternoon = hour > 12;
  int formatted_hour = hour == 0 ? 12 : is_afternoon ? hour - 12 : hour;
  oled.print(formatted_hour);
  oled.print(F(":"));
  String formatted_minute = minute > 9 ? minute : "0" + String(minute);
  oled.print(formatted_minute);
  oled.print(is_afternoon ? F("pm") : F("am"));
  oled.print(F("   "));
}

// TODO: make this generic display_stat()
void display_humidity() {
  oled.set1X();
  oled.setCursor(0,0);
  oled.setCol(45);
  oled.println("Humidity");
  oled.set2X();
  oled.setCol(52);
  oled.print(int(humidity));
  oled.println(" %  ");
}

void display_moisture() {
  oled.set1X();
  oled.setCursor(0,0);
  oled.setCol(40);
  oled.println("Moisture");
  oled.set2X();
  oled.setCol(56);
  oled.print(moisture);
  oled.println("  ");
}

void display_lumen() {
  oled.set1X();
  oled.setCursor(0,0);
  oled.setCol(50);
  oled.println("Lumen");
  oled.set2X();
  oled.setCol(50);
  oled.print(lumen);
  oled.println("  ");
}

void display_lights_detail() {
  oled.set1X();
  oled.setCursor(0,0);
  oled.setCol(42);
  oled.println("Lights on");
  oled.setCol(17);
  int lights_hour = int(LIGHTS_ON_TIME / 60);
  int lights_min = int(LIGHTS_ON_TIME % 60);
  int is_afternoon = lights_hour > 12;
  int formatted_hour = lights_hour == 0 ? 12 : is_afternoon ? lights_hour - 12 : lights_hour;
  oled.print(formatted_hour);
  oled.print(":");
  String formatted_minute = lights_min > 9 ? lights_min : "0" + String(lights_min);
  oled.print(formatted_minute);
  oled.print(is_afternoon ? F("pm") : F("am"));
  oled.print(" - ");
  
  lights_hour = int(LIGHTS_OFF_TIME / 60);
  lights_min = int(LIGHTS_OFF_TIME % 60);
  is_afternoon = lights_hour > 12;
  formatted_hour = lights_hour == 0 ? 12 : is_afternoon ? lights_hour - 12 : lights_hour;
  oled.print(formatted_hour);
  oled.print(":");
  formatted_minute = lights_min > 9 ? lights_min : "0" + String(lights_min);
  oled.print(formatted_minute);
  oled.print(is_afternoon ? F("pm") : F("am"));
}
