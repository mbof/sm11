Control sketch for the Kuman SM11 Arduino UNO car.

This is a control sketch for the Kuman SM11 Arduino UNO car kit, with the
following objectives compared with the code originally provided:

* Use event-based programming to avoid unresponsiveness.

* Increase code readability.

* Organize the code to make it easier to create new control modes.


It does not completely implement the original software features. Currently
implemented:

* Manual infrared control

* Ultrasound obstacle-avoiding mode

* (New) Pre-programmed "dances"


Currently unimplemented:

* Line-following mode

* Infrared proximity sensor obstacle-avoiding mode
