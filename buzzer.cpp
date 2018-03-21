// Buzzer control.

#include <Arduino.h>
#include "buzzer.h"
#include "sm11_pinout.h"

unsigned long Buzzer::beep_end_timestamp_ms = 0;

void Buzzer::control() {
  if (Buzzer::beep_end_timestamp_ms > millis()) {
    digitalWrite(PIN_BUZZER, HIGH);
  } else {
    digitalWrite(PIN_BUZZER, LOW);
  }
}

void Buzzer::start(unsigned long duration_ms) {
  Buzzer::beep_end_timestamp_ms = millis() + duration_ms;
}

