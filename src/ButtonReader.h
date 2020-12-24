#ifndef HUMIDISTAT_BUTTONREADER_H
#define HUMIDISTAT_BUTTONREADER_H

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

/// Read button state from Ks0256 LCD keypad shield.
class ButtonReader {
private:
	uint8_t pin_btn; //!< Pin corresponding to the keypad
public:
	/// Constructor
	/// \param pin_btn Pin corresponding to the keypad
	explicit ButtonReader(uint8_t pin_btn);

	/// Read the button state.
	/// \return instance of Buttons corresponding to the pressed button
	Buttons read() const;
};

#endif //HUMIDISTAT_BUTTONREADER_H
