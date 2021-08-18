#ifndef HUMIDISTAT_KS0466VOLTLADDER_H
#define HUMIDISTAT_KS0466VOLTLADDER_H

#include <stdint.h>

#include "Buttons.h"

/// Implementation of the VoltLadder interface for the Keyestudio Ks0466 keypad.
class Ks0466VoltLadder {
public:
	Buttons voltageToButton(uint16_t voltage);
};


#endif //HUMIDISTAT_KS0466VOLTLADDER_H
