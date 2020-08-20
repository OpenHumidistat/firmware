#ifndef HUMIDISTAT_HUMIDISTAT_H
#define HUMIDISTAT_HUMIDISTAT_H

#include "DHT.h"
#include "PID_v1.h"

/// Control humidity using PID by driving two solenoid valves.
/// Holds references to a DHT instance (for reading humidity using DHT22 sensor) and a PID instance.
/// Adjust the public setpoint variable and call update() with the solenoid pins to use.
class Humidistat {
private:
	DHT &dht;
	PID *pid;

	double pv = 0; //!< Process variable
	double cv = 128; //!< Control variable
	double sp = 0; //!< Setpoint

	double Kp = 0;
	double Ki = 0;
	double Kd = 0;
public:
	uint8_t setpoint = 50; //!< Public setpoint

	/// Constructor.
	/// \param dht Pointer to DHT instance
	/// \param Kp Proportional gain
	/// \param Ki Integral gain
	/// \param Kd Differential gain
	explicit Humidistat(DHT *dht, double Kp, double Ki, double Kd);

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
	float getHumidity() const;

	/// Read the temperature.
	/// \return Temperature (Celsius)
	float getTemperature() const;

	/// Get the control (solenoid valve) value
	/// \return control value
	uint8_t getCv() const;
};

#endif //HUMIDISTAT_HUMIDISTAT_H
