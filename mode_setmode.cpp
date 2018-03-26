// "Set mode" mode for the car.

#include <Arduino.h>
#include "buzzer.h"
#include "ir_keys.h"
#include "mode.h"
#include "mode_setmode.h"


SetModeMode::SetModeMode(ControlContext * ctx) {
  this->ctx = ctx;
  this->next_mode = CTRL_MODE_INFRARED_MANUAL;
  ctx->register_mode(CTRL_MODE_SET_MODE, this);
}

void SetModeMode::handle_ir_keypress(unsigned long key_value) {
  switch (key_value) {
    case IR_KEY_1: this->next_mode = CTRL_MODE_INFRARED_MANUAL; break;
    case IR_KEY_2: this->next_mode = CTRL_MODE_DANCE; break;
    case IR_KEY_3: this->next_mode = CTRL_MODE_ULTRASOUND_CRAZY; break;
    case IR_KEY_4: this->next_mode = CTRL_MODE_TRACK_LINE; break;
    case IR_KEY_5: this->next_mode = CTRL_MODE_INFRARED_FOLLOW; break;
    case IR_KEY_HASH: this->ctx->set_mode(this->next_mode); break;
    default: return;
  }
  Buzzer::start(100);
}

void SetModeMode::control() {
  // Nothing.
}

