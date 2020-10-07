#include "Arduino.h"
#include "SerialLogger.h"

SerialLogger::SerialLogger(Humidistat *humidistat, ThermistorReader (*trs)[4], unsigned long interval) :
		humidistat(*humidistat), trs(*trs), interval(interval) {}

void SerialLogger::begin() {
	Serial.begin(19200);
	// Indicate that we're ready
	Serial.println("RDY");
}

void SerialLogger::log() {
	lastTime = millis();

	char buf[62];
	sprintf(buf, "%f %3d %f %3d %f %f %f %f",
			humidistat.getHumidity(),
			humidistat.setpoint,
			humidistat.getTemperature(),
			humidistat.controlValue,
			trs[0].readTemp(),
			trs[1].readTemp(),
			trs[2].readTemp(),
			trs[3].readTemp()
			);
	Serial.println(buf);
}

void SerialLogger::update() {
	// Listen for RDY signal
	if(Serial.available()) {
		char buf[8];
		size_t numRead = Serial.readBytesUntil('\r', buf, 7);
		buf[numRead] = '\0';

		if(strcmp(buf, "RDY") == 0) {
			// Print header and set ready state
			Serial.println(header);
			ready = true;
		}
	}

	if(ready && (millis() - lastTime > interval))
		log();
}
