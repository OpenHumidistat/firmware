#ifndef HUMIDISTAT_BUTTONREADER_H
#define HUMIDISTAT_BUTTONREADER_H

#include <stdint.h>

#include "VoltLadder.h"

/// Read button state from a voltage ladder-style keypad.
class ButtonReader {
private:
	VoltLadder &voltLadder;
	uint8_t pin_btn; //!< Pin corresponding to the keypad
public:
	/// Constructor.
	/// \param pin_btn    Pin corresponding to the keypad
	/// \param voltLadder Pointer to VoltLadder instance
	explicit ButtonReader(uint8_t pin_btn, VoltLadder *voltLadder);

	/// Read the button state.
	/// \return instance of Buttons corresponding to the pressed button
	Buttons read() const;
};

#endif //HUMIDISTAT_BUTTONREADER_H
