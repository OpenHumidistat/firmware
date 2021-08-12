#ifndef HUMIDISTAT_SERIALLOGGER_H
#define HUMIDISTAT_SERIALLOGGER_H

#include <stdint.h>
#include <Array.h>

#include "Humidistat.h"
#include "sensor/ThermistorReader.h"

///
class SerialLogger {
private:
	const Humidistat &humidistat;
	Array<const ThermistorReader*, 4> trs;

	const char *header = "Time Humidity Setpoint Temperature ControlValue T0 T1 T2 T3 pTerm iTerm dTerm";
	const uint16_t interval; //!< Logging interval (in millis)

	unsigned long lastTime = 0;   //!< Last time line was written (in millis)
	bool ready = false;
public:
	/// Constructor.
	/// \param humidistat Pointer to a Humidistat instance
	/// \param trs        Array of 4 pointers to ThermistorReader instances
	/// \param interval   Logging interval (in ms)
	explicit SerialLogger(const Humidistat *humidistat, Array<const ThermistorReader*, 4> trs, uint16_t interval);

	/// Setup the serial interface
	void begin(uint32_t baud);

	/// Write a line to serial
	void log();

	/// Log a line every interval, once data has been received
	void update();
};

#endif //HUMIDISTAT_SERIALLOGGER_H
