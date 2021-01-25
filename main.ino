/*
 * TODO
 * INCREASE WAIT TIME
 * HOOK UP POT TO MOISTURE SENSOR
 * LED BAR GRAPH FOR POT (shift register.. :/)
 * MAKE SDCARD READABLE TO EXTERNAL
 *   - perhaps a button press changes functions with SDcard loop and instead
 *   - of writing, it reads and then writes/send to a different location?
 *   - or I can take the card out and it doesnt break the system or card, load it on comp,
 *     and put back in and then have it recognize it and resume
 *
 * AUTO RESET ON FAILURE
 *    #define RESET_PIN 7
 *    digitalWrite(RESET_PIN, LOW); // resets
 *    pin7 is connectect directly to RST pin
 *
 * LED DC light?
 *
 * ABLE TO TURN OFF NOTIFICATION LIGHTS
 *
 * MAKE LIGHT TIMES CONFIGURABLE
 *
*/

// CONFIGURABLES
#define LIGHTS_ON_TIME 480 // (08:00) time when lights turn off (minutes)
#define LIGHTS_OFF_TIME 1200 // (20:00) time when lights turn off (minutes)
#define MOISTURE_RES_LEVEL 500  // how dry water is before watering
#define TICKS_BEFORE_WATER 20  // how long to stay dry before watering
#define WATER_MAX_RUN_TICK_LIMIT 1 // max water pump running time (ticks)
//
//
//
//
//

// Interrupt
int timer1_counter;
int counter = 0;

// RTC GLOBALS
#include <Wire.h> // must be included here so that Arduino library object file references work
#include <RtcDS3231.h>
RtcDS3231<TwoWire> Rtc(Wire);

// OLED GLOBALS
#include "SSD1306Ascii.h"
#include "SSD1306AsciiWire.h"
#define I2C_ADDRESS 0x3C // 0X3C+SA0 - 0x3C or 0x3D
#define RST_PIN -1 // Define proper RST_PIN if required.
SSD1306AsciiWire oled;
#define INCLUDE_SCROLLING 0

// SDCARD Globals
#include <SD.h>
#include <SPI.h>
#define SD_CLK_PIN 4

// LUMEN
#define VIN 5
#define R 1000
#define LUX_PIN A0
#define PLANT_LIGHT_PIN 9
bool lights_on;

// WATER PUMP
#define WATER_PUMP_PIN 8

// Moisture Sensor
#define MOISTURE_PIN A2
int moisture_value;
int ticks_waited = 0;
bool needs_water = false;

// LED status pins
#define ABOUT_TO_WATER_PIN 5

// BUZZER
#define BUZZER_PIN 10
int buzzer_time = 0;

// Variables
int lumenValue;
int temperatureValue;
String dateValue;
String timeValue;
String dateString;
char daysOfTheWeek[7][12] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

void setupAndWaitSerial(int speed) {
  delay(2000);
  Serial.begin(speed);
  Serial.println(F("Waiting for Serial..."));
  while (! Serial);
  Serial.println(F("Serial Connected!"));
}

