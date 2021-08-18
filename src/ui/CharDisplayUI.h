#ifndef HUMIDISTAT_CHARDISPLAYUI_H
#define HUMIDISTAT_CHARDISPLAYUI_H

#include <LiquidCrystal.h>
#include "ControllerUI.h"
#include "control/SingleHumidistat.h"

/// TUI for 16x2 character LCD.
/// Holds a reference to a LiquidCrystal instance for writing to display.
/// Displays current mode, PV, SP, and CV the first line of the display, and temperatures on the second line.
/// Use keypad to adjust setpoint (UP/DOWN for fine, LEFT/RIGHT for coarse).
class CharDisplayUI : public ControllerUI {
private:
	LiquidCrystal& liquidCrystal;
	SingleHumidistat& humidistat;

	void draw() override;
	void drawSplash() override;
	void drawInfo() override;
	void clear() override;
	void setCursor(uint8_t col, uint8_t row) override;
	bool handleInput(Buttons button) override;

public:
	/// Contructor.
	/// \param liquidCrystal Pointer to a LiquidCrystal instance
	/// \param buttonReader  Pointer to a ButtonReader instance
	/// \param humidistat    Pointer to a SingleHumidistat instance
	/// \param trs           Array of 4 pointers to ThermistorReader instances
	explicit CharDisplayUI(LiquidCrystal *liquidCrystal, const ButtonReader *buttonReader, SingleHumidistat *humidistat,
	                       Array<const ThermistorReader *, 4> trs);

	void begin() override;
};


#endif //HUMIDISTAT_CHARDISPLAYUI_H
