#include "FlowController.h"
#include "Controller.h"
#include "ipow.h"

FlowController::FlowController(const FlowSensor *fs, const ConfigStore *cs, uint8_t solenoidPin, uint8_t pwmRes)
		: Controller(cs, cs->FC_Kp, cs->FC_Ki, cs->FC_Kd, cs->FC_Kf, cs->FC_dt, cs->S_lowValue, 1, 0, cs->S_lowValue),
		  fs(*fs), solenoidPin(solenoidPin), pwmRes(pwmRes) {}

void FlowController::update() {
	if (millis() - sensorLastRead < cs.FC_dt)
		return;
	sensorLastRead = millis();

	pid.setAuto(active);

	// Read flowrate (and update PV if not NaN)
	double flowrate = fs.readFlowrate();
	if (!isnan(flowrate))
		pv = flowrate;

	// Run PID cycle if active
	pid.compute();

	// Actuate solenoid (convert normalised double CV to integer PWM value)
	analogWrite(solenoidPin, static_cast<int>(cv * ipow(2, pwmRes)));
}

void FlowController::updatePIDParameters() {
	pid.setGains(cs.FC_Kp, cs.FC_Ki, cs.FC_Kd, cs.FC_Kf, cs.dt);
	pid.cvMin = cs.S_lowValue;
}
