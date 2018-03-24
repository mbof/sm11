// Manual control mode for the car.

#ifndef mode_manual_h
#define mode_manual_h

#include "mode.h"

class ManualMode : public Mode {
  public:
    ManualMode(ControlContext * ctx);
    // This method will be called on infrared keypress.
    virtual void handle_ir_keypress(unsigned long key_value);
    // This method will be called in the control loop.
    virtual void control();
  private:
    unsigned long last_keypress_timestamp_ms;
};

#endif
