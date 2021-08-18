#ifndef HUMIDISTAT_CONFIG_V1_H
#define HUMIDISTAT_CONFIG_V1_H

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
#define HUMIDISTAT_CONTROLLER_SINGLE

namespace config {
	/// Serial communication symbol rate (baud)
	const uint32_t serialRate = 115200;

	/// Set to true to override the values stored in EEPROM and use the default PID parameters defined below.
	const bool overrideEEPROM = false;

	/// EEPROM address for storing the block
	const uint8_t EEPROMAddress = 0;

	/// Global interval for PID/logger (based on polling rate of sensor, in millis)
	const uint16_t dt = 250;

	///@{
	/// Humidity controller PID parameters
	const double HC_Kp = 1.00;
	const double HC_Ki = 0.025;
	const double HC_Kd = 2.50;
	///@}

	///@{
	/// Flow controller PID parameters
	const double FC_Kp = 1.00;
	const double FC_Ki = 1.00;
	const double FC_Kd = 1.00;
	///@}

	/// Minimum solenoid duty cycle (deadband)
	const uint8_t S_lowValue = 210;

	/// Total flowrate (for cascade controller) (L/min)
	const double HC_totalFlowrate = 2;

	///@{
	/// Pins
	const uint8_t PIN_DHT = 2;
	const uint8_t PIN_BTN = A0;
	const uint8_t PIN_S1 = 3;
	const uint8_t PIN_S2 = 4;
	const uint8_t PIN_F1 = A1;
	const uint8_t PIN_F2 = A2;
	///@}

	///@{
	/// 16x2 LCD pins
	const uint8_t PIN_LCD_RS = 8;
	const uint8_t PIN_LCD_ENABLE = 9;
	const uint8_t PIN_LCD_D0 = 4;
	const uint8_t PIN_LCD_D1 = 5;
	const uint8_t PIN_LCD_D2 = 6;
	const uint8_t PIN_LCD_D3 = 7;
	///@}

	/// ST7920 LCD pins
	const uint8_t PIN_LCD_CS = 10;
}


#endif //HUMIDISTAT_CONFIG_V1_H
