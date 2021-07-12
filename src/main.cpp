#include <Arduino.h>
#include <LiquidCrystal.h>
#include <Array.h>

#include "ButtonReader.h"
#include "ControllerUI.h"
#include "Humidistat.h"
#include "SerialLogger.h"
#include "ThermistorReader.h"

// Pins
const uint8_t PIN_DHT = 2;
const uint8_t PIN_BTN = A0;
const uint8_t PIN_S1 = 3;
const uint8_t PIN_S2 = 11;

// Define either HUMIDISTAT_DHT (for DHT22/AM2302 sensors) or HUMIDISTAT_SHT (for Sensirion SHT85 sensors).
// In the former case, the data pin of the sensor should be connected to PIN_DHT. In the latter case, the sensor
// should be connected to the I2C bus. On the Arduino Uno, these pins (SDA and SCL) are shared with A4 and A5.
#define HUMIDISTAT_DHT

#ifdef HUMIDISTAT_DHT
#include "DHTHumiditySensor.h"
DHT dht(PIN_DHT, DHT22);
DHTHumiditySensor hs(&dht);
//                         NTC pins
ThermistorReader trs[] = {1, 2, 3, 4};
Array<ThermistorReader*, 4> trsp{{&trs[0], &trs[1], &trs[2], &trs[3]}};
#endif
#ifdef HUMIDISTAT_SHT
#include "SHTHumiditySensor.h"
SHTSensor sht;
SHTHumiditySensor hs(&sht);
Array<ThermistorReader*, 4> trsp{{}};
#endif

// Global interval for PID/logger (based on polling rate of sensor, in millis)
const uint16_t dt = 500;

//                          LCD pins
LiquidCrystal liquidCrystal(8, 9, 4, 5, 6, 7);
ButtonReader buttonReader(PIN_BTN);
Humidistat humidistat(&hs, 210, dt, 1.00, 0.025, 2.50);
ControllerUI controllerUI(&liquidCrystal, &buttonReader, &humidistat, trsp);
SerialLogger serialLogger(&humidistat, trsp, dt);

void setup() {
	// Set PWM frequency on D3 and D11 to 245.10 Hz
	TCCR2B = TCCR2B & B11111000 | B00000101;

#ifdef HUMIDISTAT_DHT
	dht.begin();
#endif
#ifdef HUMIDISTAT_SHT
	Wire.begin();
	sht.init();
#endif
	serialLogger.begin();
}

void loop() {
	controllerUI.update();
	humidistat.update(PIN_S1, PIN_S2);
	serialLogger.update();
}
