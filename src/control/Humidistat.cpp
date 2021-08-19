#include "Humidistat.h"

Humidistat::Humidistat(const ConfigStore *cs, HumiditySensor *hs, double kp, double ki, double kd, uint16_t dt,
                       double cvMin, double cvMax)
		: Controller(cs, kp, ki, kd, dt, cvMin, cvMax, 50, (cvMin + cvMax) / 2), hs(*hs) {}

double Humidistat::getHumidity() const {
	return hs.getHumidity();
}

double Humidistat::getTemperature() const {
	return hs.getTemperature();
}

void Humidistat::updatePIDParameters() {
	pid.setGains(cs.HC_Kp, cs.HC_Ki, cs.HC_Kd, cs.dt);
	pid.setCvMin(cs.S_lowValue);
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
