#include "Arduino.h"
#include "SerialLogger.h"

SerialLogger::SerialLogger(Humidistat *humidistat) : humidistat(*humidistat) {}

void SerialLogger::begin() {
	Serial.begin(19200);
	// Indicate that we're ready
	Serial.println("RDY");
}

void SerialLogger::log() {
	lastTime = millis();

	char buf[50];
	sprintf(buf, "%4.1f %3d %4.1f %3d",
			humidistat.getHumidity(),
			humidistat.setpoint,
			humidistat.getTemperature(),
			humidistat.getCv()
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
	if(ready){
		if (millis() - lastTime > interval) {
			log();
		}
	}
}
