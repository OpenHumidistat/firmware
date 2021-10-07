#ifndef HUMIDISTAT_SERIALLOGGER_H
#define HUMIDISTAT_SERIALLOGGER_H

#include <stdint.h>
#include <Array.h>

#include "asprintf.h"
#include "control/SingleHumidistat.h"
#include "control/CascadeHumidistat.h"
#include "sensor/ThermistorReader.h"

/// Logs humidistat data over serial.
/// \tparam Humidistat_t Either SingleHumidistat or CascadeHumidistat
template<class Humidistat_t>
class SerialLogger {
private:
	const Humidistat_t &humidistat;
	const Array<const ThermistorReader *, 4> trs;

	// Can't specialize constexpr...
	static const char header[];

	const uint16_t interval;    //!< Logging interval (in millis)
	unsigned long lastTime = 0; //!< Last time line was written (in millis)
	bool ready = false;

	/// Write a line to serial
	void log();

public:
	/// Constructor.
	/// \param humidistat Pointer to a Humidistat instance
	/// \param trs        Array of 4 pointers to ThermistorReader instances
	/// \param interval   Logging interval (in ms)
	explicit SerialLogger(const Humidistat_t *humidistat, Array<const ThermistorReader *, 4> trs, uint16_t interval)
			: humidistat(*humidistat), trs(trs), interval(interval) {}

	/// Setup the serial interface
	static void begin(uint32_t baud) {
		Serial.begin(baud);
		// Indicate that we're ready
		Serial.println("RDY");
	}

	/// Log a line every interval, once data has been received
	void update() {
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

		if (ready) {
			if (millis() - lastTime >= interval) {
				lastTime = millis();
				log();
			}
		}
	}
};

template<>
const char SerialLogger<SingleHumidistat>::header[] = "Time Humidity Setpoint Temperature ControlValue T0 T1 T2 T3 "
													  "pTerm iTerm dTerm";

template<>
const char SerialLogger<CascadeHumidistat>::header[] = "Time PV SP T CV inner0PV inner0CV inner1PV inner1CV "
													   "pTerm iTerm dTerm "
                                                       "inner0pTerm inner0iTerm inner0dTerm "
                                                       "inner1pTerm inner1iTerm inner1dTerm";

template<>
void SerialLogger<SingleHumidistat>::log() {
	double pTerm, iTerm, dTerm;
	humidistat.getTerms(pTerm, iTerm, dTerm);

	char *buf = asprintf("%lu %.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f",
	                     lastTime,
	                     humidistat.getHumidity(),
	                     humidistat.sp,
	                     humidistat.getTemperature(),
	                     humidistat.cv,
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

template<>
void SerialLogger<CascadeHumidistat>::log() {
	double outerPTerm, outerITerm, outerDTerm;
	humidistat.getTerms(outerPTerm, outerITerm, outerDTerm);

	double innerPTerms[2], innerITerms[2], innerDTerms[2];
	humidistat.getInner(0)->getTerms(innerPTerms[0], innerITerms[0], innerDTerms[0]);
	humidistat.getInner(1)->getTerms(innerPTerms[1], innerITerms[1], innerDTerms[1]);

	char *buf = asprintf("%lu %.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f",
	                     lastTime,
	                     humidistat.getHumidity(),
	                     humidistat.sp,
	                     humidistat.getTemperature(),
	                     humidistat.cv,
	                     humidistat.getInner(0)->pv,
	                     humidistat.getInner(0)->cv,
	                     humidistat.getInner(1)->pv,
	                     humidistat.getInner(1)->cv,
	                     outerPTerm,
	                     outerITerm,
	                     outerDTerm,
	                     innerPTerms[0],
	                     innerITerms[0],
	                     innerDTerms[0],
	                     innerPTerms[1],
	                     innerITerms[1],
	                     innerDTerms[1]
	);

	Serial.println(buf);
	delete buf;
}

#endif //HUMIDISTAT_SERIALLOGGER_H
