#include <Arduino.h>

#include "SingleHumidistat.h"

SingleHumidistat::SingleHumidistat(HumiditySensor *hs, const ConfigStore *cs, Array<uint8_t, 2> pins_solenoid)
		: Humidistat(cs->HC_Kp, cs->HC_Ki, cs->HC_Kd, cs->dt, cs->S_lowValue, 255, hs, cs, (255 + cs->S_lowValue) / 2),
		  pins_solenoid{pins_solenoid[0], pins_solenoid[1]} {}

void SingleHumidistat::update() {
	runCycle();

	// Write it to the pins
	analogWrite(pins_solenoid[0], controlValue);
	analogWrite(pins_solenoid[1], 255 + cs.S_lowValue - controlValue);
}
