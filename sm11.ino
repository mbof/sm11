#include "buzzer.h"
#include "motor.h"
#include "sm11_pinout.h"

// *** Infrared Remote Control ***
#include "IRremote.h"
IRrecv irrecv(PIN_INFRARED_RECV);
// Results of IR decode.
decode_results results;
// Timestamp of last received IR code.
unsigned long ir_code_received_timestamp_ms = 0;
// Duration for which a single IR code is effective.
#define IR_WAIT_MS 120
// IR keys: numeric keypad
#define IR_KEY_1 0x00FD00FF
#define IR_KEY_2 0x00FD807F
#define IR_KEY_3 0x00FD40BF
#define IR_KEY_4 0x00FD20DF
#define IR_KEY_5 0x00FDA05F
#define IR_KEY_6 0x00FD609F
#define IR_KEY_7 0x00FD10EF
#define IR_KEY_8 0x00FD906F
#define IR_KEY_9 0x00FD50AF
#define IR_KEY_0 0x00FDB04F
#define IR_KEY_STAR 0x00FD30CF
#define IR_KEY_HASH 0x00FD708F
// IR keys: directional pad
#define IR_KEY_N 0x00FD8877
#define IR_KEY_W 0x00FD28D7
#define IR_KEY_E 0x00FD6897
#define IR_KEY_S 0x00FD9867
#define IR_KEY_OK 0x00FDA857


// *** Bluetooth control ***
// Incoming byte from Bluetooth.
int incomingByte = 0;
// Buffer for an incoming Bluetooth instruction.
String inputString = "";
// Whether a newline was received last.
boolean newLineReceived = false;
// Whether a packet start character was received last.
boolean startBit  = false; 


// *** High-level control mode ***
typedef enum {
  // Manual control of the car (front / back / left / right).
  CTRL_MODE_INFRARED_MANUAL = 0,
  // In this mode, IR input will set the next mode.
  CTRL_MODE_INFRARED_INPUT_MODE,
  // Dance mode.
  CTRL_MODE_DANCE,
  // Advance until an obstacle is detected with ultrasound, turn, repeat.
  CTRL_MODE_ULTRASOUND_CRAZY,
  // Follow a line on the ground.
  CTRL_MODE_INFRARED_FOLLOW
} CtrlMode;

CtrlMode ctrl_mode = CTRL_MODE_INFRARED_MANUAL;
// In mode input mode, holds the next control mode.
CtrlMode next_ctrl_mode = CTRL_MODE_INFRARED_MANUAL;


// *** A few fun dances ***
unsigned long ctrl_dance_start_ms = 0;
#define CTRL_DANCE_STEP_MS 200
#define CTRL_DANCE_LENGTH 20
const char dances[10][CTRL_DANCE_LENGTH + 1] = {
// 12342234323442341234
  "LRLRLRLRF.B.FFBBFFBB", // 0
  "L.R.L.R.LRFBFFBBL.R.", // 1
  "L.L.L.L.R.R.R.R.F.B.", // 2
  "L.R.L...R.L.R...LRLR", // 3
  "F.B.F...B.F.B...LRLR", // 4
  "FFFF--------........", // 5
  "FFFF................", // 6
  "FFFFFFFF------------", // 7
  "FFFFFFFF............", // 8
  "F...B..RF...BL..RLLR"  // 9
};
const char * ctrl_dance_string;


// *** Ultrasound constants ***
// How long to wait for echo, in microseconds.
#define ULTRASOUND_MAX_ECHO_WAIT_MICROSECONDS 10000
// Centimeters per microsecond of echo delay.
#define ULTRASOUND_CM_PER_MICROSECOND 58
// Distance below which the car will turn away from an obstacle.
#define ULTRASOUND_OBSTACLE_DISTANCE_CM 38
// Timestamp of last encounter with an obstacle in milliseconds.
unsigned long last_obstacle_encounter_timestamp_ms = 0;
// How long to turn away from a detected obstacle (minimum).
#define ULTRASOUND_OBSTACLE_TURN_MINIMUM_MS 400


Motor motor_left(
  PIN_LEFT_MOTOR_ENABLE, PIN_LEFT_MOTOR_GO, PIN_LEFT_MOTOR_BACK);
Motor motor_right(
  PIN_RIGHT_MOTOR_ENABLE, PIN_RIGHT_MOTOR_GO, PIN_RIGHT_MOTOR_BACK);

void setup() {
  Sm11Pinout::setup();
  irrecv.enableIRIn();
}

void dance_control() {
  unsigned long dance_position_ms = millis() - ctrl_dance_start_ms;
  int dance_position_int = dance_position_ms / CTRL_DANCE_STEP_MS;
  if (dance_position_int < 0 || dance_position_int > CTRL_DANCE_LENGTH) {
    motor_left.stop_motor(false); motor_right.stop_motor(false);
    ctrl_mode = CTRL_MODE_INFRARED_MANUAL;
    return;
  }
  switch (ctrl_dance_string[dance_position_int]) {
    case 'F': motor_left.set_power(255); motor_right.set_power(255); break;
    case 'B': motor_left.set_power(-255); motor_right.set_power(-255); break;
    case 'L':  motor_left.set_power(150); motor_right.set_power(-150); break;
    case 'R': motor_left.set_power(-150); motor_right.set_power(150); break;
    case '-': motor_left.stop_motor(true); motor_right.stop_motor(true); break;
    default: motor_left.stop_motor(false); motor_right.stop_motor(false); break;
  }
}

