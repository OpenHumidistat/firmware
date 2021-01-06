#include <Arduino.h>
#include "ControllerUI.h"

ControllerUI::ControllerUI(LiquidCrystal *liquidCrystal, const ButtonReader *buttonReader, Humidistat *humidistat,
                           ThermistorReader (*trs)[4]) :
		liquidCrystal(*liquidCrystal),
		buttonReader(*buttonReader),
		humidistat(*humidistat),
		trs(*trs) {
	this->liquidCrystal.begin(16, 2);
}

void ControllerUI::update() {
	// Show splash screen and info (draw it once) for a short time after boot
	if (millis() < splashDuration) {
		if (!splashDrawn) {
			splash();
			splashDrawn = true;
		}
		return;
	}
	if (millis() - splashDuration < infoDuration) {
		if (!infoDrawn) {
			info();
			infoDrawn = true;
		}
		return;
	}
	// Clear screen once after splash and info are shown
	if (!screenCleared) {
		liquidCrystal.clear();
		screenCleared = true;
	}

	if (millis() - lastPressed > inputInterval)
		input();
	if (millis() - lastRefreshed > RefreshInterval)
		updateDisplay();
}

void ControllerUI::updateDisplay() {
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
	printNTC(0, 1, 0);
	printNTC(3, 1, 1);
	printNTC(6, 1, 2);
	printNTC(9, 1, 3);

	lastRefreshed = millis();
}

void ControllerUI::input() {
	bool pressed;
	if (humidistat.active) {
		pressed = adjustValue(humidistat.setpoint, 0, 100);
	} else {
		pressed = adjustValue(humidistat.controlValue, humidistat.getLowValue(), 255);
	}
	if (pressed) {
		lastPressed = millis();
		updateDisplay();
	}
}

bool ControllerUI::adjustValue(uint8_t &value, uint8_t min, uint8_t max) {
	switch (buttonReader.read()) {
		case Buttons::UP:
			if (value < max)
				value++;
			return true;
		case Buttons::DOWN:
			if (value > min)
				(value)--;
			return true;
		case Buttons::LEFT:
			if (value >= min + adjustStep)
				value -= adjustStep;
			else
				value = min;
			return true;
		case Buttons::RIGHT:
			if (value <= max - adjustStep)
				value += adjustStep;
			else
				value = max;
			return true;
		case Buttons::SELECT:
			// Toggle active state
			humidistat.active = !humidistat.active;
			return true;
		default:
			return false;
	}
}

void ControllerUI::blink(uint8_t col, uint8_t row, char *buf) {
	liquidCrystal.setCursor(col, row);
	if (millis() % (2 * blinkInterval) > blinkInterval) {
		liquidCrystal.print(buf);
	} else {
		// Create char array of spaces with same length as buf
		size_t len = strlen(buf);
		char clrBuf[len + 1];
		memset(clrBuf, ' ', len);
		clrBuf[len] = '\0';

		liquidCrystal.print(clrBuf);
	}
}

void ControllerUI::printNTC(uint8_t col, uint8_t row, uint8_t i) {
	double temp = trs[i].readTemp();
	char buf[3];
	if (isnan(temp)) {
		sprintf(buf, "%2u", 0);
	} else {
		sprintf(buf, "%2u", (int) temp);
	}

	liquidCrystal.setCursor(col, row);
	liquidCrystal.print(buf);
}

void ControllerUI::splash() {
	liquidCrystal.clear();
	liquidCrystal.setCursor(0, 0);
	liquidCrystal.print("Humidistat");
	liquidCrystal.setCursor(0, 1);
	liquidCrystal.print("Lars Veldscholte");
}

void ControllerUI::info() {
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

		char buf[15];
		sprintf(buf, "%3.2f %3.2f %3.2f", Kp, Ki, Kd);

		liquidCrystal.setCursor(0, 1);
		liquidCrystal.print(buf);
	}
}
