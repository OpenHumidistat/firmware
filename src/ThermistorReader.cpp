#include "ThermistorReader.h"

ThermistorReader::ThermistorReader(uint8_t pin) : pin(pin) {}

float ThermistorReader::getThermistorResistance() const {
	float V_NTC = (float) analogRead(pin) / 1024 * 5;
	return R_series * (V_high/V_NTC - 1);
}

float ThermistorReader::readTemp() const {
	return B / log(getThermistorResistance()/r_inf) - 273;
}
