#include <Arduino.h>
#include "SerialLogger.h"
#include "asprintf.h"

SerialLogger::SerialLogger(const Humidistat *humidistat, Array<const ThermistorReader*, 4> trs, uint16_t interval) :
		humidistat(*humidistat), trs(trs), interval(interval) {}

void SerialLogger::begin(uint32_t baud) {
	Serial.begin(baud);
	// Indicate that we're ready
	Serial.println("RDY");
}

void SerialLogger::log() {
	double pTerm, iTerm, dTerm;
	humidistat.getTerms(pTerm, iTerm, dTerm);

	char *buf = asprintf("%lu %.2f %3d %.2f %3d %.2f %.2f %.2f %.2f %.2f %.2f %.2f",
	                     lastTime,
	                     humidistat.getHumidity(),
	                     humidistat.setpoint,
	                     humidistat.getTemperature(),
	                     humidistat.controlValue,
	                     trs[0] ? trs[0]->readTemp() : NAN,
	                     trs[1] ? trs[1]->readTemp() : NAN,
	                     trs[2] ? trs[2]->readTemp() : NAN,
	                     trs[3] ? trs[3]->readTemp() : NAN,
	                     pTerm,
	                     iTerm,
	                     dTerm
	);

	Serial.println(buf);
	delete buf;
}

void SerialLogger::update() {
	if (ready) {
		if (millis() - lastTime >= interval) {
			lastTime = millis();
			log();
		}
		return;
	}

	// Listen for RDY signal
	if (Serial.available()) {
		char buf[8];
		size_t numRead = Serial.readBytesUntil('\r', buf, 7);
		buf[numRead] = '\0';

		if (strcmp(buf, "RDY") == 0) {
			// Print header and set ready state
			Serial.println(header);
			ready = true;
		}
	}
}
