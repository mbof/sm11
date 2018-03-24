// Track control mode for the car.

#include <Arduino.h>
#include "mode.h"
#include "mode_track.h"
#include "sm11_pinout.h"


TrackMode::TrackMode(ControlContext * ctx) {
  this->ctx = ctx;
  ctx->register_mode(CTRL_MODE_TRACK_LINE, this);
}

void TrackMode::handle_ir_keypress(unsigned long key_value) {
  this->ctx->set_mode(CTRL_MODE_INFRARED_MANUAL);
  this->ctx->mode->handle_ir_keypress(key_value);
}

void TrackMode::control() {

  // LOW means white undersurface, HIGH means black undersurface.

  bool is_white_left = digitalRead(PIN_LINE_FOLLOW_LEFT) == LOW;
  bool is_white_right = digitalRead(PIN_LINE_FOLLOW_RIGHT) == LOW;

  if (is_white_left && is_white_right) {
    this->ctx->set_power(100, 100);
  } else if (is_white_left && !is_white_right) {
    this->ctx->set_power(150, 50);
  } else if (!is_white_left && is_white_right) {
    this->ctx->set_power(50, 150);
  } else {
    this->ctx->stop();
  }
}

