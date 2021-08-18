#include "Humidistat.h"

Humidistat::Humidistat(double kp, double ki, double kd, uint16_t dt, double cvMin, double cvMax, HumiditySensor *hs,
                       const ConfigStore *cs, uint8_t defaultCV)
		: Controller(kp, ki, kd, dt, cvMin, cvMax, cs), hs(*hs), controlValue(defaultCV) {}

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

	// Convert public int setpoint to double for PID
	sp = setpoint;

	// Read humidity (if not NaN)
	hs.readSample();
	if (!isnan(hs.getHumidity()))
		pv = hs.getHumidity();

	// Put public controlValue into cv (only matters if user adjusted controlValue)
	cv = controlValue;

	// Run PID cycle if active (pid writes into self->cv)
	pid.compute();

	// Convert double control value to int (only matters if PID is active)
	controlValue = static_cast<uint8_t>(cv);
}
