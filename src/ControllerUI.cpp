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
		if(abs(humidistat.setpoint - humidistat.getHumidity()) > tolerance) {
			blink(6, 0, buf);
		}
		else {
			liquidCrystal.setCursor(6, 0);
			liquidCrystal.print(buf);
		}
	}

	// Control value
	{
		char buf[4];
		sprintf(buf, "%3d", humidistat.controlValue);
		liquidCrystal.setCursor(7, 1);
		liquidCrystal.print(buf);
	}

	// Active status
	liquidCrystal.setCursor(15, 1);
	liquidCrystal.print((int) humidistat.active);
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
			if(value < max)
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
			if(value <= max - 10)
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
	if(millis() % (2*blinkInterval) > blinkInterval) {
		liquidCrystal.print(buf);
	} else {
		// Create char array of spaces with same length as buf
		size_t len = strlen(buf);
		char clrBuf[len+1];
		memset(clrBuf, ' ', len);
		clrBuf[len+1] = '\0';

		liquidCrystal.print(clrBuf);
	}
}
