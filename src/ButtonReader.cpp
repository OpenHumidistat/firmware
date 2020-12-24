#include <Arduino.h>
#include "ButtonReader.h"

ButtonReader::ButtonReader(uint8_t pin_btn) : pin_btn(pin_btn) {}

Buttons ButtonReader::read() const {
	int adc_key_in = analogRead(pin_btn);

	if (adc_key_in > 1000) return Buttons::NONE;
	if (adc_key_in < 50)   return Buttons::RIGHT;
	if (adc_key_in < 150)  return Buttons::UP;
	if (adc_key_in < 300)  return Buttons::DOWN;
	if (adc_key_in < 500)  return Buttons::LEFT;
	if (adc_key_in < 750)  return Buttons::SELECT;
	return Buttons::NONE;
}
