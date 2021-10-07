#ifndef HUMIDISTAT_SHTHUMIDITYSENSOR_H
#define HUMIDISTAT_SHTHUMIDITYSENSOR_H

#include <math.h>
#include <Wire.h>
#include <SHTSensor.h>

/// Implementation of the HumiditySensor interface for the Sensirion SHT85 sensor.
class SHTHumiditySensor {
private:
	SHTSensor &sht;

	double t = NAN, h = NAN;
public:
	explicit SHTHumiditySensor(SHTSensor *sht);
	double getHumidity() const;
	double getTemperature() const;
	void begin();
	void readSample();
};


#endif //HUMIDISTAT_SHTHUMIDITYSENSOR_H
