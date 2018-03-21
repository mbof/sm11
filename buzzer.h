// Buzzer control.

#ifndef buzzer_h
#define buzzer_h

class Buzzer {
  public:
    // Start a beep of a certain duration.
    static void start(unsigned long duration_ms);
    // Call this in the control loop.
    static void control();
  private:
    static unsigned long beep_end_timestamp_ms;
};

#endif
