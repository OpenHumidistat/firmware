#include <Arduino.h>
#include "ControllerUI.h"

ControllerUI::ControllerUI(Print *display, const ButtonReader *buttonReader, Humidistat *humidistat,
                           Array<const ThermistorReader *, 4> trs) :
		display(*display), buttonReader(*buttonReader), humidistat(*humidistat), trs(trs) {}

void ControllerUI::update() {
	// Show splash screen and info (draw it once) for a short time after boot
	if (millis() < splashDuration) {
		if (!splashDrawn) {
			drawSplash();
			splashDrawn = true;
		}
		return;
	}
	if (millis() - splashDuration < infoDuration) {
		if (!infoDrawn) {
			drawInfo();
			infoDrawn = true;
		}
		return;
	}
	// Clear screen once after splash and info are shown
	if (!screenCleared) {
		clear();
		screenCleared = true;
	}

	if (millis() - lastPressed > inputInterval)
		input();
	if (millis() - lastRefreshed > RefreshInterval) {
		draw();
	}
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
		draw();
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
	setCursor(col, row);
	if (millis() % (2 * blinkInterval) > blinkInterval) {
		display.print(buf);
	} else {
		// Create char array of spaces with same length as buf
		size_t len = strlen(buf);
		char clrBuf[len + 1];
		memset(clrBuf, ' ', len);
		clrBuf[len] = '\0';

		display.print(clrBuf);
	}
}

void ControllerUI::printNTC(uint8_t col, uint8_t row, uint8_t i) {
	double temp = trs[i]->readTemp();
	char buf[3];
	if (isnan(temp)) {
		sprintf(buf, "%2u", 0);
	} else {
		sprintf(buf, "%2u", (int) temp);
	}

	setCursor(col, row);
	display.print(buf);
}
