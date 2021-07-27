#include <SPI.h>
#include "GraphicalDisplayUI.h"
#include "advanceEnum.h"

GraphicalDisplayUI::GraphicalDisplayUI(U8G2 *u8g2, const ButtonReader *buttonReader, Humidistat *humidistat,
                                       Array<const ThermistorReader *, 4> trs, EEPROMConfig *eepromConfig) :
		ControllerUI(u8g2, buttonReader, humidistat, trs), u8g2(*u8g2), eepromConfig(*eepromConfig) {}

void GraphicalDisplayUI::begin() {
	SPI.begin();
	u8g2.begin();
}

void GraphicalDisplayUI::draw() {
	u8g2.clearBuffer();
	drawTabBar();
	switch(currentTab) {
		case Tab::main:
			drawMain();
			break;
		case Tab::config:
			drawConfig();
			break;
	}
	u8g2.sendBuffer();
	frame++;
}

void GraphicalDisplayUI::drawSplash() {
	u8g2.setCursor(0, 24);
	u8g2.setFont(u8g2_font_helvR14_tr);
	u8g2.print("Humidistat");
	u8g2.sendBuffer();
}

void GraphicalDisplayUI::drawInfo() {

}

void GraphicalDisplayUI::clear() {
	u8g2.clear();
}

void GraphicalDisplayUI::setCursor(uint8_t col, uint8_t row) {
	u8g2.setCursor(col, row);
}

