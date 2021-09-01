#include <Arduino.h>

#include "FlowSensor.h"
#include "ipow.h"

FlowSensor::FlowSensor(uint8_t pin) : pin(pin) {}

double FlowSensor::readFlowrate() const {
	uint16_t value = analogRead(pin);

	// Calculate flowrate from voltage using polynomial approximation
	return ( coeffs[0] * ipow(value, 5)
	       + coeffs[1] * ipow(value, 4)
	       + coeffs[2] * ipow(value, 3)
	       + coeffs[3] * ipow(value, 2)
	       + coeffs[4] * value
	       + coeffs[5]);
}
