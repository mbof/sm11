// Track control mode for the car.

#ifndef mode_track_h
#define mode_track_h

#include "mode.h"

class TrackMode : public Mode {
  public:
    TrackMode(ControlContext * ctx);
    // This method will be called on infrared keypress.
    virtual void handle_ir_keypress(unsigned long key_value);
    // This method will be called in the control loop.
    virtual void control();
};

#endif