void GraphicalDisplayUI::drawMain() {
	u8g2.setFont(u8g2_font_6x12_tr);

	// Humidity box
	u8g2.drawFrame(-1, 13, 52, 33);
	u8g2.drawVLine(13, 27, 19);
	u8g2.drawStr(0, 23, "Humidity");
	u8g2.drawHLine(0, 26, 51);

	u8g2.drawStr(0, 35, "PV");

	if(humidistat.active) {
		u8g2.drawBox(0, 36, 13, 9);
		u8g2.setDrawColor(0);
	}
	u8g2.drawStr(0, 44, "SP");
	u8g2.setDrawColor(1);

	{
		char buf[7];
		sprintf(buf, "%5.1f%%", humidistat.getHumidity());
		u8g2.drawStr(14, 35, buf);
	}
	{
		char buf[7];
		sprintf(buf, "%3u.0%%", humidistat.setpoint);
		u8g2.drawStr(14, 44, buf);
	}

	// CV
	if(!humidistat.active) {
		u8g2.drawBox(0, 46, 15, 9);
		u8g2.setDrawColor(0);
	}
	u8g2.drawStr(0, 54, "CV: ");
	u8g2.setDrawColor(1);
	u8g2.setCursor(20, 54);
	u8g2.print(humidistat.controlValue);

	// Mode
	if(humidistat.active)
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
	{
		char buf[7];
		sprintf(buf, "%6.2f", pTerm);
		u8g2.drawStr(62, 23, buf);
	}
	{
		char buf[7];
		sprintf(buf, "%6.2f", iTerm);
		u8g2.drawStr(62, 32, buf);
	}
	{
		char buf[7];
		sprintf(buf, "%6.2f", dTerm);
		u8g2.drawStr(62, 41, buf);
	}

	// Temperature box
	u8g2.setCursor(105, 23);
	u8g2.print(humidistat.getTemperature(), 1);

	// Thermistors
	for (size_t i = 0; i < trs.size(); ++i) {
		if(trs[i]) {
			printNTC(105, 23-9*i, i);
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

void GraphicalDisplayUI::drawConfig() {
	u8g2.setFont(u8g2_font_6x12_tr);

	if(currentPar == ConfigPar::Kp) {
		u8g2.drawBox(0, 14, 13, 9);
		u8g2.setDrawColor(0);
	}
	u8g2.drawStr(0, 22, "Kp");
	u8g2.setDrawColor(1);
	if(currentPar == ConfigPar::Ki) {
		u8g2.drawBox(0, 24, 13, 9);
		u8g2.setDrawColor(0);
	}
	u8g2.drawStr(0, 32, "Ki");
	u8g2.setDrawColor(1);
	if(currentPar == ConfigPar::Kd) {
		u8g2.drawBox(0, 34, 13, 9);
		u8g2.setDrawColor(0);
	}
	u8g2.drawStr(0, 42, "Kd");
	u8g2.setDrawColor(1);
	u8g2.drawVLine(13, 14, 29);

	{
		char buf[7];
		sprintf(buf, "%6.2f", eepromConfig.configStore.Kp);
		u8g2.drawStr(12, 22, buf);
	}
	{
		char buf[7];
		sprintf(buf, "%6.2f", eepromConfig.configStore.Ki);
		u8g2.drawStr(12, 32, buf);
	}
	{
		char buf[7];
		sprintf(buf, "%6.2f", eepromConfig.configStore.Kd);
		u8g2.drawStr(12, 42, buf);
	}

	// Mode
	if(eepromConfig.configStore.loadedFromEEPROM)
		u8g2.drawStr(80, 10, "EEPROM");

	if(configSaveTimer != 0) {
		u8g2.drawStr(85, 22, "saved");
		u8g2.setCursor(115, 22);
		u8g2.print(configSaveTimer*inputInterval/1000);
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


void GraphicalDisplayUI::drawTabBar() {
	u8g2.setFont(u8g2_font_6x12_tr);
	u8g2.drawFrame(0, 0, 128, 14);

	if(currentTab == Tab::main) {
		u8g2.drawBox(1, 1, 32, 12);
		u8g2.setDrawColor(0);
	}
	u8g2.drawStr(5, 10, "Main");
	u8g2.setDrawColor(1);

	if(currentTab == Tab::config) {
		u8g2.drawBox(32, 1, 46, 12);
		u8g2.setDrawColor(0);
	}

	u8g2.drawStr(38, 10, "Config");
	u8g2.setDrawColor(1);
	u8g2.drawVLine(78, 1, 12);

	// Spinning indicator
	u8g2.setFont(u8g2_font_unifont_t_symbols);
	uint8_t i = (frame/3) % 4;
	u8g2.drawGlyph(118, 10, 0x25f3-i);
}

bool GraphicalDisplayUI::handleInput(Buttons button) {
	// Decrement cooldown timer
	if(configSaveTimer != 0)
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
	if(buttonPressedFor > longPressDuration)
		sign *= adjustStep;

	// Tab-specific handling
	switch(currentTab) {
		case Tab::main:
			return handleInputMain(button, sign);
		case Tab::config:
			return handleInputConfig(button, sign);
	}
}

bool GraphicalDisplayUI::handleInputMain(Buttons button, int8_t sign) {
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
		adjustValue(delta, humidistat.controlValue, humidistat.getLowValue(), 255);
	}
	return true;
}

bool GraphicalDisplayUI::handleInputConfig(Buttons button, int8_t sign) {
	switch(button) {
		case Buttons::RIGHT:
			advanceEnum(currentPar);
			return true;
		case Buttons::SELECT:
			if(buttonPressedFor > longPressDuration) {
				eepromConfig.reset();
			// Check that cooldown has elapsed
			} else if(configSaveTimer == 0) {
				eepromConfig.save();
				configSaveTimer = configSaveCooldown;
			}
			humidistat.updatePIDParameters();
			return true;
	}

	double delta = 0.01 * sign;
	switch(currentPar) {
		case ConfigPar::Kp:
			eepromConfig.configStore.Kp += delta;
			return true;
		case ConfigPar::Ki:
			eepromConfig.configStore.Ki += delta;
			return true;
		case ConfigPar::Kd:
			eepromConfig.configStore.Kd += delta;
			return true;
		case ConfigPar::dt:
			eepromConfig.configStore.dt += delta;
			return true;
		case ConfigPar::lowValue:
			eepromConfig.configStore.lowValue += delta;
			return true;
	}
}