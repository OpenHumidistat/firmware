#include "Arduino.h"
#include "ControllerUI.h"

ControllerUI::ControllerUI(LiquidCrystal *liquidCrystal, const ButtonReader *buttonReader, Humidistat *humidistat) :
		liquidCrystal(*liquidCrystal),
		buttonReader(*buttonReader),
		humidistat(*humidistat) {
	this->liquidCrystal.begin(16, 2);
}

void ControllerUI::update() {
	updateDisplay();
	if (millis() - lastTime > inputInterval) {
		input();
	}
}

void ControllerUI::updateDisplay() {
	// Update current humidity and temperature readings
	{
		char buf[6];
		sprintf(buf, "H%4.1f", humidistat.getHumidity());
		liquidCrystal.setCursor(0, 0);
		liquidCrystal.print(buf);
	}

	{
		char buf[6];
		sprintf(buf, "T%4.1f", humidistat.getTemperature());
		liquidCrystal.setCursor(11, 0);
		liquidCrystal.print(buf);
	}

	// Setpoint
	{
		char buf[5];
		sprintf(buf, "%3d%%", humidistat.setpoint);
		liquidCrystal.setCursor(6, 0);
		liquidCrystal.print(buf);
	}

	// Error
	{
		char buf[7];
		sprintf(buf, "E% 5.1f", humidistat.setpoint-humidistat.getHumidity());
		liquidCrystal.setCursor(0, 1);
		liquidCrystal.print(buf);
	}

	// Control value
	{
		char buf[4];
		sprintf(buf, "%3d", humidistat.getCv());
		liquidCrystal.setCursor(7, 1);
		liquidCrystal.print(buf);
	}

}

void ControllerUI::input() {
	lastTime = millis();

	uint8_t &h = humidistat.setpoint;

	switch (buttonReader.read()) {
		case Buttons::UP:
			h++;
			break;
		case Buttons::DOWN:
			if (h > 0)
				h--;
			break;
		case Buttons::LEFT:
			if (h >= 10)
				h -= 10;
			else
				h = 0;
			break;
		case Buttons::RIGHT:
			h += 10;
			break;
		default:
			break;
	}
	if (h > 100)
		h = 100;
}
