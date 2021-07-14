#ifndef HUMIDISTAT_VOLTLADDER_H
#define HUMIDISTAT_VOLTLADDER_H

#include <stdint.h>

/// Possible button values
enum class Buttons {
	NONE,
	RIGHT,
	UP,
	DOWN,
	LEFT,
	SELECT,
};

/// Interface for a 5-button voltage ladder keypad
class VoltLadder {
public:
	virtual Buttons voltageToButton(uint16_t voltage) = 0;
};


#endif //HUMIDISTAT_VOLTLADDER_H
