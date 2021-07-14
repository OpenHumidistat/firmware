#ifndef HUMIDISTAT_THERMISTORREADER_H
#define HUMIDISTAT_THERMISTORREADER_H

#include <stdint.h>

class ThermistorReader {
private:
	const uint8_t ref_pin = 5;     //!< Reference (high) voltage pin number
	const uint8_t pin;             //!< NTC pin number
	const double R_series = 10000; //!< Resistance of R2 in voltage divider (Ohm)
	const double B = 3950;         //!< Thermistor's value of B in the thermistor equation (K)
	const double r_inf = 0.01752;  //!< Thermistor's value of R_inf in the thermistor equation (Ohm)

	/// Calculate the resistance of the thermistor in the voltage divider.
	/// \return Resistance (Ohm)
	double getThermistorResistance() const;
public:
	/// Constructor.
	/// \param pin NTC pin number
	ThermistorReader(uint8_t pin);

	/// Get the temperature of the thermistor.
	/// \return Temperature (Celsius)
	double readTemp() const;
};


#endif //HUMIDISTAT_THERMISTORREADER_H
