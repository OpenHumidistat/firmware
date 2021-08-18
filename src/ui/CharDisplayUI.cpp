#include "CharDisplayUI.h"

CharDisplayUI::CharDisplayUI(LiquidCrystal *liquidCrystal, const ButtonReader *buttonReader,
                             SingleHumidistat *humidistat, Array<const ThermistorReader *, 4> trs)
		: ControllerUI(liquidCrystal, buttonReader, trs), liquidCrystal(*liquidCrystal), humidistat(*humidistat) {}

void CharDisplayUI::draw() {
	lastRefreshed = millis();
	// Update current humidity and temperature readings
	printf(2, 0, "%4.1f", humidistat.getHumidity());
	printf(12, 1, "%4.1f", humidistat.getTemperature());

	// Setpoint
	{
		char *buf = asprintf("%3u%%", humidistat.setpoint);
		if (abs(humidistat.setpoint - humidistat.getHumidity()) > tolerance) {
			blink(7, 0, buf);
		} else {
			liquidCrystal.setCursor(7, 0);
			liquidCrystal.print(buf);
		}
		delete buf;
	}

	// Control value
	printf(12, 0, "%3u", humidistat.controlValue);

	// Active status
	liquidCrystal.setCursor(0, 0);
	liquidCrystal.print(humidistat.active);

	// Thermistors
	for (size_t i = 0; i < trs.size(); ++i) {
		if (trs[i]) {
			printNTC(3 * i, 1, i);
		}
	}
}

void CharDisplayUI::clear() {
	liquidCrystal.clear();
}

void CharDisplayUI::setCursor(uint8_t col, uint8_t row) {
	liquidCrystal.setCursor(col, row);
}

void CharDisplayUI::drawSplash() {
	liquidCrystal.clear();
	liquidCrystal.setCursor(0, 0);
	liquidCrystal.print("Humidistat");
	liquidCrystal.setCursor(0, 1);
	liquidCrystal.print("Lars Veldscholte");
}

void CharDisplayUI::drawInfo() {
	liquidCrystal.clear();
	printf(0, 0, "dt %4u lv %3u",
	       humidistat.getConfigStore()->dt,
	       humidistat.getConfigStore()->S_lowValue
	);
	printf(0, 1, "%3.2f %4.3f %3.2f",
	       humidistat.getConfigStore()->HC_Kp,
	       humidistat.getConfigStore()->HC_Ki,
	       humidistat.getConfigStore()->HC_Kd
	);
}

void CharDisplayUI::begin() {
	liquidCrystal.begin(16, 2);
}

bool CharDisplayUI::handleInput(Buttons button) {
	int8_t delta;
	switch (button) {
		case Buttons::UP:
			delta = 1;
			break;
		case Buttons::DOWN:
			delta = -1;
			break;
		case Buttons::LEFT:
			delta = static_cast<int8_t>(-adjustStep);
			break;
		case Buttons::RIGHT:
			delta = static_cast<int8_t>(adjustStep);
			break;
		case Buttons::SELECT:
			// Toggle active state
			humidistat.active = !humidistat.active;
			return true;
		default:
			return false;
	}

	if (humidistat.active) {
		adjustValue(delta, humidistat.setpoint, 0, 100);
	} else {
		adjustValue(delta, humidistat.controlValue, humidistat.getConfigStore()->S_lowValue, 255);
	}
	return true;
}