unsigned long measure_ultrasound_distance_cm() {
  // Send an ultrasound impulsion of 10 microseconds.
  digitalWrite(PIN_ULTRASOUND_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(PIN_ULTRASOUND_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_ULTRASOUND_TRIG, LOW);
  // Read echo port high level time (unit:μs). Don't wait too long.
  unsigned long pulse_length = pulseIn(
    PIN_ULTRASOUND_ECHO, HIGH, ULTRASOUND_MAX_ECHO_WAIT_MICROSECONDS);  
  if (pulse_length == 0) {  // Timed out waiting for pulse
    return ULTRASOUND_MAX_ECHO_WAIT_MICROSECONDS / ULTRASOUND_CM_PER_MICROSECOND;
  } else {
    return pulse_length / ULTRASOUND_CM_PER_MICROSECOND;
  }
}

void ultrasound_control(void) {
  // If we've encountered an obstacle not too long ago, keep turning.
  if (last_obstacle_encounter_timestamp_ms > 0) {
    unsigned long time_since_last_obstacle_encounter_ms = (
      millis() - last_obstacle_encounter_timestamp_ms);
    if (time_since_last_obstacle_encounter_ms < ULTRASOUND_OBSTACLE_TURN_MINIMUM_MS) {
      return;
    }
  }

  // Let's check to see if we're OK to proceed.
  unsigned long distance_cm = measure_ultrasound_distance_cm();
  if (distance_cm > ULTRASOUND_OBSTACLE_DISTANCE_CM) {
    // No obstacle: advance... slowly.
    motor_left.set_power(100); motor_right.set_power(100);
  } else {
    last_obstacle_encounter_timestamp_ms = millis();
    // Obstacle: start turning.
    motor_left.set_power(150); motor_right.set_power(-150);
    Buzzer::start(100);
  }
}

void start_dance(int dance_number) {
  ctrl_mode = CTRL_MODE_DANCE;
  ctrl_dance_string = dances[dance_number];
  ctrl_dance_start_ms = millis();
}

void maybe_process_ir_command()
{
  if (!irrecv.decode(&results)) {
    return;
  }
  if (ctrl_mode == CTRL_MODE_INFRARED_INPUT_MODE) {
    switch (results.value) {
      case IR_KEY_0: next_ctrl_mode = CTRL_MODE_INFRARED_MANUAL; Buzzer::start(100); break;
      case IR_KEY_1: next_ctrl_mode = CTRL_MODE_ULTRASOUND_CRAZY; Buzzer::start(100); break;
      case IR_KEY_2: next_ctrl_mode = CTRL_MODE_INFRARED_FOLLOW; Buzzer::start(100); break;
      case IR_KEY_HASH: ctrl_mode = next_ctrl_mode; Buzzer::start(500); break;
    }
  } else {
    CtrlMode old_ctrl_mode = ctrl_mode;
    ctrl_mode = CTRL_MODE_INFRARED_MANUAL;
    ir_code_received_timestamp_ms = millis();
    switch (results.value) {
      case IR_KEY_N: motor_left.set_power(255); motor_right.set_power(255); break;
      case IR_KEY_S: motor_left.set_power(-255); motor_right.set_power(-255); break;
      case IR_KEY_E: motor_left.set_power(150); motor_right.set_power(-150); break;
      case IR_KEY_W: motor_left.set_power(-150); motor_right.set_power(150); break;
      case IR_KEY_0: start_dance(0); break;
      case IR_KEY_1: start_dance(1); break;
      case IR_KEY_2: start_dance(2); break;
      case IR_KEY_3: start_dance(3); break;
      case IR_KEY_4: start_dance(4); break;
      case IR_KEY_5: start_dance(5); break;
      case IR_KEY_6: start_dance(6); break;
      case IR_KEY_7: start_dance(7); break;
      case IR_KEY_8: start_dance(8); break;
      case IR_KEY_9: start_dance(9); break;
      case IR_KEY_STAR: ctrl_mode = CTRL_MODE_INFRARED_INPUT_MODE; Buzzer::start(100); break;
      default: ctrl_mode = old_ctrl_mode;
    }
  }
  irrecv.resume(); // received next Infrared decode
}

/*main loop*/
void loop()
{
  maybe_process_ir_command();

  // High-level control.
  switch (ctrl_mode) {
    case CTRL_MODE_INFRARED_MANUAL:
      if (millis() - ir_code_received_timestamp_ms > IR_WAIT_MS) {
        motor_left.stop_motor(false);
        motor_right.stop_motor(false);
      };
      break;
    case CTRL_MODE_DANCE:
      dance_control();
      break;
    case CTRL_MODE_ULTRASOUND_CRAZY:
      ultrasound_control();
      break;
    case CTRL_MODE_INFRARED_FOLLOW:
      // Unimplemented.
      ctrl_mode = CTRL_MODE_INFRARED_MANUAL;
      break;
  }

  // Low-level control.
  motor_left.control();
  motor_right.control();
  Buzzer::control();
}

//Serial read data
void serialEvent()
{
  while (Serial.available())
  {
    incomingByte = Serial.read();   //One byte by one byte reads 
    if (incomingByte == '$')  // '$' means the start of packet
    {
      startBit = true;
    }
    if (startBit == true)
    {
      inputString += (char) incomingByte;     // The received data constitutes a completed packet.
    }
    if (incomingByte == '#')    // '#' means the end of packet
    {
      newLineReceived = true;
      startBit = false;
      Serial.println(inputString);
    }
  }
}
