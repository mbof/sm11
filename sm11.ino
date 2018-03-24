#include "buzzer.h"
#include "ir_keys.h"
#include "mode.h"
#include "mode_dance.h"
#include "mode_manual.h"
#include "mode_setmode.h"
#include "mode_track.h"
#include "mode_ultrasound.h"
#include "motor.h"
#include "sm11_pinout.h"
#include "IRremote.h"


// IR Receiver controller.
IRrecv irrecv(PIN_INFRARED_RECV);
// Results of IR decode.
decode_results results;


// Motor low-level controllers.
Motor motor_left(PIN_LEFT_MOTOR_ENABLE, PIN_LEFT_MOTOR_GO, PIN_LEFT_MOTOR_BACK);
Motor motor_right(PIN_RIGHT_MOTOR_ENABLE, PIN_RIGHT_MOTOR_GO, PIN_RIGHT_MOTOR_BACK);


// Control context.
ControlContext ctx(&motor_left, &motor_right);


// High-level control modes.
ManualMode manual_mode(&ctx);
SetModeMode set_mode_mode(&ctx);
DanceMode dance_mode(&ctx);
UltrasoundMode ultrasound_mode(&ctx);
TrackMode track_mode(&ctx);


// Setup.
void setup() {
  Sm11Pinout::setup();
  irrecv.enableIRIn();
  ctx.set_mode(CTRL_MODE_INFRARED_MANUAL);
}

// Routine that sees if infrared input was received; and if so, processes it.
void maybe_process_ir_command() {
  // Check if we've received an infrared code.
  if (!irrecv.decode(&results)) {
    return;
  }
  // In all modes, the star key enters the "set mode" mode.
  if (results.value == IR_KEY_STAR) {
    ctx.set_mode(CTRL_MODE_SET_MODE);
    Buzzer::start(100);
  } else {
    // Defer to key-handling defined by current mode.
    ctx.mode->handle_ir_keypress(results.value);
  }
  // Set up reception of the next infrared code.
  irrecv.resume();
}

// Main loop.
void loop() {
  maybe_process_ir_command();

  // High-level control.
  ctx.mode->control();

  // Low-level control.
  ctx.motor_left->control();
  ctx.motor_right->control();
  Buzzer::control();
}
