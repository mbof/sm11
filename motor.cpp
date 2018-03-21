// Low-level motor control.

#include <Arduino.h>
#include "motor.h"

Motor::Motor(int enable_pin, int go_pin, int back_pin) {
  this->enable_pin = enable_pin;
  this->go_pin = go_pin;
  this->back_pin = back_pin;
  this->power = 0;
  this->freewheel = false;
}

void Motor::set_power(int power) {
  if (power > 255) {
    power = 255;
  }
  if (power < -255) {
    power = -255;
  }
  this->power = power;
}

void Motor::stop_motor(bool freewheel) {
  this->freewheel = freewheel;
  this->power = 0;
}

void Motor::control() {
  if (this->power == 0 && !freewheel) {
    digitalWrite(this->enable_pin, HIGH);
    digitalWrite(this->go_pin, LOW);
    digitalWrite(this->back_pin, LOW);
    return;
  }
  if (this->power > 0) {
    analogWrite(enable_pin, this->power);
    digitalWrite(this->go_pin, HIGH);
    digitalWrite(this->back_pin, LOW);
  } else {
    analogWrite(enable_pin, -this->power);
    digitalWrite(this->back_pin, HIGH);
    digitalWrite(this->go_pin, LOW);
  }
}

