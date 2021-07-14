#ifndef HUMIDISTAT_KS0256VOLTLADDER_H
#define HUMIDISTAT_KS0256VOLTLADDER_H

#include "VoltLadder.h"

class Ks0256VoltLadder : public VoltLadder {
	Buttons voltageToButton(uint16_t voltage) override;
};


#endif //HUMIDISTAT_KS0256VOLTLADDER_H
