#ifndef HUMIDISTAT_GRAPHICALDISPLAYUI_H
#define HUMIDISTAT_GRAPHICALDISPLAYUI_H

#include <U8g2lib.h>
#include <SPI.h>
#include <etl/span.h>
#include <cmath>

#include CONFIG_HEADER
#include "ConfigPar.h"
#include "advanceEnum.h"
#include "ControllerUI.h"
#include "EEPROMConfig.h"
#include "../control/SingleHumidistat.h"
#include "../control/CascadeHumidistat.h"
#include "SetpointProfileRunner.h"

/// TUI for 128*64 px graphical display using U8g2.
/// Holds references to a U8g2lib instance for writing to display, an EEPROMConfig instance to edit the config, and
/// to a Humidistat instance to show/edit its state.
/// \tparam Humidistat_t Either SingleHumidistat or CascadeHumidistat
template<class Humidistat_t>
class GraphicalDisplayUI : public ControllerUI {
private:
	/// Tab definitions
	enum class Tab {
		main,
		info,
		config,
		_last = config,
	};

	/// Config tab selection definitions
	enum class Selection {
		par,
		number,
		actions,
	};

	/// Config tab action definitions
	enum class Action {
		save,
		reset,
		_last = reset
	};

	U8G2 &u8g2;
	EEPROMConfig &eepromConfig;
	Humidistat_t &humidistat;
	SetpointProfileRunner &spr;

	// States
	Tab currentTab = Tab::main;     //!< Currently active tab
	uint8_t currentPar = 0;         //!< Currently active config parameter
	Selection currentSelection = Selection::par;
	Action currentAction = Action::save;
	uint8_t currentDigit = 3;

	uint8_t frame = 0;              //!< Frame counter (overflows, but that's OK)
	uint8_t configSaveTimer = 0;    //!< Timer containing the current value of the cooldown on saving config to EEPROM

	const uint16_t longPressDuration = config::longPressDuration;
	const uint8_t configSaveCooldown = config::configSaveCooldown;

	const uint8_t nConfigPars;     //!< Total number of config parameters
	const ConfigPar configPars[12]; //!< Array of config parameters

	/// Draw the Main tab
	// (declaration, implementation specialised)
	void drawMain();

	void drawTabInfo() {
		u8g2.setFont(u8g2_font_6x12_tr);

		// Tab bar
		u8g2.drawBox(12, 1, 26, 12);
		u8g2.setDrawColor(0);
		u8g2.drawStr(13, 10, "Info");
		u8g2.setDrawColor(1);

		u8g2.drawStr(2, 10, "C.");
		u8g2.drawStr(40, 10, "C.");

		// Temperature box
		u8g2.drawVLine(67, 27, 28);
		u8g2.drawStr(0, 23, "Temperatures");
		u8g2.drawHLine(0, 26, 128);
		u8g2.drawStr(0, 35, "Chamber");
		printf(70, 35, "%3.1f", humidistat.getTemperature());
		u8g2.drawStr(0, 43, "Thermistors");
		u8g2.drawHLine(68, 44, 60);

		// Thermistors
		for (size_t i = 0; i < trs.size(); ++i) {
			printf(70 + 15*i, 43, "%1u", i + 1);
			printNTC(70 + 15*i, 53, i);
		}

		// Bottom bar
		u8g2.drawHLine(0, 54, 128);
		u8g2.setFont(u8g2_font_unifont_t_75);
		u8g2.drawGlyph(0, 66, 9664);
		u8g2.setFont(u8g2_font_6x12_tr);
		u8g2.drawStr(10, 62, "tab");
	}

