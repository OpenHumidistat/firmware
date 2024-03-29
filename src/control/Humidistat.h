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
	/// \param cs Pointer to a ConfigStore instance
	/// \param hs Pointer to a HumiditySensor instance
	/// \param Kp Proportional gain
	/// \param Ki Integral gain (in 1/s)
	/// \param Kd Derivative gain (in s)
	/// \param Kf Feed-forward gain
	/// \param dt Timestep (in ms)
	/// \param cvMin Lower limit for control value
	/// \param cvMax Upper limit for control value
	Humidistat(const ConfigStore *cs, HumiditySensor *hs, double Kp, double Ki, double Kd, double Kf, uint16_t dt,
			   double cvMin, double cvMax);

	/// Read the humidity.
	/// \return Relative humidity (percent)
	double getHumidity() const;

	/// Read the temperature.
	/// \return Temperature (Celsius)
	double getTemperature() const;

	double getCvMin() const;
	double getCvMax() const;
};


#endif //HUMIDISTAT_HUMIDISTAT_H
