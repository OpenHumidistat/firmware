#ifndef HUMIDISTAT_GRAPHICALDISPLAYUI_H
#define HUMIDISTAT_GRAPHICALDISPLAYUI_H

#include <U8g2lib.h>
#include <SPI.h>

#include "advanceEnum.h"
#include "ControllerUI.h"
#include "EEPROMConfig.h"
#include "../control/SingleHumidistat.h"

/// Tab definitions
enum class Tab {
	main,
	config,
	_last = config,
};

/// Definitions of parameters in config tab
enum class ConfigPar {
	Kp,
	Ki,
	Kd,
	dt,
	lowValue,
	_last = lowValue,
};

/// TUI for 128*64 px graphical display using U8g2.
/// Holds references to a U8g2lib instance for writing to display, an EEPROMConfig instance to edit the config, and
/// to a Humidistat instance to show/edit its state.
/// \tparam Humidistat_t Either SingleHumidistat or CascadeHumidistat
template<class Humidistat_t>
class GraphicalDisplayUI : public ControllerUI {
private:
	U8G2 &u8g2;
	EEPROMConfig &eepromConfig;
	Humidistat_t &humidistat;

	Tab currentTab = Tab::main;
	ConfigPar currentPar = ConfigPar::Kp;
	uint8_t frame = 0;              //!< Frame counter (overflows, but that's OK)
	uint8_t buttonPressedFor = 0;   //!< Button press counter (in input cycles)
	uint8_t configSaveTimer = 0;    //!< Timer containing the current value of the cooldown on saving config to EEPROM

	const uint8_t longPressDuration = 3;                          //!< Duration for counting a press as 'long'
	                                                              //!< (in input cycles)
	const uint8_t configSaveCooldown = 20 * 1000 / inputInterval; //!< Cooldown on saving the config to EEPROM
	                                                              //!< (in input cycles)

	void draw() override {
		lastRefreshed = millis();
		u8g2.clearBuffer();
		drawTabBar();
		switch (currentTab) {
			case Tab::main:
				drawMain(humidistat);
				break;
			case Tab::config:
				drawConfig(humidistat);
				break;
		}
		u8g2.sendBuffer();
		frame++;
	}

	void drawSplash() override {
		u8g2.setCursor(0, 24);
		u8g2.setFont(u8g2_font_helvR14_tr);
		u8g2.print("Humidistat");
		u8g2.sendBuffer();
	}

	void drawInfo() override {

	}

	void clear() override {
		u8g2.clear();
	}

	void setCursor(uint8_t col, uint8_t row) override {
		u8g2.setCursor(col, row);
	}

	bool handleInput(Buttons button) override {
		// Decrement cooldown timer
		if (configSaveTimer != 0)
			configSaveTimer--;

		// First handle common input actions between tabs
		int8_t sign;
		switch (button) {
			case Buttons::NONE:
				// Nothing pressed; reset button press counter
				buttonPressedFor = 0;
				return false;
			case Buttons::UP:
				sign = 1;
				break;
			case Buttons::DOWN:
				sign = -1;
				break;
			case Buttons::LEFT:
				advanceEnum(currentTab);
				return true;
		}
		buttonPressedFor++;
		if (buttonPressedFor > longPressDuration)
			sign *= adjustStep;

		// Tab-specific handling
		switch (currentTab) {
			case Tab::main:
				return handleInputMain(button, sign);
			case Tab::config:
				return handleInputConfig(button, sign);
		}
	}

