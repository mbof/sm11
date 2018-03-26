// Mode to follow an object using front-facing infrared sensors.

#ifndef mode_follow_h
#define mode_follow_h

#include "mode.h"

class FollowMode : public Mode {
  public:
    FollowMode(ControlContext * ctx);
    // This method will be called on infrared keypress.
    virtual void handle_ir_keypress(unsigned long key_value);
    // This method will be called in the control loop.
    virtual void control();
};

#endif
