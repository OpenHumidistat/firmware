#include <Arduino.h>

#include "SingleHumidistat.h"
#include "ipow.h"

SingleHumidistat::SingleHumidistat(const ConfigStore *cs, HumiditySensor *hs, Array<uint8_t, 2> pins_solenoid,
								   uint8_t pwmRes)
		: Humidistat(cs, hs, cs->HC_Kp, cs->HC_Ki, cs->HC_Kd, cs->dt, cs->S_lowValue, 1),
		  pins_solenoid{pins_solenoid[0], pins_solenoid[1]}, pwmRes(pwmRes) {}

void SingleHumidistat::update() {
	runCycle();

	// Actuate solenoids (convert normalised double CV to integer PWM value)
	analogWrite(pins_solenoid[0], static_cast<int>(cv * ipow(2, pwmRes)));
	analogWrite(pins_solenoid[1], static_cast<int>((pid.cvMin + 1 - cv) * ipow(2, pwmRes)));
}
}
