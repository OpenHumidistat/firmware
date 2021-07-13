#include "SHTHumiditySensor.h"

SHTHumiditySensor::SHTHumiditySensor(SHTSensor *sht) : sht(*sht) {}

double SHTHumiditySensor::getHumidity() const {
	sht.readSample();
	return sht.getHumidity();
}

double SHTHumiditySensor::getTemperature() const {
	sht.readSample();
	return sht.getTemperature();
}

void SHTHumiditySensor::begin() {
	Wire.begin();
	sht.init();
}
