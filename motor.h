// Low-level motor control.

#ifndef motor_h
#define motor_h

class Motor {
  public:
    // Initialize with the Enable, Go and Back pins.
    Motor(int enable_pin, int go_pin, int back_pin);
    // Set the speed of the motor between -255 and 255.
    void set_power(int power);
    // Stop the motor, maybe freewheeling.
    void stop_motor(bool freewheel);
    // Call this in the control loop.
    void control();
  private:
    int enable_pin;
    int go_pin;
    int back_pin;
    int power;
    bool freewheel;
};

#endif
