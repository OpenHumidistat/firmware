#include <Arduino.h>
#include "FlowSensor.h"

FlowSensor::FlowSensor(uint8_t pin) : pin(pin) {}

double FlowSensor::readFlowrate() const {
	double voltage = (double) analogRead(pin) / 1023 * 3.3;

	// Calculate flowrate from voltage using polynomial approximation
	return coeffs[0]*pow(voltage, 5)
		+ coeffs[1]*pow(voltage, 4)
		+ coeffs[2]*pow(voltage, 3)
		+ coeffs[3]*pow(voltage, 2)
		+ coeffs[4]*voltage
		+ coeffs[5];
}
