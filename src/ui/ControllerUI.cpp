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

	if (millis() - lastPressed > inputInterval) {
		bool pressed = handleInput(buttonReader.read());

		if (pressed) {
			lastPressed = millis();
			draw();
		}
		if (millis() - lastRefreshed > RefreshInterval) {
			draw();
		}
	}
}

void ControllerUI::blink(uint8_t col, uint8_t row, const char *buf) {
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
	if (isnan(temp)) {
		printf(col, row, "%2u", 0);
	} else {
		printf(col, row, "%2u", (int) temp);
	}
}

void ControllerUI::adjustValue(int8_t delta, uint8_t &value, uint8_t min, uint8_t max) {
	// Clip value to [min, max] before uint overflow happens
	if(value + delta < min) {
		value = min;
		return;
	}
	if(value + delta > max) {
		value = max;
		return;
	}
	value += delta;
}
