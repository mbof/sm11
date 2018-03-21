// Initialize pins etc. on startup.

#include <Arduino.h>
#include "sm11_pinout.h"

int serial_putc(char c, struct __file *) {
  Serial.write(c);
  return c;
}

void Sm11Pinout::setup() {
  pinMode(PIN_LEFT_MOTOR_GO, OUTPUT);
  pinMode(PIN_LEFT_MOTOR_BACK, OUTPUT);
  pinMode(PIN_LEFT_MOTOR_ENABLE, OUTPUT);
  pinMode(PIN_RIGHT_MOTOR_GO, OUTPUT);
  pinMode(PIN_RIGHT_MOTOR_BACK, OUTPUT);
  pinMode(PIN_RIGHT_MOTOR_ENABLE, OUTPUT);
  pinMode(PIN_BUZZER, OUTPUT);
  pinMode(PIN_LEFT_LED, OUTPUT);
  pinMode(PIN_RIGHT_LED, OUTPUT);
  pinMode(PIN_KEY, INPUT_PULLUP);
  pinMode(PIN_ULTRASOUND_ECHO, INPUT);
  pinMode(PIN_ULTRASOUND_TRIG, OUTPUT);
  pinMode(PIN_LINE_FOLLOW_RIGHT, INPUT);
  pinMode(PIN_LINE_FOLLOW_LEFT, INPUT);
  pinMode(PIN_PROXIMITY_RIGHT, INPUT);
  pinMode(PIN_PROXIMITY_LEFT, INPUT);
  pinMode(PIN_INFRARED_RECV, INPUT_PULLUP);

  digitalWrite(PIN_BUZZER, LOW);
  digitalWrite(PIN_LEFT_LED, HIGH);
  digitalWrite(PIN_RIGHT_LED, HIGH);

  Serial.begin(9600);  // Set Bluetooth baud rate 9600
  fdevopen(&serial_putc, 0);
}


