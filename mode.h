// Abstract base class for modes of the car.

#ifndef mode_h
#define mode_h

#include "motor.h"


// High-level control mode.
typedef enum {
  // Manual control of the car (front / back / left / right).
  CTRL_MODE_INFRARED_MANUAL = 0,
  // In this mode, IR input will set the next mode.
  CTRL_MODE_SET_MODE,
  // Dance mode.
  CTRL_MODE_DANCE,
  // Advance until an obstacle is detected with ultrasound, turn, repeat.
  CTRL_MODE_ULTRASOUND_CRAZY,
  // Follow a line on the ground.
  CTRL_MODE_TRACK_LINE,
  // Last value (used to define the length of an array indexed by modes)
  CTRL_MODE_LAST_VALUE
} CtrlMode;


class ControlContext;

class Mode {
  public:
    // This method will be called on infrared keypress.
    virtual void handle_ir_keypress(unsigned long key_value) = 0;
    // This method will be called in the control loop.
    virtual void control() = 0;
  protected:
    ControlContext * ctx;
};

class ControlContext {
  public:
    Motor * motor_left;
    Motor * motor_right;
    Mode * mode;
    // Constructor;
    ControlContext(Motor * motor_left, Motor * motor_right);
    // Set power of both motors.
    void set_power(int power_left, int power_right);
    // Stop both motors.
    void stop();
    // Set left motor power.
    void set_power_left(int power);
    // Set right motor power.
    void set_power_right(int power);
    // Stop left.
    void stop_left();
    // Stop right.
    void stop_right();
    // Register modes.
    void register_mode(CtrlMode ctrl_mode, Mode * mode);
    // Set next control mode.
    void set_mode(CtrlMode ctrl_mode);
  private:
    Mode * mode_registry[CTRL_MODE_LAST_VALUE];
};

#endif
