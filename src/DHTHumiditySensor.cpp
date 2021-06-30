#include "DHTHumiditySensor.h"

DHTHumiditySensor::DHTHumiditySensor(DHT *dht) : dht(*dht) {}

double DHTHumiditySensor::getHumidity() const {
	return dht.readHumidity();
}

double DHTHumiditySensor::getTemperature() const {
	return dht.readTemperature();
}
