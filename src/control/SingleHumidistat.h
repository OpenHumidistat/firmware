#ifndef HUMIDISTAT_SINGLEHUMIDISTAT_H
#define HUMIDISTAT_SINGLEHUMIDISTAT_H

#include <stdint.h>
#include <Array.h>

#include "aliases.h"
#include "Humidistat.h"
#include "EEPROMConfig.h"

/// Control humidity using PID by driving two solenoid valves.
/// Adjust the public setpoint variable and call update() with the solenoid pins to use.
class SingleHumidistat : public Humidistat {
private:
	const uint8_t pins_solenoid[2];
	const uint8_t pwmRes;

public:
	/// Constructor.
	/// \param cs            Pointer to a ConfigStore instance
	/// \param hs            Pointer to a HumiditySensor instance
	/// \param pins_solenoid Array of 2 integers corresponding to the solenoid pins
	/// \param pwmRes        PWM resolution (bits)
	SingleHumidistat(const ConfigStore *cs, HumiditySensor *hs, Array<uint8_t, 2> pins_solenoid, uint8_t pwmRes);

	/// Run the humidistat:
	/// Read the humidity, run a cycle of the PID loop and drive the solenoid valves at the appropriate value.
	void update();
};

#endif //HUMIDISTAT_SINGLEHUMIDISTAT_H