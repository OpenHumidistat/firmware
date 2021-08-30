#ifndef HUMIDISTAT_KS0256VOLTLADDER_H
#define HUMIDISTAT_KS0256VOLTLADDER_H

#include <stdint.h>

#include "Buttons.h"

/// Implementation of the VoltLadder interface for the Keyestudio Ks0256 keypad.
class Ks0256VoltLadder {
public:
	Buttons voltageToButton(uint16_t voltage) const;
};


#endif //HUMIDISTAT_KS0256VOLTLADDER_H
