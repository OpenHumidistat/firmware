#ifndef HUMIDISTAT_CASCADEHUMIDISTAT_H
#define HUMIDISTAT_CASCADEHUMIDISTAT_H

#include <etl/span.h>
#include <etl/array.h>

#include "aliases.h"
#include "Humidistat.h"
#include "../sensor/FlowSensor.h"
#include "FlowController.h"

/// Control humidity using cascade PID: outer PID loop sets setpoints of two inner flow controllers, which drive a
/// solenoid valve each.
/// Adjust the public setpoint variable and call update().
class CascadeHumidistat : public Humidistat {
private:
	FlowController fcs[2];

public:
	/// Constructor.
	/// \param cs            Pointer to a ConfigStore instance
	/// \param hs            Pointer to a HumiditySensor instance
	/// \param flowSensors   Array of 2 pointers to FlowSensor instances
	/// \param pins_solenoid Array of 2 integers corresponding to the solenoid pins
	/// \param pwmRes        PWM resolution (bits)
	CascadeHumidistat(HumiditySensor *hs, const ConfigStore *cs, etl::span<const FlowSensor, 2> flowSensors,
					  etl::array<uint8_t, 2> pins_solenoid, uint8_t pwmRes);

	/// Get a pointer to a inner FlowController instance.
	/// \param n the index of the FlowController (0 or 1)
	/// \return pointer to the FlowController instance.
	const FlowController* getInner(uint8_t n) const;

	// Overridden from Controller
	void update();
	void updatePIDParameters();
};


#endif //HUMIDISTAT_CASCADEHUMIDISTAT_H
