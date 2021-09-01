#ifndef HUMIDISTAT_FLOWSENSOR_H
#define HUMIDISTAT_FLOWSENSOR_H

#include <stdint.h>

#include "ipow.h"

/// Read flow rate using a Omron D6F-P0010 MEMS flow sensor.
class FlowSensor {
private:
	const uint8_t pin;
	/// Coefficients of the polynomial approximation to the sensor response (and voltage mapping)
	static constexpr double coeffs[] = {
			 0.094003 * ipow(3.3 / 1023, 5),
			-0.564312 * ipow(3.3 / 1023, 4),
			 1.374705 * ipow(3.3 / 1023, 3),
			-1.601495 * ipow(3.3 / 1023, 2),
			 1.060657 / 1023 * 3.3,
			-0.269996,
	};

public:
	/// Constructor.
	/// \param pin Sensor pin number
	explicit FlowSensor(uint8_t pin);

	/// Read the flow rate.
	/// \return flow rate (L/min)
	double readFlowrate() const;
};


#endif //HUMIDISTAT_FLOWSENSOR_H
