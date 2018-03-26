## Introduction

This is a control sketch for the [Kuman SM11 Arduino UNO car kit](http://www.kumantech.com/kuman-arduino-uno-bluetooth-irafred-remote-control-smart-robot-car-kit-sm11_p0415.html),
using event-based programming to ensure responsiveness, and modularizing the
code to increase readability and make it easier to create new control modes.

It does not fully implement the original software features. Currently
implemented are the following:

* Manual infrared control

* Ultrasound obstacle-avoiding mode

* Line-following mode

* (New) Pre-programmed "dances"


Currently unimplemented:

* Infrared proximity sensor obstacle-avoiding mode and follow mode

* Bluetooth control


## Download & Installation

1. Clone or download this repository to your computer.

2. Open the sketch file (`sm11/sm11.ino`) with the [Arduino IDE](https://www.arduino.cc/en/Main/Software)

3. Upload to your SM11 car kit using USB. (Don't forget to remove the Bluetooth module
when uploading a sketch, as it seems to confuse the car.)

## Usage

The car can be in different modes. To switch between modes, press the `*` key on the
infrared remote control then the number of the mode, then the `#` key. When changing
modes, a short beep will confirm each keypress. In any mode, using the directional
keys (arrows) of the infrared remote control will also put the car back into manual
mode.

There are currently 4 modes:

1. **Manual/dance mode** (default): the car is controlled manually with the directional
keys (arrows) of the infrared remote control. Keys from `0` to `9` will also let the
car execute a series of predefined moves ("dance").

2. **Dance mode**: the car will execute a series of predefined moves ("dance") when
pressing any of the keys from `0` to `9` on the remote control. After executing a
dance, the car will return to manual mode.

3. **Ultrasound mode**: the car will move forward until detecting an obstacle with the
ultrasound sensor (the forward-pointing yellow sensor at the front); when it encounters
an obstacle, it will emit a short beep and make a right turn, then try moving forward
again.

4. **Line-tracking mode**: if the car is placed on a track consisting of a black line
on white background, it will follow the black line by moving forward and adjusting its
bearing left or right when its line-tracking sensors (downwards-pointing sensors
close to the ground) detect that it's hitting the black line.

## Architecture and how to extend

The architecture of the code is as follows:

* The `ControlContext` class, defined in `mode.cpp`, takes care of high-level "executive"
control of the car: which mode is it in, should it go forwards, backwards, or turn left,
etc.

* The modes themselves are implemented in the `mode_manual.cpp`, `mode_dance.cpp`,
`mode_ultrasound.cpp` and `mode_track.cpp` files. Each mode provides at least two
functions:
  - `handle_ir_keypress`, which gets called each time a key is pressed on the
infrared remote; and
  - `control`, which gets called every loop.

* The low-level outputs (motor & buzzer) are controlled in the `Motor` and `Buzzer`
classes' respective `control` functions, which are called once every loop.

* The loop first evaluates whether an infrared keypress has been received; if so, then
it is passed to the control context for processing, which passes it on to the correct
mode; then, it calls the `control()` method for the mode; finally it calls the
`control()` method for the low-level controllers.

To extend the functionality, refer to this [example commit](https://github.com/mbof/sm11/commit/16faaa354a73bb84aac7c4a372524a20ac0ce7b6) which
implements line-tracking functionality.
