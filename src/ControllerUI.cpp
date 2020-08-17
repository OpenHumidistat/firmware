#include "Arduino.h"
#include "ControllerUI.h"

ControllerUI::ControllerUI(LiquidCrystal *liquidCrystal, const ButtonReader *buttonReader, Humidistat *humidistat) :
		liquidCrystal(*liquidCrystal),
		buttonReader(*buttonReader),
		humidistat(*humidistat) {
	Serial.begin(9600);
	this->liquidCrystal.begin(16, 2);

	this->liquidCrystal.setCursor(0, 0);
	this->liquidCrystal.print("H");
	this->liquidCrystal.setCursor(9, 0);
	this->liquidCrystal.print("%");
	this->liquidCrystal.setCursor(11, 0);
	this->liquidCrystal.print("T");
	this->liquidCrystal.setCursor(0, 1);
	this->liquidCrystal.print("p");
	this->liquidCrystal.setCursor(0, 1);
	this->liquidCrystal.print("p");
}

void ControllerUI::update() {
	updateDisplay();
	if (millis() - lastTime > inputInterval) {
		input();
	}
}

void ControllerUI::updateDisplay() {
	// Update current humidity and temperature readings
	liquidCrystal.setCursor(1, 0);
	liquidCrystal.print(humidistat.getHumidity(), 1);

	liquidCrystal.setCursor(12, 0);
	liquidCrystal.print(humidistat.getTemperature(), 1);

	// Setpoint
	liquidCrystal.setCursor(7, 0);
	liquidCrystal.print("  ");
	liquidCrystal.setCursor(6, 0);
	liquidCrystal.print(humidistat.setpoint);

	// Error
	liquidCrystal.setCursor(4, 1);
	liquidCrystal.print("  ");
	liquidCrystal.setCursor(1, 1);
	liquidCrystal.print(humidistat.getHumidity()-humidistat.setpoint, 1);

	// Control value
	liquidCrystal.setCursor(7, 1);
	liquidCrystal.print(humidistat.getCv());
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