	/// Draw the Config tab
	void drawConfig() {
		u8g2.setFont(u8g2_font_6x12_tr);

		// Tab bar
		u8g2.drawBox(25, 1, 38, 12);
		u8g2.setDrawColor(0);
		u8g2.drawStr(26, 10, "Config");
		u8g2.setDrawColor(1);

		u8g2.drawStr(2, 10, "C.");
		u8g2.drawVLine(12, 1, 12);
		u8g2.drawStr(14, 10, "I.");

		// Print config parameters in scrolling menu
		for (uint8_t i = 0; i < 3; i++) {
			// Index of parameter to draw
			int8_t nPar = currentPar + i - 1;

			// Handling for first and last parameter
			if (currentPar == 0) nPar++;
			if (currentPar == nConfigPars - 1) nPar--;

			uint8_t row = 22 + i * 10;

			char *buf = configPars[nPar].asprint();
			u8g2.drawStr(0, row, buf);
			delete buf;

			if (currentSelection != Selection::actions) {
				uint8_t x, w;
				// Draw cursor on active parameter/digit
				if (currentSelection == Selection::par) {
					x = 0;
					w = 40;
				} else if (currentSelection == Selection::number) {
					x = 60 + currentDigit * 6;
					// Skip the decimal separator
					if (configPars[currentPar].var.type == ConfigPar::ConfigParType::d &&
					    currentDigit > configPars[currentPar].magnitude())
						x += 6;
					w = 6;
				}

				if (nPar == currentPar) {
					u8g2.setDrawColor(2);
					u8g2.drawBox(x, row - 8, w, 10);
					u8g2.setDrawColor(1);
				}
			}
		}

		// Actions
		u8g2.drawStr(100, 32, "Save");
		u8g2.drawStr(100, 42, "Reset");
		if (currentSelection == Selection::actions) {
			uint8_t y;
			if (currentAction == Action::save) {
				y = 32 - 8;
			}
			if (currentAction == Action::reset) {
				y = 42 - 8;
			}

			u8g2.setDrawColor(2);
			u8g2.drawBox(100, y, 40, 10);
			u8g2.setDrawColor(1);
		}

		// Mode
		if (eepromConfig.configStore.loadedFromEEPROM)
			u8g2.drawStr(80, 10, "EEPROM");

		if (configSaveTimer != 0) {
			u8g2.drawStr(85, 22, "saved");
			u8g2.setCursor(115, 22);
			u8g2.print(configSaveTimer * refreshInterval / 1000);
		}


		// Bottom bar
		u8g2.setFont(u8g2_font_unifont_t_75);
		u8g2.drawHLine(0, 54, 128);
		u8g2.drawGlyph(0, 66, 9664);
		u8g2.drawGlyph(30, 66, 9650);
		u8g2.drawGlyph(36, 66, 9660);
		u8g2.drawGlyph(65, 66, 9654);
		u8g2.drawGlyph(95, 66, 9679);
		u8g2.setFont(u8g2_font_6x12_tr);
		if (currentSelection == Selection::par) {
			u8g2.drawStr(10, 62, "tab");
			u8g2.drawStr(45, 62, "par");
			u8g2.drawStr(75, 62, "menu");
			u8g2.drawStr(105, 62, "edit");
		}
		if (currentSelection == Selection::number) {
			u8g2.drawStr(45, 62, "adj");
			u8g2.drawStr(105, 62, "OK");
		}
		if (currentSelection == Selection::actions) {
			u8g2.drawStr(10, 62, "back");
			u8g2.drawStr(45, 62, "");
			u8g2.drawStr(75, 62, "back");
			u8g2.drawStr(105, 62, "OK");
		}
	}

