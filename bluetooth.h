#include "Arduino.h"
#include "globals.h"
#include <SoftwareSerial.h>

SoftwareSerial BTHC08Serial(9, 10);

void bt_setup() {
  BTHC08Serial.begin(9600);
  
  Serial.println(F("------- BLUETOOTH -------"));
  Serial.println(F("Waiting for Bluetooth..."));
  while (! BTHC08Serial);
  Serial.println(F("✔ Bluetooth Connected!"));
}


void bt_loop() {
   
  
  if (BTHC08Serial.available()) {
    while (BTHC08Serial.available() > 0) {
      char inByte = BTHC08Serial.read();
      Serial.write(inByte);
    }    
  } else if (Serial.available()) {
    BTHC08Serial.println(Serial.readString()); 
  }
}
