#ifndef HUMIDISTAT_THERMISTORREADER_H
#define HUMIDISTAT_THERMISTORREADER_H

#include "Arduino.h"

class ThermistorReader {
private:
	const uint8_t ref_pin = 5; //!< Reference (high) voltage pin number
	uint8_t pin;            //!< NTC pin number
	float R_series = 10000; //!< Resistance of R2 in voltage divider (Ohm)
	float B = 3950;         //!< Thermistor's value of B in the thermistor equation (K)
	float r_inf = 0.01752;  //!< Thermistor's value of R_inf in the thermistor equation (Ohm)

	/// Calculate the resistance of the thermistor in the voltage divider.
	/// \return Resistance (Ohm)
	float getThermistorResistance() const;
public:
	/// Constructor.
	/// \param pin NTC pin number
	ThermistorReader(uint8_t pin);

	/// Get the temperature of the thermistor.
	/// \return Temperature (Celsius)
	float readTemp() const;
};


#endif //HUMIDISTAT_THERMISTORREADER_H
