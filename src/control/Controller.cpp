#include "Controller.h"

Controller::Controller(const ConfigStore *cs, double Kp, double Ki, double Kd, double Kf, uint16_t dt, double cvMin,
					   double cvMax, double defaultSP, double defaultCV)
	: pid(&pv, &cv, &sp, Kp, Ki, Kd, Kf, dt, cvMin, cvMax), cs(*cs), sp(defaultSP), cv(defaultCV) {}

void Controller::getTerms(double &pTerm, double &iTerm, double &dTerm) const {
	pTerm = pid.pTerm;
	iTerm = pid.iTerm;
	dTerm = pid.dTerm;
}

const ConfigStore *Controller::getConfigStore() {
	return &cs;
}
