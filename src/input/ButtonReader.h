#ifndef HUMIDISTAT_BUTTONREADER_H
#define HUMIDISTAT_BUTTONREADER_H

#include <stdint.h>

#include "aliases.h"
#include "Buttons.h"

/// Read button state from a voltage ladder-style keypad.
class ButtonReader {
private:
	const VoltLadder &voltLadder; //!< Reference to a voltLadder instance
	uint8_t pin_btn;              //!< Pin corresponding to the keypad

	Buttons lastState = Buttons::NONE; //!< Last sampled state of the keypad
	unsigned long pressedSince = 0;    //!< Time of the start of the keypress (in micros)
public:
	/// Constructor.
	/// \param pin_btn    Pin corresponding to the keypad
	/// \param voltLadder Pointer to VoltLadder instance
	explicit ButtonReader(uint8_t pin_btn, const VoltLadder *voltLadder);

	/// Sample the button signal. Call this in the main loop.
	void sample();

	/// Get the button state
	/// \return instance of Buttons corresponding to the currently pressed button
	Buttons isPressed() const;

	/// Get the duration the key has been pressed. This can be used for debouncing using a stable interval.
	/// \return keypress duration (in micros)
	uint32_t getPressedFor() const;
};

#endif //HUMIDISTAT_BUTTONREADER_H