void setup ()
{
  setupAndWaitSerial(9600);
  setupInterrupt();
  play_intro_sound();
  delay(300);
  oledSetup();
  sdSetup();
  rtcSetup();
  pinMode(ABOUT_TO_WATER_PIN, OUTPUT);
  pinMode(PLANT_LIGHT_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  show_splash_screen();
}

void loop () {
  rtcLoop();
  sdLoop();
  lumenLoop();
  oledLoop();
  digitalWrite(WATER_PUMP_PIN, needs_water);
  digitalWrite(PLANT_LIGHT_PIN, lights_on);
}

void play_intro_sound() {
  noTone(BUZZER_PIN);
  tone(BUZZER_PIN, 700);
  delay(300);
  tone(BUZZER_PIN, 500);
  delay(300);
  tone(BUZZER_PIN, 600);
  delay(300);
  noTone(BUZZER_PIN);
}

void show_splash_screen() {
  oled.set2X();
  oled.setCol(40);
  oled.println("Smart");
  oled.setCol(30);
  oled.println("Garden ");
  oled.set1X();
  oled.setCol(105);
  oled.print("v1");
  delay(1000);
  oled.set1X();
  oled.clear();
}

void setupInterrupt() {
  pinMode(8, OUTPUT);
  noInterrupts();           // disable all interrupts
  TCCR1A = 0;
  TCCR1B = 0;

  // Set timer1_counter to the correct value for our interrupt interval
  timer1_counter = 34286; //34286;   // preload timer 65536-16MHz/256/2Hz

  TCNT1 = timer1_counter;   // preload timer
  TCCR1B |= (1 << CS12);    // 256 prescaler
  TIMSK1 |= (1 << TOIE1);   // enable timer overflow interrupt
  interrupts();             // enable all interrupts
}


ISR(TIMER1_OVF_vect) {        // interrupt service routine
  TCNT1 = timer1_counter;   // preload timer
  if (buzzer_time != 0) {
    buzzer_time--;
    tone(BUZZER_PIN, 450);
  } else {
    noTone(BUZZER_PIN);
  }
  moisture_value = analogRead(MOISTURE_PIN);
  // Is moisture resistance above the set level? (too dry)
      // set dry notification led on (blink)
      // Wait a little bit (TICKS_BEFORE_WATER)
      // Unless its been watering for longer than the max limit
      //     - water until its wet (below the set level)
      //     - reset counters and turn dry notification led off
  // Is moisture resistance below level? (wet)
      // reset counters and turn dry notification led off

  if (moisture_value > MOISTURE_RES_LEVEL) {
    ticks_waited++;
    if (!needs_water) {
      digitalWrite(ABOUT_TO_WATER_PIN, ticks_waited % 2);
    }
    if (ticks_waited >= TICKS_BEFORE_WATER) {
      digitalWrite(ABOUT_TO_WATER_PIN, ticks_waited % 2);
      if (needs_water && ticks_waited - TICKS_BEFORE_WATER == WATER_MAX_RUN_TICK_LIMIT ) {
        needs_water = 0;
        ticks_waited = 0;
        digitalWrite(ABOUT_TO_WATER_PIN, 0);
      } else {
        needs_water = 1;
      }
    } else {
      needs_water = 0;
    }
  } else {
      digitalWrite(ABOUT_TO_WATER_PIN, LOW);
      needs_water = 0;
      ticks_waited = 0;
  }
}

void oledSetup () {
  oled.begin(&Adafruit128x64, I2C_ADDRESS);
  oled.setFont(TimesNewRoman13);
  Serial.println("OLED SETUP");
}

void oledLoop () {
  oled.setCursor(0,0);
  oled.print(dateValue);
  oled.print("  ");
  oled.setCol(105);
  oled.print(temperatureValue);
  oled.print(" F");
  oled.println(" ");
  oled.print(timeValue);
  oled.println(" ");
  oled.print("Lumen: ");
  oled.print(lumenValue);
  oled.println(" ");
  oled.print("Water: ");
  oled.print(moisture_value);
}

void sdSetup() {
  if (!SD.begin(SD_CLK_PIN)) {
      oled.println(F("Initialization failed!"));
      oled.println(F("Check to make sure you"));
      oled.println(F("have inserted an SDCard"));
      buzzer_time = 3;
      while (1);
  }
}

void sdLoop() {
  File myFile;
  myFile = SD.open("log.txt", FILE_WRITE);
  if (myFile) {
    myFile.print(dateValue + " " + timeValue);
    myFile.print(",");
    myFile.print(temperatureValue);
    myFile.print(",");
    myFile.print(lumenValue);
    myFile.print(",");
    myFile.print(moisture_value);
    myFile.print(",");
    myFile.print(lights_on);
    myFile.print(",");
    myFile.println(ticks_waited);
    myFile.close();
    oled.setCol(100);
    oled.println(F("    "));
  } else {
    // if the file didn't open, print an error:
    Serial.println("sd error");
    oled.setCol(100);
    oled.println(F("!log"));
  }
}

int getLumens() {
  long int rLi = analogRead(LUX_PIN);
  if (rLi == 0) {
      return 0;
  }
  long int RLDR = (R * (1023 - rLi)) / rLi;
  long int phys = 500L * 1000L / RLDR;
  return phys;
}

void lumenLoop() {
  lumenValue = getLumens();
}

void rtcSetup ()
{
  Wire.begin();
  Rtc.Begin();
  RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
  Serial.println(F("initialization done."));
  if (!Rtc.IsDateTimeValid())
    {
        if (Rtc.LastError() != 0)
        {
            // we have a communications error
            // see https://www.arduino.cc/en/Reference/WireEndTransmission for
            // what the number means
            Serial.print(F("RTC communications error = "));
            Serial.println(Rtc.LastError());
        }
        else
        {
            // Common Causes:
            //    1) first time you ran and the device wasn't running yet
            //    2) the battery on the device is low or even missing

            Serial.println(F("RTC lost confidence in the DateTime!"));

            // following line sets the RTC to the date & time this sketch was compiled
            // it will also reset the valid flag internally unless the Rtc device is
            // having an issue

            Rtc.SetDateTime(compiled);
        }
    }

    if (!Rtc.GetIsRunning())
    {
        Serial.println(F("RTC was not actively running, starting now"));
        Rtc.SetIsRunning(true);
    }

    RtcDateTime now = Rtc.GetDateTime();
    Serial.println("Now");
    char datestring[20];
    char timestring[20];

    snprintf_P(datestring,
      20,
      PSTR("%02u/%02u/%04u"),
      now.Month(),
      now.Day(),
      now.Year()
    );

    snprintf_P(timestring,
      20,
      PSTR("%02u:%02u:%02u"),
      now.Hour(),
      now.Minute(),
      now.Second()
    );

    if (now < compiled)
    {
        Serial.println(F("RTC is older than compile time!  (Updating DateTime)"));
        Rtc.SetDateTime(compiled);
        delay(1000);
        Serial.print(datestring);
        Serial.print(F(" "));
        Serial.println(timestring);
    
        Serial.println(now);
    }
    else if (now > compiled)
    {
        Serial.println(F("RTC is newer than compile time. (this is expected)"));
    }
    else if (now == compiled)
    {
        Serial.println(F("RTC is the same as compile time! (not expected but all is fine)"));
    }

    // never assume the Rtc was last configured by you, so
    // just clear them to your needed state
    Rtc.Enable32kHzPin(false);
    Rtc.SetSquareWavePin(DS3231SquareWavePin_ModeNone);
}

void rtcLoop () {
  //
    RtcDateTime now = Rtc.GetDateTime();
    RtcTemperature temp = Rtc.GetTemperature();
    char datestring[20];
    char timestring[20];

    int hour = now.Hour();
    int minute = now.Minute();

    snprintf_P(datestring,
      20,
      PSTR("%02u/%02u/%04u"),
      now.Month(),
      now.Day(),
      now.Year()
    );

    snprintf_P(timestring,
      20,
      PSTR("%02u:%02u:%02u"),
      hour,
      minute,
      now.Second()
    );

    int hour_mins = 0;
    if (hour > 0) hour_mins = hour * 60;
    bool past_on_time = hour_mins + minute >= LIGHTS_ON_TIME;
    bool before_end_time = hour_mins + minute <= LIGHTS_OFF_TIME;

    if (before_end_time && past_on_time) {
      Serial.println("LIGHTS OFF");
      lights_on = false;
    } else {
      Serial.println("LIGHTS ON");
      lights_on = true;
    }

    dateValue = datestring;
    timeValue = timestring;
    temperatureValue = temp.AsFloatDegF();
}
