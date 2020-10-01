#ifndef HUMIDISTAT_SERIALLOGGER_H
#define HUMIDISTAT_SERIALLOGGER_H

#include "Humidistat.h"
#include "ThermistorReader.h"

///
class SerialLogger {
private:
	Humidistat &humidistat;
	ThermistorReader (&trs)[4];

	const char *header = "Humidity Setpoint Temperature ControlValue T0 T1 T2 T3";
	const unsigned long interval; //!< Logging interval (in millis)

	unsigned long lastTime = 0;   //!< Last time line was written (in millis)
	bool ready = false;
public:
	/// Constructor.
	/// \param humidistat Pointer to a Humidistat instance
	explicit SerialLogger(Humidistat *humidistat, ThermistorReader (*trs)[4], unsigned long interval);

	/// Setup the serial interface
	void begin();

	/// Write a line to serial
	void log();

	/// Log a line every interval, once data has been received
	void update();
};

#endif //HUMIDISTAT_SERIALLOGGER_H
