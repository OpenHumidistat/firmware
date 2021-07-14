#include <Arduino.h>
#include "ButtonReader.h"

ButtonReader::ButtonReader(uint8_t pin_btn, VoltLadder *voltLadder) : voltLadder(*voltLadder), pin_btn(pin_btn) {}

Buttons ButtonReader::read() const {
	return voltLadder.voltageToButton(analogRead(pin_btn));
}
