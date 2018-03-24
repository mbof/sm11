// Ultrasound control mode for the car.

#ifndef mode_ultrasound_h
#define mode_ultrasound_h

#include "mode.h"

class UltrasoundMode : public Mode {
  public:
    UltrasoundMode(ControlContext * ctx);
    // This method will be called on infrared keypress.
    virtual void handle_ir_keypress(unsigned long key_value);
    // This method will be called in the control loop.
    virtual void control();
  private:
    // Timestamp of last encounter with an obstacle in milliseconds.
    unsigned long last_obstacle_encounter_timestamp_ms = 0;
};

#endif
