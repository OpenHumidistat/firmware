#include <Arduino.h>

#include "SingleHumidistat.h"

SingleHumidistat::SingleHumidistat(const ConfigStore *cs, HumiditySensor *hs, Array<uint8_t, 2> pins_solenoid)
		: Humidistat(cs, hs, cs->HC_Kp, cs->HC_Ki, cs->HC_Kd, cs->dt, cs->S_lowValue, 255),
		  pins_solenoid{pins_solenoid[0], pins_solenoid[1]} {}

void SingleHumidistat::update() {
	runCycle();

	// Write it to the pins
	analogWrite(pins_solenoid[0], static_cast<int>(cv));
	analogWrite(pins_solenoid[1], static_cast<int>(255 + cs.S_lowValue - cv));
}