	void drawMain(SingleHumidistat &humidistat) {
		u8g2.setFont(u8g2_font_6x12_tr);

		// Humidity box
		u8g2.drawFrame(-1, 13, 52, 33);
		u8g2.drawVLine(13, 27, 19);
		u8g2.drawStr(0, 23, "Humidity");
		u8g2.drawHLine(0, 26, 51);

		u8g2.drawStr(0, 35, "PV");

		if (humidistat.active) {
			u8g2.drawBox(0, 36, 13, 9);
			u8g2.setDrawColor(0);
		}
		u8g2.drawStr(0, 44, "SP");
		u8g2.setDrawColor(1);

		printf(14, 35, "%5.1f%%", humidistat.getHumidity());
		printf(14, 44, "%3u.0%%", humidistat.setpoint);

		// CV
		if (!humidistat.active) {
			u8g2.drawBox(0, 46, 15, 9);
			u8g2.setDrawColor(0);
		}
		u8g2.drawStr(0, 54, "CV: ");
		u8g2.setDrawColor(1);
		u8g2.setCursor(20, 54);
		u8g2.print(humidistat.controlValue);

		// Mode
		if (humidistat.active)
			u8g2.drawStr(80, 10, "auto");
		else
			u8g2.drawStr(80, 10, "manual");

		// PID box
		double pTerm, iTerm, dTerm;
		humidistat.getTerms(pTerm, iTerm, dTerm);

		u8g2.drawFrame(52, 13, 47, 33);
		u8g2.drawStr(54, 23, "P");
		u8g2.drawStr(54, 32, "I");
		u8g2.drawStr(54, 41, "D");
		u8g2.drawVLine(60, 13, 33);

		printf(62, 23, "%6.2f", pTerm);
		printf(62, 32, "%6.2f", iTerm);
		printf(62, 41, "%6.2f", dTerm);

		// Temperature box
		u8g2.setCursor(105, 23);
		u8g2.print(humidistat.getTemperature(), 1);

		// Thermistors
		for (size_t i = 0; i < trs.size(); ++i) {
			if (trs[i]) {
				printNTC(105, 23 - 9 * i, i);
			}
		}

		// Bottom bar
		u8g2.drawHLine(0, 54, 128);
		u8g2.setFont(u8g2_font_unifont_t_symbols);
		u8g2.drawGlyph(0, 66, 9664);
		u8g2.drawGlyph(40, 66, 9650);
		u8g2.drawGlyph(50, 66, 9660);
		u8g2.drawGlyph(90, 66, 9679);
		u8g2.setFont(u8g2_font_6x12_tr);
		u8g2.drawStr(10, 62, "tab");
		u8g2.drawStr(60, 62, "adj");
		u8g2.drawStr(100, 62, "mode");
	}

	void drawConfig(SingleHumidistat &humidistat) {
		u8g2.setFont(u8g2_font_6x12_tr);

		if (currentPar == ConfigPar::Kp) {
			u8g2.drawBox(0, 14, 13, 9);
			u8g2.setDrawColor(0);
		}
		u8g2.drawStr(0, 22, "Kp");
		u8g2.setDrawColor(1);
		if (currentPar == ConfigPar::Ki) {
			u8g2.drawBox(0, 24, 13, 9);
			u8g2.setDrawColor(0);
		}
		u8g2.drawStr(0, 32, "Ki");
		u8g2.setDrawColor(1);
		if (currentPar == ConfigPar::Kd) {
			u8g2.drawBox(0, 34, 13, 9);
			u8g2.setDrawColor(0);
		}
		u8g2.drawStr(0, 42, "Kd");
		u8g2.setDrawColor(1);
		u8g2.drawVLine(13, 14, 29);

		printf(12, 22, "%6.2f", eepromConfig.configStore.HC_Kp);
		printf(12, 32, "%6.2f", eepromConfig.configStore.HC_Ki);
		printf(12, 42, "%6.2f", eepromConfig.configStore.HC_Kd);

		// Mode
		if (eepromConfig.configStore.loadedFromEEPROM)
			u8g2.drawStr(80, 10, "EEPROM");

		if (configSaveTimer != 0) {
			u8g2.drawStr(85, 22, "saved");
			u8g2.setCursor(115, 22);
			u8g2.print(configSaveTimer * inputInterval / 1000);
		}

		// Bottom bar
		u8g2.setFont(u8g2_font_unifont_t_symbols);
		u8g2.drawGlyph(0, 66, 9664);
		u8g2.drawGlyph(30, 66, 9650);
		u8g2.drawGlyph(36, 66, 9660);
		u8g2.drawGlyph(65, 66, 9654);
		u8g2.drawGlyph(95, 66, 9679);
		u8g2.setFont(u8g2_font_6x12_tr);
		u8g2.drawStr(10, 62, "tab");
		u8g2.drawStr(45, 62, "adj");
		u8g2.drawStr(75, 62, "par");
		u8g2.drawStr(105, 62, "save");
	}

