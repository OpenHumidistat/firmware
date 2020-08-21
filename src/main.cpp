#include "Arduino.h"
#include "DHT.h"
#include "LiquidCrystal.h"

#include "ButtonReader.h"
#include "ControllerUI.h"
#include "Humidistat.h"
#include "SerialLogger.h"

// Pins
const uint8_t PINS_LCD[] = {8, 9, 4, 5, 6, 7};
const uint8_t PIN_DHT = 2;
const uint8_t PIN_BTN = A0;
const uint8_t PIN_S1 = 3;
const uint8_t PIN_S2 = 11;

LiquidCrystal liquidCrystal(PINS_LCD[0], PINS_LCD[1], PINS_LCD[2], PINS_LCD[3], PINS_LCD[4], PINS_LCD[5]);
ButtonReader buttonReader(PIN_BTN);
DHT dht(PIN_DHT, DHT22);
Humidistat humidistat(&dht, 1, 1, 1);
ControllerUI controllerUI(&liquidCrystal, &buttonReader, &humidistat);
SerialLogger serialLogger(&humidistat);

void setup() {
	dht.begin();
	serialLogger.begin();
}

void loop() {
	controllerUI.update();
	humidistat.update(PIN_S1, PIN_S2);
	serialLogger.update();
}
