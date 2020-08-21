#ifndef HUMIDISTAT_SERIALLOGGER_H
#define HUMIDISTAT_SERIALLOGGER_H

#include "Humidistat.h"

///
class SerialLogger {
private:
	Humidistat &humidistat;

	const char *header = "Humidity Setpoint Temperature ControlValue";
	const unsigned long interval = 100; //!< Logging interval (in millis)

	unsigned long lastTime = 0;         //!< Last time line was written (in millis)
	bool ready = false;
public:
	/// Constructor.
	/// \param humidistat Pointer to a Humidistat instance
	explicit SerialLogger(Humidistat *humidistat);

	/// Setup the serial interface
	void begin();

	/// Write a line to serial
	void log();

	/// Log a line every interval, once data has been received
	void update();
};

#endif //HUMIDISTAT_SERIALLOGGER_H
