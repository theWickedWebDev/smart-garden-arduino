#include "globals.h"
#include "Arduino.h"

#define R 1000
#define VIN 5

int get_lumens() {
  int rLi = analogRead(LUMEN_PIN); 
  float Vo = float(rLi) * (VIN / float(1023));
  float RLDR = (R * (VIN - Vo))/Vo;
  int phys=500/(RLDR/1000);
  return phys;
}

int get_moisture() {
  return analogRead(MOISTURE_PIN);
}
