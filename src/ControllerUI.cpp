#include "Arduino.h"
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
	if (millis() - lastTime > inputInterval) {
		updateDisplay();
		input();
	}
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
			blink(6, 0, buf);
		} else {
			liquidCrystal.setCursor(6, 0);
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
}

void ControllerUI::input() {
	lastTime = millis();

	if (humidistat.active) {
		adjustValue(humidistat.setpoint, 0, 100);
	} else {
		adjustValue(humidistat.controlValue, humidistat.getLowValue(), 255);
	}
}

void ControllerUI::adjustValue(uint8_t &value, uint8_t min, uint8_t max) {
	switch (buttonReader.read()) {
		case Buttons::UP:
			if (value < max)
				value++;
			break;
		case Buttons::DOWN:
			if (value > min)
				(value)--;
			break;
		case Buttons::LEFT:
			if (value >= min + 10)
				value -= 10;
			else
				value = min;
			break;
		case Buttons::RIGHT:
			if (value <= max - 10)
				value += 10;
			else
				value = max;
			break;
		case Buttons::SELECT:
			humidistat.active = !humidistat.active;
		default:
			break;
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
		clrBuf[len + 1] = '\0';

		liquidCrystal.print(clrBuf);
	}
}

void ControllerUI::printNTC(uint8_t col, uint8_t row, uint8_t i) {
	float temp = trs[i].readTemp();
	char buf[3];
	if (isnan(temp)) {
		sprintf(buf, "%2u", 0);
	} else {
		sprintf(buf, "%2u", (int) temp);
	}

	liquidCrystal.setCursor(col, row);
	liquidCrystal.print(buf);
}
