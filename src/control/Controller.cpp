#include <Arduino.h>
#include "Controller.h"

Controller::Controller(double Kp, double Ki, double Kd, uint16_t dt, double cvMin, double cvMax,
                       const ConfigStore *cs)
		: pid(&pv, &cv, &sp, Kp, Ki, Kd, dt, cvMin, cvMax), cs(*cs) {}

void Controller::getTerms(double &pTerm, double &iTerm, double &dTerm) const {
	pTerm = pid.pTerm;
	iTerm = pid.iTerm;
	dTerm = pid.dTerm;
}

const ConfigStore *Controller::getConfigStore() {
	return &cs;
}
