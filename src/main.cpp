#include "Arduino.h"
#include "DHT.h"
#include "LiquidCrystal.h"

#include "ButtonReader.h"
#include "ControllerUI.h"
#include "Humidistat.h"
#include "SerialLogger.h"
#include "ThermistorReader.h"

// Pins
const uint8_t PINS_LCD[] = {8, 9, 4, 5, 6, 7};
const uint8_t PIN_DHT = 2;
const uint8_t PIN_BTN = A0;
const uint8_t PIN_S1 = 3;
const uint8_t PIN_S2 = 11;

// Global interval for PID/logger (based on polling rate of DHT22)
const unsigned long dt = 2000;

//                         NTC pins
ThermistorReader trs[4] = {1, 2, 3, 4};
//                          LCD pins
LiquidCrystal liquidCrystal(8, 9, 4, 5, 6, 7);
ButtonReader buttonReader(PIN_BTN);
DHT dht(PIN_DHT, DHT22);
Humidistat humidistat(&dht, 220, dt, 0.2, 0.02, 1);
ControllerUI controllerUI(&liquidCrystal, &buttonReader, &humidistat, &trs);
SerialLogger serialLogger(&humidistat, &trs, dt);

void setup() {
	dht.begin();
	serialLogger.begin();
}

void loop() {
	controllerUI.update();
	humidistat.update(PIN_S1, PIN_S2);
	serialLogger.update();
}
