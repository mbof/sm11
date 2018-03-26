// Mode to follow an object using front-facing infrared sensors.

#include "mode_follow.h"
#include "sm11_pinout.h"


FollowMode::FollowMode(ControlContext * ctx) {
  this->ctx = ctx;
  ctx->register_mode(CTRL_MODE_INFRARED_FOLLOW, this);
}

void FollowMode::handle_ir_keypress(unsigned long key_value) {
  this->ctx->maybe_yield_to_manual(key_value);
}

void FollowMode::control() {
  // LOW means there is something close to the sensor.
  bool proximity_left = (digitalRead(PIN_PROXIMITY_LEFT) == LOW);
  bool proximity_right = (digitalRead(PIN_PROXIMITY_RIGHT) == LOW);

  if (!proximity_left && !proximity_right) {
    this->ctx->set_power(150, 150);
    return;
  }
  if (!proximity_left && proximity_right) {
    this->ctx->set_power(150, -150);
    return;
  }
  if (proximity_left && !proximity_right) {
    this->ctx->set_power(-150, 150);
    return;
  }
  this->ctx->stop();
}

