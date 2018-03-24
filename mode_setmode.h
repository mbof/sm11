// "Set mode" mode for the car.

#ifndef mode_setmode_h
#define mode_setmode_h

#include "mode.h"

class SetModeMode : public Mode {
  public:
    SetModeMode(ControlContext * ctx);
    // This method will be called on infrared keypress.
    virtual void handle_ir_keypress(unsigned long key_value);
    // This method will be called in the control loop.
    virtual void control();
  private:
    CtrlMode next_mode;
};

#endif
