// Abstract base class for modes of the car.

#include "ir_keys.h"
#include "mode.h"
#include "motor.h"


ControlContext::ControlContext(Motor * motor_left, Motor * motor_right) {
  this->motor_left = motor_left;
  this->motor_right = motor_right;
}

void ControlContext::set_power(int left_power, int right_power) {
  this->set_power_left(left_power);
  this->set_power_right(right_power);
}

void ControlContext::stop() {
  this->stop_left();
  this->stop_right();
}

void ControlContext::set_power_right(int power) {
  this->motor_right->set_power(power);
}

void ControlContext::set_power_left(int power) {
  this->motor_left->set_power(power);
}

void ControlContext::stop_left() {
  this->motor_left->stop_motor();
}

void ControlContext::stop_right() {
  this->motor_right->stop_motor();
}

void ControlContext::set_mode(CtrlMode ctrl_mode) {
  this->mode = this->mode_registry[ctrl_mode];
  this->stop_left(); this->stop_right();
}

void ControlContext::register_mode(CtrlMode ctrl_mode, Mode * mode) {
  this->mode_registry[ctrl_mode] = mode;
}

void ControlContext::maybe_yield_to_manual(unsigned long key_value) {
  switch (key_value) {
    case IR_KEY_N:
    case IR_KEY_S:
    case IR_KEY_E:
    case IR_KEY_W:
      this->set_mode(CTRL_MODE_INFRARED_MANUAL);
      this->mode->handle_ir_keypress(key_value);
      return;
  }
}

