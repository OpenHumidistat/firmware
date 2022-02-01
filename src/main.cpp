#include <Arduino.h>

#include CONFIG_HEADER
#include "aliases.h"

#include "EEPROMConfig.h"
#include "SerialLogger.h"
#include "input/ButtonReader.h"
#include "sensor/ThermistorReader.h"
#include "SetpointProfileRunner.h"

// Beware: Lots of preprocessor fuckery to get conditional compilation based on config settings below

// Humidity sensor
#ifdef HUMIDISTAT_DHT
DHT dht(config::PIN_DHT, DHT22);
DHTHumiditySensor hs(&dht);
#endif
#ifdef HUMIDISTAT_SHT
SHTSensor sht;
SHTHumiditySensor hs(&sht);
#endif

// Thermistors
ThermistorReader trs[] = {
		ThermistorReader(config::PIN_T1),
		ThermistorReader(config::PIN_T2),
		ThermistorReader(config::PIN_T3),
		ThermistorReader(config::PIN_T4)
};

// Input
VoltLadder voltLadder;
ButtonReader buttonReader(config::PIN_BTN, &voltLadder);

EEPROMConfig eepromConfig;

// PWM frequency and resolution: MCU-dependent
#ifdef ARDUINO_TEENSYLC
const uint8_t pwmRes = 16;
#else
const uint8_t pwmRes = 8;
#endif

// Humidity controller
#ifdef HUMIDISTAT_CONTROLLER_SINGLE
#include "control/SingleHumidistat.h"
SingleHumidistat humidistat(&hs, &eepromConfig.configStore, {{config::PIN_S1, config::PIN_S2}}, pwmRes);
using cHumidistat = SingleHumidistat;
#endif
#ifdef HUMIDISTAT_CONTROLLER_CASCADE
#include "sensor/FlowSensor.h"
#include "control/CascadeHumidistat.h"
FlowSensor flowSensors[] = {FlowSensor(config::PIN_F1), FlowSensor(config::PIN_F2)};
CascadeHumidistat humidistat(&hs, &eepromConfig.configStore, flowSensors, {config::PIN_S1, config::PIN_S2}, pwmRes);
using cHumidistat = CascadeHumidistat;
#endif

// UI
#ifdef HUMIDISTAT_UI_CHAR
#include <LiquidCrystal.h>
#include "ui/CharDisplayUI.h"
LiquidCrystal liquidCrystal(config::PIN_LCD_RS, config::PIN_LCD_ENABLE, config::PIN_LCD_D0, config::PIN_LCD_D1,
							config::PIN_LCD_D2, config::PIN_LCD_D3);
CharDisplayUI ui(&liquidCrystal, &buttonReader, &humidistat, trs);
#endif
#ifdef HUMIDISTAT_UI_GRAPH
#include <U8g2lib.h>
#include "ui/GraphicalDisplayUI.h"

const uint8_t interval = 100; // seconds
Point points[] = {
		{0, 10},
		{1*interval, 20},
		{2*interval, 30},
		{3*interval, 40},
		{4*interval, 50},
		{5*interval, 60},
		{6*interval, 65},
		{7*interval, 70},
		{8*interval, 75},
		{9*interval, 80},
		{10*interval, 85},
		{11*interval, 90},
		{12*interval, 95},
};
SetpointProfileRunner spr(&humidistat, points);

U8G2_ST7920_128X64_F_HW_SPI u8g2(U8G2_R0, config::PIN_LCD_CS);
GraphicalDisplayUI<cHumidistat> ui(&u8g2, &buttonReader, &humidistat, trs, &eepromConfig, &spr);
#endif

SerialLogger<cHumidistat> serialLogger(&humidistat, trs, eepromConfig.configStore.dt);

void setup() {
#ifdef ARDUINO_AVR_UNO
	// Set PWM frequency on D3 and D11 to 490.20 Hz
	// See: https://arduinoinfo.mywikis.net/wiki/Arduino-PWM-Frequency
	TCCR2B = TCCR2B & B11111000 | B00000100;
#endif
#ifdef ARDUINO_TEENSYLC
	// Set PWM frequency to 250 Hz
	analogWriteFrequency(config::PIN_S1, 500);
	// Increase PWM resolution from default 8-bits
	analogWriteResolution(pwmRes);
#endif

	hs.begin();
	serialLogger.begin(config::serialRate);
	ui.begin();
}

void loop() {
	buttonReader.sample();
	ui.update();
	humidistat.update();
	serialLogger.update();
#ifdef HUMIDISTAT_UI_GRAPH
	spr.update();
#endif
}
