#ifndef HUMIDISTAT_CONFIG_H
#define HUMIDISTAT_CONFIG_H

#include <Arduino.h>

/// Define either HUMIDISTAT_DHT (for DHT22/AM2302 sensors) or HUMIDISTAT_SHT (for Sensirion SHT85 sensors).
/// In the former case, the data pin of the sensor should be connected to PIN_DHT. In the latter case, the sensor
/// should be connected to the I2C bus. On the Arduino Uno, these pins (SDA and SCL) are shared with A4 and A5.
#define HUMIDISTAT_SHT

/// Define either HUMIDISTAT_UI_CHAR HUMIDISTAT_UI_GRAPH for a HD44780 16x02 character display, or a ST7920 graphical
/// display respectively. In the former case, the display must be connected to the pins specified below (PIN_LCD_x).
/// in the latter case, the display must be connected to the hardware SPI bus.
#define HUMIDISTAT_UI_GRAPH

/// Define either HUMIDISTAT_INPUT_KS0256 or HUMIDISTAT_INPUT_KS0466. In either case, the keypad must be connected to
/// PIN_BTN specified below.
#define HUMIDISTAT_INPUT_KS0466

/// Define either HUMIDISTAT_CONTROLLER_SINGLE or HUMIDISTAT_CONTROLLER_CASCADE. In the latter case, flow sensors
/// must be connected to PIN_F1 and PIN_F2.
#define HUMIDISTAT_CONTROLLER_CASCADE

namespace config {
	/// Serial communication symbol rate (baud)
	const uint32_t serialRate = 115200;

	/// Set to true to override the values stored in EEPROM and use the default PID parameters defined below.
	const bool overrideEEPROM = false;

	/// EEPROM address for storing the block
	const uint8_t EEPROMAddress = 0;

	/// Global interval for PID/logger (based on polling rate of sensor, in millis)
#ifdef HUMIDISTAT_DHT
	const uint16_t dt = 500;
#endif
#ifdef HUMIDISTAT_SHT
	const uint16_t dt = 250;
#endif

	/// @name Humidity controller PID parameters
	///@{
	const double HC_Kp = 0.01;
	const double HC_Ki = 0.001;
	const double HC_Kd = 0.01;
	const double HC_Kf = 0.01;
	///@}

	/// @name Flow controller PID parameters
	///@{
	const double FC_Kp = 0.005;
	const double FC_Ki = 0.05;
	const double FC_Kd = 0;
	const double FC_Kf = 0;
	const uint16_t FC_dt = 100;
	///@}

	/// Minimum solenoid duty cycle (deadband)
	const double S_lowValue = 0.75;

	/// Total flowrate (for cascade controller) (L/min)
	const double HC_totalFlowrate = 2;

	/// @name Pins
	///@{
	const uint8_t PIN_DHT = 2;
	const uint8_t PIN_BTN = A0;
	const uint8_t PIN_S1 = 3;
	const uint8_t PIN_S2 = 4;
	const uint8_t PIN_F1 = A1;
	const uint8_t PIN_F2 = A2;
	///@}

	/// @name 16x2 LCD pins
	///@{
	const uint8_t PIN_LCD_RS = 8;
	const uint8_t PIN_LCD_ENABLE = 9;
	const uint8_t PIN_LCD_D0 = 4;
	const uint8_t PIN_LCD_D1 = 5;
	const uint8_t PIN_LCD_D2 = 6;
	const uint8_t PIN_LCD_D3 = 7;
	///@}

	/// ST7920 LCD pins
	const uint8_t PIN_LCD_CS = 10;

	/// @name UI
	///@{

	/// For debouncing: interval the keypad state must be stable for (in micros)
	const uint16_t buttonDebounceInterval = 500;

	/// Repeat interval for keypress events (in millis)
	const uint16_t inputInterval = 200;

	/// Interval for blinking display elements (in millis)
	const uint16_t blinkInterval = 500;

	/// Duration for which to show the splash screen (in millis)
	const uint16_t splashDuration = 1000;

	/// Duration for which to show the info screen (in millis)
	const uint16_t infoDuration = 3000;

	/// Interval for updating the display (in millis)
	const uint16_t refreshInterval = 100;

	/// Step size by which to in-/de-crement for coarse adjustment
	const uint8_t adjustStep = 10;

	/// Tolerance in difference between process variable and setpoint outside which the setpoint blinks (in
	/// percentage points)
	const double tolerance = 0.01;
	///@}

	/// @name For GraphicalDisplayUI:
	///@{

	/// Duration for counting a press as 'long' (in millis)
	const uint16_t longPressDuration = 500;

	/// Cooldown on saving the config to EEPROM (in refresh cycles)
	const uint8_t configSaveCooldown = 20 * 1000 / refreshInterval;
	///@}
}


#endif //HUMIDISTAT_CONFIG_H
