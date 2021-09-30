#include "PID.h"

PID::PID(const double *pv, double *cv, const double *sp, double Kp, double Ki, double Kd, double Kf, uint16_t dt,
		 double cvMin,
         double cvMax)
	: pv(*pv), cv(*cv), sp(*sp), cvMin(cvMin), cvMax(cvMax) {
	setGains(Kp, Ki, Kd, Kf, dt);
	init();
}

void PID::init() {
	integral = cv / Ki;
	lastPv = pv;
	lastE = sp - pv;
}

bool PID::compute() {
	// Terminate if not in auto
	if (!inAuto)
		return false;

	// Proportional error
	double e = sp - pv;
	// Derivative (Derivative on Measurement)
	double dPv = pv - lastPv; // Backwards difference
	// Integral error
	double delta = (lastE + e) / 2; // Trapezoidal integration
	if ((cv < cvMax || delta < 0) && (cv > cvMin || delta > 0)) // Anti-windup through conditional integration
		integral += delta;

	pTerm = Kp * e;
	iTerm = Ki * integral;
	dTerm = -Kd * dPv;
	fTerm = Kf * sp;

	cv = clip(pTerm + iTerm + dTerm + fTerm);

	lastPv = pv;
	lastE = e;
	return true;
}

double PID::clip(double value) const {
	if (value > cvMax)
		return cvMax;
	if (value < cvMin)
		return cvMin;
	return value;
}

void PID::setAuto(bool inAuto) {
	// When going from manual to auto, run init() for bumpless transfer
	if (inAuto && !this->inAuto)
		init();
	this->inAuto = inAuto;
}

void PID::setGains(double Kp, double Ki, double Kd, double Kf, uint16_t dt) {
	this->dt = dt;
	this->Kp = Kp;

	// The timestep is constant, so we include it in Ki and Kd for convenience
	this->Ki = Ki * dt / 1000;
	this->Kd = Kd / (static_cast<double>(dt) / 1000);

	this->Kf = Kf;

	init();
}
