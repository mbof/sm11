// Manual control mode for the car.

#include <Arduino.h>
#include "ir_keys.h"
#include "mode.h"
#include "mode_manual.h"


// Duration for which a single IR code is effective.
#define IR_WAIT_MS 120


ManualMode::ManualMode(ControlContext * ctx) {
  this->ctx = ctx;
  this->last_keypress_timestamp_ms = 0;
  ctx->register_mode(CTRL_MODE_INFRARED_MANUAL, this);
}

void ManualMode::handle_ir_keypress(unsigned long key_value) {
  this->last_keypress_timestamp_ms = millis();
  switch (key_value) {
    case IR_KEY_N: this->ctx->set_power(255, 255); break;
    case IR_KEY_S: this->ctx->set_power(-255, -255); break;
    case IR_KEY_E: this->ctx->set_power(150, -150); break;
    case IR_KEY_W: this->ctx->set_power(-150, 150); break;
    case IR_KEY_0:
    case IR_KEY_1:
    case IR_KEY_2:
    case IR_KEY_3:
    case IR_KEY_4:
    case IR_KEY_5:
    case IR_KEY_6:
    case IR_KEY_7:
    case IR_KEY_8:
    case IR_KEY_9:
      this->ctx->set_mode(CTRL_MODE_DANCE);
      this->ctx->mode->handle_ir_keypress(key_value);
      break;
  }
}

void ManualMode::control() {
  if (millis() - last_keypress_timestamp_ms > IR_WAIT_MS) {
    this->ctx->stop();
  };
}

