#include "Ks0466VoltLadder.h"

Buttons Ks0466VoltLadder::voltageToButton(uint16_t voltage) {
	if (voltage > 1000) return Buttons::NONE;
	if (voltage < 75)   return Buttons::LEFT;
	if (voltage < 250)  return Buttons::UP;
	if (voltage < 425)  return Buttons::DOWN;
	if (voltage < 625)  return Buttons::RIGHT;
	if (voltage < 875)  return Buttons::SELECT;

	return Buttons::NONE;
}
