#include <Arduino.h>
#include "PID.h"

PID::PID(const double *pv, double *cv, const double *sp, double kp, double ki, double kd, uint16_t dt, double cvMin,
		 double cvMax)
//                                           The timestep is constant, so we include it in Ki and Kd for convenience
		: pv(*pv), cv(*cv), sp(*sp), Kp(kp), Ki(ki * (double) dt / 1000), Kd(kd / ((double) dt / 1000)), dt(dt),
		  cvMin(cvMin), cvMax(cvMax) {
	lastComputed = millis();
	init();
}

void PID::init() {
	integral = cv / Ki;
	lastPv = pv;
	lastE = sp - pv;
}

bool PID::compute() {
	// Terminate if not in auto, or if last run was a shorter time ago than the timestep.
	if (!inAuto)
		return false;
	if (millis() - lastComputed < dt)
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

	cv = clip(pTerm + iTerm + dTerm);

	lastComputed = millis();
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
