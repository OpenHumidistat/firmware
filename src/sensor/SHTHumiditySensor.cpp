#include "SHTHumiditySensor.h"

SHTHumiditySensor::SHTHumiditySensor(SHTSensor *sht) : sht(*sht) {}

double SHTHumiditySensor::getHumidity() const {
	return h;
}

double SHTHumiditySensor::getTemperature() const {
	return t;
}

void SHTHumiditySensor::begin() {
	Wire.begin();
	sht.init();
}

void SHTHumiditySensor::readSample() {
	sht.readSample();
	t = sht.getTemperature();
	h = sht.getHumidity();
}
