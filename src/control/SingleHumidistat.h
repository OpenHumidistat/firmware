#ifndef HUMIDISTAT_SINGLEHUMIDISTAT_H
#define HUMIDISTAT_SINGLEHUMIDISTAT_H

#include <stdint.h>
#include <etl/array.h>

#include "aliases.h"
#include "Humidistat.h"
#include "EEPROMConfig.h"

/// Control humidity using PID by driving two solenoid valves.
/// Adjust the public setpoint variable and call update().
class SingleHumidistat : public Humidistat {
private:
	const uint8_t pins_solenoid[2];
	const uint8_t pwmRes;

public:
	/// Constructor.
	/// \param hs            Pointer to a HumiditySensor instance
	/// \param cs            Pointer to a ConfigStore instance
	/// \param pins_solenoid Array of 2 integers corresponding to the solenoid pins
	/// \param pwmRes        PWM resolution (bits)
	SingleHumidistat(HumiditySensor *hs, const ConfigStore *cs, etl::array<uint8_t, 2> pins_solenoid, uint8_t pwmRes);

	// Overridden from Controller
	void update();
	void updatePIDParameters();
};

#endif //HUMIDISTAT_SINGLEHUMIDISTAT_H