	/// Draw common elements in Main tab
	void DrawMainCommon() {
		u8g2.setFont(u8g2_font_6x12_tr);

		// Tab bar
		u8g2.drawBox(1, 1, 42, 12);
		u8g2.setDrawColor(0);
		u8g2.drawStr(1, 10, "Control");
		u8g2.setDrawColor(1);

		u8g2.drawStr(44, 10, "I.");
		u8g2.drawVLine(55, 1, 12);
		u8g2.drawStr(57, 10, "C.");
		u8g2.drawVLine(70, 1, 12);

		// Humidity box
		u8g2.drawVLine(13, 27, 29);
		u8g2.drawStr(0, 23, "Humidity");
		u8g2.drawHLine(0, 26, 51);

		u8g2.drawStr(0, 35, "PV");

		if (humidistat.active) {
			u8g2.drawBox(0, 36, 13, 9);
			u8g2.setDrawColor(0);
		}
		{
			char buf[] = "SP";
			if (abs(humidistat.pv - humidistat.sp) > tolerance * 100) {
				blink(0, 44, buf);
			} else {
				u8g2.drawStr(0, 44, buf);
			}
		}
		u8g2.setDrawColor(1);

		printf(14, 35, "%5.1f%%", humidistat.getHumidity());
		printf(14, 44, "%5.1f%%", humidistat.sp);

		// CV
		if (!humidistat.active) {
			u8g2.drawBox(0, 45, 13, 9);
			u8g2.setDrawColor(0);
		}
		u8g2.drawStr(0, 53, "CV");
		u8g2.setDrawColor(1);
		printf(20, 53, "%3.0f%%", humidistat.cv * 100);

		// Mode
		if (humidistat.active)
			u8g2.drawStr(80, 10, "auto");
		else
			u8g2.drawStr(80, 10, "manual");

		// Bottom bar
		u8g2.drawHLine(0, 54, 128);
		u8g2.setFont(u8g2_font_unifont_t_75);
		u8g2.drawGlyph(0, 66, 9664);
		u8g2.drawGlyph(40, 66, 9650);
		u8g2.drawGlyph(50, 66, 9660);
		u8g2.drawGlyph(90, 66, 9679);
		u8g2.setFont(u8g2_font_6x12_tr);
		u8g2.drawStr(10, 62, "tab");
		u8g2.drawStr(60, 62, "adj");
		u8g2.drawStr(100, 62, "mode");
	}

	/// Draw the tab bar
	void drawTabBar() {
		u8g2.setFont(u8g2_font_6x12_tr);
		u8g2.drawFrame(0, 0, 128, 14);

		// Spinning indicator
		u8g2.setFont(u8g2_font_unifont_t_75);
		uint8_t i = (frame / 2) % 4;
		u8g2.drawGlyph(118, 10, 0x25f3 - i);
	}

	bool handleInput(Buttons state, uint16_t pressedFor) override {
		// First handle common input actions between tabs
		if (state == Buttons::NONE) {
			return false;
		}

		// Tab-specific handling
		switch (currentTab) {
			case Tab::main:
				return handleInputMain(state, pressedFor);
			case Tab::info:
				return handleInputInfo(state, pressedFor);
			case Tab::config:
				return handleInputConfig(state, pressedFor);
		}
	}

	/// Handle input on the Main tab
	bool handleInputMain(Buttons state, uint16_t pressedFor) {
		int8_t delta = 0;
		if (state == Buttons::LEFT) {
			advanceEnum(currentTab);
			return true;
		} else if (state == Buttons::RIGHT) {
			spr.toggle();
		} else if (state == Buttons::UP) {
			delta = 1;
		} else if (state == Buttons::DOWN) {
			delta = -1;
		} else if (state == Buttons::SELECT) {
			// Toggle active state
			humidistat.active = !humidistat.active;
			return true;
		}

		// Long press coarse adjustment
		if (pressedFor > longPressDuration)
			delta *= adjustStep;
		if (pressedFor > longPressDuration * 10)
			delta *= 10;

		if (humidistat.active) {
			adjustValue(delta, humidistat.sp, 0, 100);
		} else {
			adjustValue(delta*0.01, humidistat.cv, humidistat.getCvMin(), humidistat.getCvMax());
		}
		return true;
	}

	bool handleInputInfo(Buttons state, uint16_t pressedFor) {
		if (state == Buttons::LEFT) {
			advanceEnum(currentTab);
			return true;
		}
		return false;
	}

