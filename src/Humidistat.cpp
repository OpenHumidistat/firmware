#include <Arduino.h>
#include "Humidistat.h"

Humidistat::Humidistat(DHT *dht, uint8_t lowValue, int sampleTime, double Kp, double Ki, double Kd)
		: dht(*dht), lowValue(lowValue), sampleTime(sampleTime), Kp(Kp), Ki(Ki), Kd(Kd) {
	pid = new PID(&pv, &cv, &sp, Kp, Ki, Kd, DIRECT);
	pid->SetOutputLimits(lowValue, 255);
	pid->SetSampleTime(sampleTime);
}

Humidistat::Humidistat(const Humidistat &obj) : dht(obj.dht), lowValue(obj.lowValue), sampleTime(obj.sampleTime),
                                                pv(obj.pv), cv(obj.cv), sp(obj.sp), Kp(obj.Kp),
                                                Ki(obj.Ki), Kd(obj.Kd), setpoint(obj.setpoint) {
	pid = new PID(*obj.pid);
}

Humidistat::~Humidistat() {
	delete pid;
}

Humidistat &Humidistat::operator=(const Humidistat &obj) {
	if (this != &obj) {
		dht = obj.dht;
		pid = new PID(*obj.pid);

		pv = obj.pv;
		cv = obj.cv;
		sp = obj.cv;

		Kp = obj.Kp;
		Ki = obj.Ki;
		Kd = obj.Kd;

		setpoint = obj.setpoint;
	}
	return *this;
}

double Humidistat::getHumidity() const {
	return dht.readHumidity();
}

double Humidistat::getTemperature() const {
	return dht.readTemperature();
}

void Humidistat::update(uint8_t pinS1, uint8_t pinS2) {
	pid->SetMode(active);

	// Convert public int setpoint to double for PID
	sp = (double) setpoint;

	// Read humidity (if not NaN)
	float humidity = getHumidity();
	if (!isnan(humidity))
		pv = humidity;

	// Put public controlValue into cv (only matters if user adjusted controlValue)
	cv = controlValue;

	// Run PID cycle if active (pid writes into self->cv)
	pid->Compute();

	// Convert double control value to int (only matters if PID is active)
	controlValue = (uint8_t) cv;

	// Write it to the pins
	analogWrite(pinS1, controlValue);
	analogWrite(pinS2, 255 + lowValue - controlValue);
}

uint8_t Humidistat::getLowValue() const {
	return lowValue;
}
