#ifndef HUMIDISTAT_DHTHUMIDITYSENSOR_H
#define HUMIDISTAT_DHTHUMIDITYSENSOR_H

#include "HumiditySensor.h"
#include "DHT.h"

class DHTHumiditySensor : public HumiditySensor {
private:
	DHT &dht;
public:
	explicit DHTHumiditySensor(DHT *dht);
	double getHumidity() const override;
	double getTemperature() const override;
};


#endif //HUMIDISTAT_DHTHUMIDITYSENSOR_H