	/// Handle input on the Config tab
	bool handleInputConfig(Buttons state, uint8_t pressedFor) {
		// Ugly state machine below, maybe refactor?
		if (currentSelection == Selection::par) {
			if (state == Buttons::SELECT) {
				currentSelection = Selection::number;
				return true;
			}
			if (state == Buttons::LEFT) {
				advanceEnum(currentTab);
				return true;
			}
			if (state == Buttons::RIGHT) {
				currentSelection = Selection::actions;
				return true;
			}
			if (state == Buttons::UP) {
				// Go up in parameter list
				currentPar = currentPar - 1 % nConfigPars;
				// Handle wrap-around
				if (currentPar == 255) currentPar = nConfigPars - 1;
				return true;
			}
			if (state == Buttons::DOWN) {
				// Go down in parameter list
				currentPar = (currentPar + 1) % nConfigPars;
				return true;
			}
		} else if (currentSelection == Selection::number) {
			// Move selected digit left/right
			if (state == Buttons::LEFT) {
				currentDigit--;
				if (currentDigit == 255) currentDigit = 3;
				return true;
			}
			if (state == Buttons::RIGHT) {
				currentDigit = (currentDigit + 1) % 4;
				return true;
			}
			// Go back to parameter selection
			if (state == Buttons::SELECT) {
				currentSelection = Selection::par;
				return true;
			}
			// Adjust digit up/down
			if (state == Buttons::UP) {
				configPars[currentPar].adjust(ipow(10, 3 - currentDigit));
				return true;
			}
			if (state == Buttons::DOWN) {
				configPars[currentPar].adjust(-ipow(10, 3 - currentDigit));
				return true;
			}
		} else if (currentSelection == Selection::actions) {
			if (state == Buttons::LEFT || state == Buttons::RIGHT) {
				currentSelection = Selection::par;
				return true;
			}
			if (state == Buttons::UP || state == Buttons::DOWN) {
				advanceEnum(currentAction);
				return true;
			}
			if (state == Buttons::SELECT) {
				humidistat.updatePIDParameters();
				if (currentAction == Action::save) {
					if (configSaveTimer == 0) {
						eepromConfig.save();
						configSaveTimer = configSaveCooldown;
					}
					return true;
				}
				if (currentAction == Action::reset) {
					eepromConfig.reset();
					return true;
				}
			}
		}
	}

	void draw() override {
		lastRefreshed = millis();

		u8g2.clearBuffer();
		drawTabBar();
		switch (currentTab) {
			case Tab::main:
				drawMain();
				break;
			case Tab::info:
				drawTabInfo();
				break;
			case Tab::config:
				drawConfig();
				break;
		}
		u8g2.sendBuffer();

		// Keep track of frames
		frame++;

		// Decrement cooldown timer
		if (configSaveTimer != 0)
			configSaveTimer--;
	}

	void drawSplash() override {
		u8g2.setFont(u8g2_font_helvB12_tr);
		u8g2.drawStr(0, 24, "OpenHumidistat");

		u8g2.setFont(u8g2_font_6x12_tr);
		u8g2.drawStr(0, 40, "Lars Veldscholte");

		u8g2.setFont(u8g2_font_5x7_tr);
		u8g2.drawStr(0, 50, "https://github.com/");
		u8g2.drawStr(0, 60, "Compizfox/OpenHumidistat");

		u8g2.sendBuffer();
	}

	void drawInfo() override {}

	void clear() override {
		u8g2.clear();
	}

