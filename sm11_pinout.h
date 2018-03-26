// Pinout setup for the SM11.

#ifndef sm11_pinout_h
#define sm11_pinout_h

#include <Arduino.h>

#define PIN_INFRARED_RECV 2
#define PIN_LEFT_LED 3
#define PIN_RIGHT_LED 4
#define PIN_RIGHT_MOTOR_ENABLE 5
#define PIN_RIGHT_MOTOR_GO 6
#define PIN_RIGHT_MOTOR_BACK 7   
#define PIN_LEFT_MOTOR_GO 8
#define PIN_LEFT_MOTOR_BACK 9
#define PIN_LEFT_MOTOR_ENABLE 10
#define PIN_BUZZER 12   
#define PIN_KEY 13
#define PIN_ULTRASOUND_TRIG A0
#define PIN_ULTRASOUND_ECHO A1
#define PIN_LINE_FOLLOW_RIGHT A2
#define PIN_LINE_FOLLOW_LEFT A3
#define PIN_PROXIMITY_RIGHT A4
#define PIN_PROXIMITY_LEFT A5

class Sm11Pinout {
  public:
    static void setup();
};

#endif
