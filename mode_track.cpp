// Track control mode for the car.

#include <Arduino.h>
#include "mode.h"
#include "mode_track.h"
#include "sm11_pinout.h"


#define MIN_TURN_MS 200

TrackMode::TrackMode(ControlContext * ctx) {
  this->ctx = ctx;
  this->last_turn_start_ms = 0;
  ctx->register_mode(CTRL_MODE_TRACK_LINE, this);
}

void TrackMode::handle_ir_keypress(unsigned long key_value) {
  this->ctx->maybe_yield_to_manual(key_value);
}

void TrackMode::control() {
  if (millis() - this->last_turn_start_ms < MIN_TURN_MS) {
    // Keep turning.
    return;
  }

  // LOW means white undersurface, HIGH means black undersurface.

  bool is_white_left = digitalRead(PIN_LINE_FOLLOW_LEFT) == LOW;
  bool is_white_right = digitalRead(PIN_LINE_FOLLOW_RIGHT) == LOW;

  if (is_white_left && is_white_right) {
    this->ctx->set_power(60, 60);
  } else if (is_white_left && !is_white_right) {
    this->last_turn_start_ms = millis();
    this->ctx->set_power(120, -120);
  } else if (!is_white_left && is_white_right) {
    this->last_turn_start_ms = millis();
    this->ctx->set_power(-120, 120);
  } else {
    this->ctx->stop();
  }
}

