// Abstract base class for modes of the car.

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
  this->motor_left->stop_motor(false);
}

void ControlContext::stop_right() {
  this->motor_right->stop_motor(false);
}

void ControlContext::set_mode(CtrlMode ctrl_mode) {
  this->mode = this->mode_registry[ctrl_mode];
  this->stop_left(); this->stop_right();
}

void ControlContext::register_mode(CtrlMode ctrl_mode, Mode * mode) {
  this->mode_registry[ctrl_mode] = mode;
}

