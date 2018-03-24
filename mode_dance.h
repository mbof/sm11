// Dance control mode for the car.

#ifndef mode_dance_h
#define mode_dance_h

#include "mode.h"

class DanceMode : public Mode {
  public:
    DanceMode(ControlContext * ctx);
    // This method will be called on infrared keypress.
    virtual void handle_ir_keypress(unsigned long key_value);
    // This method will be called in the control loop.
    virtual void control();
    // Start a dance.
    void start_dance(int dance_number);
  private:
    const char * current_dance;
    unsigned long dance_start_ms;
};

#endif
