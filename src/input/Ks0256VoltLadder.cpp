#include "Ks0256VoltLadder.h"

Buttons Ks0256VoltLadder::voltageToButton(uint16_t voltage) const {
	if (voltage > 1000) return Buttons::NONE;
	if (voltage < 50)   return Buttons::RIGHT;
	if (voltage < 150)  return Buttons::UP;
	if (voltage < 300)  return Buttons::DOWN;
	if (voltage < 500)  return Buttons::LEFT;
	if (voltage < 750)  return Buttons::SELECT;

	return Buttons::NONE;
}
