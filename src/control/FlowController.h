#ifndef HUMIDISTAT_FLOWCONTROLLER_H
#define HUMIDISTAT_FLOWCONTROLLER_H

#include "Controller.h"
#include "PID.h"
#include "../sensor/FlowSensor.h"
#include "../EEPROMConfig.h"

/// Controls flow.
/// Holds a reference to a FlowSensor instance.
class FlowController : public Controller {
private:
	const FlowSensor &fs;
	const uint8_t solenoidPin;
	const uint8_t pwmRes;

public:
	/// Constructor.
	/// \param fs          Pointer to a FlowSensor instance
	/// \param cs          Pointer to a ConfigStore instance
	/// \param solenoidPin Solenoid pin
	/// \param pwmRes      PWM resolution (bits)
	FlowController(const FlowSensor *fs, const ConfigStore *cs, uint8_t solenoidPin, uint8_t pwmRes);

	// Overridden from Controller
	void update();
	void updatePIDParameters();
};


#endif //HUMIDISTAT_FLOWCONTROLLER_H
