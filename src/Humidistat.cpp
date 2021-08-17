#include <Arduino.h>
#include "Humidistat.h"

Humidistat::Humidistat(HumiditySensor *hs, const ConfigStore *configStore)
		: hs(*hs), cs(*configStore) {
	pid = new PID(&pv, &cv, &sp, this->cs.Kp, this->cs.Ki, this->cs.Kd, this->cs.dt, this->cs.lowValue, 255);
}

Humidistat::Humidistat(const Humidistat &obj)
: hs(obj.hs), cs(obj.cs), pv(obj.pv), cv(obj.cv), sp(obj.sp), setpoint(obj.setpoint) {
	pid = new PID(&pv, &cv, &sp, cs.Kp, cs.Ki, cs.Kd, cs.dt, cs.lowValue, 255);
}

Humidistat::~Humidistat() {
	delete pid;
}

Humidistat &Humidistat::operator=(const Humidistat &obj) {
	if (this != &obj) {
		pv = obj.pv;
		cv = obj.cv;
		sp = obj.sp;
		setpoint = obj.setpoint;
		controlValue = obj.controlValue;
		active = obj.active;

		delete pid;
		pid = new PID(&pv, &cv, &sp, this->cs.Kp, this->cs.Ki, this->cs.Kd, this->cs.dt, this->cs.lowValue, 255);
	}
	return *this;
}

double Humidistat::getHumidity() const {
	return hs.getHumidity();
}

double Humidistat::getTemperature() const {
	return hs.getTemperature();
}

void Humidistat::update(uint8_t pinS1, uint8_t pinS2) {
	if (millis() - sensorLastRead < cs.dt)
		return;
	sensorLastRead = millis();

	pid->setAuto(active);

	// Convert public int setpoint to double for PID
	sp = setpoint;

	// Read humidity (if not NaN)
	hs.readSample();

	if (!isnan(hs.getHumidity()))
		pv = hs.getHumidity();

	// Put public controlValue into cv (only matters if user adjusted controlValue)
	cv = controlValue;

	// Run PID cycle if active (pid writes into self->cv)
	pid->compute();

	// Convert double control value to int (only matters if PID is active)
	controlValue = static_cast<uint8_t>(cv);

	// Write it to the pins
	analogWrite(pinS1, controlValue);
	analogWrite(pinS2, 255 + cs.lowValue - controlValue);
}

uint8_t Humidistat::getLowValue() const {
	return cs.lowValue;
}

void Humidistat::getTerms(double &pTerm, double &iTerm, double &dTerm) const {
	pTerm = pid->pTerm;
	iTerm = pid->iTerm;
	dTerm = pid->dTerm;
}

void Humidistat::getGains(double &Kp, double &Ki, double &Kd) const {
	Kp = this->cs.Kp;
	Ki = this->cs.Ki;
	Kd = this->cs.Kd;
}

uint16_t Humidistat::getDt() const {
	return cs.dt;
}

void Humidistat::updatePIDParameters() {
	pid->setGains(cs.Kp, cs.Ki, cs.Kd, cs.dt);
	pid->setCvMin(cs.lowValue);
}
