#include "Arduino.h"
#include "globals.h"
#include <SPI.h>
#include <SD.h>

#define countof(a) (sizeof(a) / sizeof(a[0]))

File myFile;

void sd_setup() {
  if (USE_SERIAL) {
    Serial.println(F("-------SD CARD-------"));
    Serial.println(F("Initializing..."));
  }

  if (!SD.begin(SD_CLK_PIN)) {
    if (USE_SERIAL) { Serial.println(F("✖ Init failed!")); }
    has_error = true;
    error_msg = "MicroSD Init failed!";
//    while (1);
  }
  if (USE_SERIAL) { Serial.println(F("✔ SD Card Success!")); }
}

void sd_display_read_all_on_serial() {
  if (USE_SERIAL) {
    File myFile;
    myFile = SD.open(SD_LOG_FILENAME);

    if (myFile) {
      if (USE_SERIAL) {
        Serial.println(SD_LOG_FILENAME);
      }
      // read from the file until there's nothing else in it:
      while (myFile.available()) {
        Serial.write(myFile.read());
      }
      // close the file:
      myFile.close();
    } else {
      // if the file didn't open, print an error:
      if (USE_SERIAL) {
        Serial.print("✖ Error opening: ");
        Serial.println(SD_LOG_FILENAME);
      }
      has_error = true;
      error_msg = "Error opening: " + String(SD_LOG_FILENAME);
    }
  }
}

void log_stats() {
  File myFile;
  myFile = SD.open(SD_LOG_FILENAME, FILE_WRITE);

  if (myFile) {
    myFile.print(time_right_now.Month());
    myFile.print("/");
    myFile.print(time_right_now.Day());
    myFile.print("/");
    myFile.print(time_right_now.Year());
    myFile.print(",");
    myFile.print(time_right_now.Hour());
    myFile.print(":");
    myFile.print(time_right_now.Minute());
    myFile.print(":");
    myFile.print(time_right_now.Second());
    myFile.print(",");
    myFile.print(humidity);
    myFile.print(",");
    myFile.print(moisture);
    myFile.print(",");
    myFile.print(lumen);
    myFile.print(",");
    myFile.println(temperature);
    // close the file:
    myFile.close();
  } else {
    // if the file didn't open, print an error:
    if (USE_SERIAL) {
      Serial.print(F("✖ Failed to Write to SD"));
    }
    has_error = true;
    error_msg = "Failed to Write to SD";
    sd_setup();
  }

}
