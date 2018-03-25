// Dance control mode for the car.

#include <Arduino.h>
#include "ir_keys.h"
#include "mode.h"
#include "mode_dance.h"


#define CTRL_DANCE_STEP_MS 200
#define CTRL_DANCE_LENGTH 20
#define CTRL_DANCE_COUNT 10

const char dances[CTRL_DANCE_COUNT][CTRL_DANCE_LENGTH + 1] = {
  "LRLRLRLRF.B.FFBBFFBB", // 0
  "L.R.L.R.LRFBFFBBL.R.", // 1
  "L.L.L.L.R.R.R.R.F.B.", // 2
  "L.R.L...R.L.R...LRLR", // 3
  "F.B.F...B.F.B...LRLR", // 4
  "FFFF--------........", // 5
  "FFFF................", // 6
  "FFFFFFFF------------", // 7
  "FFFFFFFF............", // 8
  "F...B..RF...BL..RLLR"  // 9
};


DanceMode::DanceMode(ControlContext * ctx) {
  this->ctx = ctx;
  this->dance_start_ms = 0;
  this->current_dance = dances[0];
  ctx->register_mode(CTRL_MODE_DANCE, this);
}

void DanceMode::handle_ir_keypress(unsigned long key_value) {
  switch (key_value) {
    case IR_KEY_0: this->start_dance(0); break;
    case IR_KEY_1: this->start_dance(1); break;
    case IR_KEY_2: this->start_dance(2); break;
    case IR_KEY_3: this->start_dance(3); break;
    case IR_KEY_4: this->start_dance(4); break;
    case IR_KEY_5: this->start_dance(5); break;
    case IR_KEY_6: this->start_dance(6); break;
    case IR_KEY_7: this->start_dance(7); break;
    case IR_KEY_8: this->start_dance(8); break;
    case IR_KEY_9: this->start_dance(9); break;
    default:
      this->ctx->maybe_yield_to_manual(key_value);
      return;
  }
}

void DanceMode::start_dance(int dance_number) {
  if (dance_number < 0) {
    dance_number = 0;
  }
  if (dance_number > CTRL_DANCE_COUNT - 1) {
    dance_number = CTRL_DANCE_COUNT - 1;
  }
  this->current_dance = dances[dance_number];
  this->dance_start_ms = millis();
}

void DanceMode::control() {
  unsigned long dance_position_ms = millis() - this->dance_start_ms;
  int dance_position_int = dance_position_ms / CTRL_DANCE_STEP_MS;
  if (dance_position_int < 0 || dance_position_int > CTRL_DANCE_LENGTH) {
    this->ctx->stop_left(); this->ctx->stop_right();
    this->ctx->set_mode(CTRL_MODE_INFRARED_MANUAL);
    return;
  }
  switch (this->current_dance[dance_position_int]) {
    case 'F': this->ctx->set_power(255, 255); break;
    case 'B': this->ctx->set_power(-255, -255); break;
    case 'L': this->ctx->set_power(150, -150); break;
    case 'R': this->ctx->set_power(-150, 150); break;
    case '-': this->ctx->set_power(0, 0); break;
    default: this->ctx->stop(); break;
  }
}

