#ifndef HUMIDISTAT_CONTROLLERUI_H
#define HUMIDISTAT_CONTROLLERUI_H

#include <stdint.h>
#include <Array.h>
#include <Print.h>

#include CONFIG_HEADER
#include "input/ButtonReader.h"
#include "sensor/ThermistorReader.h"
#include "asprintf.h"

/// User interface (display and input) for humidistat.
/// Hold references to ButtonReader for keypad input, and Humidistat for updating the humidity setpoint.
class ControllerUI {
private:
	Print &display;
	const ButtonReader &buttonReader;

	unsigned long lastPressed = 0; //!< Last time a keypress event occurred (in millis)

	const uint16_t buttonDebounceInterval = config::buttonDebounceInterval;
	const uint16_t inputInterval = config::inputInterval;
	const uint16_t blinkInterval = config::blinkInterval;
	const uint16_t splashDuration = config::splashDuration;
	const uint16_t infoDuration = config::infoDuration;

	bool splashDrawn = false;
	bool infoDrawn = false;
	bool screenCleared = false;

	/// Draw main interface (main loop).
	virtual void draw() = 0;
	/// Draw splash screen.
	virtual void drawSplash() = 0;
	/// Draw info screen.
	virtual void drawInfo() = 0;

	/// Clear screen.
	virtual void clear() = 0;
	/// Set cursor to coordinates.
	/// \param col LCD column
	/// \param row LCD row
	virtual void setCursor(uint8_t col, uint8_t row) = 0;

	/// Handle input.
	/// \param state Keypad state
	/// \param pressedFor Duration the key has been pressed (in millis)
	/// \return 1 if button was pressed, 0 if not
	virtual bool handleInput(Buttons state, uint16_t pressedFor) = 0;

protected:
	Array<const ThermistorReader*, 4> trs;

	unsigned long lastRefreshed = 0; //!< Last time display was updated (in millis)

	const uint16_t refreshInterval = config::refreshInterval;
	const uint8_t adjustStep = config::adjustStep;
	const uint8_t tolerance = config::tolerance;

	/// Constructor.
	/// \param display      Pointer to a Print instance
	/// \param buttonReader Pointer to a ButtonReader instance
	/// \param humidistat   Pointer to a Humidistat instance
	/// \param trs          Array of 4 pointers to ThermistorReader instances
	explicit ControllerUI(Print *display, const ButtonReader *buttonReader, Array<const ThermistorReader *, 4> trs);

	/// Print blinking text.
	/// \param col LCD column
	/// \param row LCD row
	/// \param buf Buffer of text to blink
	void blink(uint8_t col, uint8_t row, const char *buf);

	/// Print temperature read from thermistors. Handles NaN values as 0
	/// \param col LCD column
	/// \param row LCD row
	/// \param i   ThermistorReader index
	void printNTC(uint8_t col, uint8_t row, uint8_t i);

	/// In-/de-crement a variable, while clipping it to [min, max].
	/// \param delta  The amount by which to in-/de-crement
	/// \param value  Value to adjust
	/// \param min    Lower limit
	/// \param max    Upper limit
	static void adjustValue(int8_t delta, double &value, uint8_t min, uint8_t max);

	/// Print formatted data to display, at (col, row). Calculates lengths and creates appropriate buffer internally.
	/// \param col  LCD column
	/// \param row  LCD row
	/// \param fmt  Format string
	/// \param args Arguments specifying data to print
	template <typename... T>
	void printf(uint8_t col, uint8_t row, const char *fmt, T... args) {
		char *buf = asprintf(fmt, args...);

		setCursor(col, row);
		display.print(buf);

		delete buf;
	}

public:
	/// Initialize the display.
	virtual void begin() = 0;

	/// Update the display and handle input: set Humidistat's setpoint
	void update();
};

#endif //HUMIDISTAT_CONTROLLERUI_H
