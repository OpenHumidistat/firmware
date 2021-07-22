#ifndef HUMIDISTAT_GRAPHICALDISPLAYUI_H
#define HUMIDISTAT_GRAPHICALDISPLAYUI_H

#include <U8g2lib.h>

#include "ControllerUI.h"
#include "EEPROMConfig.h"

/// Tab definitions
enum class Tab {
	main,
	config,
	_count,
};

/// Definitions of parameters in config tab
enum class ConfigPar {
	Kp,
	Ki,
	Kd,
	dt,
	lowValue,
	_count,
};

/// TUI for 128*64 px graphical display using U8g2.
/// Holds a reference to a U8g2lib instance for writing to display.
class GraphicalDisplayUI : public ControllerUI {
private:
	U8G2 &u8g2;
	EEPROMConfig &eepromConfig;

	Tab currentTab = Tab::main;
	ConfigPar currentPar = ConfigPar::Kp;
	uint8_t frame = 0;              //!< Frame counter (overflows, but that's OK)
	uint8_t buttonPressedFor = 0;   //!< Button press counter (in input cycles)
	uint8_t configSaveTimer = 0;    //!< Timer containing the current value of the cooldown on saving config to EEPROM

	const uint8_t longPressDuration = 3;                          //!< Duration for counting a press as 'long' =
	                                                              //!< (in input cycles)
	const uint8_t configSaveCooldown = 20 * 1000 / inputInterval; //!< Cooldown on saving the config to EEPROM
	                                                              //!< (in input cycles)

	void draw() override;
	void drawSplash() override;
	void drawInfo() override;
	void clear() override;
	void setCursor(uint8_t col, uint8_t row) override;
	bool handleInput(Buttons button) override;

	void drawMain();
	void drawConfig();
	void drawTabBar();

	bool handleInputMain(Buttons button, int8_t sign);
	bool handleInputConfig(Buttons button, int8_t sign);
public:
	/// Constructor.
	/// \param u8g2         Pointer to a U8G2 instance
	/// \param buttonReader Pointer to a ButtonReader instance
	/// \param humidistat   Pointer to a Humidistat instance
	/// \param trs          Array of 4 pointers to ThermistorReader instances
	/// \param eepromConfig Pointer to a EEPROMConfig instance
	explicit GraphicalDisplayUI(U8G2 *u8g2, const ButtonReader *buttonReader, Humidistat *humidistat,
	                            Array<const ThermistorReader *, 4> trs, EEPROMConfig *eepromConfig);

	void begin() override;
};


#endif //HUMIDISTAT_GRAPHICALDISPLAYUI_H
