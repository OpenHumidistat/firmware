#ifndef HUMIDISTAT_SHTHUMIDITYSENSOR_H
#define HUMIDISTAT_SHTHUMIDITYSENSOR_H

#include <Arduino.h>
#include <Wire.h>
#include <SHTSensor.h>

#include "HumiditySensor.h"

class SHTHumiditySensor : public HumiditySensor {
private:
	SHTSensor &sht;
public:
	explicit SHTHumiditySensor(SHTSensor *sht);
	double getHumidity() const override;
	double getTemperature() const override;
	void begin() override;
};


#endif //HUMIDISTAT_SHTHUMIDITYSENSOR_H
