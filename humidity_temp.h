#include <Adafruit_AHT10.h>

Adafruit_AHT10 aht;
Adafruit_Sensor *aht_humidity, *aht_temp;
sensors_event_t _humidity;
sensors_event_t temp;

void setup_humidity_temp() {
  if (USE_SERIAL) {
    Serial.println(F("------- TEMP / HUMIDITY -------"));
    Serial.println(F("Adafruit AHT10 test!"));
  }
  if (!aht.begin()) {
    if (USE_SERIAL) { Serial.println(F("✖ Failed to find AHT10 chip")); }
    has_error = true;
    error_msg = "Failed to find AHT10 chip";
    while (1) { delay(10); }
  }
  if (USE_SERIAL) { Serial.println(F("✔ AHT10 Found!")); }

  aht_temp = aht.getTemperatureSensor();
  aht_temp->printSensorDetails();

  aht_humidity = aht.getHumiditySensor();
  aht_humidity->printSensorDetails();
}

void check_humidity_temp() {
  aht_humidity->getEvent(&_humidity);
  aht_temp->getEvent(&temp);
  temperature = temp.temperature;
  humidity = _humidity.relative_humidity;
}
