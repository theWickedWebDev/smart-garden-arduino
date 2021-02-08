#include "globals.h"
#include "Arduino.h"
#include <RtcDS3231.h>
#include <RtcUtility.h>
#include <Wire.h>
RtcDS3231<TwoWire> Rtc(Wire);
RtcDateTime time_right_now;

void rtc_setup () {
  Wire.begin();
  Rtc.Begin();
  RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);

  if (USE_SERIAL) {
     Serial.println(F("✔ RTC - initialization done."));
  }

  if (!Rtc.IsDateTimeValid()) {
    if (Rtc.LastError() != 0) {
        if (USE_SERIAL) {
          Serial.print(F("✖ RTC communications error = "));
          Serial.println(Rtc.LastError());
        }
      has_error = true;
      error_msg = "RTC communications error = " + Rtc.LastError();
    } else {
        if (USE_SERIAL) {
          Serial.println(F("RTC lost confidence in the DateTime!"));
        }

        Rtc.SetDateTime(compiled);
    }
  }

  if (!Rtc.GetIsRunning()) {
    if (USE_SERIAL) {
      Serial.println(F("RTC was not actively running, starting now"));
    }
    Rtc.SetIsRunning(true);
  }

  RtcDateTime now = Rtc.GetDateTime();

  // Never assume the RTC was last configured by you, so just clear them to your needed state
  Rtc.Enable32kHzPin(false);
  Rtc.SetSquareWavePin(DS3231SquareWavePin_ModeNone);
}
