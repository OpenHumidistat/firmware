#ifndef HUMIDISTAT_KS0466VOLTLADDER_H
#define HUMIDISTAT_KS0466VOLTLADDER_H

#include "VoltLadder.h"

class Ks0466VoltLadder : public VoltLadder {
	Buttons voltageToButton(uint16_t voltage) override;
};


#endif //HUMIDISTAT_KS0466VOLTLADDER_H
