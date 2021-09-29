#ifndef HUMIDISTAT_CASCADEHUMIDISTAT_H
#define HUMIDISTAT_CASCADEHUMIDISTAT_H

#include <Array.h>

#include "aliases.h"
#include "Humidistat.h"
#include "../sensor/FlowSensor.h"
#include "FlowController.h"

class CascadeHumidistat : public Humidistat {
private:
	FlowController fcs[2];

public:
	/// Constructor.
	/// \param hs
	/// \param cs
	/// \param flowSensors
	/// \param pins_solenoid
	/// \param pwmRes        PWM resolution (bits)
	CascadeHumidistat(HumiditySensor *hs, const ConfigStore *cs, Array<const FlowSensor*, 2> flowSensors,
					  Array<uint8_t, 2> pins_solenoid, uint8_t pwmRes);

	///
	void update();

	///
	/// \return
	const FlowController* getInner(uint8_t n) const;

	/// Update the PID parameters from the configStore.
	void updatePIDParameters();
};


#endif //HUMIDISTAT_CASCADEHUMIDISTAT_H
