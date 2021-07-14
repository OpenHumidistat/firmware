#include "CharDisplayUI.h"

CharDisplayUI::CharDisplayUI(LiquidCrystal *liquidCrystal, const ButtonReader *buttonReader, Humidistat *humidistat,
                             Array<const ThermistorReader*, 4> trs) :
		ControllerUI(liquidCrystal, buttonReader, humidistat, trs), liquidCrystal(*liquidCrystal) {}

void CharDisplayUI::draw() {
	// Update current humidity and temperature readings
	{
		char buf[5];
		sprintf(buf, "%4.1f", humidistat.getHumidity());
		liquidCrystal.setCursor(2, 0);
		liquidCrystal.print(buf);
	}

	{
		char buf[5];
		sprintf(buf, "%4.1f", humidistat.getTemperature());
		liquidCrystal.setCursor(12, 1);
		liquidCrystal.print(buf);
	}

	// Setpoint
	{
		char buf[5];
		sprintf(buf, "%3u%%", humidistat.setpoint);
		if (abs(humidistat.setpoint - humidistat.getHumidity()) > tolerance) {
			blink(7, 0, buf);
		} else {
			liquidCrystal.setCursor(7, 0);
			liquidCrystal.print(buf);
		}
	}

	// Control value
	{
		char buf[4];
		sprintf(buf, "%3u", humidistat.controlValue);
		liquidCrystal.setCursor(12, 0);
		liquidCrystal.print(buf);
	}

	// Active status
	liquidCrystal.setCursor(0, 0);
	liquidCrystal.print((int) humidistat.active);

	// Thermistors
	for (size_t i = 0; i < trs.size(); ++i) {
		if(trs[i]) {
			printNTC(3*i, 1, i);
		}
	}
	lastRefreshed = millis();
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
	{
		char buf[15];
		sprintf(buf, "dt %4u lv %3u", humidistat.getDt(), humidistat.getLowValue());

		liquidCrystal.setCursor(0, 0);
		liquidCrystal.print(buf);
	}
	{
		double Kp, Ki, Kd;
		humidistat.getGains(Kp, Ki, Kd);

		char buf[16];
		sprintf(buf, "%3.2f %4.3f %3.2f", Kp, Ki, Kd);

		liquidCrystal.setCursor(0, 1);
		liquidCrystal.print(buf);
	}
}

void CharDisplayUI::begin() {
	liquidCrystal.begin(16, 2);
}