	void drawTabBar() {
		u8g2.setFont(u8g2_font_6x12_tr);
		u8g2.drawFrame(0, 0, 128, 14);

		if (currentTab == Tab::main) {
			u8g2.drawBox(1, 1, 32, 12);
			u8g2.setDrawColor(0);
		}
		u8g2.drawStr(5, 10, "Main");
		u8g2.setDrawColor(1);

		if (currentTab == Tab::config) {
			u8g2.drawBox(32, 1, 46, 12);
			u8g2.setDrawColor(0);
		}

		u8g2.drawStr(38, 10, "Config");
		u8g2.setDrawColor(1);
		u8g2.drawVLine(78, 1, 12);

		// Spinning indicator
		u8g2.setFont(u8g2_font_unifont_t_symbols);
		uint8_t i = (frame / 2) % 4;
		u8g2.drawGlyph(118, 10, 0x25f3 - i);
	}

	bool handleInputMain(Buttons button, int8_t sign) {
		switch (button) {
			case Buttons::SELECT:
				// Toggle active state
				humidistat.active = !humidistat.active;
				return true;
		}
		int8_t delta = sign;
		if (humidistat.active) {
			adjustValue(delta, humidistat.setpoint, 0, 100);
		} else {
			adjustValue(delta, humidistat.controlValue, eepromConfig.configStore.S_lowValue, 255);
		}
		return true;
	}

	bool handleInputConfig(Buttons button, int8_t sign) {
		switch (button) {
			case Buttons::RIGHT:
				advanceEnum(currentPar);
				return true;
			case Buttons::SELECT:
				if (buttonPressedFor > longPressDuration) {
					eepromConfig.reset();
					// Check that cooldown has elapsed
				} else if (configSaveTimer == 0) {
					eepromConfig.save();
					configSaveTimer = configSaveCooldown;
				}
				humidistat.updatePIDParameters();
				return true;
		}

		double delta = 0.01 * sign;
		switch (currentPar) {
			case ConfigPar::Kp:
				eepromConfig.configStore.HC_Kp += delta;
				return true;
			case ConfigPar::Ki:
				eepromConfig.configStore.HC_Ki += delta;
				return true;
			case ConfigPar::Kd:
				eepromConfig.configStore.HC_Kd += delta;
				return true;
			case ConfigPar::dt:
				eepromConfig.configStore.dt += static_cast<uint16_t>(delta);
				return true;
			case ConfigPar::lowValue:
				eepromConfig.configStore.S_lowValue += static_cast<uint8_t>(delta);
				return true;
		}
	}

public:
	/// Constructor.
	/// \param u8g2         Pointer to a U8G2 instance
	/// \param buttonReader Pointer to a ButtonReader instance
	/// \param humidistat   Pointer to a Humidistat instance
	/// \param trs          Array of 4 pointers to ThermistorReader instances
	/// \param eepromConfig Pointer to a EEPROMConfig instance
	explicit GraphicalDisplayUI(U8G2 *u8g2, const ButtonReader *buttonReader, Humidistat_t *humidistat,
	                            Array<const ThermistorReader *, 4> trs, EEPROMConfig *eepromConfig)
			: ControllerUI(u8g2, buttonReader, trs), u8g2(*u8g2), eepromConfig(*eepromConfig),
			  humidistat(*humidistat) {}

	void begin() override {
		SPI.begin();
		u8g2.begin();
	}
};


#endif //HUMIDISTAT_GRAPHICALDISPLAYUI_H
