#include <Arduino.h>
#include "ButtonReader.h"

ButtonReader::ButtonReader(uint8_t pin_btn, const VoltLadder *voltLadder) : voltLadder(*voltLadder), pin_btn(pin_btn) {}

void ButtonReader::sample() {
	Buttons state = voltLadder.voltageToButton(analogRead(pin_btn));
	if (state != lastState) {
		lastState = state;
		pressedSince = micros();
	}
}

Buttons ButtonReader::isPressed() const {
	return lastState;
}

uint32_t ButtonReader::getPressedFor() const {
	return micros() - pressedSince;
}
