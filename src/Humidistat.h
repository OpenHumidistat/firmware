#ifndef HUMIDISTAT_HUMIDISTAT_H
#define HUMIDISTAT_HUMIDISTAT_H

#include <stdint.h>

#include "PID.h"
#include "sensor/HumiditySensor.h"
#include "EEPROMConfig.h"

/// Control humidity using PID by driving two solenoid valves.
/// Holds references to a DHT instance (for reading humidity using DHT22 sensor) and a PID instance.
/// Adjust the public setpoint variable and call update() with the solenoid pins to use.
class Humidistat {
private:
	HumiditySensor &hs;
	PID *pid;
	const ConfigStore &cs;

	double pv = 0; //!< Process variable
	double cv = 0; //!< Control variable
	double sp = 0; //!< Setpoint

	double temperature;
	unsigned long sensorLastRead = 0;
public:
	uint8_t setpoint = 50;
	uint8_t controlValue = (255 + cs.lowValue) / 2;
	bool active = false;

	/// Constructor.
	/// \param hs          Pointer to a HumiditySensor instance
	/// \param configStore Pointer to a ConfigStore instance
	explicit Humidistat(HumiditySensor *hs, const ConfigStore *configStore);

	/// Copy constructor.
	/// \param obj
	Humidistat(const Humidistat &obj);

	/// Destructor.
	~Humidistat();

	/// Assignment operator
	/// \param obj
	/// \return
	Humidistat &operator=(const Humidistat &obj);

	/// Run the humidistat:
	/// Read the humidity, run a cycle of the PID loop and drive the solenoid valves at the appropriate value.
	/// \param pinS1 Pin corresponding to the "humid air" solenoid valve
	/// \param pinS2 Pin corresponding to the "dry air" solenoid valve
	void update(uint8_t pinS1, uint8_t pinS2);

	/// Read the humidity.
	/// \return Relative humidity (percent)
	double getHumidity() const;

	/// Read the temperature.
	/// \return Temperature (Celsius)
	double getTemperature() const;

	/// Get the lower bound of the control value.
	/// \return Lower bound
	uint8_t getLowValue() const;

	/// Get the three PID terms by reference.
	/// \param pTerm
	/// \param iTerm
	/// \param dTerm
	void getTerms(double &pTerm, double &iTerm, double &dTerm) const;

	/// Get the three PID gains by reference.
	/// \param Kp
	/// \param Ki
	/// \param Kd
	void getGains(double &Kp, double &Ki, double &Kd) const;

	/// Get the timestep.
	uint16_t getDt() const;

	/// Update the PID parameters from the configStore.
	void updatePIDParameters();
};

#endif //HUMIDISTAT_HUMIDISTAT_H
