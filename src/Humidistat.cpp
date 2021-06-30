#include <Arduino.h>
#include "Humidistat.h"

Humidistat::Humidistat(HumiditySensor *hs, uint8_t lowValue, unsigned long dt, double Kp, double Ki, double Kd)
		: hs(*hs), lowValue(lowValue), dt(dt), Kp(Kp), Ki(Ki), Kd(Kd) {
	pid = new PID(&pv, &cv, &sp, Kp, Ki, Kd, dt, lowValue, 255);
}

Humidistat::Humidistat(const Humidistat &obj) : hs(obj.hs), lowValue(obj.lowValue), dt(obj.dt), pv(obj.pv),
                                                cv(obj.cv), sp(obj.sp), Kp(obj.Kp), Ki(obj.Ki), Kd(obj.Kd),
                                                setpoint(obj.setpoint) {
	pid = new PID(&pv, &cv, &sp, Kp, Ki, Kd, dt, lowValue, 255);
}

Humidistat::~Humidistat() {
	delete pid;
}

Humidistat &Humidistat::operator=(const Humidistat &obj) {
	if (this != &obj) {
		pv = obj.pv;
		cv = obj.cv;
		sp = obj.sp;
		Kp = obj.Kp;
		Ki = obj.Ki;
		Kd = obj.Kd;
		setpoint = obj.setpoint;
		controlValue = obj.controlValue;
		active = obj.active;

		delete pid;
		pid = new PID(&pv, &cv, &sp, Kp, Ki, Kd, dt, lowValue, 255);
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
	pid->setAuto(active);

	// Convert public int setpoint to double for PID
	sp = (double) setpoint;

	// Read humidity (if not NaN)
	double humidity = getHumidity();
	if (!isnan(humidity))
		pv = humidity;

	// Put public controlValue into cv (only matters if user adjusted controlValue)
	cv = controlValue;

	// Run PID cycle if active (pid writes into self->cv)
	pid->compute();

	// Convert double control value to int (only matters if PID is active)
	controlValue = (uint8_t) cv;

	// Write it to the pins
	analogWrite(pinS1, controlValue);
	analogWrite(pinS2, 255 + lowValue - controlValue);
}

uint8_t Humidistat::getLowValue() const {
	return lowValue;
}

void Humidistat::getTerms(double &pTerm, double &iTerm, double &dTerm) const {
	pTerm = pid->pTerm;
	iTerm = pid->iTerm;
	dTerm = pid->dTerm;
}

void Humidistat::getGains(double &Kp, double &Ki, double &Kd) const {
	Kp = this->Kp;
	Ki = this->Ki;
	Kd = this->Kd;
}

uint16_t Humidistat::getDt() const {
	return dt;
}
