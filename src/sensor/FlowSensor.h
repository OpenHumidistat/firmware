#ifndef HUMIDISTAT_FLOWSENSOR_H
#define HUMIDISTAT_FLOWSENSOR_H

#include <stdint.h>

/// Read flow rate using a Omron D6F-P0010 MEMS flow sensor.
class FlowSensor {
private:
	const uint8_t pin;
	/// Coefficients of the polynomial approximation to the sensor response.
	static constexpr double coeffs[] = {0.094003, -0.564312, 1.374705, -1.601495, 1.060657, -0.269996};

public:
	/// Constructor.
	/// \param pin Sensor pin number
	FlowSensor(uint8_t pin);

	/// Read the flow rate.
	/// \return flow rate (L/min)
	double readFlowrate() const;
};


#endif //HUMIDISTAT_FLOWSENSOR_H
