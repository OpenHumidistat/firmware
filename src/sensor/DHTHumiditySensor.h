#ifndef HUMIDISTAT_DHTHUMIDITYSENSOR_H
#define HUMIDISTAT_DHTHUMIDITYSENSOR_H

#include "DHT.h"

/// Implementation of the HumiditySensor interface for the DHT22/AM2302 sensor.
class DHTHumiditySensor {
private:
	DHT &dht;
public:
	explicit DHTHumiditySensor(DHT *dht);
	double getHumidity() const;
	double getTemperature() const;
	void begin();
	void readSample();
};


#endif //HUMIDISTAT_DHTHUMIDITYSENSOR_H
