#ifndef HUMIDISTAT_CONTROLLER_H
#define HUMIDISTAT_CONTROLLER_H

#include <stdint.h>

#include "PID.h"
#include "../EEPROMConfig.h"

/// Base class for a controller.
/// Owns a PID instance, and holds a reference to a ConfigStore instance.
class Controller {
protected:
	PID pid;
	const ConfigStore &cs;

	unsigned long sensorLastRead = 0;

public:
	bool active = false;

	double pv = 0; //!< Process variable
	double sp;     //!< Setpoint
	double cv;     //!< Control variable

	/// Constructor.
	/// \param cs Pointer to a ConfigStore instance
	/// \param Kp Proportional gain
	/// \param Ki Integral gain (in 1/s)
	/// \param Kd Derivative gain (in s)
	/// \param dt Timestep (in ms)
	/// \param cvMin Lower limit for control value
	/// \param cvMax Upper limit for control value
	/// \param defaultSP Default value for the setpoint
	/// \param defaultCV Default value for the control variable
	Controller(const ConfigStore *cs, double Kp, double Ki, double Kd, uint16_t dt, double cvMin, double cvMax,
	           double defaultSP, double defaultCV);

	/// Get the three PID terms by reference.
	/// \param pTerm
	/// \param iTerm
	/// \param dTerm
	void getTerms(double &pTerm, double &iTerm, double &dTerm) const;

	/// Run the controller:
	/// Typically reads a sensor, runs a cycle of the PID loop and drives some actuator.
	void update();

	/// Get a pointer to the ConfigStore instance.
	/// \return pointer to the ConfigStore instance.
	const ConfigStore* getConfigStore();
};


#endif //HUMIDISTAT_CONTROLLER_H
