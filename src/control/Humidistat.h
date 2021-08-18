#ifndef HUMIDISTAT_HUMIDISTAT_H
#define HUMIDISTAT_HUMIDISTAT_H

#include "aliases.h"
#include "Controller.h"
#include "EEPROMConfig.h"

/// Base class for a humidistat.
/// Holds a reference to a HumiditySensor instance.
class Humidistat : public Controller {
protected:
	HumiditySensor &hs;

	/// Run a cycle of the controller.
	/// Reads a sample from the humidity controller and runs PID.
	void runCycle();

public:
	/// Constructor.
	/// \param Kp    Proportional gain
	/// \param Ki    Integral gain (in 1/s)
	/// \param Kd    Derivative gain (in s)
	/// \param dt    Timestep (in ms)
	/// \param cvMin Lower limit for control value
	/// \param cvMax Upper limit for control value
	/// \param hs    Pointer to a HumiditySensor instance
	/// \param cs    Pointer to a ConfigStore instance
	/// \param defaultCV Default value for the control variable
	Humidistat(double kp, double ki, double kd, uint16_t dt, double cvMin, double cvMax, HumiditySensor *hs,
	           const ConfigStore *cs, uint8_t defaultCV);

	uint8_t setpoint = 50;
	uint8_t controlValue;

	/// Read the humidity.
	/// \return Relative humidity (percent)
	double getHumidity() const;

	/// Read the temperature.
	/// \return Temperature (Celsius)
	double getTemperature() const;

	/// Update the PID parameters from the configStore.
	void updatePIDParameters();
};


#endif //HUMIDISTAT_HUMIDISTAT_H
