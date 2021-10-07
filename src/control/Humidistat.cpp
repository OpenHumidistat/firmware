#include "Humidistat.h"

Humidistat::Humidistat(const ConfigStore *cs, HumiditySensor *hs, double Kp, double Ki, double Kd, double Kf, uint16_t
                       dt, double cvMin, double cvMax)
		: Controller(cs, Kp, Ki, Kd, Kf, dt, cvMin, cvMax, 50, (cvMin + cvMax) / 2), hs(*hs) {}

double Humidistat::getHumidity() const {
	return hs.getHumidity();
}

double Humidistat::getTemperature() const {
	return hs.getTemperature();
}

void Humidistat::runCycle() {
	if (millis() - sensorLastRead < cs.dt)
		return;
	sensorLastRead = millis();

	pid.setAuto(active);

	// Read humidity (if not NaN)
	hs.readSample();
	if (!isnan(hs.getHumidity()))
		pv = hs.getHumidity();

	// Run PID cycle if active (pid writes into this->cv)
	pid.compute();
}

double Humidistat::getCvMin() const {
	return pid.cvMin;
}

double Humidistat::getCvMax() const {
	return pid.cvMax;
}