	void setCursor(uint8_t col, uint8_t row) override {
		u8g2.setCursor(col, row);
	}

public:
	///@{
	/// Constructor.
	/// \param u8g2         Pointer to a U8G2 instance
	/// \param buttonReader Pointer to a ButtonReader instance
	/// \param humidistat   Pointer to a Humidistat instance
	/// \param trs          Array of 4 pointers to ThermistorReader instances
	/// \param eepromConfig Pointer to a EEPROMConfig instance
	explicit GraphicalDisplayUI(U8G2 *u8g2, const ButtonReader *buttonReader, SingleHumidistat *humidistat,
	                            etl::span<const ThermistorReader, 4> trs, EEPROMConfig *eepromConfig,
								SetpointProfileRunner *spr)
			: ControllerUI(u8g2, buttonReader, trs), u8g2(*u8g2), eepromConfig(*eepromConfig),
			  humidistat(*humidistat), spr(*spr), nConfigPars(5), configPars{
					{&eepromConfig->configStore.HC_Kp,      "Kp"},
					{&eepromConfig->configStore.HC_Ki,      "Ki"},
					{&eepromConfig->configStore.HC_Kd,      "Kd"},
					{&eepromConfig->configStore.dt,         "dt"},
					{&eepromConfig->configStore.S_lowValue, "LV"},
			} {}

	explicit GraphicalDisplayUI(U8G2 *u8g2, const ButtonReader *buttonReader, CascadeHumidistat *humidistat,
	                            etl::span<const ThermistorReader, 4> trs, EEPROMConfig *eepromConfig,
			                    SetpointProfileRunner *spr)
			: ControllerUI(u8g2, buttonReader, trs), u8g2(*u8g2), eepromConfig(*eepromConfig),
			  humidistat(*humidistat), spr(*spr), nConfigPars(12), configPars{
					{&eepromConfig->configStore.HC_Kp, "HC Kp"},
					{&eepromConfig->configStore.HC_Ki, "HC Ki"},
					{&eepromConfig->configStore.HC_Kd, "HC Kd"},
					{&eepromConfig->configStore.HC_Kf, "HC Kf"},
					{&eepromConfig->configStore.FC_Kp, "FC Kp"},
					{&eepromConfig->configStore.FC_Ki, "FC Ki"},
					{&eepromConfig->configStore.FC_Kd, "FC Kd"},
					{&eepromConfig->configStore.FC_Kf, "FC Kf"},
					{&eepromConfig->configStore.FC_dt, "FC dt"},
					{&eepromConfig->configStore.HC_totalFlowrate, "Total FR"},
					{&eepromConfig->configStore.dt, "dt"},
					{&eepromConfig->configStore.S_lowValue, "LV"},
			} {}
	///@}

	void begin() override {
		SPI.begin();
		u8g2.begin();
	}
};

template<>
void GraphicalDisplayUI<SingleHumidistat>::drawMain() {
	DrawMainCommon();

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
		printNTC(105, 23 - 9 * i, i);
	}
}

template<>
void GraphicalDisplayUI<CascadeHumidistat>::drawMain() {
	DrawMainCommon();

	// Flow box
	u8g2.drawFrame(50, 13, 65, 42);
	u8g2.drawStr(55, 23, "F");

	{
		char buf[] = "wet";
		if (std::abs(humidistat.getInner(0)->pv - humidistat.getInner(0)->sp) > tolerance * 2) {
			blink(66, 23, buf);
		} else {
			u8g2.drawStr(66, 23, buf);
		}
	}
	{
		char buf[] = "dry";
		if (std::abs(humidistat.getInner(1)->pv - humidistat.getInner(1)->sp) > tolerance * 2) {
			blink(91, 23, buf);
		} else {
			u8g2.drawStr(91, 23, buf);
		}
	}

	u8g2.drawHLine(50, 26, 64);
	u8g2.drawVLine(64, 13, 42);
	u8g2.drawVLine(89, 13, 42);

	u8g2.drawStr(52, 35, "PV");
	u8g2.drawStr(52, 44, "CV");

	printf(65, 35, "%3.2f", humidistat.getInner(0)->pv);
	printf(65, 44, "%3.0f%%", humidistat.getInner(0)->cv * 100);
	printf(90, 35, "%3.2f", humidistat.getInner(1)->pv);
	printf(90, 44, "%3.0f%%", humidistat.getInner(1)->cv * 100);
}

#endif //HUMIDISTAT_GRAPHICALDISPLAYUI_H
