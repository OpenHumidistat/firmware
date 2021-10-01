#include "CascadeHumidistat.h"

CascadeHumidistat::CascadeHumidistat(HumiditySensor *hs, const ConfigStore *cs,
                                     Array<const FlowSensor *, 2> flowSensors, Array<uint8_t, 2> pins_solenoid,
									 uint8_t pwmRes)
		: Humidistat(cs, hs, cs->HC_Kp, cs->HC_Ki, cs->HC_Kd, cs->HC_Kf, cs->dt, 0, 1),
		  fcs{FlowController(flowSensors[0], cs, pins_solenoid[0], pwmRes),
		      FlowController(flowSensors[1], cs, pins_solenoid[1], pwmRes)} {
	fcs[0].active = true;
	fcs[1].active = true;
}

void CascadeHumidistat::update() {
	runCycle();

	// Set CV of humidity controller as SP of flow controllers
	fcs[0].sp =      cv  * cs.HC_totalFlowrate;
	fcs[1].sp = (1 - cv) * cs.HC_totalFlowrate;

	fcs[0].update();
	fcs[1].update();
}

const FlowController *CascadeHumidistat::getInner(uint8_t n) const {
	return &fcs[n];
}

void CascadeHumidistat::updatePIDParameters() {
	pid.setGains(cs.HC_Kp, cs.HC_Ki, cs.HC_Kd, cs.HC_Kf, cs.dt);
	fcs[0].updatePIDParameters();
	fcs[1].updatePIDParameters();
}
