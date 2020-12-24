#include <Arduino.h>
#include "ThermistorReader.h"

ThermistorReader::ThermistorReader(uint8_t pin) : pin(pin) {}

double ThermistorReader::getThermistorResistance() const {
	// Read temperature using reference 3.3V on A5 pin
	double V_NTC = (float) analogRead(pin) / analogRead(ref_pin);
	return R_series * (1 / V_NTC - 1);
}

double ThermistorReader::readTemp() const {
	return B / log(getThermistorResistance() / r_inf) - 273;
}
