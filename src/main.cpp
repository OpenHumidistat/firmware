#include <Arduino.h>
#include <Array.h>

#include CONFIG_HEADER
#include "aliases.h"

#include "EEPROMConfig.h"
#include "SerialLogger.h"
#include "input/ButtonReader.h"
#include "sensor/ThermistorReader.h"

// Beware: Lots of preprocessor fuckery to get conditional compilation based on config settings below

// Humidity sensor
#ifdef HUMIDISTAT_DHT
DHT dht(config::PIN_DHT, DHT22);
DHTHumiditySensor hs(&dht);
//                        NTC pins
ThermistorReader trs[] = {ThermistorReader(1), ThermistorReader(2), ThermistorReader(3), ThermistorReader(4)};
Array<ThermistorReader*, 4> trsp{{&trs[0], &trs[1], &trs[2], &trs[3]}};
#endif
#ifdef HUMIDISTAT_SHT
SHTSensor sht;
SHTHumiditySensor hs(&sht);
Array<ThermistorReader *, 4> trsp{{}};
#endif

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
SingleHumidistat humidistat(&eepromConfig.configStore, &hs, {{config::PIN_S1, config::PIN_S2}}, pwmRes);
using cHumidistat = SingleHumidistat;
#endif
#ifdef HUMIDISTAT_CONTROLLER_CASCADE
#include "sensor/FlowSensor.h"
#include "control/CascadeHumidistat.h"
FlowSensor flowSensors[] = {FlowSensor(config::PIN_F1), FlowSensor(config::PIN_F2)};
CascadeHumidistat humidistat(&hs, &eepromConfig.configStore, {{&flowSensors[0], &flowSensors[1]}},
							 {{config::PIN_S1, config::PIN_S2}}, pwmRes);
using cHumidistat = CascadeHumidistat;
#endif

// UI
#ifdef HUMIDISTAT_UI_CHAR
#include <LiquidCrystal.h>
#include "ui/CharDisplayUI.h"
LiquidCrystal liquidCrystal(config::PIN_LCD_RS, config::PIN_LCD_ENABLE, config::PIN_LCD_D0, config::PIN_LCD_D1, config::PIN_LCD_D2, config::PIN_LCD_D3);
CharDisplayUI ui(&liquidCrystal, &buttonReader, &humidistat, trsp);
#endif
#ifdef HUMIDISTAT_UI_GRAPH
#include <U8g2lib.h>
#include "ui/GraphicalDisplayUI.h"
U8G2_ST7920_128X64_F_HW_SPI u8g2(U8G2_R0, config::PIN_LCD_CS);
GraphicalDisplayUI<cHumidistat> ui(&u8g2, &buttonReader, &humidistat, trsp, &eepromConfig);
#endif

SerialLogger<cHumidistat> serialLogger(&humidistat, trsp, eepromConfig.configStore.dt);

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
}
