#ifndef HUMIDISTAT_HUMIDITYSENSOR_H
#define HUMIDISTAT_HUMIDITYSENSOR_H

/// Interface for humidity sensors
class HumiditySensor {
public:
	virtual double getHumidity() const = 0;
	virtual double getTemperature() const = 0;
	virtual void begin() = 0;
};


#endif //HUMIDISTAT_HUMIDITYSENSOR_H
