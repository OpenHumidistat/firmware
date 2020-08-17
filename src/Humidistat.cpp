#include "Arduino.h"
#include "Humidistat.h"

Humidistat::Humidistat(DHT *dht, double Kp, double Ki, double Kd) : dht(*dht), Kp(Kp), Ki(Ki), Kd(Kd) {
	pid = new PID(&pv, &cv, &sp, Kp, Ki, Kd, DIRECT);
	pid->SetMode(AUTOMATIC);
}

Humidistat::Humidistat(const Humidistat &obj) : dht(obj.dht), pv(obj.pv), cv(obj.cv), sp(obj.sp), Kp(obj.Kp),
                                                Ki(obj.Ki), Kd(obj.Kd), setpoint(obj.setpoint) {
	pid = new PID(*obj.pid);
}

Humidistat::~Humidistat() {
	delete pid;
}

Humidistat &Humidistat::operator=(const Humidistat &obj) {
	if(this != &obj) {
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

float Humidistat::getHumidity() const {
	return dht.readHumidity();
}

float Humidistat::getTemperature() const {
	return dht.readTemperature();
}

void Humidistat::update(uint8_t pinS1, uint8_t pinS2) {
	// Convert public int setpoint to double for PID
	sp = (double) setpoint;
	// Read humidity and run PID cycle
	pv = getHumidity();

	if (pid->Compute()) {
		// Convert double control value to int and write it to the pins
		analogWrite(pinS1, getCv());
		analogWrite(pinS2, 255 - getCv());
	}
}

uint8_t Humidistat::getCv() const {
	return (uint8_t) cv;
}
