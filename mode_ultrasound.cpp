// Manual control mode for the car.

#include <Arduino.h>
#include "buzzer.h"
#include "mode.h"
#include "mode_ultrasound.h"
#include "sm11_pinout.h"


// How long to wait for echo, in microseconds.
#define ULTRASOUND_MAX_ECHO_WAIT_MICROSECONDS 10000
// Centimeters per microsecond of echo delay.
#define ULTRASOUND_CM_PER_MICROSECOND 58
// Distance below which the car will turn away from an obstacle.
#define ULTRASOUND_OBSTACLE_DISTANCE_CM 38
// How long to turn away from a detected obstacle (minimum).
#define ULTRASOUND_OBSTACLE_TURN_MINIMUM_MS 400


UltrasoundMode::UltrasoundMode(ControlContext * ctx) {
  this->ctx = ctx;
  this->last_obstacle_encounter_timestamp_ms = 0;
  ctx->register_mode(CTRL_MODE_ULTRASOUND_CRAZY, this);
}

void UltrasoundMode::handle_ir_keypress(unsigned long key_value) {
  this->ctx->maybe_yield_to_manual(key_value);
}

unsigned long measure_ultrasound_distance_cm() {
  // Send an ultrasound impulsion of 10 microseconds.
  digitalWrite(PIN_ULTRASOUND_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(PIN_ULTRASOUND_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_ULTRASOUND_TRIG, LOW);
  // Read echo port high level time (unit:μs). Don't wait too long.
  unsigned long pulse_length = pulseIn(
    PIN_ULTRASOUND_ECHO, HIGH, ULTRASOUND_MAX_ECHO_WAIT_MICROSECONDS);
  if (pulse_length == 0) {  // Timed out waiting for pulse
    return ULTRASOUND_MAX_ECHO_WAIT_MICROSECONDS / ULTRASOUND_CM_PER_MICROSECOND;
  } else {
    return pulse_length / ULTRASOUND_CM_PER_MICROSECOND;
  }
}

void UltrasoundMode::control() {
  // If we've encountered an obstacle not too long ago, keep turning.
  if (this->last_obstacle_encounter_timestamp_ms > 0) {
    unsigned long time_since_last_obstacle_encounter_ms = (
      millis() - this->last_obstacle_encounter_timestamp_ms);
    if (time_since_last_obstacle_encounter_ms < ULTRASOUND_OBSTACLE_TURN_MINIMUM_MS) {
      return;
    }
  }

  // Let's check to see if we're OK to proceed.
  unsigned long distance_cm = measure_ultrasound_distance_cm();
  if (distance_cm > ULTRASOUND_OBSTACLE_DISTANCE_CM) {
    // No obstacle: advance... slowly.
    this->ctx->set_power(100, 100);
  } else {
    last_obstacle_encounter_timestamp_ms = millis();
    // Obstacle: start turning.
    this->ctx->set_power(150, -150);
    Buzzer::start(100);
  }
}

